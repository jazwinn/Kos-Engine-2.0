/********************************************************************/
/*!
\file       ColliderSystem.cpp
\author		Toh Yu Heng 2301294, Rayner Tan Wei Chen 2301449
\par		t.yuheng@digipen.edu, raynerweichen.tan@digipen.edu
\date		Oct 01 2025
\brief		Implements the ColliderSystem, responsible for creating and 
            synchronizing static collision shapes (Box, Sphere, Capsule) 
            within the ECS world using NVIDIA PhysX.

            This system:
            - Iterates over all entities with collider components
            - Generates corresponding PhysX PxShape instances if missing
            - Updates shape transforms based on entity TransformComponents
            - Creates and attaches PxRigidStatic actors for static colliders
            - Updates existing actor poses each frame

            Rigidbodies and CharacterControllers are skipped to avoid 
            conflicts with dynamic or controlled physics objects.

Copyright (C) 2025 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the
prior written consent of DigiPen Institute of Technology is prohibited.
*/
/********************************************************************/

#include "Config/pch.h"
#include "ColliderSystem.h"
#include "Physics/PhysicsManager.h"

using namespace physics;

namespace ecs {
	void ColliderSystem::Init() {
        onDeregister.Add([](EntityID id) {
            ECS* ecs = ECS::GetInstance();
            if (BoxColliderComponent* box = ecs->GetComponent<BoxColliderComponent>(id)) {
                if (PxShape* shape = static_cast<PxShape*>(box->shape)) {
                    if (PxRigidActor* actor = shape->getActor()) {
                        PhysicsManager::GetInstance()->GetScene()->removeActor(*actor);
                        actor->release();
                    }
                    shape->release();
                    box->shape = nullptr;
                }
            }
            if (SphereColliderComponent* sphere = ecs->GetComponent<SphereColliderComponent>(id)) {
                if (PxShape* shape = static_cast<PxShape*>(sphere->shape)) {
                    if (PxRigidActor* actor = shape->getActor()) {
                        PhysicsManager::GetInstance()->GetScene()->removeActor(*actor);
                        actor->release();
                    }
                    shape->release();
                    sphere->shape = nullptr;
                }
            }
            if (CapsuleColliderComponent* capsule = ecs->GetComponent<CapsuleColliderComponent>(id)) {
                if (PxShape* shape = static_cast<PxShape*>(capsule->shape)) {
                    if (PxRigidActor* actor = shape->getActor()) {
                        PhysicsManager::GetInstance()->GetScene()->removeActor(*actor);
                        actor->release();
                    }
                    shape->release();
                    capsule->shape = nullptr;
                }
            }
        });
    }

