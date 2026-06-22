#ifndef FAILSCREEN_H
#define FAILSCREEN_H

#include <vector>
#include <string>
#include <memory>
#include <cstdint>
#include <unordered_map>
#include "../../Defines.h"
#include "../services/Services.h"
#include <lib-nxa-sdk/NexusSDK.h>

#include <imgui/imgui.h>

namespace Nekres {

    class FailScreen {
    public:
        FailScreen(AddonAPI_t* api, HMODULE hSelf) : m_api(api), m_hSelf(hSelf) {}
        virtual ~FailScreen() {
            for (auto& handle : m_audioHandles) {
                handle.Stop();
            }
        }

        // Triggered upon death. Receives the EGameLanguage (e.g. 0=EN, 2=FR, 3=DE, 4=ES).
        virtual void OnDeath(int language) = 0;

        // Public non-virtual Render method handling ImGui boilerplate
        bool Render(float timeSinceDeath);

    protected:
        AddonAPI_t* m_api;
        HMODULE m_hSelf;

        // Derived classes must implement this to draw their actual content.
        // Return false when the animation has finished.
        virtual bool DrawContent(ImDrawList* drawList, float timeSinceDeath, ImVec2 screenSize, float finalScale) = 0;

        // Audio Management
        std::vector<NexusSDK::AudioHandle> m_audioHandles;
        
        void PlaySoundEffect(const std::string& resourceName) {
            m_audioHandles.push_back(NexusSDK::Audio->Play(resourceName, "Screen"));
        }


    };

}

#endif // FAILSCREEN_H
