#include "ScreenOrchestrator.h"
#include "Settings.h"
#include <imgui/imgui.h>
#include <cstdlib>

#include "screens/DarkSouls.h"
#include "screens/GrandTheftAuto.h"
#include "screens/RytlocksCritterRampage.h"
#include "screens/AngryPepe.h"
#include "screens/Sekiro.h"
#include "screens/WinXp.h"
#include <lib-nxa-sdk/NexusSDK.h>
#include "services/Services.h"


namespace Nekres {

    ScreenOrchestrator::ScreenOrchestrator(AddonAPI_t* api, HMODULE hSelf)
        : m_api(api), m_hSelf(hSelf)
    {
    }

    void ScreenOrchestrator::Render()
    {
        static bool s_fontsLoaded = false;
        if (!s_fontsLoaded) {
            NexusSDK::Content->GetFont(IDR_FONT_CONSOLA, 40.0f);
            NexusSDK::Content->GetFont(IDR_FONT_CONSOLA, 20.0f);
            NexusSDK::Content->GetFont(IDR_FONT_CONSOLA, 16.0f);
            NexusSDK::Content->GetFont(IDR_FONT_ATHELAS, 250.0f);
            s_fontsLoaded = true;
        }

        if (m_isPlayingAnimation && m_activeScreen) {
            float timeSinceDeath = static_cast<float>(ImGui::GetTime()) - m_timeOfDeath;
            m_isPlayingAnimation = m_activeScreen->Render(timeSinceDeath);
            if (!m_isPlayingAnimation) {
                m_isPreviewing = false;
            }
        }
    }

    void ScreenOrchestrator::TriggerPreview()
    {
        m_isPlayingAnimation = true;
        m_isPreviewing = true;
        m_timeOfDeath = static_cast<float>(ImGui::GetTime());
        
        m_activeScreen = CreateFailScreen(Settings::ActiveScreen);

        int language = 0; // Default to English
        if (Services::m_rtapi && Services::m_rtapi->Data()) {
            language = Services::m_rtapi->Data()->Language;
        }

        m_activeScreen->OnDeath(language);
    }

    void ScreenOrchestrator::TriggerDeath(int language)
    {
        m_isPlayingAnimation = true;
        m_timeOfDeath = static_cast<float>(ImGui::GetTime());
        
        int screenToPlay = Settings::ActiveScreen;
        if (Settings::Randomize) {
            std::vector<int> pool;
            if (Settings::EnableDarkSouls) pool.push_back(0);
            if (Settings::EnableGrandTheftAuto) pool.push_back(1);
            if (Settings::EnableRytlocksCritterRampage) pool.push_back(2);
            if (Settings::EnableAngryPepe) pool.push_back(3);
            if (Settings::EnableSekiro) pool.push_back(4);
            if (Settings::EnableWinXp) pool.push_back(5);

            if (!pool.empty()) {
                screenToPlay = pool[rand() % pool.size()];
            }
        }
        
        m_activeScreen = CreateFailScreen(screenToPlay);
        m_activeScreen->OnDeath(language);
    }

    void ScreenOrchestrator::StopAll()
    {
        m_isPlayingAnimation = false;
        m_isPreviewing = false;
        m_activeScreen.reset();
    }

    std::unique_ptr<FailScreen> ScreenOrchestrator::CreateFailScreen(int screenIndex)
    {
        switch (screenIndex) {
            case 0: return std::make_unique<DarkSouls>(m_api, m_hSelf);
            case 1: return std::make_unique<GrandTheftAuto>(m_api, m_hSelf);
            case 2: return std::make_unique<RytlocksCritterRampage>(m_api, m_hSelf);
            case 3: return std::make_unique<AngryPepe>(m_api, m_hSelf);
            case 4: return std::make_unique<Sekiro>(m_api, m_hSelf);
            case 5: return std::make_unique<WinXp>(m_api, m_hSelf);
            default: return std::make_unique<DarkSouls>(m_api, m_hSelf);
        }
    }
}
