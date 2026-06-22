#ifndef SERVICES_H
#define SERVICES_H

#include "../../Defines.h"
#include <imgui/imgui.h>

#ifdef USE_MUMBLE
#include "Gw2MumbleService.h"
#endif

#ifdef USE_RTAPI
#include "RealTimeApiService.h"
#endif

#include "NexusService.h"
#include <lib-nxa-sdk/NexusSDK.h>

namespace Nekres::Services
{
#ifdef USE_MUMBLE
    inline Gw2MumbleService* m_mumble = nullptr;
    inline Gw2MumbleService* Mumble(AddonAPI_t* api = nullptr)
    {
        if (!m_mumble && api)
            m_mumble = new Gw2MumbleService(api);
        return m_mumble;
    }
#endif

#ifdef USE_RTAPI
    inline RealTimeApiService* m_rtapi = nullptr;
    inline RealTimeApiService* RTAPI(AddonAPI_t* api = nullptr)
    {
        if (!m_rtapi && api)
            m_rtapi = new RealTimeApiService(api);
        return m_rtapi;
    }
#endif

    inline NexusService* m_nexus = nullptr;
    inline NexusService* Nexus(AddonAPI_t* api = nullptr)
    {
        if (!m_nexus && api)
            m_nexus = new NexusService(api);
        return m_nexus;
    }

    inline float cachedUiScale = 1.0f;
    inline float GetFinalScale() {
        float gameUiScale = cachedUiScale;
#ifdef USE_MUMBLE
        if (m_mumble && m_mumble->Identity()) {
            switch (m_mumble->Identity()->UISize) {
                case Mumble::EUIScale::Small: gameUiScale = 0.9f; break;
                case Mumble::EUIScale::Normal: gameUiScale = 1.0f; break;
                case Mumble::EUIScale::Large: gameUiScale = 1.1f; break;
                case Mumble::EUIScale::Larger: gameUiScale = 1.2f; break;
            }
            cachedUiScale = gameUiScale;
        }
#endif
        return gameUiScale * (ImGui::GetIO().DisplaySize.y / 1080.0f);
    }
}

#endif
