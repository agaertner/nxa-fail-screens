#ifndef RYTLOCKSCRITTERRAMPAGE_H
#define RYTLOCKSCRITTERRAMPAGE_H

#include "FailScreen.h"
#include "../utils/AnimationCurve.h"

namespace Nekres {
    class RytlocksCritterRampage : public FailScreen {
    public:
        RytlocksCritterRampage(AddonAPI_t* api, HMODULE moduleHandle);
        ~RytlocksCritterRampage() override = default;

        void OnDeath(int language) override;
        bool DrawContent(ImDrawList* drawList, float timeSinceDeath, ImVec2 screenSize, float finalScale) override;

    private:
        AnimationCurve m_spriteOpacityCurve;
        AnimationCurve m_bgOpacityCurve;
    };
}

#endif // RYTLOCKSCRITTERRAMPAGE_H
