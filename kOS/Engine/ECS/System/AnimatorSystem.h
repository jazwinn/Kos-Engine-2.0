#ifndef ANIMATORSYS_H
#define ANIMATORSYS_H

#include "System.h"
#include "ECS/ECSList.h"

namespace ecs {

    class AnimatorSystem : public ISystem {

    public:

        void Init() override;
        void Update() override;

        REFLECTABLE(AnimatorSystem)
    };

}

#endif // ANIMATORSYS_H