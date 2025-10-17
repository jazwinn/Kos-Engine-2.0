/********************************************************************/
/*!
\file      Hierachy.h
\author    Ng Jaz winn, jazwinn.ng , 2301502
\par       jazwinn.ng@digipen.edu
\date      Nov 11, 2025
\brief     This header file declares the `Hierachy` class, which manages
		   the parent-child relationships between entities within the ECS framework.
		   - m_SetParent: Sets a parent-child relationship between two entities.
		   - m_RemoveParent: Removes an entity’s parent relationship.
		   - m_GetParent: Retrieves the parent of a specified entity.
		   - m_GetChild: Retrieves the children of a specified entity.
		   - m_UpdateChildScene: Updates the scene of all child entities recursively.

This file enables hierarchical organization of entities, allowing for complex
scene structures and efficient management of entity relationships.

Copyright (C) 2024 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the
prior written consent of DigiPen Institute of Technology is prohibited.
*/
/********************************************************************/

#ifndef HIERACHY_H

#include "ECS.h"

namespace ecs {


	class Hierachy {

	public:

		/******************************************************************/
		/*!
			\fn        void m_SetParent(EntityID parent, EntityID child)
			\brief     Defines a parent-child relationship between two entities.
			\param[in] parent The ID of the parent entity.
			\param[in] child  The ID of the child entity.
			\details   Establishes a hierarchical relationship, setting the specified
					   entity as a child of the parent entity.
		*/
		/******************************************************************/
		static void m_SetParent(EntityID parent, EntityID child);

		/******************************************************************/
		/*!
			\fn        void m_RemoveParent(EntityID child)
			\brief     Removes the parent of a specified child entity.
			\param[in] child The ID of the child entity.
			\details   Detaches the child entity from its parent, if any, and updates
					   the parent’s child list accordingly.
		*/
		/******************************************************************/
		static void m_RemoveParent(EntityID child);

		/******************************************************************/
		/*!
			\fn        std::optional<EntityID> m_GetParent(EntityID child)
			\brief     Retrieves the parent of a specified entity, if one exists.
			\param[in] child The ID of the child entity.
			\return    An optional containing the parent entity ID, or nullopt if none.
			\details   Allows querying of an entity's parent within the hierarchy.
		*/
		/******************************************************************/
		static std::optional<EntityID> GetParent(EntityID child);

		/******************************************************************/
		/*!
			\fn        std::optional<std::vector<EntityID>> m_GetChild(EntityID parent)
			\brief     Retrieves a list of children for a specified parent entity.
			\param[in] parent The ID of the parent entity.
			\return    An optional vector containing the IDs of the child entities, or nullopt if none.
			\details   Allows querying of an entity's children within the hierarchy.
		*/
		/******************************************************************/
		static std::optional<std::vector<EntityID>> m_GetChild(EntityID parent);

		/******************************************************************/
		/*!
			\fn        void m_UpdateChildScene(EntityID parent)
			\brief     Recursively updates the scene association of a parent entity and its children.
			\param[in] parent The ID of the parent entity whose scene is to be propagated.
			\details   Ensures all child entities belong to the same scene as the parent by
					   recursively updating scene associations.
		*/
		/******************************************************************/
		static void m_UpdateChildScene(EntityID parent);

	};
}

#define HIERACHY_H
#endif HIREACHY_H

