#ifndef CUBERENDERER_H
#define CUBERENDERER_H

#include "Component.h"

namespace ecs {

    class CubeRendererComponent : public Component {
    public:
        REFLECTABLE(CubeRendererComponent);

    };
}
#endif