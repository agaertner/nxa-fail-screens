#include "RytlocksCritterRampage.h"


namespace Nekres {

    RytlocksCritterRampage::RytlocksCritterRampage(AddonAPI_t* api, HMODULE moduleHandle)
        : FailScreen(api, moduleHandle)
    {
        // Sprite opacity: 0->1 over 1s, hold 7s, reflect back over 1s
        m_spriteOpacityCurve.AddKeyframe(0.0f, 0.0f);
        m_spriteOpacityCurve.AddKeyframe(1.0f, 1.0f);
        m_spriteOpacityCurve.AddKeyframe(8.0f, 1.0f);
        m_spriteOpacityCurve.AddKeyframe(9.0f, 0.0f);

        // Background opacity: 0->1 over 1s, hold 8s, reflect back over 1s. OnComplete=Dispose
        m_bgOpacityCurve.AddKeyframe(0.0f, 0.0f);
        m_bgOpacityCurve.AddKeyframe(1.0f, 1.0f);
        m_bgOpacityCurve.AddKeyframe(9.0f, 1.0f);
        m_bgOpacityCurve.AddKeyframe(10.0f, 0.0f);
    }

    void RytlocksCritterRampage::OnDeath(int language)
    {


        PlaySoundEffect(IDR_SFX_RYTLOCK);
    }

    bool RytlocksCritterRampage::DrawContent(ImDrawList* drawList, float timeSinceDeath, ImVec2 screenSize, float finalScale)
    {
        if (timeSinceDeath > 10.0f) {
            return false;
        }

        float bgOpacity = m_bgOpacityCurve.Evaluate(timeSinceDeath);
        float spriteOpacity = m_spriteOpacityCurve.Evaluate(timeSinceDeath);

        // Draw background
        ImU32 bgCol = ImGui::GetColorU32(ImVec4(0.0f, 0.0f, 0.0f, bgOpacity * 0.7f));
        drawList->AddRectFilled(ImVec2(0, 0), screenSize, bgCol);

        // Animate sprite frames: 6 frames per sheet, 2 cols x 3 rows, frame size 190x153
        float frameDuration = 1.0f / 6.0f; // ~5 fps matching AnimationUtil.Animate(6, 2, 190, 153, 5)
        int currentFrame = static_cast<int>(timeSinceDeath / frameDuration) % 6;

        int col = currentFrame % 2;
        int row = currentFrame / 2;

        // Only using sheet 0 as in the original (sheets 1-5 are commented out)
        Texture_t* sheet = NexusSDK::Content->GetTexture(IDB_TEX_RYTLOCK_0);

        if (sheet && sheet->Resource) {
            float cellWidth = 190.0f;
            float cellHeight = 153.0f;

            float renderWidth = cellWidth * 2.0f * finalScale;
            float renderHeight = cellHeight * 2.0f * finalScale;

            // Center horizontally, vertically at midpoint
            float x = (screenSize.x - renderWidth) * 0.5f;
            float y = screenSize.y * 0.5f;

            float colStride = 191.0f;
            float rowStride = 156.5f;
            
            float uvX1 = (col * colStride) / (float)sheet->Width;
            float uvY1 = (row * rowStride) / (float)sheet->Height;
            float uvX2 = ((col * colStride) + cellWidth) / (float)sheet->Width;
            float uvY2 = ((row * rowStride) + cellHeight) / (float)sheet->Height;

            ImU32 spriteCol = ImGui::GetColorU32(ImVec4(1.0f, 1.0f, 1.0f, spriteOpacity));
            drawList->AddImage((void*)sheet->Resource, ImVec2(x, y), ImVec2(x + renderWidth, y + renderHeight), ImVec2(uvX1, uvY1), ImVec2(uvX2, uvY2), spriteCol);
        }

        return true;
    }
}
