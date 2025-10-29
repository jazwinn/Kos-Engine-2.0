#ifndef PARTICLESYSTEM_H
#define PARTICLESYSTEM_H
#include "ECS/ECS.h"
#include "System.h"
#include "Flex/NvFlex.h"
#include "Flex/NvFlexExt.h"

namespace ecs {
    class ParticleSystem : public ISystem {
    public:
        void Init() override;
        void Update(const std::string&) override;

        // OpenGL resources
        GLuint positionVBO;
        // Spawn a new particle
        void EmitParticle(EntityID entityId, const glm::vec3& position,
            const glm::vec3& velocity, float lifetime, ParticleComponent*& particle);
        
        // Update particle lifetimes and kill dead particles
        void UpdateParticleLifetimes(float dt, ParticleComponent*& particle);
        
        // Handle particle emission from emitter components
        void UpdateEmitters(float dt);
        

        REFLECTABLE(ParticleSystem);
    };
}
#endif