/******************************************************************/
/*!
\file      Prefab.cpp
\author    Ng Jaz winn, jazwinn.ng , 2301502
\par       jazwinn.ng@digipen.edu
\date      Nov 11, 2024
\brief     This file contains the definations for the prefab class.
           It reads a json file and stores all its data. When the prefab
           is called in the game. It creates an entiy and copy
           the prefab data to the new entity



Copyright (C) 2024 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the
prior written consent of DigiPen Institute of Technology is prohibited.
*/
/********************************************************************/

#include "Config/pch.h"
#include "Prefab.h"
#include "DeSerialization/json_handler.h"
#include "Debugging/Logging.h"
#include "ECS/Hierachy.h"
#include "ECS/ECS.h"
#include "Scene/SceneManager.h"
#include "../Kos Editor/AssetManager/AssetManager.h" // Double check if Jaz wants to do complete seperation of editor and engine
#include <RAPIDJSON/filewritestream.h>
#include <RAPIDJSON/prettywriter.h>
#include <RAPIDJSON/writer.h>
#include <RAPIDJSON/stringbuffer.h>

namespace prefab 
{
    void AssignPrefabToNameComponent(ecs::EntityID parentid, std::string scenename) {
        const auto& vecChild = ecs::Hierachy::m_GetChild(parentid);
        if (!vecChild.has_value()) return;
        for (auto& childid : vecChild.value()) {
            ecs::ECS* ecs = ecs::ECS::GetInstance();
            ecs::NameComponent* nc = ecs->GetComponent<ecs::NameComponent>(childid);
            nc->isPrefab = true;
            nc->prefabName = scenename;

            if (ecs::Hierachy::m_GetChild(childid).has_value()) {
                AssignPrefabToNameComponent(childid, scenename);
            }
        }
    }

    // Creating Prefab Instance
    int Prefab::m_CreatePrefab(std::string prefabscene, std::string insertscene)
    {
        if (prefabscene == insertscene) {
            LOGGING_ERROR("Cannot load onto itself");
            return -1;
        }

        //check if prefabscene exist
        ecs::ECS* ecs = ecs::ECS::GetInstance();

        if (insertscene.empty()) {
            for (auto& scene : ecs->sceneMap) {

                if (scene.second.isActive && (!scene.second.isPrefab)) {
                    insertscene = scene.first;
                    break;
                }
            }      
        }

        if (ecs->sceneMap.find(prefabscene) == ecs->sceneMap.end()) {
            LOGGING_ERROR("Prefab not loaded into scene");
            return -1;
        }
       
        ecs::EntityID newId = ecs->CreateEntity(insertscene);

        DeepUpdatePrefab(ecs->sceneMap.at(prefabscene).prefabID, newId);


        AssignPrefabToNameComponent(newId, prefabscene);

        return newId;
    }

    void Prefab::m_SaveEntitytoPrefab(ecs::EntityID id)
    {
        ecs::ECS* ecs = ecs::ECS::GetInstance();
        ecs::NameComponent* nc = ecs->GetComponent<ecs::NameComponent>(id);
        std::string m_jsonFilePath{ AssetManager::GetInstance()->GetAssetManagerDirectory() + "/Prefabs/" }; //TODO allow drag and drop onto content browser
        std::string filename;

        short count{};
        do {
            if (count > 0) {
                filename = nc->entityName + "_" + std::to_string(count) + ".prefab";
            }
            else {
                filename = nc->entityName + ".prefab";
            }
            count++;
        } while (ecs->sceneMap.find(filename) != ecs->sceneMap.end());

        nc->isPrefab = true;
        nc->prefabName = filename;

        std::string path = m_jsonFilePath + filename;
        scenes::SceneManager::m_GetInstance()->CreateNewScene(path);
        LOGGING_DEBUG(path.c_str());

        /*******************************SERIALIZATION START******************************************/

        Serialization::JsonFileValidation(path);

        // Create JSON object to hold the updated values
        rapidjson::Document doc;
        doc.SetArray();  // Initialize as an empty array

        rapidjson::Document::AllocatorType& allocator = doc.GetAllocator();

        std::unordered_set<ecs::EntityID> savedEntities;  //track saved entities

        //Start saving the entities
        Serialization::SaveEntity(id, doc, allocator, savedEntities);

        // Write the JSON back to file
        rapidjson::StringBuffer writeBuffer;
        rapidjson::PrettyWriter<rapidjson::StringBuffer> writer(writeBuffer);
        doc.Accept(writer);

        std::ofstream outputFile(path);
        if (outputFile) {
            outputFile << writeBuffer.GetString();
            outputFile.close();
        }

        LOGGING_INFO("Save Prefab Successful");

        /*******************************SERIALIZATION END******************************************/

        // load prefab
        scenes::SceneManager::m_GetInstance()->LoadScene(path);
    }

