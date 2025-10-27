/******************************************************************/
/*!
\file      Window.h
\author    Sean Tiu
\par       s.tiu@digipen.edu
\date      2nd Oct, 2024
\brief

           This file contains the declaration of the `AppWindow` class,
           which is responsible for managing the main application window,
           including window creation, rendering, and cleanup.

           The `AppWindow` class utilizes GLFW for creating the window
           and OpenGL for rendering, along with ImGui for rendering the
           graphical user interface. The class provides methods for
           initializing the window, handling the drawing loop, and cleaning up
           resources when the window is closed.

Copyright (C) 2024 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the
prior written consent of DigiPen Institute of Technology is prohibited.
*/
/******************************************************************/


#ifndef APPWINDOW_H
#define APPWINDOW_H

#pragma once
#include <GLFW/glfw3.h>
#include "Inputs/Input.h"


namespace Application {

    /**
     * @brief Represents the application window and its associated properties.
     *
     * The `AppWindow` class is responsible for managing the main application window,
     * including initialization, drawing, and cleanup.
     */
    class AppWindow {

    public:

        /**
         * @brief Initializes the window, context, and necessary components.
         *
         * This function sets up the GLFW window and OpenGL context, prepares
         * the rendering pipeline, and initializes ImGui for UI handling.
         *
         * @return int 0 if initialization is successful, or a negative error code on failure.
         */
        int init();
        /**
         * @brief Handles the drawing loop of the window.
         *
         * This function handles the rendering loop, managing the OpenGL draw
         * calls and ImGui frame rendering. The loop continues until the window is closed.
         *
         * @return int 0 if drawing completed successfully, or a negative error code on failure.
         */
        int Draw();

        /**
         * @brief Cleans up resources and terminates the application window.
         *
         * This function releases any allocated resources, terminates the OpenGL
         * context, and performs other cleanup operations before the application exits.
         *
         * @return int 0 if cleanup is successful, or a negative error code on failure.
         */
        int CleanUp();

        /**
         * @brief Pointer to the GLFW window.
         *
         * This represents the handle to the application window created using GLFW.
         */
        GLFWwindow* window;

        /**
         * @brief Pointer to the GLFW monitor.
         *
         * This points to the primary monitor where the window is displayed.
         */
        static GLFWmonitor* monitor;

        /**
         * @brief Pointer to the GLFW video mode structure.
         *
         * This contains details about the display's resolution, refresh rate, and color depth.
         */
        static const GLFWvidmode* mode;

        /**
         * @brief Height of the application window in pixels.
         */
        static float windowHeight;

        /**
         * @brief Width of the application window in pixels.
         */
        static float windowWidth;

        static bool fullScreen;

        static GLFWcursor* currCursor;

        void setCursorImage(const std::string& image, bool centered);

        bool enabledFullScreen{ true };//use this to set launch application fullscreen or not

    };
}

#endif // APPWINDOW_H