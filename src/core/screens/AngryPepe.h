#ifndef ANGRYPEPE_H
#define ANGRYPEPE_H

#include "FailScreen.h"
#include "../utils/AnimationCurve.h"

namespace Nekres {
    class AngryPepe : public FailScreen {
    public:
        AngryPepe(AddonAPI_t* api, HMODULE moduleHandle);
        ~AngryPepe() override = default;

        void OnDeath(int language) override;
        bool DrawContent(ImDrawList* drawList, float timeSinceDeath, ImVec2 screenSize, float finalScale) override;

    private:
        // Single _ragePercent tween 0->1 over 3.5s
        AnimationCurve m_rageCurve;
    };
}

#endif // ANGRYPEPE_H