    void Prefab::OverwriteScenePrefab(ecs::EntityID id) {
        ecs::ECS* ecs = ecs::ECS::GetInstance();
        ecs::NameComponent* nc = ecs->GetComponent<ecs::NameComponent>(id);
        if (!nc->isPrefab) return;

        const std::string& prefabName = nc->prefabName;

        if (ecs->sceneMap.find(prefabName) != ecs->sceneMap.end()) {

            const auto& sceneData = ecs->sceneMap.at(prefabName);
            ecs::EntityID prefabID = sceneData.prefabID;

            DeepUpdatePrefab(id, prefabID);
        }
        


    }

    void Prefab::UpdateAllPrefab(const std::string& prefabSceneName) {
        ecs::ECS* ecs = ecs::ECS::GetInstance();
        if (ecs->sceneMap.find(prefabSceneName) == ecs->sceneMap.end()) return;
        const auto& prefabData = ecs->sceneMap.find(prefabSceneName);

        ecs::EntityID prefabID = prefabData->second.prefabID;

        for (const auto& [id, signature] : ecs->GetEntitySignatureData()) {
            ecs::NameComponent* nc = ecs->GetComponent<ecs::NameComponent>(id);
            ecs::TransformComponent* tc = ecs->GetComponent<ecs::TransformComponent>(id);
            if (!(tc->m_haveParent) && nc->isPrefab && (nc->prefabName == prefabSceneName)) {
                DeepUpdatePrefab(prefabID, id);
            }
        }
    }

 
    void Prefab::DeepUpdatePrefab(ecs::EntityID idA, ecs::EntityID idB) {
        ecs::ECS* ecs = ecs::ECS::GetInstance();
        if (idA == idB)return;

        const auto signatureA = ecs->GetEntitySignature(idA);
        const auto signatureB = ecs->GetEntitySignature(idB);

        //update components
        const auto& componentKey = ecs->GetComponentKeyData();
        for (const auto& [ComponentName, key] : componentKey) {
            auto action = ecs->componentAction[ComponentName];

            if (signatureA.test(key)) {

                if (action->Compare(idA, idB) == false) { // if A != B or B does not exist, call duplicate (Assign and create(if missing) component
                    action->DuplicateComponent(idA, idB);
                }
            }
            else if (signatureB.test(key)) { //if A does not have component, B has, remove B's component
                action->RemoveComponent(idB);
            }

        }


        //Objective: Make both have the same number of children
        auto childsA = ecs::Hierachy::m_GetChild(idA);
        auto childsB = ecs::Hierachy::m_GetChild(idB);

        if (!childsA.has_value() && !childsB.has_value())return; // both id do not have children

        int countA = 0, countB = 0; //number of children both id have

        if (childsA.has_value()) {
            countA = childsA.value().size();
        }

        if (childsB.has_value()) {
            countB = childsB.value().size();
        }

        //make A == B
        if (countA > countB) { 
            int diff = countA - countB;
            const auto& scene = ecs->GetSceneByEntityID(idB);
            for (int n{}; n < diff; n++) {
                ecs::Hierachy::m_SetParent(idB, ecs->CreateEntity(scene));
            }
        }

        if (countA < countB) {
            int diff = countB - countA;
            std::vector<ecs::EntityID> childsVecB = childsB.value();
            for (int n{}; n < diff; n++) {
                ecs->DeleteEntity(childsVecB[n]);
            }

        }



        const auto childsVecA = ecs::Hierachy::m_GetChild(idA);
        const auto childsVecB = ecs::Hierachy::m_GetChild(idB);

        if (childsVecA.has_value() && childsVecB.has_value()) {
            //recurse the children
            for (int n{}; n < childsVecA.value().size(); n++) {
                DeepUpdatePrefab(childsVecA.value()[n], childsVecB.value()[n]);
            }

        }



   }


    void Prefab::OverwritePrefab_Component(ecs::EntityID entityID, const std::string& componentName, const std::string& prefabSceneName) {
        ecs::ECS* ecs = ecs::ECS::GetInstance();
        if (ecs->sceneMap.find(prefabSceneName) == ecs->sceneMap.end()) return;
        auto iter = ecs->sceneMap.find(prefabSceneName);
        if (iter == ecs->sceneMap.end()) return;
        ecs::EntityID prefabID = iter->second.prefabID;
        if (prefabID == entityID) return;

        auto action = ecs->componentAction[componentName];
        action->DuplicateComponent(entityID, prefabID);

        // Update all Associated Prefabs;
        for (const auto& [id, signature] : ecs->GetEntitySignatureData()) {
            ecs::NameComponent* nc = ecs->GetComponent<ecs::NameComponent>(id);
            if (nc->isPrefab && (nc->prefabName == prefabSceneName)) {
                // Remove Comp in Revert will already check if it contains comp 
				action->DuplicateComponent(prefabID, id);
                /*RevertToPrefab_Component(id.first, componentName, nc->prefabName);*/
            }
        }

        //save prefab
		scenes::SceneManager::m_GetInstance()->SaveScene(prefabSceneName);
    }

