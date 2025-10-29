#include "Config/pch.h"
#include "ParticleSystem.h"
#include "glm/glm.hpp"

namespace ecs {

    void ParticleSystem::Init() {

        onRegister.Add([](EntityID id) {
            ECS* ecs = ECS::GetInstance();
            ParticleComponent* particle = ecs->GetComponent<ParticleComponent>(id);

            // ---------- FleX Initialization ----------
            NvFlexInitDesc desc = {};
            desc.deviceIndex = 0;
            desc.enableExtensions = true;
            desc.computeType = eNvFlexCUDA;

            particle->library = (void*)NvFlexInit(NV_FLEX_VERSION, nullptr, &desc);
            if (!particle->library) {
                LOGGING_ERROR("Failed to initialize FleX.\n");
                return;
            }

            NvFlexSolverDesc solverDesc;
            NvFlexSetSolverDescDefaults(&solverDesc);
            solverDesc.maxParticles = particle->max_Particles;
            particle->solver = (void*)NvFlexCreateSolver((NvFlexLibrary*)particle->library, &solverDesc);

            //Allocate Flex Buffer
            /*
            pointer 0 -> positionbuffer
            pointer 1 -> velocitybuffer
            pointer 2 -> phasebuffer
            */
            particle->pointers[0] = (void*)NvFlexAllocBuffer((NvFlexLibrary*)particle->library, particle->max_Particles, sizeof(glm::vec4), eNvFlexBufferHost);
            particle->pointers[1] = (void*)NvFlexAllocBuffer((NvFlexLibrary*)particle->library, particle->max_Particles, sizeof(glm::vec3), eNvFlexBufferHost);
            particle->pointers[2] = (void*)NvFlexAllocBuffer((NvFlexLibrary*)particle->library, particle->max_Particles, sizeof(int), eNvFlexBufferHost);

            //Map Buffers
            glm::vec4* positions = (glm::vec4*)NvFlexMap((NvFlexBuffer*)particle->pointers[0], eNvFlexMapWait);
            glm::vec3* velocities = (glm::vec3*)NvFlexMap((NvFlexBuffer*)particle->pointers[1], eNvFlexMapWait);
            int* phases = (int*)NvFlexMap((NvFlexBuffer*)particle->pointers[2], eNvFlexMapWait);

            //init the maps
            for (int i = 0; i < particle->max_Particles; ++i) {
                positions[i] = glm::vec4(0.0f, 0.0f, 0.0f, 0.0f); // w=0 means INACTIVE
                velocities[i] = glm::vec3(0.0f);
                phases[i] = NvFlexMakePhase(0, eNvFlexPhaseSelfCollide);
            }

            NvFlexUnmap((NvFlexBuffer*)particle->pointers[0]);
            NvFlexUnmap((NvFlexBuffer*)particle->pointers[1]);
            NvFlexUnmap((NvFlexBuffer*)particle->pointers[2]);

            NvFlexSetParticles((NvFlexSolver*)particle->solver, (NvFlexBuffer*)particle->pointers[0], nullptr);
            NvFlexSetVelocities((NvFlexSolver*)particle->solver, (NvFlexBuffer*)particle->pointers[1], nullptr);
            NvFlexSetPhases((NvFlexSolver*)particle->solver, (NvFlexBuffer*)particle->pointers[2], nullptr);


            //init the particle velocity
            particle->particleLifetimes.resize(particle->max_Particles, 0.0f);
            particle->freeIndices.clear();
            particle->freeIndices.reserve(particle->max_Particles);

            for (int i = 0; i < particle->max_Particles; ++i) {
                particle->freeIndices.push_back(i);
            }

            particle->no_of_Particles = 0;

            // Initialize emitter timers (one entry per entity)
            particle->emitterTimers.resize(1, 0.0f);  // Single entity
            particle->durationTimers.resize(1, 0.0f);


            // ---------- FleX Parameters ----------
            NvFlexParams params;
            NvFlexGetParams((NvFlexSolver*)particle->solver, &params);
            params.gravity[0] = 0.0f;
            params.gravity[1] = -9.8f;
            params.gravity[2] = 0.0f;
            params.radius = 0.05f;
            params.dynamicFriction = 0.1f;
            params.particleFriction = 0.1f;
            params.restitution = 0.3f;
            params.numIterations = 3;
            params.relaxationFactor = 1.0f;
            NvFlexSetParams((NvFlexSolver*)particle->solver, &params);


            });

        onDeregister.Add([](EntityID id) {
            ECS* ecs = ECS::GetInstance();
            auto* particle = ecs->GetComponent<ParticleComponent>(id);

            if (particle->pointers[0]) {
                NvFlexFreeBuffer((NvFlexBuffer*)particle->pointers[0]);
                particle->pointers[0] = nullptr;
            }
            if (particle->pointers[1]) {
                NvFlexFreeBuffer((NvFlexBuffer*)particle->pointers[1]);
                particle->pointers[1] = nullptr;
            }
            if (particle->pointers[2]) {
                NvFlexFreeBuffer((NvFlexBuffer*)particle->pointers[2]);
                particle->pointers[2] = nullptr;
            }

            // Destroy solver BEFORE library
            if (particle->solver) {
                NvFlexDestroySolver((NvFlexSolver*)particle->solver);
                particle->solver = nullptr;
            }

            // Destroy library last
            if (particle->library) {
                NvFlexShutdown((NvFlexLibrary*)particle->library);
                particle->library = nullptr;
            }

            // Clear tracking data
            particle->particleLifetimes.clear();
            particle->freeIndices.clear();
            particle->emitterTimers.clear();
            particle->durationTimers.clear();
            particle->no_of_Particles = 0;
            });
    }