	void ColliderSystem::Update(const std::string& scene) {
		ECS* ecs = ECS::GetInstance();
		const auto& entities = m_entities.Data();
        for (EntityID id : entities) {
            TransformComponent* trans = ecs->GetComponent<TransformComponent>(id);
            NameComponent* name = ecs->GetComponent<NameComponent>(id);

            if (!trans || trans->scene != scene || name->hide) { continue; }
            if (ecs->HasComponent<RigidbodyComponent>(id)) { continue; }
            if (ecs->HasComponent<CharacterControllerComponent>(id)) { continue; }

            glm::vec3 scale = trans->LocalTransformation.scale;
            const float minScale = 0.001f;
            scale.x = glm::max(glm::abs(scale.x), minScale);
            scale.y = glm::max(glm::abs(scale.y), minScale);
            scale.z = glm::max(glm::abs(scale.z), minScale);

            PxRigidStatic* staticActor = nullptr;

            PxFilterData filterData;
            filterData.word0 = name->Layer;

            if (BoxColliderComponent* box = ecs->GetComponent<BoxColliderComponent>(id)) {
                glm::vec3 sz = glm::max(glm::abs(box->box.size * scale), glm::vec3(minScale));
                PxShape* shape = static_cast<PxShape*>(box->shape);

                if (!shape) {
                    shape = PhysicsManager::GetInstance()->GetPhysics()->createShape(
                        PxBoxGeometry(sz.x * 0.5f, sz.y * 0.5f, sz.z * 0.5f),
                        *PhysicsManager::GetInstance()->GetDefaultMaterial(),
                        true
                    );
                    box->shape = shape;
                }

                glm::vec3 c = box->box.center * scale;
                shape->setLocalPose(PxTransform{ PxVec3{ c.x, c.y, c.z } });
                ToPhysxIsTrigger(shape, box->isTrigger);

                shape->setSimulationFilterData(filterData);
                shape->setQueryFilterData(filterData);

                glm::vec3 worldCenter = trans->LocalTransformation.position + box->box.center * scale;
                glm::vec3 worldSize = box->box.size * scale;
                box->box.bounds.center = worldCenter;
                box->box.bounds.size = worldSize;
                box->box.bounds.extents = worldSize * 0.5f;
                box->box.bounds.min = worldCenter - box->box.bounds.extents;
                box->box.bounds.max = worldCenter + box->box.bounds.extents;

                if (shape->getActor()) { staticActor = shape->getActor()->is<PxRigidStatic>(); }
            }
            else if (SphereColliderComponent* sphere = ecs->GetComponent<SphereColliderComponent>(id)) {
                float radius = sphere->sphere.radius * glm::max(scale.x, glm::max(scale.y, scale.z));
                PxShape* shape = static_cast<PxShape*>(sphere->shape);

                if (!shape) {
                    shape = PhysicsManager::GetInstance()->GetPhysics()->createShape(
                        PxSphereGeometry(radius),
                        *PhysicsManager::GetInstance()->GetDefaultMaterial(),
                        true
                    );
                    sphere->shape = shape;
                }

                glm::vec3 c = sphere->sphere.center * scale;
                shape->setLocalPose(PxTransform{ PxVec3{ c.x, c.y, c.z } });
                ToPhysxIsTrigger(shape, sphere->isTrigger);

                shape->setSimulationFilterData(filterData);
                shape->setQueryFilterData(filterData);

                if (shape->getActor()) { staticActor = shape->getActor()->is<PxRigidStatic>(); }
            }
            else if (CapsuleColliderComponent* capsule = ecs->GetComponent<CapsuleColliderComponent>(id)) {
                float radius = capsule->capsule.radius * glm::max(scale.x, scale.z);
                float halfHeight = (capsule->capsule.height * scale.y) * 0.5f;
                PxShape* shape = static_cast<PxShape*>(capsule->shape);

                if (!shape) {
                    shape = PhysicsManager::GetInstance()->GetPhysics()->createShape(
                        PxCapsuleGeometry(radius, halfHeight),
                        *PhysicsManager::GetInstance()->GetDefaultMaterial(),
                        true
                    );
                    capsule->shape = shape;
                }

                PxTransform localPose(PxVec3{ capsule->capsule.center.x * scale.x, capsule->capsule.center.y * scale.y, capsule->capsule.center.z * scale.z });
                
                switch (capsule->capsule.capsuleDirection) {
                    case CapsuleDirection::Y:
                        localPose.q = PxQuat{ PxHalfPi, PxVec3{ 0.0f, 0.0f, 1.0f } };
                        break;
                    case CapsuleDirection::Z:
                        localPose.q = PxQuat{ -PxHalfPi, PxVec3{ 0.0f, 1.0f, 0.0f } };
                        break;
                    default: 
                        break;
                }

                shape->setLocalPose(localPose);
                ToPhysxIsTrigger(shape, capsule->isTrigger);

                shape->setSimulationFilterData(filterData);
                shape->setQueryFilterData(filterData);

                if (shape->getActor()) { staticActor = shape->getActor()->is<PxRigidStatic>(); }
            }

            if (!staticActor) {
                PxTransform pos{ PxVec3{ trans->LocalTransformation.position.x, trans->LocalTransformation.position.y, trans->LocalTransformation.position.z } };
                staticActor = PhysicsManager::GetInstance()->GetPhysics()->createRigidStatic(pos);
                staticActor->userData = reinterpret_cast<void*>(static_cast<std::uintptr_t>(id));
                PhysicsManager::GetInstance()->GetScene()->addActor(*staticActor);
                if (BoxColliderComponent* box = ecs->GetComponent<BoxColliderComponent>(id)) {
                    staticActor->attachShape(*static_cast<PxShape*>(box->shape));
                }
                else if (SphereColliderComponent* sphere = ecs->GetComponent<SphereColliderComponent>(id)) {
                    staticActor->attachShape(*static_cast<PxShape*>(sphere->shape));
                }
                else if (CapsuleColliderComponent* capsule = ecs->GetComponent<CapsuleColliderComponent>(id)) {
                    staticActor->attachShape(*static_cast<PxShape*>(capsule->shape));
                }
            }
            else {
                PxTransform currentPos{ PxVec3{ trans->LocalTransformation.position.x, trans->LocalTransformation.position.y, trans->LocalTransformation.position.z } };
                staticActor->setGlobalPose(currentPos);
            }
        }
	}
}