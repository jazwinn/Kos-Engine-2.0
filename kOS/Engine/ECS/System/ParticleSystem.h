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

        // Helper functions
        void buffer(ParticleComponent* particle);
        // Spawn a new particle
        void EmitParticle(EntityID entityId, const glm::vec3& position,
            const glm::vec3& velocity, float lifetime);
        // Update particle lifetimes and kill dead particles
        void UpdateParticleLifetimes(float dt);
        // Handle particle emission from emitter components
        void UpdateEmitters(float dt);
        // Update visual properties (scale, rotation, color over lifetime)
        void UpdateParticleVisuals(float dt);

        //// Get count of currently alive particles
        //inline int GetActiveParticleCount(int max_particles) const {
        //    return max_particles - static_cast<int>(freeIndices.size());
        //}

        //// Check if a specific particle is alive
        //inline bool IsParticleAlive(int index, int max_particles) const {
        //    return index >= 0 && index < max_particles && particleLifetimes[index] > 0.0f;
        //}


        REFLECTABLE(ParticleSystem);
    };
}
#endif