    void ParticleSystem::Update(const std::string& scene) {
        ECS* ecs = ECS::GetInstance();
        const auto& entities = m_entities.Data();
        float dt = ecs->m_GetDeltaTime();
        UpdateEmitters(dt);

        for (EntityID id : entities) {
            ParticleComponent* particle = ecs->GetComponent<ParticleComponent>(id);

            if (!particle || !particle->solver) {
                continue;
            }

            // Update particle lifetimes and deactivate dead ones
            UpdateParticleLifetimes(dt, particle);

            // Sync updated particles to Flex
            NvFlexSetParticles((NvFlexSolver*)particle->solver, (NvFlexBuffer*)particle->pointers[0], nullptr);
            NvFlexSetVelocities((NvFlexSolver*)particle->solver, (NvFlexBuffer*)particle->pointers[1], nullptr);

            // Run Flex simulation
            NvFlexUpdateSolver((NvFlexSolver*)particle->solver, dt, 4, false);

            // Retrieve simulated positions from Flex
            NvFlexGetParticles((NvFlexSolver*)particle->solver, (NvFlexBuffer*)particle->pointers[0], nullptr);

            // TODO: Send to render pipeline
            // You can access positions here if needed for rendering
            //Retrieve the scale + rotation
            float scale = particle->size;
            float rot = particle->rotation;
       
        }
    }

    void ParticleSystem::EmitParticle(EntityID entityId, const glm::vec3& position, const glm::vec3& velocity, float lifetime, ParticleComponent*& particle) {
        // Check if we have any free slots
        if (particle->freeIndices.empty()) {
            LOGGING_WARN("No free particle slots for entity %d\n", entityId);
            return;
        }

        // FIX: Get a free particle index (O(1) operation)
        int particleIdx = particle->freeIndices.back();
        particle->freeIndices.pop_back();

        // FIX: Set particle tracking data
        particle->particleLifetimes[particleIdx] = lifetime;
        particle->no_of_Particles++;

        // Map Flex buffers and ACTIVATE particle
        glm::vec4* positions = (glm::vec4*)NvFlexMap((NvFlexBuffer*)particle->pointers[0], eNvFlexMapWait);
        glm::vec3* velocities = (glm::vec3*)NvFlexMap((NvFlexBuffer*)particle->pointers[1], eNvFlexMapWait);

        if (positions && velocities) {
            // FIX: Set position with w=1.0 to make particle ACTIVE
            positions[particleIdx] = glm::vec4(position, 1.0f);
            velocities[particleIdx] = velocity;

            // FIX: Unmap the buffers
            NvFlexUnmap((NvFlexBuffer*)particle->pointers[0]);
            NvFlexUnmap((NvFlexBuffer*)particle->pointers[1]);
        }
    }

