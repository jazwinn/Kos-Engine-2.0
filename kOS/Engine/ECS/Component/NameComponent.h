/******************************************************************/
/*!
\file      NameComponent.h
\author    Ng Jaz winn, jazwinn.ng , 2301502
\par       jazwinn.ng@digipen.edu
\date      Oct 02, 2024
\brief     This file contains data for the name componetn


Copyright (C) 2024 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the
prior written consent of DigiPen Institute of Technology is prohibited.
*/
/********************************************************************/
#ifndef NAMECOM_H
#define NAMECOM_H

#include "Component.h"
#include "ECS/Layers.h"


namespace ecs {

	/******************************************************************/
	/*!
	\class     NameComponent
	\brief    Contains the entity's name
	*/
	/******************************************************************/
	class NameComponent : public Component {

	public:

		bool hide{ false };

		bool isPrefab{ false };

		bool syncPrefab{ false };

		layer::LAYERS Layer{ layer::DEFAULT };

		std::string entityName;

		std::string entityTag = "Default";

		std::string prefabName{};

		REFLECTABLE(NameComponent, entityName, Layer, entityTag, isPrefab, prefabName, hide);
	};

}




#endif NAMECOM_H