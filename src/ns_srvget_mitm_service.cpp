/*
 * Copyright (c) 2018 p-sam
 *
 * This program is free software; you can redistribute it and/or modify it
 * under the terms and conditions of the GNU General Public License,
 * version 2, as published by the Free Software Foundation.
 */

#include <switch.h>
#include "ns_srvget_mitm_service.hpp"
#include "file_utils.hpp"
#include "ini.h"

static int _ProcessControlDataIniHandler(void* user, const char* section, const char* name, const char* value) {
    Nacp* nacp = (Nacp*)user;

    if (strcasecmp(section, "override_nacp") == 0) {
        if (strcasecmp(name, "name") == 0) {
            for (unsigned int i = 0; i < sizeof(nacp->lang_entries) / sizeof(nacp->lang_entries[0]); i++) {
                strncpy(nacp->lang_entries[i].name, value, sizeof(nacp->lang_entries[i].name) - 1);
            }
        } else if (strcasecmp(name, "author") == 0) {
            for (unsigned int i = 0; i < sizeof(nacp->lang_entries) / sizeof(nacp->lang_entries[0]); i++) {
                strncpy(nacp->lang_entries[i].author, value, sizeof(nacp->lang_entries[i].author) - 1);
            }
        } else if (strcasecmp(name, "display_version") == 0) {
            strncpy(nacp->display_version, value, sizeof(nacp->display_version) - 1);
        }
    }

    return 1;
}

/*
 * Applies NACP overrides and icon override.
 * ALSO bumps display_version to force qlaunch to refresh its cache.
 */
static void _ProcessControlData(u64 tid, u8* buf, size_t buf_size, u32* out_size) {
    if (buf_size < 0x4000 || buf_size < sizeof(Nacp)) {
        return;
    }

    Nacp* nacp = (Nacp*)buf;

    char path[128] = {0};

    /* Optional config.ini overrides */
    snprintf(path, sizeof(path) - 1,
             "sdmc:/atmosphere/contents/%016lx/config.ini", tid);
    ini_parse(path, _ProcessControlDataIniHandler, nacp);

    /*
     * 🔑 CRITICAL FIX FOR FW 21+
     * Force qlaunch to refresh cached control data
     * by bumping the display version.
     */
    strncpy(nacp->display_version, "99.99", sizeof(nacp->display_version) - 1);

    /* Icon override */
    snprintf(path, sizeof(path) - 1,
             "sdmc:/atmosphere/contents/%016lx/icon.jpg", tid);

    FILE* f = fopen(path, "rb");
    if (!f) {
        return;
    }

    fseek(f, 0, SEEK_END);
    long icon_size = ftell(f);
    fseek(f, 0, SEEK_SET);

    const size_t icon_offset = 0x4000;
    long max_icon = (long)buf_size - (long)icon_offset;
    if (icon_size > max_icon) icon_size = max_icon;
    if (icon_size < 0) icon_size = 0;

    void* dst = (void*)(buf + icon_offset);
    size_t read = fread(dst, 1, (size_t)icon_size, f);
    fclose(f);

    if (read > 0) {
        u64 total = icon_offset + read;
        if (total > 0xFFFFFFFFULL) total = 0xFFFFFFFFULL;
        *out_size = (u32)total;
    }
}

bool NsAm2MitmService::ShouldMitm(const ams::sm::MitmProcessInfo& client_info) {
    FILE_LOG_IPC(NSAM2_MITM_SERVICE_NAME, client_info, "() // true");
    return true;
}

ams::Result NsAm2MitmService::GetApplicationControlData(
    u8 source,
    u64 application_id,
    const ams::sf::OutBuffer& buffer,
    ams::sf::Out<u32> out_size) {

    const struct {
        u8 source;
        u8 pad[7];
        u64 application_id;
    } in = { source, {0}, application_id };

    u32 tmp_size = 0;

    Result rc = serviceDispatchInOut(
        this->forward_service.get(),
        (u32)NsAm2CmdId::GetApplicationControlData,
        in,
        tmp_size,
        .buffer_attrs = { SfBufferAttr_HipcAutoSelect | SfBufferAttr_Out },
        .buffers = { { buffer.GetPointer(), buffer.GetSize() } }
    );

    if (R_SUCCEEDED(rc)) {
        _ProcessControlData(application_id,
                            buffer.GetPointer(),
                            buffer.GetSize(),
                            &tmp_size);
        out_size.SetValue(tmp_size);
    }

    return rc;
}

