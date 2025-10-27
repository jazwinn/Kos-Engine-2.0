/******************************************************************/
/*!
\file      ECS.cpp
\author    Ng Jaz winn, jazwinn.ng , 2301502
\par       jazwinn.ng@digipen.edu
\date      8 Nov, 2024
\brief     This header file defines the `LayerStack` class and associated
		   enums for handling layer management in the ECS framework.
		   - LayerStack: Manages named layers that contain entities.
		   - m_ChangeLayerName: Renames a specified layer.
		   - m_SwapEntityLayer: Moves an entity from one layer to another.
		   - m_RetrieveEntityID: Retrieves all entity IDs within a specified layer.

This file allows flexible layer-based organization of entities in the ECS system,
providing functionality for managing multiple layers for scene composition.



Copyright (C) 2024 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the
prior written consent of DigiPen Institute of Technology is prohibited.
*/
/********************************************************************/

#include "Config/pch.h"
#include "Layers.h"
#include "Debugging/Logging.h"
#include "ECS/ECS.h"

namespace layer {

	//bool LayerStack::m_CreateLayer(std::string layer)
	//{
	//	// check if layer is already existing
	//	if (m_layerMap.find(layer) != m_layerMap.end()) {
	//		LOGGING_WARN("Layer of same name has already been created");
	//		return false;
	//	}

	//	//creates new layer
	//	m_layerMap[layer];


	//	return true;
	//}

	//bool LayerStack::m_DeleteLayer(std::string layer)
	//{
	//	ecs::ECS* ecs = ecs::ECS::m_GetInstance();
	//	// check if layer is already existing
	//	if (m_layerMap.find(layer) == m_layerMap.end()) {
	//		LOGGING_WARN("Layer does no exist");
	//		return false;
	//	}

	//	for (ecs::EntityID& x : m_layerMap[layer]) {
	//		static_cast<ecs::NameComponent*>(ecs->m_ECS_CombinedComponentPool[ecs::TYPENAMECOMPONENT]->m_GetEntityComponent(x))->Layer = "Default";
	//	
	//	}

	//	return false;
	//}

	LayerStack::LayerStack()
	{
		m_layerBitSet.set();// assign all bits to true

		//set max layers
		m_layerMap[DEFAULT].first = "Default";
		m_layerMap[LAYER1].first = "Layer 1";
		m_layerMap[LAYER2].first = "Layer 2";
		m_layerMap[LAYER3].first = "Layer 3";
		m_layerMap[LAYER4].first = "Layer 4";
		m_layerMap[LAYER5].first = "Layer 5";
		m_layerMap[LAYER6].first = "Layer 6";
		m_layerMap[LAYER7].first = "Layer 7";
		m_layerMap[LAYER8].first = "Layer 8";


	}

	void LayerStack::m_ChangeLayerName(LAYERS layer, std::string newName)
	{
		if (m_layerMap.find(layer) == m_layerMap.end()) {

			LOGGING_WARN("Layer does no exist");
			return ;
		}

		m_layerMap[layer].first = newName;

	}

	bool LayerStack::m_SwapEntityLayer(LAYERS newlayer, LAYERS oldlayer, ecs::EntityID id)
	{
		// same layer, do nothing
		if (newlayer == oldlayer) return true;

		if (m_layerMap.find(newlayer) == m_layerMap.end()) {
			
			LOGGING_WARN("Layer does no exist");
			return false;
		}

		if (m_layerMap.find(oldlayer) == m_layerMap.end()) {

			LOGGING_WARN("Layer does no exist");
			return false;
		}

		// add id to new layer
		//m_layerMap[newlayer].second.push_back(id);
		//m_layerMap[oldlayer].second.erase(std::find(m_layerMap[oldlayer].second.begin(), m_layerMap[oldlayer].second.end(), id));

		//assign ecs layer
		ecs::NameComponent* nc = ecs::ECS::GetInstance()->GetComponent<ecs::NameComponent>(id);
		nc->Layer = newlayer;

		return true;
	}

	void LayerStack::m_DisableLayer(LAYERS layer)
	{
		m_layerBitSet.reset(layer);

	}

	void LayerStack::m_EnableLayer(LAYERS layer)
	{
		m_layerBitSet.set(layer);
	}

	bool LayerStack::m_IsLayerVisable(int layer)
	{
		return m_layerBitSet.test((LAYERS)layer);
	}

	std::vector<ecs::EntityID> LayerStack::m_RetrieveEntityID(LAYERS layer)
	{
		if (m_layerMap.find(layer) == m_layerMap.end()) {

			LOGGING_WARN("Layer does no exist");
			return std::vector<ecs::EntityID>();
		}

		return m_layerMap[layer].second;
	}

	void LayerStack::m_hideEntitywithChild(ecs::EntityID id)
	{
		ecs::ECS* ecs = ecs::ECS::GetInstance();
		ecs::NameComponent* nc = ecs->GetComponent<ecs::NameComponent>(id);
		ecs::TransformComponent* tc = ecs->GetComponent<ecs::TransformComponent>(id);
		nc->hide = true;

		if (tc->m_childID.size() > 0) {
			for (auto child_id : tc->m_childID) {
				m_hideEntitywithChild(child_id);
			}

		}
		
	}

	void LayerStack::m_unhideEntitywithChild(ecs::EntityID id)
	{
		ecs::ECS* ecs = ecs::ECS::GetInstance();
		ecs::NameComponent* nc = ecs->GetComponent<ecs::NameComponent>(id);
		ecs::TransformComponent* tc = ecs->GetComponent<ecs::TransformComponent>(id);
		
		nc->hide = false;

		if (tc->m_childID.size() > 0) {
			for (auto child_id : tc->m_childID) {
				m_unhideEntitywithChild(child_id);
			}

		}
	}


}