#include "ECS/ECS.h"
#include "System.h"

namespace FMOD {
    namespace Studio {
        class System;   //for FMOD studio
    }

    class System;       //for FMOD Core
}

namespace ecs {

    class AudioSystemStudio : public ISystem {
    public:
        void Init() override;
        void Update(const std::string&) override;
        void Shutdown();

        static FMOD::Studio::System* GetStudioSystem() { return s_studio; }
        static FMOD::System* GetCoreSystem() { return s_core; }

        static bool IsInitialized() { return s_initialized; }

        static void SetPaused(bool paused);
        static void StopAll();

        REFLECTABLE(AudioSystemStudio)

    private:
        static FMOD::Studio::System* s_studio;
        static FMOD::System* s_core;
        static bool                  s_initialized;
        static bool                  s_paused;
    };
}