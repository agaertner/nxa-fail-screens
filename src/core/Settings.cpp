#include "Settings.h"

#include "Addon.h"

#include <filesystem>
#include <fstream>

const char* IS_EXAMPLE_ENABLED = "IsExampleEnabled";
const char* EXAMPLE_DROPDOWN_INDEX = "ExampleDropdownIndex";
namespace Settings
{
    std::mutex    Mutex;
    json        Settings = json::object();

    void Load(std::filesystem::path aPath)
    {
        if (!std::filesystem::exists(aPath)) { return; }

        Settings::Mutex.lock();
        {
            try
            {
                std::ifstream file(aPath);
                Settings = json::parse(file);
                file.close();
            }
            catch (json::parse_error& ex)
            {
                Nekres::Addon::Log(ELogLevel::LOGL_WARNING, "Settings.json could not be parsed.");
                Nekres::Addon::Log(ELogLevel::LOGL_WARNING, ex.what());
            }
        }
        Settings::Mutex.unlock();

        // ========================================================================
        // SETTINGS GETTER (JSON Parsing)
        // ========================================================================
        // Read the value from the parsed JSON and apply it to a global variable.
        if (!Settings[IS_EXAMPLE_ENABLED].is_null())
        {
            Settings[IS_EXAMPLE_ENABLED].get_to<bool>(IsExampleEnabled);
        }
        if (!Settings[EXAMPLE_DROPDOWN_INDEX].is_null())
        {
            Settings[EXAMPLE_DROPDOWN_INDEX].get_to<int>(ExampleDropdownIndex);
        }
    }
    void Save(std::filesystem::path aPath)
    {
        Settings::Mutex.lock();
        {
            // ========================================================================
            // SETTINGS SETTER (JSON Serialization)
            // ========================================================================
            // Assign global variables back into the JSON object before saving.
            Settings[IS_EXAMPLE_ENABLED] = IsExampleEnabled;
            Settings[EXAMPLE_DROPDOWN_INDEX] = ExampleDropdownIndex;

            std::ofstream file(aPath);
            file << Settings.dump(1, '\t') << std::endl;
            file.close();
        }
        Settings::Mutex.unlock();
    }

    // ========================================================================
    // SETTINGS DEFAULT VALUES
    // ========================================================================
    bool IsExampleEnabled = true;
    int ExampleDropdownIndex = 0;
}