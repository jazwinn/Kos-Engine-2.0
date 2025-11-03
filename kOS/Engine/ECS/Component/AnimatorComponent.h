#ifndef ANIMATOR_H
#define ANIMATOR_H

#include "Component.h"

namespace ecs {

    class AnimatorComponent : public Component {

    public:
        std::string controllerGUID{};  // Animation controller / state machine
        std::string avatarGUID{};      // Skeleton avatar/rig definition

        float m_PlaybackSpeed{ 1.0f };
        float m_CurrentTime{ 0.f };
        bool  m_IsPlaying{ true };

        REFLECTABLE(AnimatorComponent, controllerGUID, avatarGUID, m_PlaybackSpeed, m_CurrentTime, m_IsPlaying);
    };

}

#endif // ANIMATOR_H
