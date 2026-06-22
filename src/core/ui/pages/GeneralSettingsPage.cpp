#include "GeneralSettingsPage.h"
#include <lib-nxa-sdk/src/ui/controls/Label.h>
#include <lib-nxa-sdk/src/ui/controls/Button.h>
#include "../../Settings.h"
#include "../../services/Services.h"
#include <lib-nxa-sdk/NexusSDK.h>
#include <imgui/imgui.h>

namespace Nekres {

    class ImGuiControl : public NexusSDK::UI::ControlBase {
    public:
        std::function<void()> DrawFn;
        ImGuiControl(std::function<void()> drawFn) : DrawFn(drawFn) {}
    protected:
        void OnRender() override {
            if (DrawFn) DrawFn();
        }
    };

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
        combo->SetSize(ImVec2(220, 0));
        combo->OnSelectionChanged = [this](int val) {
            Settings::ActiveScreen = val;
            Settings::Save(m_settingsPath);
            if (m_onStop) m_onStop();
        };

        auto previewBtn = std::make_shared<NexusSDK::UI::Button>("PreviewBtn", local->GetString("Settings_Preview"));
        previewBtn->OnClick = [this]() {
            if (m_onPreview) m_onPreview();
        };

        auto comboAndPreview = std::make_shared<ImGuiControl>([combo, previewBtn]() {
            combo->Render();
            ImGui::SameLine(0.0f, 8.0f);
            
            // Adjust Preview button vertical position to align with the dropdown box
            float currentY = ImGui::GetCursorPosY();
            ImGui::SetCursorPosY(currentY - 2.0f); // align vertically with dropdown
            previewBtn->Render();
        });
        
        flowPanel->AddChild(comboAndPreview);

        flowPanel->AddChild(std::make_shared<ImGuiControl>([]() { ImGui::Separator(); }));

        // Randomize Screen block
        auto randCheck = std::make_shared<NexusSDK::UI::Checkbox>(local->GetString("Settings_Randomize"), &Settings::Randomize);
        randCheck->OnCheckedChanged = [this](bool val) {
            Settings::Save(m_settingsPath);
        };

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

        auto randGroup = std::make_shared<ImGuiControl>([local, randCheck, checkDS, checkGTA, checkRyt, checkPepe, checkSek, checkWinXp]() {
            randCheck->Render();

            if (Settings::Randomize) {
                ImGui::Indent();
                ImGui::TextUnformatted(local->GetString("Settings_IncludeInRandomizer").c_str());
                
                checkDS->Render();
                checkGTA->Render();
                checkRyt->Render();
                checkPepe->Render();
                checkSek->Render();
                checkWinXp->Render();
                
                ImGui::Unindent();
            }
        });
        flowPanel->AddChild(randGroup);
    }

    void GeneralSettingsPage::SetCallbacks(std::function<void()> previewCb, std::function<void()> stopCb)
    {
        m_onPreview = std::move(previewCb);
        m_onStop = std::move(stopCb);
    }
}
