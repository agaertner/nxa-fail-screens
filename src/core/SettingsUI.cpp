#include "SettingsUI.h"
#include "Settings.h"
#include "services/Services.h"
#include "ui/Theme.h"
#include <imgui/imgui.h>

namespace Nekres {

    void ContentArea::OnDraw(const NexusSDK::UI::Rectangle& bounds, float scale) {
        ImGui::PushStyleColor(ImGuiCol_ChildBg, UI::Theme::BackgroundContent);
        ImVec2 padding = ImGui::GetStyle().WindowPadding;
        padding.x += 5.0f * scale;
        padding.y += 5.0f * scale;
        ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, padding);
        
        ImGui::SetCursorScreenPos(bounds.GetMin());
        bool isVisible = ImGui::BeginChild(m_id.c_str(), ImVec2(bounds.Width, bounds.Height), true);
        ImGui::PopStyleVar();
        ImGui::PopStyleColor();

        if (isVisible) {
            if (!HeaderTitle.empty()) {
                ImGui::TextColored(UI::Theme::Accent, HeaderTitle.c_str());
                ImGui::Separator();
                ImGui::Spacing();
            }

            ImGui::PushStyleColor(ImGuiCol_Header, UI::Theme::HeaderColor);
            ImGui::PushStyleColor(ImGuiCol_HeaderHovered, UI::Theme::AccentHover);
            ImGui::PushStyleColor(ImGuiCol_HeaderActive, UI::Theme::Accent);

            NexusSDK::UI::Rectangle clientBounds = bounds;
            ImVec2 scrolledPos = ImGui::GetCursorScreenPos();
            clientBounds.X = scrolledPos.x;
            clientBounds.Y = scrolledPos.y;

            DrawChildren(clientBounds, scale);

            ImGui::PopStyleColor(3);
        }
        ImGui::EndChild();
    }

    SettingsUI::SettingsUI(const std::filesystem::path& settingsPath, AddonDefinition_t* addonDef)
        : Container(), m_settingsPath(settingsPath), m_addonDef(addonDef)
    {

        m_mainBody = std::make_shared<NexusSDK::UI::FlowPanel>();
        m_mainBody->ControlFlowDirection = NexusSDK::UI::FlowDirection::LeftToRight;
        m_mainBody->ControlPadding = 0.0f;

        m_contentPanel = std::make_shared<ContentArea>();
        m_footer = std::make_shared<UI::Footer>(m_addonDef);
        
        m_sidebarMenu = std::make_shared<NexusSDK::UI::Menu>();
        m_sidebarMenu->AccentColor = UI::Theme::Accent;
        m_sidebarMenu->TextUnselectedColor = UI::Theme::TextUnselected;
        m_sidebarMenu->BackgroundColor = UI::Theme::BackgroundSidebar;
        m_sidebarMenu->HeaderTitle = m_addonDef->Name;
        m_sidebarMenu->HeaderSubtitle = std::string("by ") + m_addonDef->Author;
        
        m_mainBody->AddChild(m_sidebarMenu);
        m_mainBody->AddChild(m_contentPanel);

        AddChild(m_mainBody);
        AddChild(m_footer);
        
        m_generalPage = std::make_shared<GeneralSettingsPage>(m_settingsPath);
        m_audioPage = std::make_shared<AudioSettingsPage>(m_settingsPath);
        
        m_sidebarMenu->AddTab({"General", "General Options", m_generalPage, nullptr});
        m_sidebarMenu->AddTab({"Sound", "Sound Options", m_audioPage, nullptr});
        
        m_sidebarMenu->OnSelectionChanged = [this](int index) {
            m_contentPanel->ClearChildren();
            m_contentPanel->HeaderTitle = m_sidebarMenu->Tabs[index].Title;
            m_contentPanel->AddChild(m_sidebarMenu->Tabs[index].PageControl);
        };

        // Build first tab by default
        if (!m_sidebarMenu->Tabs.empty()) {
            m_sidebarMenu->SelectedIndex = 0;
            m_contentPanel->HeaderTitle = m_sidebarMenu->Tabs[0].Title;
            m_contentPanel->AddChild(m_sidebarMenu->Tabs[0].PageControl);
        }
    }

    void SettingsUI::SetCallbacks(std::function<void()> previewCb, std::function<void()> stopCb)
    {
        m_generalPage->SetCallbacks(previewCb, stopCb);
    }

    void SettingsUI::OnDraw(const NexusSDK::UI::Rectangle& bounds, float scale)
    {
        ImFont* fontUI = nullptr;
        if (Services::m_nexus && Services::m_nexus->Core()) {
            fontUI = (ImFont*)Services::m_nexus->Core()->FontUI;
        }

        if (fontUI) {
            ImGui::PushFont(fontUI);
        }

        float footerHeight = (ImGui::GetTextLineHeightWithSpacing() + ImGui::GetStyle().ItemSpacing.y * 3.0f + 5.0f) * scale;

        m_sidebarMenu->SetSize(140, 0);

        ImGui::SetCursorScreenPos(bounds.GetMin());
        if (ImGui::BeginChild(m_id.c_str(), ImVec2(bounds.Width, bounds.Height), false, ImGuiWindowFlags_NoBackground)) {
            NexusSDK::UI::Rectangle clientBounds;
            clientBounds.X = ImGui::GetCursorScreenPos().x;
            clientBounds.Y = ImGui::GetCursorScreenPos().y;
            clientBounds.Width = bounds.Width;
            clientBounds.Height = bounds.Height;

            NexusSDK::UI::Rectangle bodyBounds = clientBounds;
            bodyBounds.Height = clientBounds.Height - footerHeight;
            m_mainBody->Draw(bodyBounds, scale);

            NexusSDK::UI::Rectangle footerBounds = clientBounds;
            footerBounds.Y = clientBounds.Y + clientBounds.Height - footerHeight;
            footerBounds.Height = footerHeight;
            m_footer->Draw(footerBounds, scale);
        }
        ImGui::EndChild();

        if (fontUI) {
            ImGui::PopFont();
        }
    }
}
