#ifndef ANIMATION_CURVE_H
#define ANIMATION_CURVE_H

#include <vector>
#include <algorithm>

namespace Nekres {

    struct Keyframe {
        float time;
        float value;
        
        bool operator<(const Keyframe& other) const {
            return time < other.time;
        }
    };

    class AnimationCurve {
    public:
        AnimationCurve() = default;

        void AddKeyframe(float time, float value) {
            m_keyframes.push_back({ time, value });
            std::sort(m_keyframes.begin(), m_keyframes.end());
        }

        float Evaluate(float time) const {
            if (m_keyframes.empty()) return 0.0f;
            if (m_keyframes.size() == 1) return m_keyframes.front().value;

            if (time <= m_keyframes.front().time) return m_keyframes.front().value;
            if (time >= m_keyframes.back().time) return m_keyframes.back().value;

            for (size_t i = 0; i < m_keyframes.size() - 1; ++i) {
                const auto& k1 = m_keyframes[i];
                const auto& k2 = m_keyframes[i + 1];

                if (time >= k1.time && time <= k2.time) {
                    float t = (time - k1.time) / (k2.time - k1.time);
                    return k1.value + t * (k2.value - k1.value);
                }
            }

            return 0.0f;
        }

    private:
        std::vector<Keyframe> m_keyframes;
    };

}

#endif // ANIMATION_CURVE_H
