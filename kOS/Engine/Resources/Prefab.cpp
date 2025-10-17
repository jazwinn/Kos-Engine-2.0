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
#include "ResourceManager.h"
#include "ECS/Hierachy.h"
#include "ECS/ECS.h"
#include "Scene/SceneManager.h"

#include <RAPIDJSON/filewritestream.h>
#include <RAPIDJSON/prettywriter.h>
#include <RAPIDJSON/writer.h>
#include <RAPIDJSON/stringbuffer.h>


namespace prefab {


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
       
        std::vector<ecs::EntityID> vecid;

        for (const auto& prefabentity : ecs->sceneMap.find(prefabscene)->second.sceneIDs) {
            //duplicate only parents
            if (ecs::Hierachy::GetParent(prefabentity).has_value())continue;
            vecid.push_back(ecs->DuplicateEntity(prefabentity, insertscene));
        }

        for (auto& id : vecid) {
            //assign parent as prefab
            ecs::ECS* Tempecs = ecs::ECS::GetInstance();
            ecs::NameComponent* nc = ecs->GetComponent<ecs::NameComponent>(id);
            nc->isPrefab = true;
            nc->prefabName = prefabscene;

            const auto& vecChild = ecs::Hierachy::m_GetChild(id);
            if (!vecChild.has_value()) continue;
            AssignPrefabToNameComponent(id, prefabscene);
        
        }

