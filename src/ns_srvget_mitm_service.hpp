/*
 * sys-icon-changer
 * Minimal ns:am2 MITM for overriding cached home menu icon bytes.
 *
 * Key points (switchbrew):
 * - GetApplicationControlData uses a type-0x6 output buffer (qlaunch uses 0x24000).
 * - NACP is written at buf+0, icon is written at buf+0x4000.
 * - Returns output u32 actual_size.
 */

#pragma once

#include "libams.hpp"
#include "ns.h"

#define NSAM2_MITM_SERVICE_NAME "ns:am2"

enum class NsAm2CmdId : u32 {
    GetApplicationControlData = 400,
};

#define NS_AM2_INTERFACE_INFO(C, H) \
    AMS_SF_METHOD_INFO_F(C, H, NsAm2CmdId, GetApplicationControlData, \
        (u8 source, u64 application_id, const ams::sf::OutBuffer &buffer, ams::sf::Out<u32> out_size), \
        (source, application_id, buffer, out_size))

AMS_SF_DEFINE_MITM_INTERFACE_F(NsAm2MitmInterface, NS_AM2_INTERFACE_INFO);

class NsAm2MitmService : public ams::sf::MitmServiceImplBase {
public:
    NsAm2MitmService(std::shared_ptr<Service> &&s, const ams::sm::MitmProcessInfo &c)
        : MitmServiceImplBase(std::move(s), c) {}

    NS_AM2_INTERFACE_INFO(_, AMS_SF_DECLARE_INTERFACE_METHODS);

    static bool ShouldMitm(const ams::sm::MitmProcessInfo &client_info);

    constexpr const char *GetDisplayName() {
        return NSAM2_MITM_SERVICE_NAME;
    }

    static constexpr ams::sm::ServiceName GetServiceName() {
        return ams::sm::ServiceName::Encode(NSAM2_MITM_SERVICE_NAME);
    }

    static constexpr size_t GetMaxSessions() {
        return 8;
    }
};

static_assert(IsNsAm2MitmInterface<NsAm2MitmService>);
