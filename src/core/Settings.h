#ifndef SETTINGS_H
#define SETTINGS_H

#include <mutex>

#include "thirdparty/nlohmann/json.hpp"
using json = nlohmann::json;

extern const char* ACTIVE_SCREEN;

namespace Settings
{
    extern std::mutex    Mutex;
    extern json            Settings;

    /* Loads the settings. */
    void Load(std::filesystem::path aPath);
    /* Saves the settings. */
    void Save(std::filesystem::path aPath);

    // ========================================================================
    // SETTINGS ACCESSIBILITY
    // ========================================================================
    // These variables are globally accessible across the addon's code.
    // Include "Settings.h" anywhere and access them via Settings::IsExampleEnabled.
    extern int ActiveScreen;

    extern bool Randomize;
    extern bool EnableDarkSouls;
    extern bool EnableGrandTheftAuto;
    extern bool EnableRytlocksCritterRampage;
    extern bool EnableAngryPepe;
    extern bool EnableSekiro;
    extern bool EnableWinXp;

    extern float Volume;
}

#endif