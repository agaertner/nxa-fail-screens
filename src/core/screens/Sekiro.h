#ifndef SEKIRO_H
#define SEKIRO_H

#include "FailScreen.h"
#include "../utils/AnimationCurve.h"

namespace Nekres {
    class Sekiro : public FailScreen {
    public:
        Sekiro(AddonAPI_t* api, HMODULE moduleHandle);
        ~Sekiro() override = default;

        void OnDeath(int language) override;
        bool DrawContent(ImDrawList* drawList, float timeSinceDeath, ImVec2 screenSize, float finalScale) override;

    private:
        std::string m_text;

        AnimationCurve m_bgOpacityCurve;
        AnimationCurve m_flashScaleCurve;
        AnimationCurve m_flashOpacityCurve;
        AnimationCurve m_textOpacityCurve;
    };
}

#endif // SEKIRO_H
