#include "Config/pch.h"
#include "ParticleSystem.h"
#include "glm/glm.hpp"

namespace ecs {
    constexpr EntityID NULL_ENTITY = static_cast<EntityID>(-1); // Works for unsigned types
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
            if (particle->library) {
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
            //buffer(particle);
            
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

            //init the particle velocity
            particle->particleLifetimes.resize(particle->max_Particles, 0.f);
            particle->no_of_Particles = 0;
            


            //Sending to Graphics


        });
        
        onDeregister.Add([](EntityID id) {
            ECS* ecs = ECS::GetInstance();
            auto* particle = ecs->GetComponent<ParticleComponent>(id);

            //for the shutdown 

        });


        //NvFlexUnmap(positionBuffer);
        //NvFlexUnmap(velocityBuffer);
        //NvFlexUnmap(phaseBuffer);
        //NvFlexUnmap(activeBuffer);

        //// Set initial state (0 active particles)
        //NvFlexSetParticles(solver, positionBuffer, nullptr);
        //NvFlexSetVelocities(solver, velocityBuffer, nullptr);
        //NvFlexSetPhases(solver, phaseBuffer, nullptr);
        //NvFlexSetActive(solver, activeBuffer, nullptr);
        //NvFlexSetActiveCount(solver, 0);

        //// ---------- FleX Parameters ----------
        //NvFlexParams params;
        //NvFlexGetParams(solver, &params);
        //params.gravity[0] = 0.0f;
        //params.gravity[1] = -9.8f;
        //params.gravity[2] = 0.0f;
        //params.radius = 0.05f;
        //params.dynamicFriction = 0.1f;
        //params.particleFriction = 0.1f;
        //params.restitution = 0.3f;
        //params.numIterations = 3;
        //params.relaxationFactor = 1.0f;
        //NvFlexSetParams(solver, &params);

        //// ---------- OpenGL VBO ----------
        //glGenBuffers(1, &positionVBO);
        //glBindBuffer(GL_ARRAY_BUFFER, positionVBO);
        //glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec4) * maxParticles, nullptr, GL_DYNAMIC_DRAW);
        //glBindBuffer(GL_ARRAY_BUFFER, 0);

        //// Initialize emitter timers
        //emitterTimers.resize(entities.size(), 0.0f);
        //durationTimers.resize(entities.size(), 0.0f);

        //LOGGING_INFO("ParticleSystem initialized with max %d particles.\n", maxParticles);
    }

    void ParticleSystem::buffer(ParticleComponent* particle) {
        /*
        pointer 0 -> positionbuffer
        pointer 1 -> velocitybuffer
        pointer 2 -> phasebuffer
        */
        particle->pointers[0] = (void*)NvFlexAllocBuffer((NvFlexLibrary*)particle->library, particle->max_Particles, sizeof(glm::vec4), eNvFlexBufferHost);
        particle->pointers[1] = (void*)NvFlexAllocBuffer((NvFlexLibrary*)particle->library, particle->max_Particles, sizeof(glm::vec3), eNvFlexBufferHost);
        particle->pointers[2] = (void*)NvFlexAllocBuffer((NvFlexLibrary*)particle->library, particle->max_Particles, sizeof(int), eNvFlexBufferHost);
    }

    void ParticleSystem::Update(const std::string& scene) {

    }

    //void ParticleSystem::EmitParticle(EntityID entityId, const glm::vec3& position,
    //    
    //}

    void ParticleSystem::UpdateParticleLifetimes(float dt) {
        
    }

    void ParticleSystem::UpdateEmitters(float dt) {
       
    }

    //void ParticleSystem::SyncWithFleX() {
    //    //// Build active indices array (only particles with lifetime > 0)
    //    //int* actives = (int*)NvFlexMap(activeBuffer, eNvFlexMapWait);
    //    //int activeCount = 0;

    //    //for (int i = 0; i < maxParticles; ++i) {
    //    //    if (particleLifetimes[i] > 0.0f) {
    //    //        actives[activeCount++] = i;
    //    //    }
    //    //}

    //    //NvFlexUnmap(activeBuffer);

    //    //// Update FleX with current active count and data
    //    //NvFlexSetActive(solver, activeBuffer, nullptr);
    //    //NvFlexSetActiveCount(solver, activeCount);
    //    //NvFlexSetParticles(solver, positionBuffer, nullptr);
    //    //NvFlexSetVelocities(solver, velocityBuffer, nullptr);
    //}

    //int ParticleSystem::GetActiveParticleCount() const {
    //    return maxParticles - static_cast<int>(freeIndices.size());
    //}

    //void ParticleSystem::shutdown() {
    //    // Free FleX buffers
    //    if (activeBuffer) {
    //        NvFlexFreeBuffer(activeBuffer);
    //        activeBuffer = nullptr;
    //    }
    //    if (phaseBuffer) {
    //        NvFlexFreeBuffer(phaseBuffer);
    //        phaseBuffer = nullptr;
    //    }
    //    if (velocityBuffer) {
    //        NvFlexFreeBuffer(velocityBuffer);
    //        velocityBuffer = nullptr;
    //    }
    //    if (positionBuffer) {
    //        NvFlexFreeBuffer(positionBuffer);
    //        positionBuffer = nullptr;
    //    }

    //    // Destroy solver and library
    //    if (solver) {
    //        NvFlexDestroySolver(solver);
    //        solver = nullptr;
    //    }
    //    if (library) {
    //        NvFlexShutdown(library);
    //        library = nullptr;
    //    }

    //    // Delete OpenGL VBO
    //    if (positionVBO) {
    //        glDeleteBuffers(1, &positionVBO);
    //        positionVBO = 0;
    //    }

    //    // Clear tracking data
    //    particleLifetimes.clear();
    //    freeIndices.clear();
    //    emitterTimers.clear();
    //    durationTimers.clear();
    //    maxParticles = 0;

    //    LOGGING_INFO("ParticleSystem shutdown completed.\n");
    //}
}