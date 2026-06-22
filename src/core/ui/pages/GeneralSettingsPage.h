#ifndef GENERALSETTINGSPAGE_H
#define GENERALSETTINGSPAGE_H

#include <lib-nxa-sdk/NexusSDK.h>
#include <filesystem>
#include <functional>

namespace Nekres {
    class GeneralSettingsPage : public NexusSDK::UI::Container {
    public:
        GeneralSettingsPage(const std::filesystem::path& settingsPath);
        virtual ~GeneralSettingsPage() = default;

        void SetCallbacks(std::function<void()> previewCb, std::function<void()> stopCb);

    private:
        std::filesystem::path m_settingsPath;
        std::function<void()> m_onPreview;
        std::function<void()> m_onStop;
    };
}

#endif
