#ifndef ANIMATOR_H
#define ANIMATOR_H

#include "Component.h"

namespace ecs {

    class AnimatorComponent : public Component {

    public:
        std::string controllerGUID{};  // Animation controller / state machine
        std::string avatarGUID{};      // Skeleton avatar/rig definition

        float playbackSpeed{ 1.0f };

        REFLECTABLE(AnimatorComponent, controllerGUID, avatarGUID, playbackSpeed);
    };

}

#endif // ANIMATOR_H
