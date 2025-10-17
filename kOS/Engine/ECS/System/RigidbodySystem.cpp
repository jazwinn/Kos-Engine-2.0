/********************************************************************/
/*!
\file       RigidbodySystem.cpp
\author		Toh Yu Heng 2301294
\par		t.yuheng@digipen.edu
\date		Oct 01 2025
\brief		Implements the RigidbodySystem which manages all dynamic
            rigidbody entities in the ECS using NVIDIA PhysX.

            Responsibilities:
            - Converts static colliders to dynamic rigidbodies when required
            - Initializes and configures PhysX PxRigidDynamic actors
            - Applies physics parameters such as mass, drag, gravity, etc.
            - Synchronizes ECS transforms with PhysX simulation results
            - Supports both kinematic and non-kinematic rigidbodies

            Automatically handles collider shape recreation when moving
            from static to dynamic and keeps ECS transforms up to date.

Copyright (C) 2025 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the
prior written consent of DigiPen Institute of Technology is prohibited.
*/
/********************************************************************/

#include "Config/pch.h"
#include "RigidbodySystem.h"
#include "Physics/PhysicsManager.h"
#include "Physics/PhysxUtils.h"

using namespace physics;

namespace ecs {
    void RigidbodySystem::Init() {
        onDeregister.Add([](EntityID id) {
            ECS* ecs = ECS::GetInstance();
            RigidbodyComponent* rb = ecs->GetComponent<RigidbodyComponent>(id);
            if (rb) {
                if (PxRigidDynamic* actor = reinterpret_cast<PxRigidDynamic*>(rb->actor)) {
                    PhysicsManager::GetInstance()->GetScene()->removeActor(*actor);
                    actor->release();
                    rb->actor = nullptr;
                }
            }
            });
    }

