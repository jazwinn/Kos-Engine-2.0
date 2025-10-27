/******************************************************************/
/*!
\file      Window.cpp
\author    Sean Tiu
\par       s.tiu@digipen.edu
\date      2nd Oct, 2024
\brief

           This file contains the implementation of the `AppWindow` class
           methods, which handle the window lifecycle including initialization,
           drawing/rendering loop, and cleanup.

           The `init` method sets up GLFW and OpenGL contexts for rendering,
           while the `Draw` method manages the rendering loop including
           ImGui interface rendering. The `CleanUp` method is responsible
           for releasing resources used during the window’s operation.

           The application window makes use of GLFW for window and context
           management, and OpenGL for rendering. ImGui is integrated to
           provide a graphical interface within the application.

Copyright (C) 2024 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the
prior written consent of DigiPen Institute of Technology is prohibited.
*/
/******************************************************************/

#include "Window.h"
#include "Helper/Helper.h"
#include "Graphics/GraphicsPipe.h"
#include "ECS/ECS.h"
#include "Inputs/Input.h"

#include <stb_image.h>

namespace Helper {
    std::unique_ptr<Helpers> Helpers::InstancePtr = nullptr;
}

namespace Application {

    float AppWindow::windowHeight;

    float AppWindow::windowWidth;

    bool AppWindow::fullScreen{ true };

    const GLFWvidmode* AppWindow::mode;

    GLFWmonitor* AppWindow::monitor;

    void SetWindowIcon(GLFWwindow* window) {
        GLFWimage icon;

        // Load image (ensure your path is correct)
        icon.pixels = stbi_load("../Configs/icon.png", &icon.width, &icon.height, 0, 4);
        if (!icon.pixels) {
            printf("Failed to load icon!\n");
            return;
        }

        // Set icon
        glfwSetWindowIcon(window, 1, &icon);

        // Free image data after setting the icon
        stbi_image_free(icon.pixels);
    }

    static void windowedFocusCallback([[maybe_unused]] GLFWwindow* window, int focused)
    {

        auto& audioManager = assetmanager::AssetManager::m_funcGetInstance()->m_audioManager;
        ecs::ECS* ecs = ecs::ECS::GetInstance();
        if (!focused) {

            audioManager.m_PauseAllSounds();  // Pause all sounds

            if (ecs->GetState() == ecs::RUNNING) {
                //std::cout << "Window minimized!" << std::endl;
                ecs::ECS::GetInstance()->SetState(ecs::WAIT);
                Helper::Helpers::GetInstance()->windowMinimise = true;
            }
        }
        else {

            audioManager.m_UnpauseAllSounds();  // Unpause all sounds

            if (ecs->GetState() == ecs::WAIT) {
                //std::cout << "Window restored!" << std::endl;
                ecs::ECS::GetInstance()->SetState(ecs::RUNNING);
                Helper::Helpers::GetInstance()->windowMinimise = false;
            }
        }

    }


    static void fullScreenFocusCallback(GLFWwindow* window, int focused)
    {
        static int oldWidth = static_cast<int>(AppWindow::windowWidth);
        static int oldHeight = static_cast<int>(AppWindow::windowHeight);
        auto& audioManager = assetmanager::AssetManager::m_funcGetInstance()->m_audioManager;
        ecs::ECS* ecs = ecs::ECS::GetInstance();
        if (!focused) {
            oldWidth = static_cast<int>(Helper::Helpers::GetInstance()->m_windowWidth);
            oldHeight = static_cast<int>(Helper::Helpers::GetInstance()->m_windowHeight);

            // If the window loses focus, set it to windowed mode
            glfwSetWindowMonitor(window, nullptr, 100, 100, static_cast<int>(AppWindow::windowWidth), static_cast<int>(AppWindow::windowHeight), 0);  // Change to windowed mode with a standard resolution
            AppWindow::fullScreen = false;

            audioManager.m_PauseAllSounds();  // Pause all sounds

            if (ecs->GetState() == ecs::RUNNING) {
                //std::cout << "Window minimized!" << std::endl;
                ecs::ECS::GetInstance()->SetState(ecs::WAIT);
                Helper::Helpers::GetInstance()->m_windowMinimise = true;
            }
        }
        else if (!AppWindow::fullScreen) {
            // If the window regains focus, switch back to full screen
            glfwSetWindowMonitor(window, AppWindow::monitor, 0, 0, AppWindow::mode->width, AppWindow::mode->height, AppWindow::mode->refreshRate);
            AppWindow::fullScreen = true;

            audioManager.m_UnpauseAllSounds();  // Unpause all sounds

            if (ecs->GetState() == ecs::WAIT) {
                //std::cout << "Window restored!" << std::endl;
                ecs::ECS::GetInstance()->SetState(ecs::RUNNING);
                Helper::Helpers::GetInstance()->m_windowMinimise = false;
            }
        }

    }

