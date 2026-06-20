#ifndef ADDON_H
#define ADDON_H
#include <cmath>
#include <vector>
#include <filesystem>
#include <fstream>
#include <DirectXMath.h>
#include "Settings.h"
#include "imgui/imgui.h"
#include "services/Services.h"
namespace Nekres {
    class Addon
    {
        const char* ON_ADDON_LOADED = "EV_ADDON_LOADED";
        const char* ON_ADDON_UNLOADED = "EV_ADDON_UNLOADED";

        public:
            Addon(AddonDefinition_t* p_addonDef, AddonAPI_t* p_api);

            ~Addon();


            static void Log(ELogLevel p_logLevel, const char* p_message) {
                if (m_instance) {
                    m_instance->m_api->Log(p_logLevel, m_instance->m_addonDef->Name, p_message);
                }
            }

            static std::filesystem::path GetAddonPath() {
                if (m_instance) {
                    return m_instance->m_addonPath;
                }
                return std::filesystem::path();
            }

            void Render();
            void Options();

        private:
            inline static Addon* m_instance = nullptr;

            AddonDefinition_t* m_addonDef;
            AddonAPI_t* m_api;

            std::filesystem::path m_addonPath;
            std::filesystem::path m_settingsPath;

            static void AddonRender() { m_instance->Render(); }
            static void AddonOptions() { m_instance->Options(); }
    };

}
#endif