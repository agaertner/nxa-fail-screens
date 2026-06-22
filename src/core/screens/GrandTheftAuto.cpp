#include "GrandTheftAuto.h"


namespace Nekres {

    GrandTheftAuto::GrandTheftAuto(AddonAPI_t* api, HMODULE moduleHandle)
        : FailScreen(api, moduleHandle)
    {
        // Photo vignette: 0->0.6 over 0.15s, hold 0.05s, reflect back
        m_photoOpacityCurve.AddKeyframe(0.0f, 0.0f);
        m_photoOpacityCurve.AddKeyframe(0.15f, 0.6f);
        m_photoOpacityCurve.AddKeyframe(0.20f, 0.6f);
        m_photoOpacityCurve.AddKeyframe(0.35f, 0.0f);

        // Flash vignette: starts at 2.25s, same 0->0.6->0 pattern
        m_flashOpacityCurve.AddKeyframe(0.0f, 0.0f);
        m_flashOpacityCurve.AddKeyframe(2.25f, 0.0f);
        m_flashOpacityCurve.AddKeyframe(2.40f, 0.6f);
        m_flashOpacityCurve.AddKeyframe(2.45f, 0.6f);
        m_flashOpacityCurve.AddKeyframe(2.60f, 0.0f);

        // Wasted text: near-instant appear at ~2.40s, fade out with background
        m_textOpacityCurve.AddKeyframe(0.0f, 0.0f);
        m_textOpacityCurve.AddKeyframe(2.40f, 0.0f);
        m_textOpacityCurve.AddKeyframe(2.408f, 1.0f);
        m_textOpacityCurve.AddKeyframe(7.0f, 1.0f);
        m_textOpacityCurve.AddKeyframe(8.0f, 0.0f);

        // Background opacity: 0->1 over 1s, hold 6s, reflect back over 1s
        m_bgOpacityCurve.AddKeyframe(0.0f, 0.0f);
        m_bgOpacityCurve.AddKeyframe(1.0f, 1.0f);
        m_bgOpacityCurve.AddKeyframe(7.0f, 1.0f);
        m_bgOpacityCurve.AddKeyframe(8.0f, 0.0f);
    }

    void GrandTheftAuto::OnDeath(int language)
    {
        m_resourceID = IDB_TEX_GTA_EN;
        switch (language) {
            case 2: m_resourceID = IDB_TEX_GTA_FR; break;
            case 3: m_resourceID = IDB_TEX_GTA_DE; break;
            case 4: m_resourceID = IDB_TEX_GTA_ES; break;
            default: break;
        }



        PlaySoundEffect(IDR_SFX_GTA);
    }

    bool GrandTheftAuto::DrawContent(ImDrawList* drawList, float timeSinceDeath, ImVec2 screenSize, float finalScale)
    {
        if (timeSinceDeath > 8.0f) {
            return false;
        }

        float bgOpacity = m_bgOpacityCurve.Evaluate(timeSinceDeath);
        float photoOpacity = m_photoOpacityCurve.Evaluate(timeSinceDeath);
        float flashOpacity = m_flashOpacityCurve.Evaluate(timeSinceDeath);
        float textOpacity = m_textOpacityCurve.Evaluate(timeSinceDeath);

        float tr = 149.0f/255.0f, tg = 31.0f/255.0f, tb = 32.0f/255.0f;

        ImU32 bgCol32 = ImGui::GetColorU32(ImVec4(0.0f, 0.0f, 0.0f, bgOpacity * 0.7f));

        ImVec2 p_min = ImVec2(0, 0);
        ImVec2 p_max = screenSize;

        // Draw background
        drawList->AddRectFilled(p_min, p_max, bgCol32);

        // Draw photo vignette
        Texture_t* photoTex = NexusSDK::Content->GetTexture(IDB_TEX_GTA_PHOTO);
        if (photoTex && photoTex->Resource && photoOpacity > 0.001f) {
            ImU32 photoCol = ImGui::GetColorU32(ImVec4(tr, tg, tb, photoOpacity));
            drawList->AddImage((void*)photoTex->Resource, p_min, p_max, ImVec2(0,0), ImVec2(1,1), photoCol);
        }

        // Draw flash vignette
        Texture_t* flashTex = NexusSDK::Content->GetTexture(IDB_TEX_GTA_FLASH);
        if (flashTex && flashTex->Resource && flashOpacity > 0.001f) {
            ImU32 flashCol = ImGui::GetColorU32(ImVec4(tr, tg, tb, flashOpacity));
            drawList->AddImage((void*)flashTex->Resource, p_min, p_max, ImVec2(0,0), ImVec2(1,1), flashCol);
        }

        // Draw wasted text at 1.2x scale, adjusted by UI scale
        Texture_t* wastedTex = NexusSDK::Content->GetTexture(m_resourceID);
        if (wastedTex && wastedTex->Resource && textOpacity > 0.001f) {
            float w = wastedTex->Width * 1.2f * finalScale;
            float h = wastedTex->Height * 1.2f * finalScale;

            float x = (screenSize.x - w) * 0.5f;
            float y = (screenSize.y - h) * 0.5f;

            ImU32 textCol = ImGui::GetColorU32(ImVec4(tr, tg, tb, textOpacity));
            drawList->AddImage((void*)wastedTex->Resource, ImVec2(x, y), ImVec2(x + w, y + h), ImVec2(0,0), ImVec2(1,1), textCol);
        }

        return true;
    }
}
