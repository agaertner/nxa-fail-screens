#ifndef SETTINGSUI_H
#define SETTINGSUI_H

#include <filesystem>
#include <functional>

namespace Nekres {
    class SettingsUI {
    public:
        SettingsUI(const std::filesystem::path& settingsPath);
        
        using OnPreviewCallback = std::function<void()>;
        using OnStopCallback = std::function<void()>;

        void SetCallbacks(OnPreviewCallback previewCb, OnStopCallback stopCb);
        
        void Draw();

    private:
        std::filesystem::path m_settingsPath;
        OnPreviewCallback m_onPreview;
        OnStopCallback m_onStop;
    };
}

#endif // SETTINGSUI_H
