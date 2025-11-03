/******************************************************************/
/*!
\file      Input.cpp
\author    Elijah Teo, teo.e , 2301530
\par       teo.e@digipen.edu
\date      16 Sept, 2024
\brief     defines the class functions that contains all the callback functions that will be given to GLFW along with variables to hold the return values

The header provides declarations for the Performance class functions that
handle performance tracking and output for various engine systems such as
movement, rendering, and collision.

Copyright (C) 2024 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the
prior written consent of DigiPen Institute of Technology is prohibited.
*/
/******************************************************************/

#include "Config/pch.h"
#include "Input.h"
#include "Events/EventHandler.h"  
#include "Events/InputEvents.h" 

namespace Input {
	/*--------------------------------------------------------------
	  GLOBAL VARAIBLE
	--------------------------------------------------------------*/

	// The number of frames needed for a button to be pressed before the state turns from triggered to pressed.
	float secondsBeforePressed = 0.05f;

	std::shared_ptr<InputSystem> InputSystem::m_InstancePtr = nullptr;
	// Shared pointer
	//std::shared_ptr<InputSystem> InputSystem::GetInstance(){ std::make_shared<InputSystem>(InputSystem{}) };
	
	int test2 = 10;
	int* test1 = &test2;

	// UPDATED: KeyCallback now emits events
	void KeyCallback([[maybe_unused]] GLFWwindow* window, int key, [[maybe_unused]] int scancode, int action, [[maybe_unused]] int mods) {
		auto* eventHandler = Event::EventHandler::GetInstance();

		if (action == GLFW_PRESS) {
			eventHandler->GetBus().Emit(ecs::KeyPressedEvent(key, false));
		}
		else if (action == GLFW_RELEASE) {
			eventHandler->GetBus().Emit(ecs::KeyReleasedEvent(key));
		}
		else if (action == GLFW_REPEAT) {
			eventHandler->GetBus().Emit(ecs::KeyPressedEvent(key, true));
		}
	}


	void MouseButtonCallback([[maybe_unused]] GLFWwindow* pwin, int button, int action, [[maybe_unused]] int mod) {
		auto* eventHandler = Event::EventHandler::GetInstance();

		// Get current mouse position
		glm::vec2 mousePos = InputSystem::GetInstance()->GetMousePos();

		if (action == GLFW_PRESS) {
			eventHandler->GetBus().Emit(
				ecs::MouseButtonPressedEvent(button, mousePos.x, mousePos.y)
			);
		}
		else if (action == GLFW_RELEASE) {
			eventHandler->GetBus().Emit(
				ecs::MouseButtonReleasedEvent(button, mousePos.x, mousePos.y)
			);
		}
	}

	void CursorPosCallback(GLFWwindow* window, double xpos, double ypos) {
		int width{}, height{};

		glfwGetWindowSize(window, &width, &height);
		ypos = static_cast<double>(height - ypos);

		// Get old position before updating
		glm::vec2 oldPos = InputSystem::GetInstance()->currentMousePos;

		// Update current position
		InputSystem::GetInstance()->currentMousePos.x = static_cast<float>(xpos);
		InputSystem::GetInstance()->currentMousePos.y = static_cast<float>(ypos);

		// Calculate delta
		float deltaX = static_cast<float>(xpos) - oldPos.x;
		float deltaY = static_cast<float>(ypos) - oldPos.y;

		// Emit mouse moved event
		auto* eventHandler = Event::EventHandler::GetInstance();
		eventHandler->GetBus().Emit(
			ecs::MouseMovedEvent(
				static_cast<float>(xpos),
				static_cast<float>(ypos),
				deltaX,
				deltaY
			)
		);
	}

	void WindowResizeCallback(GLFWwindow* window, int width, int height) {
		auto* eventHandler = Event::EventHandler::GetInstance();
		eventHandler->GetBus().Emit(ecs::WindowResizedEvent(width, height));
	}

	void ScrollCallback(GLFWwindow* window, double xoffset, double yoffset) {
		auto* eventHandler = Event::EventHandler::GetInstance();
		eventHandler->GetBus().Emit(
			ecs::MouseScrolledEvent(static_cast<float>(xoffset), static_cast<float>(yoffset))
		);
	}