    void ParticleSystem::UpdateParticleLifetimes(float dt, ParticleComponent*& particle) {
        glm::vec4* positions = (glm::vec4*)NvFlexMap((NvFlexBuffer*)particle->pointers[0], eNvFlexMapWait);
        if (!positions) {
            return;
        }

        for (int i = 0; i < particle->max_Particles; ++i) {
            // Only process particles that are alive
            if (particle->particleLifetimes[i] > 0.0f) {
                particle->particleLifetimes[i] -= dt;

                // Check if particle died this frame
                if (particle->particleLifetimes[i] <= 0.0f) {
                    // DEACTIVATE particle by setting w = 0 (Flex will ignore it)
                    positions[i].w = 0.0f;

                    // FIX: Return particle index to free pool for reuse
                    particle->freeIndices.push_back(i);
                    particle->no_of_Particles--;
                }
            }
        }

        NvFlexUnmap((NvFlexBuffer*)particle->pointers[0]);
    }

    void ParticleSystem::UpdateEmitters(float dt) {
        ECS* ecs = ECS::GetInstance();
        const auto& entities = m_entities.Data();

        int entityIdx = 0;
        for (const EntityID id : entities) {
            ParticleComponent* particleComp = ecs->GetComponent<ParticleComponent>(id);
            TransformComponent* transform = ecs->GetComponent<TransformComponent>(id);

            if (!particleComp || !transform) {
                entityIdx++;
                continue;
            }

            // FIX: Ensure timer vectors are sized correctly
            if (entityIdx >= particleComp->emitterTimers.size()) {
                particleComp->emitterTimers.resize(entityIdx + 1, 0.0f);
                particleComp->durationTimers.resize(entityIdx + 1, 0.0f);
            }

            // Check if emitter should be active
            bool shouldEmit = false;

            if (particleComp->looping) {
                shouldEmit = particleComp->play_On_Awake;
            }
            else {
                if (particleComp->play_On_Awake) {
                    particleComp->durationTimers[entityIdx] += dt;
                    shouldEmit = (particleComp->durationTimers[entityIdx] < particleComp->duration);
                }
            }

            if (shouldEmit) {
                particleComp->emitterTimers[entityIdx] += dt;

                // Emission rate: 10 particles per second (0.1s interval)
                float emissionInterval = 0.1f;

                while (particleComp->emitterTimers[entityIdx] >= emissionInterval) {
                    glm::vec3 pos = transform->LocalTransformation.position;

                    // Add some randomness to velocity
                    float randX = (rand() % 200 - 100) / 100.0f;
                    float randZ = (rand() % 200 - 100) / 100.0f;

                    glm::vec3 vel = glm::vec3(
                        randX * particleComp->start_Velocity * 0.5f,
                        particleComp->start_Velocity,
                        randZ * particleComp->start_Velocity * 0.5f
                    );

                    EmitParticle(id, pos, vel, particleComp->start_Lifetime, particleComp);
                    particleComp->emitterTimers[entityIdx] -= emissionInterval;

                    // Safety: don't emit too many in one frame
                    if (particleComp->emitterTimers[entityIdx] > emissionInterval * 10) {
                        particleComp->emitterTimers[entityIdx] = 0.0f;
                        break;
                    }
                }
            }

            entityIdx++;
        }
    }

}