#ifndef DARKSOULS_H
#define DARKSOULS_H

#include "FailScreen.h"
#include "../utils/AnimationCurve.h"

namespace Nekres {
    class DarkSouls : public FailScreen {
    public:
        DarkSouls(AddonAPI_t* api, HMODULE moduleHandle);
        ~DarkSouls() override = default;

        void OnDeath(int language) override;
        bool DrawContent(ImDrawList* drawList, float timeSinceDeath, ImVec2 screenSize, float finalScale) override;

    private:
        AnimationCurve m_bgOpacityCurve;
        AnimationCurve m_textOpacityCurve;
        AnimationCurve m_textScaleCurve;

        std::string m_resourceID;
    };
}

#endif // DARKSOULS_H