    void Prefab::RevertToPrefab_Component(ecs::EntityID entityID, const std::string& componentName, const std::string& prefabSceneName) {
        ecs::ECS* ecs = ecs::ECS::GetInstance();

        if (ecs->sceneMap.find(prefabSceneName) == ecs->sceneMap.end()) return;
        auto iter = ecs->sceneMap.find(prefabSceneName);
        if (iter == ecs->sceneMap.end()) return;
        ecs::EntityID prefabID = iter->second.prefabID;
        if (prefabID == entityID) return;

        auto action = ecs->componentAction[componentName];
        action->RemoveComponent(entityID);
    }

    void Prefab::LoadAllPrefabs() {
        auto* sm = scenes::SceneManager::m_GetInstance();
        ecs::ECS* ecs = ecs::ECS::GetInstance();

        std::string prefabPath = AssetManager::GetInstance()->GetAssetManagerDirectory() + "/Prefabs/"; // Should have a better way to get file directories
        if (!std::filesystem::exists(prefabPath))return;
        for (const auto& entry : std::filesystem::directory_iterator(prefabPath)) {
			auto scenename = entry.path().filename();

            if (scenename.extension().string() == ".prefab") {
                sm->ImmediateLoadScene(entry.path());
                auto& prefabData = ecs->sceneMap.at(scenename.string());
                prefabData.isPrefab = true;
				sm->SetSceneActive(scenename.string(), false);


				//find the prefab root entity id
                for (auto& id : prefabData.sceneIDs) {
                    ecs::TransformComponent* tc = ecs->GetComponent<ecs::TransformComponent>(id);
                    if (!tc->m_haveParent) {
                        ecs->sceneMap.find(scenename.string())->second.prefabID = id;
                        ecs::NameComponent* nc = ecs->GetComponent<ecs::NameComponent>(id);
						nc->prefabName = scenename.string();
                        break;
                    }
                }
            }
        }
    }

    ecs::ComponentSignature Prefab::ComparePrefabWithInstance(ecs::EntityID entityID) {
        ecs::ECS* ecs = ecs::ECS::GetInstance();
        std::string prefabName = ecs->GetComponent<ecs::NameComponent>(entityID)->prefabName;
        if (ecs->sceneMap.find(prefabName) == ecs->sceneMap.end()) return ecs::ComponentSignature();

        ecs::EntityID prefabId = ecs->sceneMap.find(prefabName)->second.prefabID;

        auto entitySignature = ecs->GetEntitySignature(entityID);
        auto prefabSignature = ecs->GetEntitySignature(prefabId);

        // Stores the resulting components which are different
        ecs::ComponentSignature result;

        const auto& componentKey = ecs->GetComponentKeyData();
        for (const auto& [ComponentName, key] : componentKey) {
            auto componentKey = ecs->GetComponentKey(ComponentName);

            if (entitySignature.test(componentKey) &&
                prefabSignature.test(componentKey)) {
                auto* idComp = ecs->GetIComponent<ecs::Component*>(ComponentName, entityID);
                auto* prefabComp = ecs->GetIComponent<ecs::Component*>(ComponentName, prefabId);

                auto& actionInvoker = ecs->componentAction[ComponentName];
                if (!actionInvoker->Compare(idComp, prefabComp)) { // 1 = same, 0 = diff
                    result.set(ecs->GetComponentKey(ComponentName));
                }
            }
            else if (prefabSignature.test(componentKey) && (!entitySignature.test(componentKey))) { // if prefab has component, but object does not
                auto& actionInvoker = ecs->componentAction[ComponentName];
                actionInvoker->AddComponent(entityID);
                actionInvoker->DuplicateComponent(prefabId, entityID);
            };


        }

        return result;
    }

    void Prefab::RefreshComponentDifferenceList(std::vector<std::string>& diffComp, ecs::EntityID entityID) {
        ecs::ECS* m_ecs = ecs::ECS::GetInstance();
        ecs::ComponentSignature sig = prefab::Prefab::ComparePrefabWithInstance(entityID);
        diffComp.clear();
        const auto& componentKey = m_ecs->GetComponentKeyData();
        for (const auto& [ComponentName, key] : componentKey) {
            if (sig.test(m_ecs->GetComponentKey(ComponentName))) {
                diffComp.push_back(ComponentName);
            }
        }
    }
}