        return vecid[0];
    }

    void Prefab::m_SaveEntitytoPrefab(ecs::EntityID id)
    {
        ecs::ECS* ecs = ecs::ECS::GetInstance();
        ecs::NameComponent* nc = ecs->GetComponent<ecs::NameComponent>(id);
        std::string m_jsonFilePath{ "Assets/Prefabs/" }; //TODO allow drag and drop onto content browser

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

        std::string path = m_jsonFilePath + filename;
        scenes::SceneManager::m_GetInstance()->CreateNewScene(path);


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

    //void Prefab::m_AssignEntitytoPrefab(std::string prefab, ecs::EntityID id)
    //{
    //    ecs::ECS* ecs = ecs::ECS::m_GetInstance();
    //    if (ecs->m_ECS_SceneMap.find(prefab) == ecs->m_ECS_SceneMap.end()) {
    //        LOGGING_WARN("Prefab not loaded");
    //        return;
    //    }
    //    // only store upmost parent id

    //    ecs::TransformComponent* tc = static_cast<ecs::TransformComponent*>(ecs->m_ECS_CombinedComponentPool[ecs::TYPETRANSFORMCOMPONENT]->m_GetEntityComponent(id));
    //    if (tc->m_haveParent) {
    //        return;
    //    }

    //}  

    void Prefab::m_UpdateAllPrefabEntity(std::string prefab)
    {
        ecs::ECS* ecs = ecs::ECS::GetInstance();

        //return if prefab dont exist
        if (ecs->sceneMap.find(prefab) == ecs->sceneMap.end()) {
            LOGGING_ERROR("Prefab do not exist");
        }

        ecs::EntityID scenePrefabID = ecs->sceneMap.find(prefab)->second.prefabID;


        //retrieve prefabs children id
        const std::vector<ecs::EntityID>& childrenid = ecs->sceneMap.find(prefab)->second.sceneIDs;

        //the most scuff way //MAYBE prefab component?
        for (const auto& id : ecs->GetEntitySignatureData()) {
            ecs::NameComponent* nc = ecs->GetComponent<ecs::NameComponent>(id.first);
            ecs::TransformComponent* tc = ecs->GetComponent<ecs::TransformComponent>(id.first);
            if (nc->isPrefab && (nc->prefabName == prefab)) {
                //if sync is turn off, skip update
                if (nc->syncPrefab == false) continue;


                //skip all prefabs children
                if (std::find(childrenid.begin(), childrenid.end(), id.first) != childrenid.end()) continue;
                if (tc->m_haveParent) {
                    auto* parentnc = ecs->GetComponent<ecs::NameComponent>(tc->m_parentID);
                    
                    if (parentnc->isPrefab && parentnc->prefabName == prefab)continue;
                }

                m_UpdatePrefab(scenePrefabID, id.first);

            }


        }

    }

    void Prefab::m_UpdatePrefab(ecs::EntityID sceneprefabID, ecs::EntityID entityid, bool isPrefabChild)
    {
        if (sceneprefabID == entityid) return;

        //update all of entity with prefab
        ecs::ECS* ecs = ecs::ECS::GetInstance();

        ecs::ComponentSignature scenePrefabSignature = ecs->GetEntitySignature(sceneprefabID);
        ecs::ComponentSignature entitySignature = ecs->GetEntitySignature(entityid);

        //check if current entity have parent and store it
        auto Parent = ecs::Hierachy::GetParent(entityid);


        const auto& componentKey = ecs->GetComponentKeyData();
        for (const auto& [ComponentName, key] : componentKey) {
            //skip transform component and name componetn
             // if entity is parent and component is transform component, skip
            if ((isPrefabChild == false) && ComponentName == ecs::TransformComponent::classname()) continue;

            //if component is namecomponent, only share layer and entity tag 
            if (ComponentName == ecs::NameComponent::classname()) {// sync only layers and tagname
                ecs::NameComponent* prefabNc = ecs->GetComponent<ecs::NameComponent>(sceneprefabID);
                ecs::NameComponent* nc = ecs->GetComponent<ecs::NameComponent>(entityid);

                nc->Layer = prefabNc->Layer;
                nc->entityTag = prefabNc->entityTag;

                continue;
            }

            auto action = ecs->componentAction[ComponentName];

            if (scenePrefabSignature.test(key)){

                //save prefab detail
                if (ComponentName == ecs::TransformComponent::classname()) {
                    //only duplicate, TRS
                    ecs::TransformComponent* tc = ecs->GetComponent<ecs::TransformComponent>(entityid);
                    ecs::TransformComponent* prefabtc = ecs->GetComponent<ecs::TransformComponent>(sceneprefabID);

                    tc->WorldTransformation.position = prefabtc->WorldTransformation.position;
                    tc->WorldTransformation.rotation = prefabtc->WorldTransformation.rotation;
                    tc->WorldTransformation.scale = prefabtc->WorldTransformation.scale;

                    continue;
                }

                //test if enityid has that component, if not add it
                if (!entitySignature.test(key)) {
                    action->AddComponent(entityid);
                }
                ecs::Component* comp = static_cast<ecs::Component*>(action->DuplicateComponent(sceneprefabID, entityid));
                comp->scene = ecs->GetSceneByEntityID(entityid);


            }
            else {
                if (entitySignature.test(key)) {
                    action->RemoveComponent(entityid);
                }
            }




        }


        //checks if duplicates entity has parent and assign it
        //if (Parent.has_value()) {
        //    ecs::Hierachy::m_SetParent(Parent.value(), entityid);
        //}

        //checks if entity has child call recursion
        const auto& scenePrefabChild = ecs::Hierachy::m_GetChild(sceneprefabID);
        if (scenePrefabChild.has_value()) {
            //clear child id of vector for new entity

            std::vector<ecs::EntityID> childID = scenePrefabChild.value();
            
            ecs::EntityID entityChild;

            //the most scuff way to do sth
            //if size of prefab is less than entity (means a prefab entity was deleted)
            //delete the entire of the childs entity and rebuilt
            {
                const auto& entitych = ecs::Hierachy::m_GetChild(entityid);
                if (entitych.has_value() && (childID.size() < entitych.value().size())) {
                    //delete all the children
                    for (const auto& id : entitych.value()) {
                        ecs->DeleteEntity(id);
                    }

                }
            }


            const auto& entitych = ecs::Hierachy::m_GetChild(entityid);
            size_t count{};
            for (const auto& prefabchild : childID) {

                //check if entityid, have child, if no create child
                if (!entitych.has_value()  || entitych.value().size() < (count +1)) {
                    //create entity and assign to entityid
                    
                    entityChild = ecs->CreateEntity(ecs->GetSceneByEntityID(entityid));
                    ecs::NameComponent* nc = ecs->GetComponent<ecs::NameComponent>(entityChild);
                    ecs::NameComponent* prefabnc = ecs->GetComponent<ecs::NameComponent>(prefabchild);
                    nc->entityName = prefabnc->entityName;

                    nc->isPrefab = true;
                    nc->prefabName = ecs->GetSceneByEntityID(prefabchild);

                    ecs::Hierachy::m_SetParent(entityid, entityChild);
                }
                else {
                    entityChild = entitych.value()[count];
                }

                //ecs::TransformComponent* tc = static_cast<ecs::TransformComponent*>(ecs->m_ECS_CombinedComponentPool[ecs::TYPETRANSFORMCOMPONENT]->m_GetEntityComponent(entityChild));

                m_UpdatePrefab(prefabchild, entityChild, true);

                count++;
            }
        }
        else {

            const auto& entitych = ecs::Hierachy::m_GetChild(entityid);
            if (entitych.has_value() && (entitych.value().size() > 0)) {
                //delete all the children
                for (const auto& id : entitych.value()) {
                    ecs->DeleteEntity(id);
                }



            }

        }




    }

    void CheckEntityisPrefab(ecs::EntityID id, std::string prefabscene) {
        ecs::ECS* ecs = ecs::ECS::GetInstance();
        ecs::NameComponent* nc = ecs->GetComponent<ecs::NameComponent>(id);
        if (!nc->isPrefab) {
            nc->isPrefab = true;
            nc->prefabName = prefabscene;
        }

        const auto& child = ecs::Hierachy::m_GetChild(id);
        if (child.has_value()) {

            for (auto& childID : child.value()) {
                CheckEntityisPrefab(childID, prefabscene);
            }

        }
    }

    void Prefab::m_OverWriteScenePrafab(ecs::EntityID id)
    {
        //delete the all the prefab entity
        ecs::ECS* ecs = ecs::ECS::GetInstance();

        ecs::NameComponent* nc = ecs->GetComponent<ecs::NameComponent>(id);

        const std::string& scene = nc->prefabName;

        if (ecs->sceneMap.find(scene) == ecs->sceneMap.end()) {
            LOGGING_ERROR("Prefab Scenen not loaded");
        }

        ecs->DeleteEntity(ecs->sceneMap.find(scene)->second.prefabID);

        //TODO if check if any new child entity, than assign it to be a prefab create a function
        CheckEntityisPrefab(id, scene);


        //duplicate param entity into prefab

        ecs->sceneMap.find(scene)->second.prefabID = ecs->DuplicateEntity(id, scene);


        //save scene
        scenes::SceneManager::m_GetInstance()->SaveScene(scene);

        //call to update all prefab

        m_UpdateAllPrefabEntity(scene);

    }

}

