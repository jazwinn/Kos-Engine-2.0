/********************************************************************/
/*!
\file      LightComponent.h
\author    Sean Tiu (2303398)
\par       Email: s.tiu@digipen.edu
\date      Oct 01, 2025
\brief     Declares the LightComponent class, which defines various
           types of lights used in the ECS-based rendering system.

           The LightComponent allows entities to function as different
           light sources within a scene. It supports the following
           types:
           - **Directional Light**
           - **Point Light**
           - **Spot Light**

           Each light type includes customizable parameters:
           - Color, diffuse, and specular strengths
           - Attenuation factors (linear and quadratic)
           - Direction vector for directional and spot lights
           - Cutoff angles for spotlights

           This component provides the necessary data for lighting
           calculations in shaders, contributing to both deferred and
           forward rendering pipelines.

Copyright (C) 2025 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the
prior written consent of DigiPen Institute of Technology is prohibited.
*/
/********************************************************************/

#ifndef LIGHTCOMP_H
#define LIGHTCOMP_H

#include "Component.h"

namespace ecs {

    class LightComponent : public Component {

    public:
        enum LightType {
            //LIGHT,
            DIRECTIONAL,
            POINTLIGHT,
            SPOTLIGHT,
        };

        LightType lightType{};

        glm::vec3 color{ 1.f };
        glm::vec3 diffuseStrength{ 1.f };
        glm::vec3 specularStrength{ 1.f };
        float linear{ 0.09f };
        float quadratic{ 0.032f };
        glm::vec3 direction{ 1.f };
        float cutOff{ 5.5f };
        float outerCutOff{ 10.5f };

        REFLECTABLE(LightComponent, lightType, color, diffuseStrength, specularStrength, linear,
            quadratic, direction, cutOff, outerCutOff);
    };

}
#endif // !LIGHTCOMP_H