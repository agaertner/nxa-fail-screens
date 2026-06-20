#include "AngryPepe.h"
#include "../../resource.h"
#include <cstdlib>

namespace Nekres {

    AngryPepe::AngryPepe(AddonAPI_t* api, HMODULE moduleHandle)
        : FailScreen(api, moduleHandle)
    {
        // Rage: 0->1 over 3.5s
        m_rageCurve.AddKeyframe(0.0f, 0.0f);
        m_rageCurve.AddKeyframe(3.5f, 1.0f);
    }

    void AngryPepe::OnDeath(int language)
    {
        EnsureTexture("TexAngryPepe", IDR_TEX_ANGRYPEPE);
        PlaySoundEffect(IDR_WAV_ANGRYPEPE);
    }

    bool AngryPepe::DrawContent(ImDrawList* drawList, float timeSinceDeath, ImVec2 screenSize, float finalScale)
    {
        if (timeSinceDeath > 3.5f) {
            return false;
        }

        float rage = m_rageCurve.Evaluate(timeSinceDeath);

        Texture_t* pepeTex = GetTexture("TexAngryPepe");

        // Draw pepe first, then red foreground overlay on top
        if (pepeTex && pepeTex->Resource) {
            // Resize keeping aspect ratio to 80% of screen
            float maxW = 0.8f * screenSize.x;
            float maxH = 0.8f * screenSize.y;

            float texW = (float)pepeTex->Width;
            float texH = (float)pepeTex->Height;
            float aspect = texW / texH;

            float w, h;
            if (maxW / aspect <= maxH) {
                w = maxW;
                h = maxW / aspect;
            } else {
                h = maxH;
                w = maxH * aspect;
            }

            // Center + shake by rage * random(-100, 100) * finalScale
            float shakeX = rage * (float)((rand() % 201) - 100) * finalScale;
            float shakeY = rage * (float)((rand() % 201) - 100) * finalScale;

            float x = (screenSize.x - w) * 0.5f + shakeX;
            float y = (screenSize.y - h) * 0.5f + shakeY;

            drawList->AddImage((void*)pepeTex->Resource, ImVec2(x, y), ImVec2(x + w, y + h));
        }

        // Red foreground overlay
        ImU32 fgCol = ImGui::GetColorU32(ImVec4(1.0f, 0.0f, 0.0f, 0.5f * rage));
        drawList->AddRectFilled(ImVec2(0, 0), screenSize, fgCol);

        return true;
    }
}