	void DropCallback([[maybe_unused]] GLFWwindow* window, int count, const char** paths) {
		InputSystem::GetInstance()->droppedFiles.clear();

		for (int i = 0; i < count; ++i) {
			InputSystem::GetInstance()->droppedFiles.emplace_back(paths[i]);
		}
	}

	void InputSystem::SetCallBack(GLFWwindow* window) {
		glfwSetKeyCallback(window, KeyCallback);
		glfwSetDropCallback(window, DropCallback);
		glfwSetMouseButtonCallback(window, MouseButtonCallback);
		glfwSetCursorPosCallback(window, CursorPosCallback);
		glfwSetWindowSizeCallback(window, WindowResizeCallback);  
		glfwSetScrollCallback(window, ScrollCallback);          
	}

	void InputSystem::HideCursor(bool check) {
		if (check) {
			glfwSetInputMode(inputWindow, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
		}
		else {
			glfwSetInputMode(inputWindow, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
		}

	}

	void InputSystem::InputInit(GLFWwindow* window) {
		InputSystem::GetInstance()->inputWindow = window;
	}

	void InputSystem::InputUpdate(float deltaTime) {
		auto* eventHandler = Event::EventHandler::GetInstance();

		for (std::pair<const int, Key>& key : keysRegistered) {
			int state;

			if (key.first == keys::LMB || key.first == keys::RMB || key.first == keys::MMB) {
				state = glfwGetMouseButton(InputSystem::GetInstance()->inputWindow, key.first);
			}
			else {
				state = glfwGetKey(InputSystem::GetInstance()->inputWindow, key.first);
			}

			// Update all prev and curr key states first
			key.second.prevKeyState = key.second.currKeyState;
			key.second.currKeyState = KeyState::UNUSED;

			// Current checks
			if (state == GLFW_PRESS) {
				if (key.second.prevKeyState == KeyState::UNUSED) {
					if (!key.second.currPressedTimer) {
						key.second.currKeyState = KeyState::TRIGGERED;
						// EMIT KEY PRESSED EVENT
						eventHandler->GetBus().Emit(ecs::KeyPressedEvent(key.first, false));
					}
					else {
						key.second.currKeyState = KeyState::WAITING;
					}

					key.second.currPressedTimer += deltaTime;
				}

				if (key.second.currPressedTimer >= secondsBeforePressed) {
					key.second.currKeyState = KeyState::PRESSED;
					// Could emit a "key held" event here if needed
				}
			}
			else if (state == GLFW_RELEASE) {
				if (key.second.currPressedTimer) {
					key.second.currKeyState = KeyState::RELEASED;
					// EMIT KEY RELEASED EVENT
					eventHandler->GetBus().Emit(ecs::KeyReleasedEvent(key.first));
					key.second.currPressedTimer = 0;
				}
			}
		}
	}

	void InputSystem::InputExitFrame(float deltaTime) {
		InputSystem::GetInstance()->prevMousePos = InputSystem::GetInstance()->currentMousePos;
	}

	bool InputSystem::IsKeyTriggered(const keyCode key) {
		if (keysRegistered[key].currKeyState == KeyState::TRIGGERED) {
			return true;
		}

		return false;
	}

	bool InputSystem::IsKeyPressed(const keyCode key) {
		if (keysRegistered[key].currKeyState == KeyState::PRESSED) {
			return true;
		}

		return false;
	}

	bool InputSystem::IsKeyReleased(const keyCode key) {
		if (keysRegistered[key].currKeyState == KeyState::RELEASED) {
			return true;
		}

		return false;
	}

	glm::vec2 InputSystem::GetMousePos() {
		return InputSystem::currentMousePos;
	}

	float InputSystem::GetAxisRaw(std::string axisType) {
		if (axisType == "Mouse X") {
			//std::cout << currentMousePos.x << ", " << currentMousePos.y << std::endl;
			//std::cout << prevMousePos.x << ", " << prevMousePos.y << std::endl;
			glm::vec2 delta = currentMousePos - prevMousePos;
			delta.x = glm::length2(delta) <= 0.01f ? 0.f : delta.x;
			return (delta.x * 0.1f);
		}
		else if (axisType == "Mouse Y") {
			glm::vec2 delta = currentMousePos - prevMousePos;
			delta.y = glm::length2(delta) <= 0.01f ? 0.f : delta.y;
			return (delta.y * 0.1f);
		}
		else {
			return 0.f;
		}
	}
}