#ifndef FAILSCREEN_H
#define FAILSCREEN_H

#include <vector>
#include <string>
#include <memory>
#include <cstdint>
#include <unordered_map>
#include "../../Defines.h"
#include "../services/Services.h"
#include "../utils/AsyncTexture.h"
#include "../utils/AsyncFont.h"
#include "../../resource.h"
#include <imgui/imgui.h>

namespace Nekres {

    class FailScreen {
    public:
        FailScreen(AddonAPI_t* api, HMODULE hSelf) : m_api(api), m_hSelf(hSelf) {}
        virtual ~FailScreen() = default;

        // Triggered upon death. Receives the EGameLanguage (e.g. 0=EN, 2=FR, 3=DE, 4=ES).
        virtual void OnDeath(int language) = 0;

        // Public non-virtual Render method handling ImGui boilerplate
        bool Render(float timeSinceDeath);

    protected:
        AddonAPI_t* m_api;
        HMODULE m_hSelf;

        // Derived classes must implement this to draw their actual content.
        // Return false when the animation has finished.
        virtual bool DrawContent(ImDrawList* drawList, float timeSinceDeath, ImVec2 screenSize, float finalScale) = 0;

        // Audio Management
        std::vector<AudioHandle> m_audioHandles;
        
        void PlaySoundEffect(int resourceId) {
            if (Services::m_audio) {
                m_audioHandles.push_back(Services::m_audio->Play(resourceId));
            }
        }

        // Resource Management (Static so they outlive screen instances and prevent dangling pointers during async loading)
        static inline std::unordered_map<std::string, std::unique_ptr<AsyncTexture>> s_textures;
        static inline std::unordered_map<std::string, std::unique_ptr<AsyncFont>> s_fonts;

        // Ensures the texture resource is loaded and returns the latest pointer.
        // Call this every frame instead of caching the returned pointer,
        // because the GPU resource may not be ready on the first call.
        void EnsureTexture(const std::string& id, int resourceId) {
            if (s_textures.find(id) == s_textures.end()) {
                s_textures[id] = std::make_unique<AsyncTexture>(id.c_str(), resourceId, m_api, m_hSelf);
                s_textures[id]->Load();
            }
        }

        Texture_t* GetTexture(const std::string& id) {
            auto it = s_textures.find(id);
            if (it != s_textures.end()) {
                return it->second->Get();
            }
            return nullptr;
        }

        static void EnsureFont(const std::string& id, int resourceId, float size, AddonAPI_t* api, HMODULE hSelf) {
            if (s_fonts.find(id) == s_fonts.end()) {
                s_fonts[id] = std::make_unique<AsyncFont>(id.c_str(), resourceId, size, api, hSelf);
                s_fonts[id]->Load();
            }
        }

        ImFont* GetFont(const std::string& id) {
            auto it = s_fonts.find(id);
            if (it != s_fonts.end()) {
                return (ImFont*)it->second->Get();
            }
            return nullptr;
        }

    public:
        // Preload all fonts at startup to prevent mid-frame D3D11 atlas rebuild crashes.
        // The static resource maps ensure the AsyncFont identifier strings live forever, preventing startup crashes.
        static void PreloadFonts(AddonAPI_t* api, HMODULE hSelf) {
            EnsureFont("FontConsola_40", IDR_FONT_CONSOLA, 40.0f, api, hSelf);
            EnsureFont("FontConsola_20", IDR_FONT_CONSOLA, 20.0f, api, hSelf);
            EnsureFont("FontConsola_16", IDR_FONT_CONSOLA, 16.0f, api, hSelf);
            EnsureFont("FontAthelas_Native250", IDR_FONT_ATHELAS, 250.0f, api, hSelf);
        }

        static void ClearResources(AddonAPI_t* api) {
            AsyncFont::ClearAll(api);
            s_fonts.clear();
            s_textures.clear();
        }
    };

}

#endif // FAILSCREEN_H
