#include <Windows.h>
//#include "resource.h"
#include "Version.h"
#include "Signature.h"
#include "core/Addon.h"

void AddonLoad(AddonAPI_t* aApi);
void AddonUnload();

HMODULE hSelf;

AddonDefinition_t AddonDef{};

Nekres::Addon* g_addon = nullptr;

BOOL APIENTRY DllMain( HMODULE hModule,
                       DWORD  ul_reason_for_call,
                       LPVOID lpReserved
                     )
{
    switch (ul_reason_for_call)
    {
        case DLL_PROCESS_ATTACH: hSelf = hModule; break;
        case DLL_THREAD_ATTACH:
        case DLL_THREAD_DETACH:
        case DLL_PROCESS_DETACH:
        break;
    }
    return TRUE;
}

extern "C" __declspec(dllexport) AddonDefinition_t* GetAddonDef()
{
    AddonDef.Signature = ADDON_SIGNATURE;
    AddonDef.APIVersion = NEXUS_API_VERSION;
    AddonDef.Name = "Addon Template";
    AddonDef.Version.Major = V_MAJOR;
    AddonDef.Version.Minor = V_MINOR;
    AddonDef.Version.Build = V_BUILD;
    AddonDef.Version.Revision = V_REVISION;
    AddonDef.Author = "Nekres.1943";
    AddonDef.Description = "A basic Nexus addon template.";
    AddonDef.Load = AddonLoad;
    AddonDef.Unload = AddonUnload;
    AddonDef.Flags = EAddonFlags::AF_None;
    AddonDef.Provider = EUpdateProvider::UP_GitHub;
    AddonDef.UpdateLink = "https://github.com/agaertner/nxa-addon-template";
    return &AddonDef;
}

void AddonLoad(AddonAPI_t* aApi)
{
    g_addon = new Nekres::Addon(&AddonDef, aApi);
}

void AddonUnload()
{
    delete g_addon;
}
