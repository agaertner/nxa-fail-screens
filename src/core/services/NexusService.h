#ifndef NEXUSSERVICE_H
#define NEXUSSERVICE_H
#include "Defines.h"

namespace Nekres::Services
{
    class NexusService
    {
        public:
            NexusService(AddonAPI_t* p_api);

            ~NexusService();

            NexusLinkData_t* Core() const;

        private:
            inline static NexusService* m_instance = nullptr;

            AddonAPI_t* m_api;
            NexusLinkData_t* m_nexus;
    };
}
#endif
