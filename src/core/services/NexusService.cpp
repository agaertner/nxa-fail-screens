#include "NexusService.h"

Nekres::Services::NexusService::NexusService(AddonAPI_t* p_api) : m_api(p_api)
{
    m_instance = this;
    m_nexus = (NexusLinkData_t*)m_api->DataLink_Get("DL_NEXUS_LINK");
}

Nekres::Services::NexusService::~NexusService()
{
    m_instance = nullptr;
}

NexusLinkData_t* Nekres::Services::NexusService::Core() const
{
    return m_nexus;
}