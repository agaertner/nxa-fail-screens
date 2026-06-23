#ifndef FOOTER_H
#define FOOTER_H

#include <lib-nxa-sdk/NexusSDK.h>
#include "../../Defines.h"

namespace Nekres::UI {
    class Footer : public NexusSDK::UI::ControlBase {
    public:
        Footer(AddonDefinition_t* addonDef);
        virtual ~Footer() = default;

    protected:
        virtual void OnDraw(const NexusSDK::UI::Rectangle& bounds, float scale) override;

    private:
        AddonDefinition_t* m_addonDef;
        void DrawStatusIndicator(const char* label, bool detected, bool active);
    };
}

#endif
