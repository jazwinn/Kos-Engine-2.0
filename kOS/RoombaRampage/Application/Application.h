/******************************************************************/
/*!
\file      Application.h
\author    Ng Jaz winn, jazwinn.ng , 2301502
\par       jazwinn.ng@digipen.edu
\date      Oct 02, 2024
\brief     This file contains the declaration to the main update 
		   loop of the engine. It initializes the various major 
		   systems and call upon them in the update loop.

Copyright (C) 2024 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the
prior written consent of DigiPen Institute of Technology is prohibited.
*/
/********************************************************************/
#ifndef APP_H
#define APP_H

#include "Debugging/Logging.h"
#include "ECS/ECS.h"
#include "Inputs/Input.h"
#include "Helper/Helper.h"
#include "Window.h"


#include "C#Mono/mono_handler.h"
#include "Events/EventsEventHandler.h"


/******************************************************************/
/*!
\class     Application
\brief     The Papplciation class is responsible for the entire engine.
		   It calls onto all the systems.
*/
/******************************************************************/
namespace Application {



	class Application {

	public:

		/******************************************************************/
		/*!
		\fn        Init()
		\brief     Initialzie all systems, managers and pipeline
		*/
		/******************************************************************/
		int Init();

		/******************************************************************/
		/*!
		\fn        Run()
		\brief     Contains the main update loop, calls onto all the programs
				   update functions.
		*/
		/******************************************************************/
		int Run();
		/******************************************************************/
		/*!
		\fn        m_Cleanup()
		\brief     Calls upon the closing of the application. Calls the cleanup
				   functions of all the other systems.
		*/
		/******************************************************************/
		int m_Cleanup();


	private:

		AppWindow lvWindow;
		Input::InputSystem Input;
		
		logging::Logger logs;
	};
}


#endif APP_H


