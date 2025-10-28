/******************************************************************/
/*!
\file      ReflecetionInvoker.h
\author    Jaz Winn Ng
\par       jazwinn.ng@digipen.edu
\date      Sept 28, 2025
\brief     ReflectionInvoker interface for handling component serialization, deserialization, and actions.

Copyright (C) 2025 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the
prior written consent of DigiPen Institute of Technology is prohibited.
*/
/******************************************************************/
#pragma once

#include "IReflectionInvoker.h"
#include "DeSerialization/SerializationReflection.h"



template <typename T>
class ActionInvoker : public IActionInvoker {
    
private:
    ecs::ECS* m_ecs;

public:
    ActionInvoker(ecs::ECS* ecs):m_ecs(ecs){}

    void Save(void* componentData, rapidjson::Value& entityData, rapidjson::Document::AllocatorType& allocator) override {
        saveComponentreflect(static_cast<T*>(componentData), entityData, allocator);
    }

    void Load(ecs::EntityID ID, const rapidjson::Value& entityData) override {
        T* component = m_ecs->AddComponent<T>(ID);

        if (component) {
            LoadComponentreflect(component, entityData);
        }
    }

    bool Compare(void* componentData1, void* componentData2) {
        return CompareComponentReflect(static_cast<T*>(componentData1), static_cast<T*>(componentData2));
    }

    bool Compare(ecs::EntityID ID, ecs::EntityID ID2) {

        T* componentData1 = m_ecs->GetComponent<T>(ID);
        T* componentData2 = m_ecs->GetComponent<T>(ID2);

        if (!componentData1 || !componentData2) {
            return false;
        }


        return CompareComponentReflect(componentData1, componentData2);
    }


    void ResetComponent(ecs::EntityID ID) override {
        m_ecs->ResetComponent<T>(ID);
    }

    void* AddComponent(ecs::EntityID ID) override {

        return m_ecs->AddComponent<T>(ID);
    }

    bool HasComponent(ecs::EntityID ID) override {
        return m_ecs->HasComponent<T>(ID);
	}

    void RemoveComponent(ecs::EntityID ID) override {
        m_ecs->RemoveComponent<T>(ID);
    }

    void* DuplicateComponent(ecs::EntityID duplicateID, ecs::EntityID newID) override {
        return m_ecs->DuplicateComponent<T>(duplicateID, newID);
    }

    void ApplyFunction(void* component, std::function<void(void*)> func) override {
        T* Component = static_cast<T*>(component);
        Component->ApplyFunction([func](auto& member) {
            func(&member); // pass member as void* to type-erased func
            });

    }


public:


    std::unordered_map<std::string, std::function<void(void*, void*)>> actions;

    void RegisterAction(const std::string& name, std::function<void(void*, void*)> func) {
        actions[name] = func;
    }

    void Execute(const std::string& name, void* arg1, void* arg2) override {
        auto it = actions.find(name);
        if (it != actions.end()) {
            it->second(arg1, arg2);
        }
        else {
            throw std::runtime_error("Action not found: " + name);
        }
    }

};