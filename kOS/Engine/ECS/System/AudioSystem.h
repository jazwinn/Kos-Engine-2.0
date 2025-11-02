/******************************************************************/
/*!
\file      AudioSystem.h
\author    Chiu Jun Jie
\par       junjie.c@digipen.edu
\date      Oct 03,2025
\brief     This file contains the declaration of the AudioSystem
           class. It handles the logic behind the Audio played in
           the engine and works with the Audio Manager interface.
            


Copyright (C) 2025 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the
prior written consent of DigiPen Institute of Technology is prohibited.
*/
/********************************************************************/
#ifndef AUDIOSYS_H
#define AUDIOSYS_H

#include "ECS/ECS.h"
#include "System.h"


namespace FMOD {
    namespace Studio {
        class System;   //for FMOD studio
    }

    class System;       //for FMOD Core
}

namespace ecs {
    class AudioSystem : public ISystem {
    public:
        void Init() override;
        void Update() override;

        static void SetPaused(bool paused);
        static void StopAll();

        static void SetListenerFromCamera(const glm::vec3& pos,
            const glm::vec3& fwd,
            const glm::vec3& up);

        //DEBUG
        static glm::vec3 GetCachedCamPos() { return s_camPos; }
        static glm::vec3 GetCachedCamFwd() { return s_camFwd; }
        static glm::vec3 GetCachedCamUp() { return s_camUp; }

        // Fmod Studio Stuff
        static FMOD::Studio::System* GetStudioSystem() { return s_studio; }
        static FMOD::System* GetCoreSystem() { return s_fmod; }
        static bool IsStudioReady() { return s_studioReady; }

        REFLECTABLE(AudioSystem)

    private:
        // Core
        static FMOD::System* s_fmod;
        static bool          s_paused;

        // Studio
        static FMOD::Studio::System* s_studio;
        static bool s_studioReady;   // true when Studio and bank initializaed 

        void InitCore_();
        void InitStudioIfBanksExist_();
        void UpdateListener_(); // single owner of listener - camer

        static inline glm::vec3 s_camPos{ 0,0,0 };
        static inline glm::vec3 s_camFwd{ 0,0,-1 };
        static inline glm::vec3 s_camUp{ 0,1,0 };
        static inline bool      s_camValid{ false };

    };
}

#endif // AUDIOSYS_H

