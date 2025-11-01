/******************************************************************/
/*!
\file      Application.cpp
\author    Ng Jaz winn, jazwinn.ng , 2301502
\par       jazwinn.ng@digipen.edu
\date      Oct 02, 2024
\brief     This file contains the main update loop of the engine.
           It initializes the various major systems and call upon
           them in the update loop. When the window is closed,
           the applciations cleanup function is called to 
           call the major systems to clean their program.
           


Copyright (C) 2024 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the
prior written consent of DigiPen Institute of Technology is prohibited.
*/
/********************************************************************/





#include "Application.h"
#include "ApplicationData.h"
#include "Resources/ResourceManager.h"
#include "Scene/SceneManager.h"
#include "Inputs/Input.h"
#include "Graphics/GraphicsManager.h"
#include "Configs/ConfigPath.h"
#include "Debugging/Performance.h"
#include "Scripting/ScriptManager.h"
#include "Physics/PhysicsManager.h"

namespace Application {

	

    /*--------------------------------------------------------------
      GLOBAL VARAIBLE
    --------------------------------------------------------------*/
    auto ecs = ecs::ECS::GetInstance();
    auto scenemanager = scenes::SceneManager::m_GetInstance();
    auto peformance = Peformance::GetInstance();
    auto input = Input::InputSystem::GetInstance();
    auto resourceManager = ResourceManager::GetInstance();


    int Application::Init() {
        
        /*--------------------------------------------------------------
          Read Config File
       --------------------------------------------------------------*/
        std::filesystem::path exePath = std::filesystem::current_path();
        std::filesystem::path root = exePath.parent_path().parent_path(); // up two levels
        std::filesystem::current_path(root);
        
		WindowSettings windowData = Serialization::ReadJsonFile<WindowSettings>(configpath::configFilePath);

        /*--------------------------------------------------------------
        INITIALIZE LOGGING SYSTEM
        --------------------------------------------------------------*/
        LOGGING_INIT_LOGS(configpath::logFilePath);
        LOGGING_INFO("Application Start");
        LOGGING_INFO("Load Log Successful");

       /*--------------------------------------------------------------
          INITIALIZE OPENGL WINDOW
       --------------------------------------------------------------*/
        lvWindow.init(windowData.windowWidth, windowData.windowHeight);
        LOGGING_INFO("Load Window Successful");

        /*--------------------------------------------------------------
           INITIALIZE ECS
        --------------------------------------------------------------*/
        ecs->Load();
        ecs->Init();
		ecs->SetState(ecs::GAMESTATE::START);
        LOGGING_INFO("Load ECS Successful");

        
        /*--------------------------------------------------------------
          INITIALIZE GRAPHICS PIPE
        --------------------------------------------------------------*/
        GraphicsManager::GetInstance()->gm_Initialize(static_cast<float>(windowData.gameResWidth), static_cast<float>(windowData.gameResHeight));
        LOGGING_INFO("Load Graphic Pipeline Successful");

        /*--------------------------------------------------------------
           INITIALIZE Resource Manager
        --------------------------------------------------------------*/
        auto resourceManager = ResourceManager::GetInstance();
        resourceManager->Init(configpath::resourceFilePath);

        /*--------------------------------------------------------------
        INITIALIZE SCIRPT
        --------------------------------------------------------------*/
        ScriptManager::m_GetInstance()->Init(exePath.string());

        /*--------------------------------------------------------------
           INITIALIZE Start Scene
        --------------------------------------------------------------*/
        resourceManager->GetResource<R_Scene>(windowData.startScene);
        LOGGING_INFO("Load Asset Successful");

        /*--------------------------------------------------------------
           INITIALIZE GRAPHICS PIPE
        --------------------------------------------------------------*/
        GraphicsManager::GetInstance()->gm_Initialize(static_cast<float>(windowData.windowWidth), static_cast<float>(windowData.windowHeight));
        LOGGING_INFO("Load Graphic Pipeline Successful");

        /*--------------------------------------------------------------
           INITIALIZE Input
        --------------------------------------------------------------*/
        //call back must happen before imgui
        input->SetCallBack(lvWindow.window);
        LOGGING_INFO("Set Input Call Back Successful");

   

        LOGGING_INFO("Application Init Successful");
        return 0;
	}



    int Application::Run() {


        //float FPSCapTime = 1.f / help->m_fpsCap;
        double lastFrameTime = glfwGetTime();
        const double fixedDeltaTime = 1.0 / 60.0;
        float accumulatedTime = 0.0;

        std::shared_ptr<GraphicsManager> graphicsManager = GraphicsManager::GetInstance();
       // ScriptManager::m_GetInstance()->RunDLL();
        /*--------------------------------------------------------------
            GAME LOOP
        --------------------------------------------------------------*/
        while (!glfwWindowShouldClose(lvWindow.window))
        {
            try {
                /* Poll for and process events */
                glfwPollEvents();

                ///*--------------------------------------------------------------
                //    Calculate time
                // --------------------------------------------------------------*/
                double currentFrameTime = glfwGetTime();
                float deltaTime = static_cast<float>(currentFrameTime - lastFrameTime);
                lastFrameTime = currentFrameTime;
                accumulatedTime += (deltaTime);

                peformance->SetDeltaTime(deltaTime);

                int currentNumberOfSteps = 0;
                while( accumulatedTime >= fixedDeltaTime) {
                    accumulatedTime -= static_cast<float>(fixedDeltaTime);
                    ++currentNumberOfSteps;
                }
                /*--------------------------------------------------------------
                    Update SceneManager // STAY THE FIRST ON TOP
                --------------------------------------------------------------*/
                scenemanager->Update();
                
                /*--------------------------------------------------------------
                    UPDATE INPUT
                --------------------------------------------------------------*/

                input->InputUpdate(deltaTime);
                /*--------------------------------------------------------------
                    UPDATE ECS
                --------------------------------------------------------------*/
                ecs->Update(static_cast<float>(fixedDeltaTime));
                
                /*--------------------------------------------------------------
                    UPDATE Render Pipeline
                --------------------------------------------------------------*/
                graphicsManager->gm_Update();

                /*--------------------------------------------------------------
                    Execute Render Pipeline
                --------------------------------------------------------------*/
                graphicsManager->gm_Render();
                
                /*--------------------------------------------------------------
                    Update IMGUI FRAME
                --------------------------------------------------------------*/
                lvWindow.Draw();


                /*--------------------------------------------------------------
                   Reset Framebuffer
                --------------------------------------------------------------*/
                graphicsManager->gm_ResetFrameBuffer();

                /*--------------------------------------------------------------
                 DRAWING/RENDERING Window
                --------------------------------------------------------------*/
                lvWindow.Draw();


                graphicsManager->gm_ClearGBuffer();

                glfwSwapBuffers(lvWindow.window);
            }
            catch (const std::exception& e) {
                LOGGING_ERROR("Exception in game loop: {}", e.what());
            }
        }
        return 0;
    }

	int Application::m_Cleanup() {

        ecs::ECS::GetInstance()->Unload();
        physics::PhysicsManager::GetInstance()->Shutdown();
        lvWindow.CleanUp();
        glfwTerminate();

        LOGGING_INFO("Application Closed");

        return 0;
	}

}