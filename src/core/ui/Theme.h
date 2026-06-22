#ifndef THEME_H
#define THEME_H

#include <imgui/imgui.h>

namespace Nekres::UI {
    struct Theme {
        inline static const ImVec4 Accent = ImVec4(0.0f, 0.75f, 1.0f, 1.0f);
        inline static const ImVec4 AccentHover = ImVec4(0.1f, 0.85f, 1.0f, 1.0f);
        inline static const ImVec4 BackgroundSidebar = ImVec4(0.12f, 0.12f, 0.14f, 1.0f);
        inline static const ImVec4 BackgroundContent = ImVec4(0.15f, 0.15f, 0.17f, 1.0f);
        inline static const ImVec4 TextSelected = ImVec4(1.0f, 1.0f, 1.0f, 1.0f);
        inline static const ImVec4 TextUnselected = ImVec4(0.6f, 0.6f, 0.65f, 1.0f);
        inline static const ImVec4 HeaderColor = ImVec4(0.18f, 0.18f, 0.20f, 1.0f);
    };
}

#endif
