#include "DeathMonitor.h"
#include "services/Services.h"

namespace Nekres {

    DeathMonitor::DeathMonitor(AddonAPI_t* api) : m_api(api)
    {
    }

    void DeathMonitor::SetCallbacks(OnDeathCallback deathCb, OnContextChangeCallback contextChangeCb)
    {
        m_onDeath = std::move(deathCb);
        m_onContextChange = std::move(contextChangeCb);
    }

    void DeathMonitor::Update(bool isPreviewing)
    {
        if (!Services::m_rtapi) return;
        
        RealTimeData* rtData = (RealTimeData*)m_api->DataLink_Get(DL_RTAPI);
        if (!rtData) return;
        
        if (rtData->MapID != m_lastMapId || rtData->GameState != m_lastGameState) {
            m_lastMapId = rtData->MapID;
            m_lastGameState = rtData->GameState;
            
            if (m_onContextChange) {
                m_onContextChange();
            }
        }

        if (rtData->GameBuild == 0 || rtData->GameState != 4) { // 4 is GS_Gameplay
            m_wasAlive = true;
            if (!isPreviewing && m_onContextChange) {
                m_onContextChange();
            }
            return;
        }

        bool isCurrentlyAlive = (rtData->CharacterState & CS_IsAlive) != 0;
        
        if (isCurrentlyAlive && !m_wasAlive) {
            m_wasAlive = true;
            if (m_onContextChange) {
                m_onContextChange();
            }
            
        } else if (m_wasAlive && !isCurrentlyAlive) {
            // Just died!
            m_wasAlive = false;
            if (m_onDeath) {
                m_onDeath(rtData->Language);
            }
        }
    }
}