    static void iconifyCallback([[maybe_unused]]GLFWwindow* window, int iconified)
    {
        auto& audioManager = assetmanager::AssetManager::m_funcGetInstance()->m_audioManager;
        ecs::ECS* ecs = ecs::ECS::GetInstance();
        if (iconified == GLFW_TRUE)
        {
            
            audioManager.m_PauseAllSounds();  // Pause all sounds

            if (ecs->GetState() == ecs::RUNNING) {
                //std::cout << "Window minimized!" << std::endl;
                ecs::ECS::GetInstance()->SetState(ecs::WAIT);
                Helper::Helpers::GetInstance()->m_windowMinimise = true;
            }
        }
        else
        {
            
            audioManager.m_UnpauseAllSounds();  // Unpause all sounds

            if (ecs->GetState() == ecs::WAIT) {
                //std::cout << "Window restored!" << std::endl;
                ecs::ECS::GetInstance()->SetState(ecs::RUNNING);
                Helper::Helpers::GetInstance()->windowMinimise = false;
            }
        }
    }

	int AppWindow::init(){
        /* Initialize the library */
        if (!glfwInit())
            return -1;

        // Create a window based on the current screen size
        windowWidth = Helper::Helpers::GetInstance()->m_windowWidth;
        windowHeight = Helper::Helpers::GetInstance()->m_windowHeight;
        
        //Set Context Version
        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
        glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);






        /* Create a windowed mode window and its OpenGL context */

        monitor = glfwGetPrimaryMonitor();
        mode = glfwGetVideoMode(monitor);
        window = glfwCreateWindow(static_cast<int>(windowWidth), static_cast<int>(windowHeight), "R00M13A-OS", enabledFullScreen ?monitor : NULL , NULL);

        Input::InputSystem::m_windowInput = window;
        if (!window)
        {
            glfwTerminate();
            return -1;
        }
        //set call back
        if(enabledFullScreen) glfwSetWindowFocusCallback(window, fullScreenFocusCallback);
        glfwSetWindowIconifyCallback(window, iconifyCallback);

        /* Make the window's context current */
        glfwMakeContextCurrent(window);

        /* Only initialize GLEW after defining OpenGL context*/
        if (glewInit() != GLEW_OK)
        {
            LOGGING_ERROR("Failed to initialize GLEW");
            return -1;
        }

