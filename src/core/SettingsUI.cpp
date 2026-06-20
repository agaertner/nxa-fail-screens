#include "SettingsUI.h"
#include "Settings.h"
#include "services/Services.h"
#include <imgui/imgui.h>

namespace Nekres {

    SettingsUI::SettingsUI(const std::filesystem::path& settingsPath)
        : m_settingsPath(settingsPath)
    {
    }

    void SettingsUI::SetCallbacks(OnPreviewCallback previewCb, OnStopCallback stopCb)
    {
        m_onPreview = std::move(previewCb);
        m_onStop = std::move(stopCb);
    }

    void SettingsUI::Draw()
    {
        ImGui::Text("Defeated Screen:");
        const char* screens[] = { "Dark Souls", "Grand Theft Auto", "Rytlock's Critter Rampage", "Angry Pepe", "Sekiro", "Win XP" };
        
        ImGui::SetNextItemWidth(200.0f);
        if (ImGui::Combo("##DefeatedScreen", &Settings::ActiveScreen, screens, IM_ARRAYSIZE(screens))) {
            Settings::Save(m_settingsPath);
            if (m_onStop) m_onStop();
        }

        ImGui::SameLine();

        if (ImGui::Button("Preview")) {
            if (m_onPreview) m_onPreview();
        }
        
        ImGui::Separator();
        
        if (ImGui::Checkbox("Randomize Screen", &Settings::Randomize)) {
            Settings::Save(m_settingsPath);
        }

        if (Settings::Randomize) {
            ImGui::Indent();
            ImGui::Text("Include in Randomizer:");
            bool changed = false;
            changed |= ImGui::Checkbox("Dark Souls", &Settings::EnableDarkSouls);
            changed |= ImGui::Checkbox("Grand Theft Auto", &Settings::EnableGrandTheftAuto);
            changed |= ImGui::Checkbox("Rytlock's Critter Rampage", &Settings::EnableRytlocksCritterRampage);
            changed |= ImGui::Checkbox("Angry Pepe", &Settings::EnableAngryPepe);
            changed |= ImGui::Checkbox("Sekiro", &Settings::EnableSekiro);
            changed |= ImGui::Checkbox("Win XP", &Settings::EnableWinXp);
            
            if (changed) {
                Settings::Save(m_settingsPath);
            }
            ImGui::Unindent();
        }

        ImGui::Separator();
        
        if (ImGui::SliderFloat("Volume", &Settings::Volume, 0.0f, 1.0f, "%.2f")) {
            Settings::Save(m_settingsPath);
            if (Services::m_audio) {
                Services::m_audio->UpdateVolume();
            }
        }
    }
}
