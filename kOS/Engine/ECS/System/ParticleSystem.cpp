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
            pointer 3 -> activebuff 
            */
            particle->pointers[0] = (void*)NvFlexAllocBuffer((NvFlexLibrary*)particle->library, particle->max_Particles, sizeof(glm::vec4), eNvFlexBufferHost);
            particle->pointers[1] = (void*)NvFlexAllocBuffer((NvFlexLibrary*)particle->library, particle->max_Particles, sizeof(glm::vec3), eNvFlexBufferHost);
            particle->pointers[2] = (void*)NvFlexAllocBuffer((NvFlexLibrary*)particle->library, particle->max_Particles, sizeof(int), eNvFlexBufferHost);
            particle->pointers[3] = (void*)NvFlexAllocBuffer((NvFlexLibrary*)particle->library, particle->max_Particles, sizeof(int), eNvFlexBufferHost);

            //Map Buffers
            glm::vec4* positions = (glm::vec4*)NvFlexMap((NvFlexBuffer*)particle->pointers[0], eNvFlexMapWait);
            glm::vec3* velocities = (glm::vec3*)NvFlexMap((NvFlexBuffer*)particle->pointers[1], eNvFlexMapWait);
            int* phases = (int*)NvFlexMap((NvFlexBuffer*)particle->pointers[2], eNvFlexMapWait);
            int* active = (int*)NvFlexMap((NvFlexBuffer*)particle->pointers[3], eNvFlexMapWait);

            //init the maps
            for (int i = 0; i < particle->max_Particles; ++i) {
                positions[i] = glm::vec4(0.0f, 0.0f, 0.0f, 0.0f); // w=0 means INACTIVE
                velocities[i] = glm::vec3(0.0f);
                phases[i] = NvFlexMakePhase(0, eNvFlexPhaseSelfCollide | eNvFlexPhaseFluid);
                active[i] = 0;
            }

            NvFlexUnmap((NvFlexBuffer*)particle->pointers[0]);
            NvFlexUnmap((NvFlexBuffer*)particle->pointers[1]);
            NvFlexUnmap((NvFlexBuffer*)particle->pointers[2]);
            NvFlexUnmap((NvFlexBuffer*)particle->pointers[3]);

            NvFlexSetParticles((NvFlexSolver*)particle->solver, (NvFlexBuffer*)particle->pointers[0], nullptr);
            NvFlexSetVelocities((NvFlexSolver*)particle->solver, (NvFlexBuffer*)particle->pointers[1], nullptr);
            NvFlexSetPhases((NvFlexSolver*)particle->solver, (NvFlexBuffer*)particle->pointers[2], nullptr);
            NvFlexSetActive((NvFlexSolver*)particle->solver, (NvFlexBuffer*)particle->pointers[3], nullptr);
            NvFlexSetActiveCount((NvFlexSolver*)particle->solver, 0);

            //init the particle velocity
            particle->particleLifetimes.resize(particle->max_Particles, 0.0f);
            particle->freeIndices.clear();
            particle->freeIndices.reserve(particle->max_Particles);

            for (int i = particle->max_Particles - 1; i >= 0; --i) {
                particle->freeIndices.push_back(i);
            }
           

            // Initialize emitter timers (one entry per entity)
            particle->emitterTimers.resize(1, 0.0f);  // Single entity
            particle->durationTimers.resize(1, 0.0f);


            // ---------- FleX Parameters ----------
            NvFlexParams params;
            NvFlexGetParams((NvFlexSolver*)particle->solver, &params);
            // Gravity settings
            params.gravity[0] = 0.0f;
            params.gravity[1] = -9.8f;  // Standard gravity
            params.gravity[2] = 0.0f;

            // Particle physical properties
            params.radius = 0.05f;
            params.solidRestDistance = params.radius;
            params.fluidRestDistance = params.radius * 0.55f; // Important for fluid behavior

            // Friction and collision
            params.dynamicFriction = 0.1f;
            params.particleFriction = 0.1f;
            params.restitution = 0.3f;
            params.adhesion = 0.0f;
            params.cohesion = 0.025f; // Fluid cohesion
            params.surfaceTension = 0.0f;
            params.viscosity = 0.001f; // Low viscosity for more fluid-like

            // Solver settings
            params.numIterations = 3;
            params.relaxationFactor = 1.0f;

            // Damping (prevents infinite motion)
            params.damping = 0.0f; // Set to 0 for no artificial damping
            params.drag = 0.0f;

            // Collision distance
            params.collisionDistance = params.radius * 0.5f;
            params.shapeCollisionMargin = params.collisionDistance * 0.05f;

            // Particle collision
            params.particleCollisionMargin = params.radius * 0.05f;

            // Sleep threshold (set high to prevent sleeping)
            params.sleepThreshold = 0.0f; // Particles never sleep
            NvFlexSetParams((NvFlexSolver*)particle->solver, &params);

            LOGGING_INFO("Flex initialized successfully for entity %d\n", id);
            LOGGING_INFO("  - Gravity: (%.2f, %.2f, %.2f)\n", params.gravity[0], params.gravity[1], params.gravity[2]);
            LOGGING_INFO("  - Radius: %.3f\n", params.radius);
            LOGGING_INFO("  - Free slots: %zu\n", particle->freeIndices.size());
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
            NvFlexSetActive((NvFlexSolver*)particle->solver, (NvFlexBuffer*)particle->pointers[3], nullptr);          // ok to call once at init, harmless to repeat
            NvFlexSetActiveCount((NvFlexSolver*)particle->solver, particle->particles_Alive);
            // Run Flex simulation
            NvFlexUpdateSolver((NvFlexSolver*)particle->solver, dt, 4, false);

            // Retrieve simulated positions from Flex
            NvFlexGetParticles((NvFlexSolver*)particle->solver, (NvFlexBuffer*)particle->pointers[0], nullptr);
            
            //Passing the position to a vector
            glm::vec4* positions = (glm::vec4*)NvFlexMap((NvFlexBuffer*)particle->pointers[0], eNvFlexMapWait);
            std::vector<glm::vec3> position_vec(particle->max_Particles); //store the position in a vector         
            for (int i = 0; i < particle->max_Particles; ++i) {
               //std::cout << i << std::endl;
                if (positions[i].w > 0.0f) {
                    position_vec[i] = glm::vec3(positions[i].x, positions[i].y, positions[i].z);
                    //std::cout << "Particle " << i << " " << position_vec[i].x << " " << position_vec[i].y << " " << position_vec[i].z << std::endl;
                }
            }
            //Unmap
            NvFlexUnmap((NvFlexBuffer*)particle->pointers[0]);

            // TODO: Send to render pipeline
            // You ca sn access positions here if needed for rendering
            //Retrieve the scale + rotation
            float scale = particle->size;
            float rot = particle->rotation;

            //create a simple shader 
            //Graphic Manager FrameBufferManager


           
       
        }
    }

    void ParticleSystem::EmitParticle(EntityID entityId, const glm::vec3& position, const glm::vec3& velocity, float lifetime, ParticleComponent*& particle) {
        // Check if we have any free slots
        if (particle->freeIndices.empty()) {
            LOGGING_WARN("No free particle slots for entity %d\n", entityId);
            return;
        }

        //if it hit the max do not create anymore particles
        if ((particle->max_Particles - particle->freeIndices.size()) < 0) {
            return;
        }

        // FIX: Get a free particle index (O(1) operation)
        int particleIdx = particle->freeIndices.back();
        particle->freeIndices.pop_back();

        // FIX: Set particle tracking data
        particle->particleLifetimes[particleIdx] = lifetime;

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
        int* alive = (int*)NvFlexMap((NvFlexBuffer*)particle->pointers[3], eNvFlexMapWait);
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
                    particle->particles_Alive--;
                    alive[i] = 0;
                }
            }
        }

        NvFlexUnmap((NvFlexBuffer*)particle->pointers[0]);
        NvFlexUnmap((NvFlexBuffer*)particle->pointers[3]);
    }

    //LOGIC ERROR IN THE EMITTER
    void ParticleSystem::UpdateEmitters(float dt) {
        ECS* ecs = ECS::GetInstance();
        const auto& entities = m_entities.Data();

        int entityIdx = 0;
        for (const EntityID id : entities) {
            ParticleComponent* particleComp = ecs->GetComponent<ParticleComponent>(id);
            TransformComponent* transform = ecs->GetComponent<TransformComponent>(id);

            if (!particleComp || !transform) {
                continue;
            }
            int* alive = (int*)NvFlexMap((NvFlexBuffer*)particleComp->pointers[3], eNvFlexMapWait);
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
                //std::cout << particleComp->emitterTimers[entityIdx] << std::endl;
                // Emission rate: 10 particles per second (0.1s interval)
                float emissionInterval = 0.1f;

                while (particleComp->emitterTimers[entityIdx] >= emissionInterval) {
                    glm::vec3 pos = transform->LocalTransformation.position;

                    // Add some randomness to velocity
                    float randX = (rand() % 200 - 100) / 100.0f;
                    float randZ = (rand() % 200 - 100) / 100.0f;

                    glm::vec3 vel = glm::vec3(
                        particleComp->start_Velocity ,
                        particleComp->start_Velocity,
                        particleComp->start_Velocity 
                    );

                    //std::cout << vel.x << " " << vel.y << " " << vel.z << std::endl;
                    EmitParticle(id, pos, vel, particleComp->start_Lifetime, particleComp);
                    particleComp->emitterTimers[entityIdx] -= emissionInterval;
                    particleComp->particles_Alive++;
                    alive[entityIdx];;
                    // Safety: don't emit too many in one frame
                    if (particleComp->emitterTimers[entityIdx] > emissionInterval * 10) {
                        particleComp->emitterTimers[entityIdx] = 0.0f;
                        break;
                    }
                }
            }
            NvFlexUnmap((NvFlexBuffer*)particleComp->pointers[3]);
            entityIdx++;
        }
    }

}