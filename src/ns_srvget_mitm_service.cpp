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
        } else if (strcasecmp(name, "startup_user_account") == 0) {
            nacp->startup_user_account = (*value == 't' || *value == '1');
        }
    }

    return 1;
}

static bool _ProcessControlData(u64 tid, u8* buf, size_t buf_size, u32* out_size) {
    bool did_override = false;

    if (buf_size < 0x4000 || buf_size < sizeof(Nacp)) {
        return false;
    }

    char path[128] = {0};

    // Optional NACP override
    snprintf(path, sizeof(path), "sdmc:/atmosphere/contents/%016lx/config.ini", tid);
    ini_parse(path, _ProcessControlDataIniHandler, buf);

    // Icon override
    snprintf(path, sizeof(path), "sdmc:/atmosphere/contents/%016lx/icon.jpg", tid);
    FILE* f = fopen(path, "rb");
    if (!f) return false;

    fseek(f, 0, SEEK_END);
    long icon_size = ftell(f);
    fseek(f, 0, SEEK_SET);

    const size_t icon_off = 0x4000;
    long max_icon = (long)buf_size - (long)icon_off;
    if (icon_size > max_icon) icon_size = max_icon;
    if (icon_size < 0) icon_size = 0;

    void* icon_dst = buf + icon_off;
    size_t bytes_read = fread(icon_dst, 1, icon_size, f);
    fclose(f);

    if (bytes_read > 0) {
        unsigned long long total = icon_off + bytes_read;
        if (total > 0xFFFFFFFFull) total = 0xFFFFFFFFull;
        *out_size = (u32)total;
        did_override = true;
    }

    return did_override;
}

bool NsAm2MitmService::ShouldMitm(const ams::sm::MitmProcessInfo& client_info) {
    bool should_mitm = true;
    FILE_LOG_IPC(NSAM2_MITM_SERVICE_NAME, client_info, "() // %s",
                 should_mitm ? "true" : "false");
    return should_mitm;
}

ams::Result NsAm2MitmService::GetApplicationControlData(
    u8 source,
    u64 application_id,
    const ams::sf::OutBuffer& buffer,
    ams::sf::Out<u32> out_size) {

    struct {
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
        .buffers      = { { buffer.GetPointer(), buffer.GetSize() } }
    );

    FILE_LOG_IPC_CLASS("(src=%u, tid=0x%016lx, outbuf=%zu) // %x",
                       source, application_id, buffer.GetSize(), rc);

    if (R_SUCCEEDED(rc)) {
        bool did_override = _ProcessControlData(
            application_id,
            (u8*)buffer.GetPointer(),
            buffer.GetSize(),
            &tmp_size
        );

        out_size.SetValue(tmp_size);

        if (did_override) {
            // Invalidate normal control cache
            Result rc_inv = serviceDispatchIn(
                this->forward_service.get(),
                (u32)NsAm2CmdId::InvalidateApplicationControlCache,
                application_id
            );

            // Invalidate qlaunch icon cache (required FW 20+)
            Result rc_icon = serviceDispatchIn(
                this->forward_service.get(),
                (u32)NsAm2CmdId::InvalidateApplicationIconCache,
                application_id
            );

            FileUtils::LogLine(
                "[%s] InvalidateApplicationControlCache(0x%016lx) -> 0x%08X | IconCache -> 0x%08X",
                NSAM2_MITM_SERVICE_NAME,
                application_id,
                rc_inv,
                rc_icon
            );
        }
    }

    return rc;
}

