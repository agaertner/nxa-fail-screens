#ifndef SETTINGSUI_H
#define SETTINGSUI_H

#include <filesystem>
#include <vector>
#include <memory>
#include <functional>
#include "../Defines.h"
#include <lib-nxa-sdk/NexusSDK.h>
#include "ui/Footer.h"
#include "ui/pages/GeneralSettingsPage.h"
#include "ui/pages/AudioSettingsPage.h"

namespace Nekres {

    class ContentArea : public NexusSDK::UI::Container {
    public:
        ContentArea() : NexusSDK::UI::Container() {}
        virtual ~ContentArea() = default;
        std::string HeaderTitle;
    protected:
        virtual void OnDraw(const NexusSDK::UI::Rectangle& bounds, float scale) override;
    };

    class SettingsUI : public NexusSDK::UI::Container {
    public:
        SettingsUI(const std::filesystem::path& settingsPath, AddonDefinition_t* addonDef);
        virtual ~SettingsUI() = default;

        void SetCallbacks(std::function<void()> previewCb, std::function<void()> stopCb);

    protected:
        virtual void OnDraw(const NexusSDK::UI::Rectangle& bounds, float scale) override;

    private:
        std::filesystem::path m_settingsPath;
        AddonDefinition_t* m_addonDef;

        std::shared_ptr<ContentArea> m_contentPanel;
        std::shared_ptr<UI::Footer> m_footer;
        std::shared_ptr<NexusSDK::UI::Menu> m_sidebarMenu;
        std::shared_ptr<NexusSDK::UI::FlowPanel> m_mainBody;
        
        std::shared_ptr<GeneralSettingsPage> m_generalPage;
        std::shared_ptr<AudioSettingsPage> m_audioPage;
    };
}

#endif // SETTINGSUI_H
