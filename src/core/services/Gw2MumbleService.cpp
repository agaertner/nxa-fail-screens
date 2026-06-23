#include "Gw2MumbleService.h"

#ifdef USE_MUMBLE
Nekres::Services::Gw2MumbleService::Gw2MumbleService(AddonAPI_t* p_api)
    : m_api(p_api),
    m_link((Mumble::Data*)p_api->DataLink_Get("DL_MUMBLE_LINK")),
    m_identity()

{
    m_instance = this;
    m_api->Events_Subscribe(MUMBLE_IDENTITY_UPDATED, OnMumbleIdentityUpdated);
}
Nekres::Services::Gw2MumbleService::~Gw2MumbleService()
{
    m_api->Events_Unsubscribe(MUMBLE_IDENTITY_UPDATED, OnMumbleIdentityUpdated);
    m_instance = nullptr;
}

Mumble::Data* Nekres::Services::Gw2MumbleService::Data() const
{
    return m_link;
}

Mumble::Identity* Nekres::Services::Gw2MumbleService::Identity() const
{
    return m_identity;
}

float Nekres::Services::Gw2MumbleService::GetUIScale() const
{
    if (!m_identity) return 1.0f;
    
    switch (m_identity->UISize) {
        case Mumble::EUIScale::Small: return 0.897f;
        case Mumble::EUIScale::Normal: return 1.0f;
        case Mumble::EUIScale::Large: return 1.111f;
        case Mumble::EUIScale::Larger: return 1.222f;
        default: return 1.0f;
    }
}
#endif
