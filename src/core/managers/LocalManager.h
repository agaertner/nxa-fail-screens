#ifndef LOCAL_MANAGER_H
#define LOCAL_MANAGER_H

#include <string>
#include <unordered_map>
#include <optional>
#include <filesystem>
#include "../../thirdparty/nlohmann/json.hpp"

namespace Nekres {

    class LocalManager {
    public:
        LocalManager(const std::filesystem::path& addonPath);
        ~LocalManager() = default;

        // Get a localized string. If language is nullopt, uses Nexus current language.
        // If not found in the target language, falls back to English ("0").
        // If the key entirely doesn't exist, returns the key itself as a fallback.
        std::string GetString(const std::string& key, std::optional<int> language = std::nullopt);

        // Converts GW2 API language ID to ISO language code (en, fr, de, es, zh)
        std::string GetIsoCode(int langId);

    private:
        void Load(const std::filesystem::path& filePath);
        void CreateDefaultFile(const std::filesystem::path& filePath);

        // Map of Key -> (LanguageID -> Translated String)
        std::unordered_map<std::string, std::unordered_map<std::string, std::string>> m_strings;
        std::filesystem::path m_filePath;
    };

}

#endif // LOCAL_MANAGER_H
