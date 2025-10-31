/******************************************************************/
/*!
\file      System.h
\author    Ng Jaz winn, jazwinn.ng , 2301502
\par       jazwinn.ng@digipen.edu
\date      Oct 02, 2025
\brief     This file contains the system class that is the parent
		   class to all systems in the entity component system

Copyright (C) 2024 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the
prior written consent of DigiPen Institute of Technology is prohibited.
*/
/********************************************************************/
#ifndef SYSTEM_H
#define SYSTEM_H


#include "ECS/Component/Component.h"
#include "ECS/SparseSet.h"
#include "Events/Delegate.h"

namespace ecs {

	class ISystem {

	public:

		inline void AssignSignature(ComponentSignature signature) {
			m_systemSignature = signature;
		}

		inline void RegisterSystem(EntityID ID) {

			if(!m_entities.ContainsEntity(ID)){
				m_entities.Set(ID, ID);
				onRegister.Invoke(ID);
			}
		}

		inline void DeregisterSystem(EntityID ID) {
			m_entities.Delete(ID);
			onDeregister.Invoke(ID);
		}

		inline void SetState(const std::bitset<GAMESTATE_COUNT>& state) {
			m_systemGameState = state;
		}

		inline bool TestState(GAMESTATE state) {
			return m_systemGameState.test(state);
		}

		ComponentSignature GetSignature() {
			return m_systemSignature;
		}

		virtual void Init() = 0;
		virtual void Update() = 0;

		Delegate<EntityID> onRegister;
		Delegate<EntityID> onDeregister;
	
	protected:

		SparseSet<EntityID> m_entities;
		ComponentSignature m_systemSignature; // set signature based on what component the systems need
		std::bitset<GAMESTATE_COUNT> m_systemGameState;
	};

}









#endif SYSTEM_H

