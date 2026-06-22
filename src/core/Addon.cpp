#include "Addon.h"
#include "Settings.h"

#include "imgui/imgui.h"
#include "services/Gw2MumbleService.h"
#include "services/NexusService.h"
#include "services/RealTimeApiService.h"


extern HMODULE hSelf;

namespace Nekres {

    Addon::Addon(AddonDefinition_t* p_addonDef, AddonAPI_t* p_api)
        : m_addonDef(p_addonDef), m_api(p_api)
    {
        m_instance = this;

        ImGui::SetCurrentContext((ImGuiContext*)m_api->ImguiContext);
        if (m_api->ImguiMalloc && m_api->ImguiFree) {
            ImGui::SetAllocatorFunctions((void* (*)(size_t, void*))m_api->ImguiMalloc, (void(*)(void*, void*))m_api->ImguiFree);
        }

        std::string folderName = m_addonDef->Name;
        folderName.erase(std::remove_if(folderName.begin(), folderName.end(), ::isspace), folderName.end());

        m_addonPath = m_api->Paths_GetAddonDirectory(folderName.c_str());
        m_settingsPath = m_addonPath / "settings.json";

        std::filesystem::create_directories(m_addonPath);
        Settings::Load(m_settingsPath);

        NexusSDK::Initialize(m_api, hSelf, m_addonPath);
        NexusSDK::Audio->SetMasterVolume(Settings::MasterVolume);
        NexusSDK::Audio->SetChannelVolume("UI", Settings::UIVolume);
        NexusSDK::Audio->SetChannelVolume("Screen", Settings::ScreenVolume);
#ifdef USE_MUMBLE
        Services::m_mumble = new Services::Gw2MumbleService(m_api);
#endif
#ifdef USE_NEXUS_LINK
        Services::m_nexus = new Services::NexusService(m_api);
#endif
#ifdef USE_RTAPI
        Services::m_rtapi = new Services::RealTimeApiService(m_api);
#endif



        HRSRC hRes = FindResourceA(hSelf, IDR_DATA_LOCALIZATION, (LPCSTR)RT_RCDATA);
        if (hRes) {
            HGLOBAL hData = LoadResource(hSelf, hRes);
            if (hData) {
                DWORD size = SizeofResource(hSelf, hRes);
                const char* data = (const char*)LockResource(hData);
                if (data && size > 0) {
                    std::string defaultJsonString(data, size);
                    NexusSDK::Local->SetDefault(defaultJsonString);
                }
            }
        }

        auto langProvider = [this]() -> int {
#ifdef USE_RTAPI
            if (Services::m_rtapi && Services::m_rtapi->Data()) {
                return Services::m_rtapi->Data()->Language;
            }
#endif
            return 0;
        };
        NexusSDK::Local->SetLanguageProvider(langProvider);
        NexusSDK::SDKLocal->SetLanguageProvider(langProvider);
        m_orchestrator = std::make_unique<ScreenOrchestrator>(m_api, hSelf);
        m_deathMonitor = std::make_unique<DeathMonitor>(m_api);
        m_settingsUI = std::make_unique<SettingsUI>(m_settingsPath, m_addonDef);

        m_deathMonitor->SetCallbacks(
            [this](int language) { m_orchestrator->TriggerDeath(language); },
            [this]() { m_orchestrator->StopAll(); }
        );

        m_settingsUI->SetCallbacks(
            [this]() { m_orchestrator->TriggerPreview(); },
            [this]() { m_orchestrator->StopAll(); }
        );

        m_api->GUI_Register(RT_Render, AddonRender);
        m_api->GUI_Register(RT_OptionsRender, AddonOptions);
        m_api->WndProc_Register(WndProc);
    }

    Addon::~Addon()
    {
        m_api->GUI_Deregister(AddonRender);
        m_api->GUI_Deregister(AddonOptions);
        m_api->WndProc_Deregister(WndProc);

        m_orchestrator.reset();
        m_deathMonitor.reset();
        m_settingsUI.reset();

        NexusSDK::Shutdown();
#ifdef USE_MUMBLE
        delete Services::m_mumble;
        Services::m_mumble = nullptr;
#endif
#ifdef USE_NEXUS_LINK
        delete Services::m_nexus;
        Services::m_nexus = nullptr;
#endif
#ifdef USE_RTAPI
        delete Services::m_rtapi;
        Services::m_rtapi = nullptr;
#endif

        Settings::Save(m_settingsPath);
        m_instance = nullptr;
    }

    void Addon::Render()
    {
        m_deathMonitor->Update(m_orchestrator->IsPreviewing());
        
        ImFont* defaultFont = NexusSDK::Content->GetFont("NXA_FONT_MENOMONIA", 18.0f);
        if (defaultFont) ImGui::PushFont(defaultFont);
        
        m_orchestrator->Render();
        
        NexusSDK::UI::RenderDialogs();
        
        if (defaultFont) ImGui::PopFont();
    }

    void Addon::Options()
    {
        ImFont* defaultFont = NexusSDK::Content->GetFont("NXA_FONT_MENOMONIA", 18.0f);
        if (defaultFont) ImGui::PushFont(defaultFont);
        
        m_settingsUI->Render();
        
        if (defaultFont) ImGui::PopFont();
    }

    UINT Addon::WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
    {
        // Allow canceling active fail screen animation by double-clicking anywhere in the game window.
        if (m_instance && m_instance->m_orchestrator) {
            if (uMsg == 0x0203) { // WM_LBUTTONDBLCLK
                if (m_instance->m_orchestrator->IsPlayingAnimation()) {
                    m_instance->m_orchestrator->StopAll();
                }
            }
        }
        return uMsg;
    }
}
