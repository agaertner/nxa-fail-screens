#include "WinXp.h"
#include "../../resource.h"
#include <cstdlib>

namespace Nekres {

    WinXp::WinXp(AddonAPI_t* api, HMODULE moduleHandle)
        : FailScreen(api, moduleHandle)
    {
        // Blue screen timeline:
        // 0-2s: error boxes spawn
        // ~2.65s: blue screen appears, boxes hidden
        // 3.9-5.9s: blue screen fades out over 2s
        m_blueScreenCurve.AddKeyframe(0.0f, 0.0f);
        m_blueScreenCurve.AddKeyframe(2.65f, 0.0f);
        m_blueScreenCurve.AddKeyframe(2.651f, 1.0f);
        m_blueScreenCurve.AddKeyframe(3.9f, 1.0f);
        m_blueScreenCurve.AddKeyframe(5.9f, 0.0f);
    }

    void WinXp::OnDeath(int language)
    {
        m_language = language;
        EnsureTexture("TexWinXp", IDR_TEX_WINXP);
        // Load fonts at their native sizes to prevent downscaling aliasing
        EnsureFont("FontConsola_40", IDR_FONT_CONSOLA, 40.0f, m_api, m_hSelf);
        EnsureFont("FontConsola_20", IDR_FONT_CONSOLA, 20.0f, m_api, m_hSelf);
        EnsureFont("FontConsola_16", IDR_FONT_CONSOLA, 16.0f, m_api, m_hSelf);
        
        m_windows.clear();
        m_hideBoxes = false;
        m_playedFinalSound = false;
        m_blueScreenOpacity = 0.0f;
    }

    bool WinXp::DrawContent(ImDrawList* drawList, float timeSinceDeath, ImVec2 screenSize, float finalScale)
    {
        if (timeSinceDeath > 5.9f) {
            return false;
        }

        // Spawn error boxes during first 2 seconds
        float boxPhaseEnd = 2.0f;
        
        if (timeSinceDeath < boxPhaseEnd) {
            int expectedBoxes = (int)(timeSinceDeath / 0.2f);
            if (expectedBoxes > 10) expectedBoxes = 10;

            while ((int)m_windows.size() < expectedBoxes) {
                WindowState w;
                w.OffsetX = (float)((rand() % 1001) - 500) * finalScale;
                w.OffsetY = (float)((rand() % 1001) - 500) * finalScale;
                m_windows.push_back(w);

                PlaySoundEffect(IDR_WAV_WINXP);
            }
        }

        if (timeSinceDeath >= 2.65f) {
            if (!m_playedFinalSound) {
                PlaySoundEffect(IDR_WAV_WINXP);
                m_playedFinalSound = true;
            }
            m_hideBoxes = true;
        }

        m_blueScreenOpacity = m_blueScreenCurve.Evaluate(timeSinceDeath);

        Texture_t* errorTex = GetTexture("TexWinXp");
        ImFont* boxFont = GetFont("FontConsola_16");

        // Draw error boxes scaled by UI scale
        if (!m_hideBoxes && errorTex && errorTex->Resource) {
            float w = (float)errorTex->Width * finalScale;
            float h = (float)errorTex->Height * finalScale;

            ImU32 titleCol = ImGui::GetColorU32(ImVec4(1,1,1,1));
            ImU32 textCol = ImGui::GetColorU32(ImVec4(0,0,0,1));

            std::string strError = Services::Local()->GetString("WinXP_Error", m_language);
            std::string strFail = Services::Local()->GetString("WinXP_Fail", m_language);

            for (const auto& win : m_windows) {
                float x = (screenSize.x - w) * 0.5f + win.OffsetX;
                float y = (screenSize.y - h) * 0.5f + win.OffsetY;

                drawList->AddImage((void*)errorTex->Resource, ImVec2(x, y), ImVec2(x + w, y + h));

                if (boxFont) {
                    drawList->AddText(boxFont, 16.0f * finalScale, ImVec2(x + 8.0f * finalScale, y + 9.0f * finalScale), titleCol, strError.c_str());
                    drawList->AddText(boxFont, 16.0f * finalScale, ImVec2(x + 50.0f * finalScale, y + 45.0f * finalScale), textCol, strFail.c_str());
                }
            }
        }

        // Draw blue screen overlay with sad face
        if (m_blueScreenOpacity > 0.001f) {
            ImU32 bsCol = ImGui::GetColorU32(ImVec4(0.0f, 121.0f/255.0f, 217.0f/255.0f, m_blueScreenOpacity));
            drawList->AddRectFilled(ImVec2(0, 0), screenSize, bsCol);

            ImU32 whiteCol = ImGui::GetColorU32(ImVec4(1.0f, 1.0f, 1.0f, m_blueScreenOpacity));

            ImFont* descFont = GetFont("FontConsola_40");
            ImFont* infoFont = GetFont("FontConsola_20");

            if (descFont && infoFont) {
                float descX = screenSize.x * 0.15f;
                float descY = screenSize.y * 0.4f;

                float smileX = descX - (15.0f * finalScale); // Shifted slightly left
                float smileY = descY - (280.0f * finalScale); // Shifted further up to increase gap

                // Reuse the 40px font and scale it up for the sad face to save massive amounts of atlas space
                drawList->AddText(descFont, 250.0f * finalScale, ImVec2(smileX, smileY), whiteCol, ":(");
                
                std::string yourCharacter = Services::Local()->GetString("WinXP_YourCharacter", m_language);

                std::string charName = (Services::m_mumble && Services::m_mumble->Identity()) ? Services::m_mumble->Identity()->Name : yourCharacter;
                if (charName.empty()) charName = yourCharacter;

                std::string desc = Services::Local()->GetString("WinXP_Desc", m_language);
                size_t pos = desc.find("{0}");
                if (pos != std::string::npos) {
                    desc.replace(pos, 3, charName);
                }

                std::string infoStr = Services::Local()->GetString("WinXP_Info", m_language);
                
                ImVec2 descPos = ImVec2(descX, descY);
                drawList->AddText(descFont, 40.0f * finalScale, descPos, whiteCol, desc.c_str());

                ImVec2 infoPos = ImVec2(descPos.x, descPos.y + 150.0f * finalScale);
                drawList->AddText(infoFont, 20.0f * finalScale, infoPos, whiteCol, infoStr.c_str());
            }
        }

        return true;
    }
}
