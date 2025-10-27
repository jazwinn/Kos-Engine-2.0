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
#include "Graphics/GraphicsPipe.h"
#include "Asset Manager/AssetManager.h"
#include "Asset Manager/SceneManager.h"
#include "Events/EventsEventHandler.h"
#include "Actions/ActionManager.h"
#include "Inputs/Input.h"


namespace Application {

    /*--------------------------------------------------------------
      GLOBAL VARAIBLE
    --------------------------------------------------------------*/
    graphicpipe::GraphicsPipe* pipe;
    assetmanager::AssetManager* resManager;
    std::vector<std::string> filePath;

    int Application::Init() {
        
        filePath = Serialization::Serialize::LoadFilePath("../configs");

        /*--------------------------------------------------------------
        INITIALIZE LOGGING SYSTEM
        --------------------------------------------------------------*/
        LOGGING_INIT_LOGS(filePath[0]);
        LOGGING_INFO("Application Start");
        LOGGING_INFO("Load Log Successful");
        logs.m_Setup_Abort_Handler();
        std::signal(SIGABRT, logging::Logger::m_Abort_Handler);

        /*--------------------------------------------------------------
          INITIALIZE WINDOW WIDTH & HEIGHT
       --------------------------------------------------------------*/
        Serialization::Serialize::LoadConfig("../configs");
        LOGGING_INFO("Load Config Successful");

       /*--------------------------------------------------------------
          INITIALIZE OPENGL WINDOW
       --------------------------------------------------------------*/
        lvWindow.init();
        LOGGING_INFO("Load Window Successful");

        /*--------------------------------------------------------------
           INITIALIZE ECS
        --------------------------------------------------------------*/
        ecs::ECS* ecs = ecs::ECS::GetInstance();
        ecs->Load();
        ecs->Init();
        LOGGING_INFO("Load ECS Successful");

        /*--------------------------------------------------------------
           INITIALIZE Asset Manager
        --------------------------------------------------------------*/
        resManager = assetmanager::AssetManager::GetInstance();
        resManager->Init(filePath[1]);
        scenes::SceneManager* scenemanager = scenes::SceneManager::m_GetInstance();
        scenemanager->LoadScene(Helper::Helpers::GetInstance()->startScene);
        LOGGING_INFO("Load Asset Successful");


        /*--------------------------------------------------------------
           INITIALIZE GRAPHICS PIPE
        --------------------------------------------------------------*/
        pipe = graphicpipe::GraphicsPipe::m_funcGetInstance();
        pipe->m_funcInit();
        LOGGING_INFO("Load Graphic Pipline Successful");

        /*--------------------------------------------------------------
           INITIALIZE Input
        --------------------------------------------------------------*/
        //call back must happen before imgui
        Input.SetCallBack(lvWindow.window);
        LOGGING_INFO("Set Input Call Back Successful");

        

        pipe->m_gameMode = true;
        ecs->m_nextState = ecs::START;
        

        LOGGING_INFO("Application Init Successful");
        return 0;
	}



    int Application::Run() {
        Helper::Helpers *help = Helper::Helpers::GetInstance();
        ecs::ECS* ecs = ecs::ECS::GetInstance();
        actions::ActionManager::m_GetManagerInstance();
        //float FPSCapTime = 1.f / help->m_fpsCap;
        double lastFrameTime = glfwGetTime();
        const double fixedDeltaTime = 1.0 / 60.0;
        help->fixedDeltaTime = static_cast<float>(fixedDeltaTime);
        ecs->deltaTime = static_cast<float>(fixedDeltaTime);
        help->accumulatedTime = 0.0;

        

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
                help->deltaTime = static_cast<float>(currentFrameTime - lastFrameTime);
                lastFrameTime = currentFrameTime;
                help->accumulatedTime += (help->deltaTime * help->timeScale);
                Helper::Helpers::GetInstance()->currentNumberOfSteps = 0;
                while (help->accumulatedTime >= (fixedDeltaTime  )) {
                    help->accumulatedTime -= static_cast<float>(fixedDeltaTime);
                    ++help->currentNumberOfSteps;
                }
                
                /*--------------------------------------------------------------
                    UPDATE INPUT
                --------------------------------------------------------------*/
                Input.m_inputUpdate();

                /*--------------------------------------------------------------
                    UPDATE ECS
                --------------------------------------------------------------*/
                //ecs->m_Update(help->m_fixedDeltaTime * help->m_timeScale); 
                ecs->Update(help->fixedDeltaTime * help->timeScale);
                //ecs->m_Update(Helper::Helpers::GetInstance()->m_deltaTime);


                /*--------------------------------------------------------------
                    UPDATE Render Pipeline
                --------------------------------------------------------------*/
                pipe->m_funcUpdate();


                /*--------------------------------------------------------------
                    DRAWING/RENDERING Window
                --------------------------------------------------------------*/
                lvWindow.Draw();



                /*--------------------------------------------------------------
                   Render Game Scene
                --------------------------------------------------------------*/
                pipe->m_funcRenderGameScene();
           

              

                help->fps = 1.f / help->deltaTime;
                

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

        lvWindow.CleanUp();
        glfwTerminate();
        LOGGING_INFO("Application Closed");

        return 0;
	}

}