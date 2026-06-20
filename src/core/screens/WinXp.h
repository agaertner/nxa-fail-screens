#ifndef WINXP_H
#define WINXP_H

#include "FailScreen.h"
#include "../utils/AnimationCurve.h"
#include <vector>

namespace Nekres {

    struct WindowState {
        float OffsetX;
        float OffsetY;
    };

    class WinXp : public FailScreen {
    public:
        WinXp(AddonAPI_t* api, HMODULE moduleHandle);
        ~WinXp() override = default;

        void OnDeath(int language) override;
        bool DrawContent(ImDrawList* drawList, float timeSinceDeath, ImVec2 screenSize, float finalScale) override;

    private:
        int m_language = 0;
        std::vector<WindowState> m_windows;
        float m_blueScreenOpacity = 0.0f;
        bool m_hideBoxes = false;
        bool m_playedFinalSound = false;

        AnimationCurve m_blueScreenCurve;
    };
}

#endif // WINXP_H
