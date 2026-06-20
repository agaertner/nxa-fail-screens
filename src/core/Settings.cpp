#include "Settings.h"

#include "Addon.h"

#include <filesystem>
#include <fstream>
#include <cmath>

const char* ACTIVE_SCREEN = "ActiveScreen";
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
                json j = json::parse(file);
                if (j.contains("ActiveScreen")) {
                    ActiveScreen = j["ActiveScreen"].get<int>();
                }
            if (j.contains("Randomize")) Randomize = j["Randomize"].get<bool>();
            if (j.contains("EnableDarkSouls")) EnableDarkSouls = j["EnableDarkSouls"].get<bool>();
            if (j.contains("EnableGrandTheftAuto")) EnableGrandTheftAuto = j["EnableGrandTheftAuto"].get<bool>();
            if (j.contains("EnableRytlocksCritterRampage")) EnableRytlocksCritterRampage = j["EnableRytlocksCritterRampage"].get<bool>();
            if (j.contains("EnableAngryPepe")) EnableAngryPepe = j["EnableAngryPepe"].get<bool>();
            if (j.contains("EnableSekiro")) EnableSekiro = j["EnableSekiro"].get<bool>();
            if (j.contains("EnableWinXp")) EnableWinXp = j["EnableWinXp"].get<bool>();
            if (j.contains("Volume")) Volume = j["Volume"].get<float>();    file.close();
            }
            catch (json::parse_error& ex)
            {
                Nekres::Addon::Log(ELogLevel::LOGL_WARNING, "Settings.json could not be parsed.");
                Nekres::Addon::Log(ELogLevel::LOGL_WARNING, ex.what());
            }
        }
        Settings::Mutex.unlock();
    }
    void Save(std::filesystem::path aPath)
    {
        Settings::Mutex.lock();
        {
            // ========================================================================
            // SETTINGS SETTER (JSON Serialization)
            // ========================================================================
            // Assign global variables back into the JSON object before saving.
        nlohmann::json j;
        j["ActiveScreen"] = ActiveScreen;
        j["Randomize"] = Randomize;
        j["EnableDarkSouls"] = EnableDarkSouls;
        j["EnableGrandTheftAuto"] = EnableGrandTheftAuto;
        j["EnableRytlocksCritterRampage"] = EnableRytlocksCritterRampage;
        j["EnableAngryPepe"] = EnableAngryPepe;
        j["EnableSekiro"] = EnableSekiro;
        j["EnableWinXp"] = EnableWinXp;
        j["Volume"] = std::round((double)Volume * 1000.0) / 1000.0;

            std::ofstream file(aPath);
            file << j.dump(1, '\t') << std::endl;
            file.close();
        }
        Settings::Mutex.unlock();
    }

    // ========================================================================
    // SETTINGS DEFAULT VALUES
    // ========================================================================
    int ActiveScreen = 0;

    bool Randomize = false;
    bool EnableDarkSouls = true;
    bool EnableGrandTheftAuto = true;
    bool EnableRytlocksCritterRampage = true;
    bool EnableAngryPepe = true;
    bool EnableSekiro = true;
    bool EnableWinXp = true;

    float Volume = 0.5f;
}