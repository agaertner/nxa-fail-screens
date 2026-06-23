#include "AudioSettingsPage.h"
#include "../../Settings.h"
#include "../../services/Services.h"
#include <lib-nxa-sdk/NexusSDK.h>

#include <chrono>

namespace Nekres {

    AudioSettingsPage::AudioSettingsPage(const std::filesystem::path& settingsPath)
        : Container(), m_settingsPath(settingsPath)
    {
        auto flowPanel = std::make_shared<NexusSDK::UI::FlowPanel>();
        flowPanel->ControlFlowDirection = NexusSDK::UI::FlowDirection::TopToBottom;
        flowPanel->ControlPadding = 2.0f;
        AddChild(flowPanel);

        auto local = NexusSDK::Local;

        float maxLabelWidth = 220.0f;

        // Master Volume Slider
        auto volSlider = std::make_shared<NexusSDK::UI::Slider>(local->GetString("Settings_MasterVolume"), &Settings::MasterVolume, 0.0f, 1.0f);
        volSlider->LabelWidth = maxLabelWidth;
        volSlider->OnValueChanged = [this](float val) {
            Settings::Save(m_settingsPath);
            NexusSDK::Audio->SetMasterVolume(val);
        };
        flowPanel->AddChild(volSlider);

        // Add extra padding after Master Volume
        auto padding = std::make_shared<NexusSDK::UI::ControlBase>();
        padding->SetSize(0, 16); // Spacer
        flowPanel->AddChild(padding);

        // UI Volume Slider
        auto uiVolSlider = std::make_shared<NexusSDK::UI::Slider>(local->GetString("Settings_UIVolume"), &Settings::UIVolume, 0.0f, 1.0f);
        uiVolSlider->LabelWidth = maxLabelWidth;
        uiVolSlider->OnValueChanged = [this](float val) {
            static auto lastClickTime = std::chrono::steady_clock::now() - std::chrono::milliseconds(500);
            Settings::Save(m_settingsPath);
            NexusSDK::Audio->SetChannelVolume("UI", val);
            
            auto now = std::chrono::steady_clock::now();
            if (std::chrono::duration_cast<std::chrono::milliseconds>(now - lastClickTime).count() > 100) {
                NexusSDK::Audio->Play(IDR_AUDIO_BUTTON_CLICK);
                lastClickTime = now;
            }
        };
        flowPanel->AddChild(uiVolSlider);

        // Screen Volume Slider
        auto screenVolSlider = std::make_shared<NexusSDK::UI::Slider>(local->GetString("Settings_ScreenVolume"), &Settings::ScreenVolume, 0.0f, 1.0f);
        screenVolSlider->LabelWidth = maxLabelWidth;
        screenVolSlider->OnValueChanged = [this](float val) {
            static auto lastSekiroTime = std::chrono::steady_clock::now() - std::chrono::milliseconds(1000);
            Settings::Save(m_settingsPath);
            NexusSDK::Audio->SetChannelVolume("Screen", val);
            
            auto now = std::chrono::steady_clock::now();
            if (std::chrono::duration_cast<std::chrono::milliseconds>(now - lastSekiroTime).count() > 300) {
                NexusSDK::Audio->Play(IDR_SFX_SEKIRO, "Screen");
                lastSekiroTime = now;
            }
        };
        flowPanel->AddChild(screenVolSlider);


    }
}
