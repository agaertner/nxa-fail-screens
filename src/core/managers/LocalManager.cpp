#include "LocalManager.h"
#include "../services/Services.h"
#include "../Addon.h"
#include <fstream>
#include <iostream>

using json = nlohmann::json;

namespace Nekres {

    LocalManager::LocalManager(const std::filesystem::path& addonPath)
    {
        m_filePath = addonPath / "localization.json";
        Load(m_filePath);
    }

    void LocalManager::Load(const std::filesystem::path& filePath)
    {
        if (!std::filesystem::exists(filePath)) {
            CreateDefaultFile(filePath);
        }

        try {
            std::ifstream file(filePath);
            json j;
            file >> j;

            for (auto& el : j.items()) {
                std::string key = el.key();
                for (auto& langEl : el.value().items()) {
                    std::string langId = langEl.key();
                    if (langEl.value().is_string()) {
                        m_strings[key][langId] = langEl.value().get<std::string>();
                    }
                }
            }
        }
        catch (const json::exception& e) {
            Addon::Log(ELogLevel::LOGL_WARNING, "localization.json could not be parsed.");
            Addon::Log(ELogLevel::LOGL_WARNING, e.what());
        }
    }

    void LocalManager::CreateDefaultFile(const std::filesystem::path& filePath)
    {
        json defaultJson = {
            {"WinXP_Error", {
                {"en", "Error"},
                {"de", "Fehler"}
            }},
            {"WinXP_Fail", {
                {"en", "Fail"},
                {"de", "Versagt"}
            }},
            {"WinXP_Desc", {
                {"en", "{0} ran into a problem and needs to revive.\nWe're just collecting some tears, and then\nwe'll grief with you."},
                {"de", "{0} ist auf ein Problem gesto\xC3\x9F""en und muss wiederbelebt werden.\nWir sammeln nur ein paar Tr\xC3\xA4nen, und dann\nwerden wir mit dir trauern."}
            }},
            {"WinXP_Info", {
                {"en", "For more information about this issue and possible fixes,\nbother your party leader."},
                {"de", "F\xC3\xBCr weitere Informationen \xC3\xBC""ber dieses Problem und m\xC3\xB6gliche L\xC3\xB6sungen,\nnerv deinen Gruppenleiter."}
            }},

            {"WinXP_YourCharacter", {
                {"en", "Your character"},
                {"de", "Dein Charakter"}
            }},
            {"Sekiro_Death", {
                {"en", "D E A T H"},
                {"fr", "M O R T"},
                {"de", "T O D"},
                {"es", "M U E R T E"}
            }}
        };

        try {
            std::ofstream file(filePath);
            file << defaultJson.dump(4);
        }
        catch (const std::exception&) {
            Addon::Log(ELogLevel::LOGL_WARNING, "Failed to create default localization.json.");
        }
    }

    std::string LocalManager::GetIsoCode(int langId)
    {
        switch (langId) {
            case 2: return "fr";
            case 3: return "de";
            case 4: return "es";
            case 5: return "zh";
            default: return "en";
        }
    }

    std::string LocalManager::GetString(const std::string& key, std::optional<int> language)
    {
        int langId = 0; // Default English

        if (language.has_value()) {
            langId = language.value();
        } else if (Services::m_rtapi && Services::m_rtapi->Data()) {
            langId = Services::m_rtapi->Data()->Language;
        }

        std::string langStr = GetIsoCode(langId);

        auto keyIt = m_strings.find(key);
        if (keyIt != m_strings.end()) {
            auto langIt = keyIt->second.find(langStr);
            if (langIt != keyIt->second.end()) {
                return langIt->second;
            }

            // Fallback to English ("en")
            auto enIt = keyIt->second.find("en");
            if (enIt != keyIt->second.end()) {
                return enIt->second;
            }
        }

        // Fallback to returning the key if nothing was found
        return key;
    }

}
