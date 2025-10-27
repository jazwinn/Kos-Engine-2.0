/******************************************************************/
/*!
\file      Layers.h
\author    Jaz Winn Ng
\par       jazwinn.ng@digipen.edu
\date      Sept 28, 2025
\brief	   Layers class manages different rendering layers within the ECS framework.
			- m_ChangeLayerName: Renames a specified layer.
			- m_SwapEntityLayer: Moves an entity from one layer to another.
			- m_DisableLayer: Disables rendering for a specified layer.
			- m_EnableLayer: Enables rendering for a specified layer.
			- m_IsLayerVisable: Checks if a specified layer is currently visible.
			- m_RetrieveEntityID: Retrieves all entity IDs within a specified layer.
			- m_hideEntitywithChild: Hides an entity and all its child entities.
			- m_unhideEntitywithChild: Unhides an entity and all its child entities.

Copyright (C) 2025 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the
prior written consent of DigiPen Institute of Technology is prohibited.
*/
/******************************************************************/

#ifndef LAYERS_H
#define LAYERS_H

#include "ECSList.h"

namespace layer{

	enum LAYERS {
		DEFAULT,
		LAYER1,
		LAYER2,
		LAYER3,
		LAYER4,
		LAYER5,
		LAYER6,
		LAYER7,
		LAYER8,

		MAXLAYER
	};

	class LayerStack {
	
	public:


		using layermap = std::unordered_map<LAYERS, std::pair<std::string,std::vector<ecs::EntityID>>>;

		/******************************************************************/
		/*!
			\fn        LayerStack::LayerStack()
			\brief     Constructs the `LayerStack` and initializes the default layers.
			\details   Sets up predefined layers (DEFAULT through LAYER8) with default names,
					   creating an organized starting point for layer management.
		*/
		/******************************************************************/
		LayerStack();// set all bits to true

		/******************************************************************/
		/*!
			\fn        void m_ChangeLayerName(LAYERS layer, std::string newName)
			\brief     Renames a specified layer.
			\param[in] layer The enum value of the layer to rename.
			\param[in] newName The new name for the layer.
			\details   Updates the specified layers name, if the layer exists.
		*/
		/******************************************************************/
		void m_ChangeLayerName(LAYERS layer, std::string newName);

		/******************************************************************/
		/*!
			\fn        bool m_SwapEntityLayer(LAYERS newlayer, LAYERS oldlayer, ecs::EntityID id)
			\brief     Moves an entity from one layer to another.
			\param[in] newlayer The layer to move the entity to.
			\param[in] oldlayer The layer from which to move the entity.
			\param[in] id       The ID of the entity to move.
			\return    True if the entity was successfully moved; otherwise, false.
			\details   Transfers the entity ID between layers and updates its layer attribute.
		*/
		/******************************************************************/
		bool m_SwapEntityLayer(LAYERS newlayer, LAYERS oldlayer, ecs::EntityID);

		/******************************************************************/
		/*!
		\fn      void m_DisableLayer(LAYERS layer)
		\brief   Disables rendering for the specified layer.
		\param   layer - The layer to be disabled.
		*/
		/******************************************************************/
		void m_DisableLayer(LAYERS layer);

		/******************************************************************/
		/*!
		\fn      void m_EnableLayer(LAYERS layer)
		\brief   Enables rendering for the specified layer.
		\param   layer - The layer to be enabled.
		*/
		/******************************************************************/
		void m_EnableLayer(LAYERS layer);

		/******************************************************************/
		/*!
		\fn      bool m_IsLayerVisable(int layer)
		\brief   Checks whether the specified layer is currently visible.
		\param   layer - The layer index to check.
		\return  True if the layer is visible, false otherwise.
		*/
		/******************************************************************/
		bool m_IsLayerVisable(int layer);

		/******************************************************************/
		/*!
			\fn        std::vector<ecs::EntityID> m_RetrieveEntityID(LAYERS layer)
			\brief     Retrieves all entity IDs within a specified layer.
			\param[in] layer The layer to retrieve entities from.
			\return    A vector containing the entity IDs in the specified layer, or an empty
					   vector if the layer does not exist.
			\details   Allows access to all entities within a specific layer.
		*/
		/******************************************************************/
		std::vector<ecs::EntityID> m_RetrieveEntityID(LAYERS layer);

		/******************************************************************/
		/*!
		\fn      void m_hideEntitywithChild(ecs::EntityID id)
		\brief   Hides the specified entity along with all of its child entities.
		\param   id - The ID of the parent entity to be hidden.
		*/
		/******************************************************************/
		void m_hideEntitywithChild(ecs::EntityID id);

		/******************************************************************/
		/*!
		\fn      void m_unhideEntitywithChild(ecs::EntityID id)
		\brief   Unhides the specified entity along with all of its child entities.
		\param   id - The ID of the parent entity to be unhidden.
		*/
		/******************************************************************/
		void m_unhideEntitywithChild(ecs::EntityID id);


		//bool m_CreateLayer(layer);

		//bool m_DeleteLayer(layer);

	public:
		layermap m_layerMap;

		// layers that are activated
		std::bitset<MAXLAYER> m_layerBitSet;
	};
}
#endif LAYERS_H