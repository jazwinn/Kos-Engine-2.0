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
    class System; 
}

namespace ecs {
    class AudioSystem : public ISystem {
    public:
        void Init() override;
        void Update(const std::string&) override;

        static void SetPaused(bool paused);
        static void StopAll(); 

        REFLECTABLE(AudioSystem)

    private:
        static FMOD::System* s_fmod;
        static bool s_paused;

    };
}

#endif // AUDIOSYS_H

