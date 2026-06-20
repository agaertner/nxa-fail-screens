#include "Sekiro.h"
#include "../../resource.h"
#include <cstdlib>

namespace Nekres {

    Sekiro::Sekiro(AddonAPI_t* api, HMODULE moduleHandle)
        : FailScreen(api, moduleHandle)
    {
        // Background opacity: 0->1 over 1s, hold 5s, reflect back over 1s
        m_bgOpacityCurve.AddKeyframe(0.0f, 0.0f);
        m_bgOpacityCurve.AddKeyframe(1.0f, 1.0f);
        m_bgOpacityCurve.AddKeyframe(6.0f, 1.0f);
        m_bgOpacityCurve.AddKeyframe(7.0f, 0.0f);

        // Flash scale: 1->1.05 over 0.08s, then reflect back
        m_flashScaleCurve.AddKeyframe(0.0f, 1.0f);
        m_flashScaleCurve.AddKeyframe(0.08f, 1.05f);
        m_flashScaleCurve.AddKeyframe(0.16f, 1.0f);

        // Flash opacity: appear at 0.08s, fade to 0 over 2s
        m_flashOpacityCurve.AddKeyframe(0.0f, 0.0f);
        m_flashOpacityCurve.AddKeyframe(0.08f, 1.0f);
        m_flashOpacityCurve.AddKeyframe(2.08f, 0.0f);

        // Text opacity: appear at 0.08s, hold until 4s, then fade over 2s
        m_textOpacityCurve.AddKeyframe(0.0f, 0.0f);
        m_textOpacityCurve.AddKeyframe(0.08f, 1.0f);
        m_textOpacityCurve.AddKeyframe(4.0f, 1.0f);
        m_textOpacityCurve.AddKeyframe(6.0f, 0.0f);
    }

    void Sekiro::OnDeath(int language)
    {
        EnsureTexture("TexSekiro", IDR_TEX_SEKIRO);
        EnsureTexture("TexSekiroOut", IDR_TEX_SEKIRO_OUT);
        EnsureFont("FontAthelas_Native250", IDR_FONT_ATHELAS, 250.0f, m_api, m_hSelf);

        std::string deathStr = Services::Local()->GetString("Sekiro_Death", language);

        int rnd = rand() % 3;
        if (rnd == 0) m_text = deathStr;
        else if (rnd == 1) m_text = "Y E E T";
        else m_text = "L M A O O O O O";

        PlaySoundEffect(IDR_WAV_SEKIRO);
    }

    bool Sekiro::DrawContent(ImDrawList* drawList, float timeSinceDeath, ImVec2 screenSize, float finalScale)
    {
        if (timeSinceDeath > 7.0f) {
            return false;
        }

        finalScale *= 0.6f;

        float bgOpacity = m_bgOpacityCurve.Evaluate(timeSinceDeath);
        float flashScale = m_flashScaleCurve.Evaluate(timeSinceDeath);
        float flashOpacity = m_flashOpacityCurve.Evaluate(timeSinceDeath);
        float textOpacity = m_textOpacityCurve.Evaluate(timeSinceDeath);

        ImU32 bgCol32 = ImGui::GetColorU32(ImVec4(0.0f, 0.0f, 0.0f, bgOpacity * 0.7f));
        ImU32 textCol32 = ImGui::GetColorU32(ImVec4(177.0f/255.0f, 12.0f/255.0f, 16.0f/255.0f, textOpacity));
        ImU32 flashCol32 = ImGui::GetColorU32(ImVec4(250.0f/255.0f, 120.0f/255.0f, 120.0f/255.0f, flashOpacity));

        drawList->AddRectFilled(ImVec2(0, 0), screenSize, bgCol32);

        Texture_t* kanjiTex = GetTexture("TexSekiro");
        Texture_t* kanjiShadowTex = GetTexture("TexSekiroOut");
        ImFont* font = GetFont("FontAthelas_Native250");

        // Measure text height so kanji can be offset upward
        float textHeight = 0.0f;
        ImVec2 textSize = ImVec2(0, 0);

        if (font && textOpacity > 0.0f) {
            ImGui::PushFont(font);
            ImGui::SetWindowFontScale(finalScale);
            textSize = ImGui::CalcTextSize(m_text.c_str());
            textHeight = textSize.y;
        }

        // Center coordinates
        float centerX = screenSize.x * 0.5f;
        float centerY = screenSize.y * 0.5f;

        if (kanjiTex && kanjiTex->Resource) {
            float w = (float)kanjiTex->Width * finalScale;
            float h = (float)kanjiTex->Height * finalScale;

            // Kanji offset: screenCenter - kanjiHeight/2 - textHeight/2 - 30
            float x = centerX - (w * 0.5f);
            float y = centerY - (h * 0.5f) - (textHeight * 0.5f) - (30.0f * finalScale);

            // Base layer: kanji + shadow at text color
            if (kanjiShadowTex && kanjiShadowTex->Resource) {
                drawList->AddImage((void*)kanjiShadowTex->Resource, ImVec2(x, y), ImVec2(x + w, y + h), ImVec2(0,0), ImVec2(1,1), textCol32);
            }
            drawList->AddImage((void*)kanjiTex->Resource, ImVec2(x, y), ImVec2(x + w, y + h), ImVec2(0,0), ImVec2(1,1), textCol32);

            // Flash layer: kanji + shadow scaled by flashScale at flash color
            float fw = w * flashScale;
            float fh = h * flashScale;
            // Flash is centered on the same base kanji position
            float fx = centerX - (fw * 0.5f);
            float fy = centerY - (fh * 0.5f) - (textHeight * 0.5f) - (30.0f * finalScale);

            if (kanjiShadowTex && kanjiShadowTex->Resource) {
                drawList->AddImage((void*)kanjiShadowTex->Resource, ImVec2(fx, fy), ImVec2(fx + fw, fy + fh), ImVec2(0,0), ImVec2(1,1), flashCol32);
            }
            drawList->AddImage((void*)kanjiTex->Resource, ImVec2(fx, fy), ImVec2(fx + fw, fy + fh), ImVec2(0,0), ImVec2(1,1), flashCol32);

            // Draw text below kanji
            if (font && textOpacity > 0.0f) {
                // Text offset: screenCenter + kanjiHeight/2 - 60
                float tx = centerX - (textSize.x * 0.5f);
                float ty = centerY + (h * 0.5f) - (60.0f * finalScale);
                
                drawList->AddText(ImVec2(tx, ty), textCol32, m_text.c_str());
                ImGui::SetWindowFontScale(1.0f);
                ImGui::PopFont();
            }
        } else if (font && textOpacity > 0.0f) {
            ImGui::SetWindowFontScale(1.0f);
            ImGui::PopFont();
        }

        return true;
    }
}
