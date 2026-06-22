#ifndef GRANDTHEFTAUTO_H
#define GRANDTHEFTAUTO_H

#include "FailScreen.h"
#include "../utils/AnimationCurve.h"

namespace Nekres {
    class GrandTheftAuto : public FailScreen {
    public:
        GrandTheftAuto(AddonAPI_t* api, HMODULE moduleHandle);
        ~GrandTheftAuto() override = default;

        void OnDeath(int language) override;
        bool DrawContent(ImDrawList* drawList, float timeSinceDeath, ImVec2 screenSize, float finalScale) override;

    private:
        AnimationCurve m_bgOpacityCurve;
        AnimationCurve m_photoOpacityCurve;
        AnimationCurve m_flashOpacityCurve;
        AnimationCurve m_textOpacityCurve;

        std::string m_resourceID;
    };
}

#endif // GRANDTHEFTAUTO_H
