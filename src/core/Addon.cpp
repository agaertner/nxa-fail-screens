#include "Addon.h"
#include <algorithm>
#include <cctype>

Nekres::Addon::Addon(AddonDefinition_t* p_addonDef, AddonAPI_t* p_api) : 
    m_addonDef(p_addonDef), m_api(p_api)
{
#ifdef USE_MUMBLE
    Services::Mumble(p_api);
#endif
    Services::Nexus(p_api);
#ifdef USE_RTAPI
    Services::RTAPI(p_api);
#endif

    m_instance = this;
    ImGui::SetCurrentContext((ImGuiContext*)m_api->ImguiContext);
    if (m_api->ImguiMalloc && m_api->ImguiFree) {
        ImGui::SetAllocatorFunctions((void* (*)(size_t, void*))m_api->ImguiMalloc, (void(*)(void*, void*))m_api->ImguiFree); // on imgui 1.80+
    }

    std::string folderName = m_addonDef->Name;
    folderName.erase(std::remove_if(folderName.begin(), folderName.end(), ::isspace), folderName.end());

    m_addonPath = m_api->Paths_GetAddonDirectory(folderName.c_str());
    m_settingsPath = m_addonPath / "settings.json";
    std::filesystem::create_directories(m_addonPath);
    Settings::Load(m_settingsPath);
    m_api->GUI_Register(ERenderType::RT_Render, AddonRender);
    m_api->GUI_Register(ERenderType::RT_OptionsRender, AddonOptions);
}

Nekres::Addon::~Addon()
{
    m_api->GUI_Deregister(AddonOptions);
    m_api->GUI_Deregister(AddonRender);
#ifdef USE_MUMBLE
    delete Services::m_mumble;
    Services::m_mumble = nullptr;
#endif
    delete Services::m_nexus;
    Services::m_nexus = nullptr;
#ifdef USE_RTAPI
    delete Services::m_rtapi;
    Services::m_rtapi = nullptr;
#endif
    delete m_api;
    Settings::Save(m_settingsPath);
    m_instance = nullptr;
}

void Nekres::Addon::Render()
{
}

void Nekres::Addon::Options()
{
    // ========================================================================
    // SETTINGS UI
    // ========================================================================
    if (ImGui::Checkbox("Enable Example Feature", &Settings::IsExampleEnabled))
    {
        Settings::Save(m_settingsPath); // Automatically save when clicked
    }

    // Dropdown (Combo) Example
    const char* exampleOptions[] = { "Option A", "Option B", "Option C" };
    const int exampleOptionsCount = 3;

    // Safety clamp to prevent out-of-bounds access if settings get corrupted
    if (Settings::ExampleDropdownIndex < 0 || Settings::ExampleDropdownIndex >= exampleOptionsCount) {
        Settings::ExampleDropdownIndex = 0;
    }
    
    ImGui::AlignTextToFramePadding();
    ImGui::Text("Example Dropdown:");
    ImGui::SameLine();
    ImGui::SetNextItemWidth(150.0f);
    
    if (ImGui::BeginCombo("##ExampleDropdown", exampleOptions[Settings::ExampleDropdownIndex]))
    {
        for (int i = 0; i < exampleOptionsCount; i++)
        {
            bool isSelected = (Settings::ExampleDropdownIndex == i);
            if (ImGui::Selectable(exampleOptions[i], isSelected))
            {
                Settings::ExampleDropdownIndex = i;
                Settings::Save(m_settingsPath);
            }
            if (isSelected)
            {
                ImGui::SetItemDefaultFocus();
            }
        }
        ImGui::EndCombo();
    }
}