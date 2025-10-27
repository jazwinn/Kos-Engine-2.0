/******************************************************************/
/*!
\file      imgui_play_pauce.cpp
\author    Jaz Winn Ng
\par       jazwinn.ng@digipen.edu
\date      Sept 28, 2025
\brief     Displays the play, pause, and stop buttons in the editor using ImGui.
			- DrawPlayPauseBar: Renders the play, pause, and stop buttons, managing game state transitions and audio control.

Copyright (C) 2025 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the
prior written consent of DigiPen Institute of Technology is prohibited.
*/
/******************************************************************/

#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"
#include "Editor.h"
#include "Resources/ResourceManager.h"
#include "Scene/SceneManager.h"

#include "Windows.h"
#include <thread>
#include "Scripting/ScriptManager.h"

namespace gui {


    void DrawLoading() {

    }

    void CloseLoadingWindow(HWND g_hwnd) {
        if (g_hwnd) {
            PostMessage(g_hwnd, WM_CLOSE, 0, 0);
        }
    }
	
    void ImGuiHandler::DrawPlayPauseBar() {
        static bool pause = true;
        ecs::ECS* ecs = ecs::ECS::GetInstance();
        if (ecs->GetState() == ecs::WAIT) {
            pause = true;
        }

        
        if (ImGui::BeginMenuBar()) {
            if (pause && ImGui::Button("Play")) {
                pause = false;
                ecs::AudioSystem::SetPaused(false);

                ecs->SetState((ecs->GetState() == ecs::STOP) ? ecs::START : ecs::RUNNING);
                if (ecs->GetNextState() == ecs::START) {

           






                }
            }
            else if (!pause && ImGui::Button("Pause")) {

                pause = true;
                ecs->SetState(ecs::WAIT);
                ecs::AudioSystem::SetPaused(true);


            }

            if ((ecs->GetState() == ecs::RUNNING || ecs->GetState() == ecs::WAIT)&& ImGui::Button("stop")) {
                if (ecs->GetState() != ecs::STOP) {
                    ecs->SetState(ecs::STOP);
                    pause = true;

                    ecs::AudioSystem::StopAll();
                    ecs::AudioSystem::SetPaused(false);

                    //stop all scene states
                    std::unordered_map<std::string, bool> saveStateScene;

                    for (const auto& scene : ecs->sceneMap) {
                        saveStateScene[scene.first] = scene.second.isActive;
                    }

                    //TODO load back scene
                    scenes::SceneManager* scenemanager = scenes::SceneManager::m_GetInstance();
                    scenemanager->ReloadScene();
                    m_clickedEntityId = -1;

                    //set back all scene to its active/inactive state
                    for (auto& scene : ecs->sceneMap) {
                        const auto& saveState = saveStateScene.find(scene.first);
                        if (saveState == saveStateScene.end())continue;

                        scene.second.isActive = saveState->second;
                    }

                    

                    //assetmanager::AssetManager::GetInstance()->m_audioManager.m_StopAllSounds();
                }

            }

        }
        ImGui::EndMenuBar();
    }
}