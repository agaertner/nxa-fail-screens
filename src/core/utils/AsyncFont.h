#ifndef ASYNCFONT_H
#define ASYNCFONT_H

#include <nexus-core/Nexus.h>
#include <string>
#include <map>

namespace Nekres {
    class AsyncFont {
    public:
        AsyncFont(const std::string& identifier, int resourceID, float fontSize, AddonAPI_t* api, HMODULE moduleHandle);
        ~AsyncFont() = default;

        // Requests Nexus to load the font from the resource
        void Load();

        // Returns the ImFont* if it has been loaded and initialized
        void* Get();

    private:
        std::string m_identifier;
        int m_resourceID;
        float m_fontSize;
        AddonAPI_t* m_api;
        HMODULE m_moduleHandle;

        bool m_isLoadRequested;

        static std::map<std::string, void*> s_loadedFonts;
        static void FontReceiveCallback(const char* identifier, void* font);
    };
}

#endif // ASYNCFONT_H