        //create icon
        SetWindowIcon(window);

        
        return 0;
	}




	int AppWindow::Draw() {



        if ((Input::InputSystem::m_isKeyPressed(keys::LeftAlt) || Input::InputSystem::m_isKeyPressed(keys::RightAlt)) && Input::InputSystem::m_isKeyTriggered(keys::ENTER)) {
            if (enabledFullScreen) {
                glfwSetWindowFocusCallback(window, windowedFocusCallback);
                glfwSetWindowMonitor(window, nullptr, 100, 100, static_cast<int>(AppWindow::windowWidth), static_cast<int>(AppWindow::windowHeight), 0);
                enabledFullScreen = false;
            }
            else {
                glfwSetWindowFocusCallback(window, fullScreenFocusCallback);
                glfwSetWindowMonitor(window, AppWindow::monitor, 0, 0, AppWindow::mode->width, AppWindow::mode->height, AppWindow::mode->refreshRate);

                enabledFullScreen = true;
            }
        }


        Helper::Helpers *help = Helper::Helpers::GetInstance();
        glClear(GL_COLOR_BUFFER_BIT);
        graphicpipe::GraphicsPipe* pipe = graphicpipe::GraphicsPipe::m_funcGetInstance();
        int display_w, display_h;
        glfwGetFramebufferSize(window, &display_w, &display_h);
        
        constexpr float targetAspect = 16.0f / 9.0f; // Lock game to this display ratio

        static GLint old_w{}, old_h{};
        static GLint curr_w{}, curr_h{};
        static GLint oldOverall_W{}, oldOverall_H{};

        if (curr_w != display_w || curr_h != display_h)
        {
            help->m_currWindowHeight = display_h;
            help->m_currWindowWidth = display_w;
            curr_w = display_w;
            curr_h = display_h;
        }
        // update viewport settings in vps only if window's dimension change
   
        if (display_h != 0)
        {
            float currAspect = static_cast<float>(static_cast<float>(display_w) / static_cast<float>(display_h));
            if (currAspect < targetAspect)
            {
                display_h = static_cast<int>(static_cast<float>(display_w) / targetAspect);
            }
            else if (currAspect > targetAspect)
            {
                display_w = static_cast<int>(static_cast<float>(display_h) * targetAspect);
            }
        }

           

            ///Do resolution handling here


        help->m_windowHeight = static_cast<float>(display_h);
        help->m_windowWidth = static_cast<float>(display_w);

        help->m_windowScalar = help->m_windowHeight / 1080.f;

        if ((old_w != help->m_windowWidth || old_h != help->m_windowHeight || curr_w != oldOverall_W || curr_h != oldOverall_H) && (help->m_windowHeight > 0 || help->m_windowWidth > 0))
        {
            oldOverall_W = curr_w;
            oldOverall_H = curr_h;
            old_w = static_cast<int>(help->m_windowWidth);
            old_h = static_cast<int>(help->m_windowHeight);
            pipe->m_funcSetupFrameBuffer();
            pipe->m_funcSetupGamePreviewFrameBuffer();
            pipe->m_funcSetupMultiLightingFrameBuffer();
            pipe->m_funcSetupCRTFrameBuffer();
            pipe->m_funcSetupAdditiveLightingFrameBuffer();
            pipe->m_funcSetupFinalPassBuffer();
            pipe->m_funcSetUpUnlitScreenFrameBuffer();
           
            int xOffsett = (curr_w - old_w) / 2;
            int yOffsett = (curr_h - old_h) / 2;

            help->m_viewportOffsetX = xOffsett;
            help->m_viewportOffsetY = yOffsett;

            glViewport(xOffsett, yOffsett, old_w, old_h);
            //glScissor(0, 0, curr_w, curr_h);

            //glViewport(0, 0, old_w, old_h);
            //glScissor(0, 0, old_w, old_h);
            glClearColor(0, 0, 0, 1.f);
           
        }
            
        glClearColor(0, 0, 0, 1.f);
       // glClearColor(static_cast<GLclampf>(help->m_colour.m_x * pipe->m_globalLightIntensity), static_cast<GLclampf>(help->m_colour.m_y * pipe->m_globalLightIntensity), static_cast<GLclampf>(help->m_colour.m_z * pipe->m_globalLightIntensity), static_cast<GLclampf>(1));
        glClear(GL_COLOR_BUFFER_BIT);

        static std::string oldCursor{ "default" };
        static bool mouseCenterCheck{ false };

        if (help->m_currMousePicture != oldCursor || help->m_isMouseCentered != mouseCenterCheck)
        {
            oldCursor = help->m_currMousePicture;
            mouseCenterCheck = help->m_isMouseCentered;
            setCursorImage(help->m_currMousePicture, help->m_isMouseCentered);
        }

        if (help->m_closeWindow) {
            glfwSetWindowShouldClose(window, GLFW_TRUE);
        }

        return 0;
	}

	int AppWindow::CleanUp() {

        glfwDestroyWindow(window);
        return 0;
	}

    void AppWindow::setCursorImage(const std::string& imageFile, bool centered)
    {

        static const char* cursorOptions[] =
        {
            "default",
            "../Assets/Texture/UI/img_startScreenCursor.png",
            "../Assets/Texture/UI/img_inGameCursor.png"
        };


        //check if image file belong to any of the above, ELSE ERROR OCCUR
        std::string test{};
        for (auto cursor : cursorOptions) {
            if (cursor == imageFile) {
                test = imageFile;
                break;
            }

        }



        if (imageFile.empty() || imageFile == "default" || test.empty()) {
            // Set default cursor
            GLFWcursor* defaultCursor = glfwCreateStandardCursor(GLFW_ARROW_CURSOR);
            if (!defaultCursor) {
                LOGGING_ERROR("Failed to create default GLFW cursor.");
                return;
            }
            glfwSetCursor(AppWindow::window, defaultCursor);
            return;
        }

        stbi_set_flip_vertically_on_load(false);

        // Load custom cursor image
        GLFWimage image;
        image.pixels = stbi_load(imageFile.c_str(), &image.width, &image.height, 0, 4);

        if (!image.pixels) {
            LOGGING_ERROR("Cursor Image Loading Failed: Recheck file path (" + imageFile + ")");

            // Fall back to default cursor
            GLFWcursor* defaultCursor = glfwCreateStandardCursor(GLFW_ARROW_CURSOR);
            if (!defaultCursor) {
                LOGGING_ERROR("Failed to create default GLFW cursor.");
                return;
            }
            glfwSetCursor(AppWindow::window, defaultCursor);
            return;
        }

        // Create custom cursor
        GLFWcursor* customCursor = glfwCreateCursor(&image, centered ? image.width / 2 : 0, centered ? image.height / 2 : 0);

        if (!customCursor) {
            LOGGING_ERROR("Cursor Creation Failed: Using default cursor instead.");
            stbi_image_free(image.pixels);

            // Fall back to default cursor
            GLFWcursor* defaultCursor = glfwCreateStandardCursor(GLFW_ARROW_CURSOR);
            if (!defaultCursor) {
                LOGGING_ERROR("Failed to create default GLFW cursor.");
                return;
            }
            glfwSetCursor(AppWindow::window, defaultCursor);
            return;
        }

        glfwSetCursor(AppWindow::window, customCursor);
        stbi_image_free(image.pixels);

        stbi_set_flip_vertically_on_load(true);
    }

  

}