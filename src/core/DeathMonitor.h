#ifndef DEATHMONITOR_H
#define DEATHMONITOR_H

#include "../Defines.h"
#include <functional>
#include <cstdint>

namespace Nekres {
    class DeathMonitor {
    public:
        DeathMonitor(AddonAPI_t* api);
        
        using OnDeathCallback = std::function<void(int language)>;
        using OnContextChangeCallback = std::function<void()>;
        
        void SetCallbacks(OnDeathCallback deathCb, OnContextChangeCallback contextChangeCb);
        
        void Update(bool isPreviewing);

    private:
        AddonAPI_t* m_api;
        bool m_wasAlive = true;
        uint32_t m_lastMapId = 0;
        uint32_t m_lastGameState = 0;
        
        OnDeathCallback m_onDeath;
        OnContextChangeCallback m_onContextChange;
    };
}

#endif // DEATHMONITOR_H
