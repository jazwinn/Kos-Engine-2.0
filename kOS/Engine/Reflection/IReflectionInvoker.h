/******************************************************************/
/*!
\file      IReflectionInvoker.h
\author    Jaz Winn Ng
\par       jazwinn.ng@digipen.edu
\date      Sept 28, 2025
\brief     Reflection Invoker interface for invoking functions with variable arguments.

Copyright (C) 2025 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the
prior written consent of DigiPen Institute of Technology is prohibited.
*/
/******************************************************************/
#pragma once

#include "Config/pch.h"
#include "DeSerialization/json_handler.h"


class IActionInvoker {
public:
    virtual ~IActionInvoker() = default;

    // Pure virtual function to draw the component's UI in ImGui or other editors
    virtual void Save(void* componentData, rapidjson::Value& entityData, rapidjson::Document::AllocatorType& allocator) = 0;

    virtual void Load(ecs::EntityID ID, const rapidjson::Value& entityData) = 0;

    virtual bool Compare(void* componentData1, void* componentData2) = 0;
    virtual bool Compare(ecs::EntityID ID, ecs::EntityID ID2) = 0;

    virtual void* AddComponent(ecs::EntityID ID) = 0;

	virtual bool HasComponent(ecs::EntityID ID) = 0;

    virtual void RemoveComponent(ecs::EntityID ID) = 0;

    virtual void ResetComponent(ecs::EntityID ID) = 0;

    virtual void* DuplicateComponent(ecs::EntityID duplicateID, ecs::EntityID newID) = 0;

    virtual void ApplyFunction(void* component, std::function<void(void*)> func) = 0;

    virtual void RegisterAction(const std::string& name, std::function<void(void*, void*)> func) = 0;
    virtual void Execute(const std::string& name, void* arg1 = nullptr, void* arg2 = nullptr) = 0;

};


