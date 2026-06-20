#ifndef AUDIOMANAGER_H
#define AUDIOMANAGER_H

#include <windows.h>
#include <xaudio2.h>
#include <mutex>
#include <unordered_map>
#include <cstdint>
#include <unordered_map>
#include "../../Defines.h"

namespace Nekres {
    struct AudioCacheEntry {
        WAVEFORMATEX* format;
        const BYTE* data;
        DWORD dataSize;
        UINT32 playBegin;
    };

    class AudioHandle {
        uint64_t m_id;
    public:
        AudioHandle() : m_id(0) {}
        AudioHandle(uint64_t id) : m_id(id) {}
        ~AudioHandle();
        
        AudioHandle(AudioHandle&& other) noexcept : m_id(other.m_id) {
            other.m_id = 0;
        }
        AudioHandle& operator=(AudioHandle&& other) noexcept;

        AudioHandle(const AudioHandle&) = delete;
        AudioHandle& operator=(const AudioHandle&) = delete;
    };

    class AudioManager {
    public:
        AudioManager(AddonAPI_t* api);
        ~AudioManager();

        AudioHandle Play(int resourceId);
        void Stop(uint64_t voiceId);
        void StopAll();
        void UpdateVolume();

    private:
        AddonAPI_t* m_api;
        IXAudio2* m_pXAudio2;
        IXAudio2MasteringVoice* m_pMasterVoice;
        bool m_initialized;
        bool m_coInitialized;
        std::mutex m_mutex;
        
        std::unordered_map<int, AudioCacheEntry> m_cache;
        uint64_t m_nextVoiceId = 1;
        std::unordered_map<uint64_t, IXAudio2SourceVoice*> m_activeVoices;

        bool FindChunk(const BYTE* data, size_t size, DWORD fourcc, DWORD& chunkSize, DWORD& chunkDataPosition);
        UINT32 TrimSilence(WAVEFORMATEX* format, const BYTE* data, DWORD size);
    };
}

#endif
