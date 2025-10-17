/******************************************************************/
/*!
\file      main.cpp
\author    Ng Jaz winn, jazwinn.ng , 2301502
\par       jazwinn.ng@digipen.edu
\date      Oct 02, 2024
\brief     This file contains the main function of the engine



Copyright (C) 2024 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the
prior written consent of DigiPen Institute of Technology is prohibited.
*/
/********************************************************************/

#include "Application.h"



int WINAPI WinMain([[maybe_unused]] HINSTANCE hInstance, [[maybe_unused]] HINSTANCE hPrevInstance, [[maybe_unused]] LPSTR lpCmdLine, [[maybe_unused]] int nCmdShow)
    {
        
        // Enable run-time memory check for debug builds.
        #if defined(DEBUG) | defined(_DEBUG)
                _CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
        #endif


        Application::Application app{};        
       
        app.Init();
       

        app.Run();

        app.m_Cleanup();




        return 0;
    } 