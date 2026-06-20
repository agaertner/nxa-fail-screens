#include "FailScreen.h"

namespace Nekres {

    bool FailScreen::Render(float timeSinceDeath) {
        ImGuiIO& io = ImGui::GetIO();
        ImGui::SetNextWindowPos(ImVec2(0, 0));
        ImGui::SetNextWindowSize(io.DisplaySize);
        ImGui::SetNextWindowBgAlpha(0.0f);

        ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0, 0));
        ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);

        bool stillPlaying = true;

        if (ImGui::Begin("##FailScreenOverlay_Base", nullptr, 
            ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize | 
            ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoScrollbar | 
            ImGuiWindowFlags_NoInputs | ImGuiWindowFlags_NoBackground |
            ImGuiWindowFlags_NoSavedSettings | ImGuiWindowFlags_NoFocusOnAppearing))
        {
            ImDrawList* drawList = ImGui::GetWindowDrawList();
            float finalScale = Services::GetFinalScale();

            // Hand over drawing control to the specific child screen
            stillPlaying = DrawContent(drawList, timeSinceDeath, io.DisplaySize, finalScale);
        }
        ImGui::End();
        ImGui::PopStyleVar(2);

        return stillPlaying; 
    }

}
