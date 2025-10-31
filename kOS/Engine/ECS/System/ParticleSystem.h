#ifndef PARTICLESYSTEM_H
#define PARTICLESYSTEM_H
#include "ECS/ECS.h"
#include "System.h"
#include "Flex/NvFlex.h"
#include "Flex/NvFlexExt.h"

namespace ecs {

    struct ParticleInstance {
        std::vector<glm::vec3> positions_Particle;
        glm::vec4 color{ 1.f,1.f,1.f,1.f };
        glm::vec3 scale{ 1.f,1.f,1.f };
        float rotate{};
    };
    class ParticleSystem : public ISystem {
    public:

        void Init() override;
        void Update(const std::string&) override;

        // Spawn a new particle
        void EmitParticle(EntityID entityId, const glm::vec3& particle_position,
            const glm::vec3& velocity, float lifetime, ParticleComponent*& particle, glm::vec4* position, glm::vec3* velocities, float* lifetime_list);
        
        // Update particle lifetimes and kill dead particles
        void UpdateParticleLifetimes(float dt, ParticleComponent*& particle, glm::vec4* positions);
        
        // Handle particle emission from emitter components
        void UpdateEmitters(float dt, EntityID id, ParticleComponent*& particleComp,  TransformComponent* transform);

        void SyncActiveBuffer(ParticleComponent* particle);

        void ExtractParticlePositionsOptimized(ParticleComponent* particle, std::vector<glm::vec3>& outPositions, glm::vec4* positions);
        

        REFLECTABLE(ParticleSystem);
    };
}
#endif