    void RigidbodySystem::Update(const std::string& scene) {
        ECS* ecs = ECS::GetInstance();
        const auto& entities = m_entities.Data();
        for (EntityID id : entities) {
            NameComponent* name = ecs->GetComponent<NameComponent>(id);
            TransformComponent* trans = ecs->GetComponent<TransformComponent>(id);
            RigidbodyComponent* rb = ecs->GetComponent<RigidbodyComponent>(id);

            if (!rb || !trans || trans->scene != scene || name->hide) { continue; }

            PxRigidDynamic* actor = reinterpret_cast<PxRigidDynamic*>(rb->actor);

            if (!actor) {
                if (BoxColliderComponent* box = ecs->GetComponent<BoxColliderComponent>(id)) {
                    if (box->shape) {
                        PxShape* shape = static_cast<PxShape*>(box->shape);
                        if (shape->getActor()) {
                            PxRigidStatic* oldStatic = shape->getActor()->is<PxRigidStatic>();
                            if (oldStatic) {
                                PhysicsManager::GetInstance()->GetScene()->removeActor(*oldStatic);
                                oldStatic->release();
                            }
                        }
                    }
                }
                else if (SphereColliderComponent* sphere = ecs->GetComponent<SphereColliderComponent>(id)) {
                    if (sphere->shape) {
                        PxShape* shape = static_cast<PxShape*>(sphere->shape);
                        if (shape->getActor()) {
                            PxRigidStatic* oldStatic = shape->getActor()->is<PxRigidStatic>();
                            if (oldStatic) {
                                PhysicsManager::GetInstance()->GetScene()->removeActor(*oldStatic);
                                oldStatic->release();
                            }
                        }
                    }
                }
                else if (CapsuleColliderComponent* capsule = ecs->GetComponent<CapsuleColliderComponent>(id)) {
                    if (capsule->shape) {
                        PxShape* shape = static_cast<PxShape*>(capsule->shape);
                        if (shape->getActor()) {
                            PxRigidStatic* oldStatic = shape->getActor()->is<PxRigidStatic>();
                            if (oldStatic) {
                                PhysicsManager::GetInstance()->GetScene()->removeActor(*oldStatic);
                                oldStatic->release();
                            }
                        }
                    }
                }
                glm::quat rot(glm::radians(trans->LocalTransformation.rotation));
                PxTransform pos{ PxVec3{ trans->LocalTransformation.position.x, trans->LocalTransformation.position.y, trans->LocalTransformation.position.z }, PxQuat{ rot.x, rot.y, rot.z, rot.w } };
                actor = PhysicsManager::GetInstance()->GetPhysics()->createRigidDynamic(pos);
                actor->setLinearDamping(rb->drag);
                actor->setAngularDamping(rb->angularDrag);
                actor->setRigidBodyFlag(PxRigidBodyFlag::eKINEMATIC, rb->isKinematic);
                actor->setActorFlag(PxActorFlag::eDISABLE_GRAVITY, !rb->useGravity);
                actor->setRigidDynamicLockFlags(ToPhysXContraints(rb->constraints));
                ToPhysxCollisionDetectionMode(actor, rb->collisionDetection);
                actor->userData = reinterpret_cast<void*>(static_cast<std::uintptr_t>(id));
                rb->actor = actor;
                PhysicsManager::GetInstance()->GetScene()->addActor(*actor);
                glm::vec3 scale = trans->LocalTransformation.scale;
                const float minScale = 0.001f;
                scale = glm::max(glm::abs(scale), glm::vec3(minScale));
                PxFilterData filterData;
                filterData.word0 = name->Layer;
                if (BoxColliderComponent* box = ecs->GetComponent<BoxColliderComponent>(id)) {
                    glm::vec3 sz = glm::max(glm::abs(box->box.size * scale), glm::vec3(minScale));
                    PxShape* newShape = PhysicsManager::GetInstance()->GetPhysics()->createShape(
                        PxBoxGeometry(sz.x * 0.5f, sz.y * 0.5f, sz.z * 0.5f),
                        *PhysicsManager::GetInstance()->GetDefaultMaterial(),
                        true
                    );
                    PxVec3 c = PxVec3(box->box.center.x * scale.x, box->box.center.y * scale.y, box->box.center.z * scale.z);
                    newShape->setLocalPose(PxTransform(c));
                    newShape->setSimulationFilterData(filterData);
                    newShape->setQueryFilterData(filterData);
                    actor->attachShape(*newShape);
                    PxRigidBodyExt::updateMassAndInertia(*actor, rb->mass);
                }
                else if (SphereColliderComponent* sphere = ecs->GetComponent<SphereColliderComponent>(id)) {
                    float radius = sphere->sphere.radius * glm::max(scale.x, glm::max(scale.y, scale.z));
                    PxShape* newShape = PhysicsManager::GetInstance()->GetPhysics()->createShape(
                        PxSphereGeometry(radius),
                        *PhysicsManager::GetInstance()->GetDefaultMaterial(),
                        true
                    );
                    PxVec3 c = PxVec3(sphere->sphere.center.x * scale.x, sphere->sphere.center.y * scale.y, sphere->sphere.center.z * scale.z);
                    newShape->setLocalPose(PxTransform(c));
                    newShape->setSimulationFilterData(filterData);
                    newShape->setQueryFilterData(filterData);
                    actor->attachShape(*newShape);
                    PxRigidBodyExt::updateMassAndInertia(*actor, rb->mass);
                }
                else if (CapsuleColliderComponent* capsule = ecs->GetComponent<CapsuleColliderComponent>(id)) {
                    float radius = capsule->capsule.radius * glm::max(scale.x, scale.z);
                    float halfHeight = (capsule->capsule.height * scale.y) * 0.5f;
                    PxShape* newShape = PhysicsManager::GetInstance()->GetPhysics()->createShape(
                        PxCapsuleGeometry(radius, halfHeight),
                        *PhysicsManager::GetInstance()->GetDefaultMaterial(),
                        true
                    );
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
                    newShape->setLocalPose(localPose);
                    newShape->setSimulationFilterData(filterData);
                    newShape->setQueryFilterData(filterData);
                    actor->attachShape(*newShape);
                    PxRigidBodyExt::updateMassAndInertia(*actor, rb->mass);
                }
                rb->prevPos = trans->LocalTransformation.position;
                rb->prevRot = trans->LocalTransformation.rotation;
                rb->isChanged = true;
            }
            if (rb->isKinematic) {
                glm::quat rot(glm::radians(trans->LocalTransformation.rotation));
                PxTransform pos{
                    PxVec3{ trans->LocalTransformation.position.x, trans->LocalTransformation.position.y, trans->LocalTransformation.position.z },
                    PxQuat{ rot.x, rot.y, rot.z, rot.w }
                };
                actor->setKinematicTarget(pos);
                rb->prevPos = trans->LocalTransformation.position;
                rb->prevRot = trans->LocalTransformation.rotation;
            }
            else {
                PxTransform currPxPos = actor->getGlobalPose();
                glm::vec3 physicsPos(currPxPos.p.x, currPxPos.p.y, currPxPos.p.z);
                PxQuat currPxQuat = currPxPos.q;
                glm::quat physicsQuat(currPxQuat.w, currPxQuat.x, currPxQuat.y, currPxQuat.z);
                glm::vec3 physicsRot = glm::degrees(glm::eulerAngles(physicsQuat));
                const float smallChangeThreshold = 0.001f;
                const float largeChangeThreshold = 0.1f;
                const float rotThreshold = 0.01f;
                bool transformWasModified = false;
                bool isLargeChange = false;
                float posDiff = 0.0f;
                if (rb->isChanged) {
                    posDiff = glm::length(trans->LocalTransformation.position - rb->prevPos);
                    float rotDiff = glm::length(trans->LocalTransformation.rotation - rb->prevRot);
                    transformWasModified = (posDiff > smallChangeThreshold) || (rotDiff > rotThreshold);
                    isLargeChange = (posDiff > largeChangeThreshold) || (rotDiff > 1.0f);
                }
                if (transformWasModified) {
                    if (isLargeChange) {
                        glm::quat newRot(glm::radians(trans->LocalTransformation.rotation));
                        PxTransform newTransform{
                            PxVec3{ trans->LocalTransformation.position.x, trans->LocalTransformation.position.y, trans->LocalTransformation.position.z },
                            PxQuat{ newRot.x, newRot.y, newRot.z, newRot.w }
                        };
                        actor->setGlobalPose(newTransform);
                        actor->setLinearVelocity(PxVec3{ 0.0f, 0.0f, 0.0f });
                        actor->setAngularVelocity(PxVec3{ 0.0f, 0.0f, 0.0f });
                    }
                    else {
                        glm::vec3 posDelta = trans->LocalTransformation.position - rb->prevPos;
                        PxVec3 currentVel = actor->getLinearVelocity();
                        PxVec3 deltaVel(posDelta.x * 60.0f, posDelta.y * 60.0f, posDelta.z * 60.0f);
                        actor->setLinearVelocity(PxVec3(deltaVel.x, currentVel.y, deltaVel.z));
                    }
                }
                currPxPos = actor->getGlobalPose();
                glm::vec3 finalPos(currPxPos.p.x, currPxPos.p.y, currPxPos.p.z);
                currPxQuat = currPxPos.q;
                glm::quat finalQuat(currPxQuat.w, currPxQuat.x, currPxQuat.y, currPxQuat.z);
                glm::vec3 finalRot = glm::degrees(glm::eulerAngles(finalQuat));
                trans->LocalTransformation.position = finalPos;
                trans->LocalTransformation.rotation = finalRot;
                rb->prevPos = finalPos;
                rb->prevRot = finalRot;
                rb->isChanged = true;
            }
        }
    }
}