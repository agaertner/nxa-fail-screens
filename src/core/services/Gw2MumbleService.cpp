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
#endif