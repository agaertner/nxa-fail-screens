#include "DarkSouls.h"
#include "../../resource.h"

namespace Nekres {

    DarkSouls::DarkSouls(AddonAPI_t* api, HMODULE moduleHandle)
        : FailScreen(api, moduleHandle)
    {
        // Text opacity: 0->1 over 2s, hold 3s, reflect back over 2s
        m_textOpacityCurve.AddKeyframe(0.0f, 0.0f);
        m_textOpacityCurve.AddKeyframe(2.0f, 1.0f);
        m_textOpacityCurve.AddKeyframe(5.0f, 1.0f);
        m_textOpacityCurve.AddKeyframe(7.0f, 0.0f);

        // Text scale: 1->1.7 over 7s
        m_textScaleCurve.AddKeyframe(0.0f, 1.0f);
        m_textScaleCurve.AddKeyframe(7.0f, 1.7f);

        // Background opacity: 0->1 over 1s, hold 6s, reflect back over 1s
        m_bgOpacityCurve.AddKeyframe(0.0f, 0.0f);
        m_bgOpacityCurve.AddKeyframe(1.0f, 1.0f);
        m_bgOpacityCurve.AddKeyframe(7.0f, 1.0f);
        m_bgOpacityCurve.AddKeyframe(8.0f, 0.0f);
    }

    void DarkSouls::OnDeath(int language)
    {
        int resourceID = IDR_TEX_DARKSOULS_EN;
        m_texId = "TexDarkSouls_EN";
        switch (language) {
            case 2: resourceID = IDR_TEX_DARKSOULS_FR; m_texId = "TexDarkSouls_FR"; break;
            case 3: resourceID = IDR_TEX_DARKSOULS_DE; m_texId = "TexDarkSouls_DE"; break;
            case 4: resourceID = IDR_TEX_DARKSOULS_ES; m_texId = "TexDarkSouls_ES"; break;
            default: break;
        }
        
        EnsureTexture(m_texId, resourceID);
        PlaySoundEffect(IDR_WAV_DARKSOULS);
    }

    bool DarkSouls::DrawContent(ImDrawList* drawList, float timeSinceDeath, ImVec2 screenSize, float finalScale)
    {
        if (timeSinceDeath > 8.0f) {
            return false;
        }

        float bgOpacity = m_bgOpacityCurve.Evaluate(timeSinceDeath);
        float textOpacity = m_textOpacityCurve.Evaluate(timeSinceDeath);
        float textScale = m_textScaleCurve.Evaluate(timeSinceDeath) * finalScale;

        ImU32 bgCol32 = ImGui::GetColorU32(ImVec4(0.0f, 0.0f, 0.0f, bgOpacity * 0.7f));
        ImU32 textCol32 = ImGui::GetColorU32(ImVec4(149.0f/255.0f, 31.0f/255.0f, 32.0f/255.0f, textOpacity));

        // Draw background
        drawList->AddRectFilled(ImVec2(0, 0), screenSize, bgCol32);

        // Draw text texture
        Texture_t* tex = GetTexture(m_texId);
        if (tex && tex->Resource) {
            float w = tex->Width * textScale;
            float h = tex->Height * textScale;

            float x = (screenSize.x - w) * 0.5f;
            float y = (screenSize.y - h) * 0.5f;

            drawList->AddImage((void*)tex->Resource, ImVec2(x, y), ImVec2(x + w, y + h), ImVec2(0,0), ImVec2(1,1), textCol32);
        }

        return true;
    }
}
