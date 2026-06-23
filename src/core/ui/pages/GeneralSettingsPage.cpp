#include "GeneralSettingsPage.h"
#include <lib-nxa-sdk/src/ui/controls/Label.h>
#include <lib-nxa-sdk/src/ui/controls/Button.h>
#include "../../Settings.h"
#include "../../services/Services.h"
#include <lib-nxa-sdk/NexusSDK.h>
#include <imgui/imgui.h>

namespace Nekres {

    // We don't need ImGuiControl anymore; we will use standard OOP layouts.

    GeneralSettingsPage::GeneralSettingsPage(const std::filesystem::path& settingsPath)
        : Container(), m_settingsPath(settingsPath)
    {
        auto flowPanel = std::make_shared<NexusSDK::UI::FlowPanel>();
        flowPanel->ControlFlowDirection = NexusSDK::UI::FlowDirection::TopToBottom;
        flowPanel->ControlPadding = 8.0f;
        AddChild(flowPanel);

        auto local = NexusSDK::Local;

        // Screen Dropdown + Preview Button
        std::vector<std::string> screens = {
            local->GetString("Screen_DarkSouls"),
            local->GetString("Screen_GrandTheftAuto"),
            local->GetString("Screen_RytlocksCritterRampage"),
            local->GetString("Screen_AngryPepe"),
            local->GetString("Screen_Sekiro"),
            local->GetString("Screen_WinXP")
        };

        auto combo = std::make_shared<NexusSDK::UI::Dropdown>("DefeatedScreen", &Settings::ActiveScreen, screens, local->GetString("Settings_DefeatedScreen").c_str());
        combo->SetSize(350.0f, 0); // Total width
        combo->OnSelectionChanged = [this](int val) {
            Settings::ActiveScreen = val;
            Settings::Save(m_settingsPath);
            if (m_onStop) m_onStop();
        };

        auto previewBtn = std::make_shared<NexusSDK::UI::Button>("PreviewBtn", local->GetString("Settings_Preview"));
        previewBtn->OnClick = [this]() {
            if (m_onPreview) m_onPreview();
        };
        previewBtn->SetPosition(0.0f, -2.0f); // align vertically with dropdown

        auto comboAndPreview = std::make_shared<NexusSDK::UI::FlowPanel>();
        comboAndPreview->ControlFlowDirection = NexusSDK::UI::FlowDirection::LeftToRight;
        comboAndPreview->ControlPadding = 8.0f;
        comboAndPreview->SetSize(0, 30); // Approximate height so FlowPanel tracks it
        comboAndPreview->AddChild(combo);
        comboAndPreview->AddChild(previewBtn);
        
        flowPanel->AddChild(comboAndPreview);

        // We can use a spacer or label with dashes instead of ImGui::Separator for pure OOP, but
        // for now a simple transparent panel with spacing works.
        auto spacer1 = std::make_shared<NexusSDK::UI::ControlBase>();
        spacer1->SetSize(0, 10);
        flowPanel->AddChild(spacer1);

        // Randomize Screen block
        auto randCheck = std::make_shared<NexusSDK::UI::Checkbox>(local->GetString("Settings_Randomize"), &Settings::Randomize);
        randCheck->OnCheckedChanged = [this](bool val) {
            Settings::Save(m_settingsPath);
        };
        flowPanel->AddChild(randCheck);

        auto checkDS = std::make_shared<NexusSDK::UI::Checkbox>(local->GetString("Screen_DarkSouls"), &Settings::EnableDarkSouls);
        checkDS->OnCheckedChanged = [this](bool val) { Settings::Save(m_settingsPath); };
        
        auto checkGTA = std::make_shared<NexusSDK::UI::Checkbox>(local->GetString("Screen_GrandTheftAuto"), &Settings::EnableGrandTheftAuto);
        checkGTA->OnCheckedChanged = [this](bool val) { Settings::Save(m_settingsPath); };

        auto checkRyt = std::make_shared<NexusSDK::UI::Checkbox>(local->GetString("Screen_RytlocksCritterRampage"), &Settings::EnableRytlocksCritterRampage);
        checkRyt->OnCheckedChanged = [this](bool val) { Settings::Save(m_settingsPath); };

        auto checkPepe = std::make_shared<NexusSDK::UI::Checkbox>(local->GetString("Screen_AngryPepe"), &Settings::EnableAngryPepe);
        checkPepe->OnCheckedChanged = [this](bool val) { Settings::Save(m_settingsPath); };

        auto checkSek = std::make_shared<NexusSDK::UI::Checkbox>(local->GetString("Screen_Sekiro"), &Settings::EnableSekiro);
        checkSek->OnCheckedChanged = [this](bool val) { Settings::Save(m_settingsPath); };

        auto checkWinXp = std::make_shared<NexusSDK::UI::Checkbox>(local->GetString("Screen_WinXP"), &Settings::EnableWinXp);
        checkWinXp->OnCheckedChanged = [this](bool val) { Settings::Save(m_settingsPath); };

        auto subGroup = std::make_shared<NexusSDK::UI::FlowPanel>();
        subGroup->ControlFlowDirection = NexusSDK::UI::FlowDirection::TopToBottom;
        subGroup->ControlPadding = 0.0f;
        subGroup->SetPosition(20.0f, 0.0f); // Indent by 20px
        
        auto subLabel = std::make_shared<NexusSDK::UI::Label>(local->GetString("Settings_IncludeInRandomizer"));
        subLabel->WrapText = false;
        subGroup->AddChild(subLabel);
        subGroup->AddChild(checkDS);
        subGroup->AddChild(checkGTA);
        subGroup->AddChild(checkRyt);
        subGroup->AddChild(checkPepe);
        subGroup->AddChild(checkSek);
        subGroup->AddChild(checkWinXp);

        // Apply initial visibility
        subGroup->SetVisible(Settings::Randomize);

        randCheck->OnCheckedChanged = [this, subGroup](bool val) {
            Settings::Save(m_settingsPath);
            subGroup->SetVisible(val);
        };

        flowPanel->AddChild(subGroup);
    }

    void GeneralSettingsPage::SetCallbacks(std::function<void()> previewCb, std::function<void()> stopCb)
    {
        m_onPreview = std::move(previewCb);
        m_onStop = std::move(stopCb);
    }
}
