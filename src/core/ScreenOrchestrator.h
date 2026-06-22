#ifndef SCREENORCHESTRATOR_H
#define SCREENORCHESTRATOR_H

#include <memory>
#include <vector>
#include "../Defines.h"
#include "screens/FailScreen.h"
#include <lib-nxa-sdk/NexusSDK.h>

namespace Nekres {
    class ScreenOrchestrator {
    public:
        ScreenOrchestrator(AddonAPI_t* api, HMODULE hSelf);
        ~ScreenOrchestrator() = default;

        void Render();
        void TriggerPreview();
        void TriggerDeath(int language);
        void StopAll();

        bool IsPreviewing() const { return m_isPreviewing; }
        void EndPreview() { m_isPreviewing = false; }
        bool IsPlayingAnimation() const { return m_isPlayingAnimation; }

    private:
        AddonAPI_t* m_api;
        HMODULE m_hSelf;

        std::unique_ptr<FailScreen> m_activeScreen;
        std::vector<std::unique_ptr<NexusSDK::AsyncFont>> m_preloadedFonts;

        bool m_isPlayingAnimation = false;
        bool m_isPreviewing = false;
        float m_timeOfDeath = 0.0f;

        std::unique_ptr<FailScreen> CreateFailScreen(int screenIndex);
    };
}

#endif // SCREENORCHESTRATOR_H
