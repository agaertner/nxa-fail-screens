#ifndef AUDIOSETTINGSPAGE_H
#define AUDIOSETTINGSPAGE_H

#include <lib-nxa-sdk/NexusSDK.h>
#include <filesystem>

namespace Nekres {
    class AudioSettingsPage : public NexusSDK::UI::Container {
    public:
        AudioSettingsPage(const std::filesystem::path& settingsPath);
        virtual ~AudioSettingsPage() = default;

    private:
        std::filesystem::path m_settingsPath;
    };
}

#endif
