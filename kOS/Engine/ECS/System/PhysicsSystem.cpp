#include "Config/pch.h"
#include "PhysicsSystem.h"
#include "Physics/PhysicsManager.h"

using namespace physics;

namespace ecs {
    void PhysicsSystem::Init() {
        auto pm = PhysicsManager::GetInstance();
        pm->Init();
    }

    void PhysicsSystem::Update(const std::string& scene) {
        auto pm = PhysicsManager::GetInstance();
        ECS* ecs = ECS::GetInstance();

        const auto& entities = m_entities.Data();

        for (EntityID id : entities) {
            auto* rb = ecs->GetComponent<RigidbodyComponent>(id);
            auto* trans = ecs->GetComponent<TransformComponent>(id);
            auto* name = ecs->GetComponent<NameComponent>(id);

            if (!rb || !trans || trans->scene != scene || name->hide) { continue; }

            if (!rb->isKinematic) {
                PxRigidActor* actor = static_cast<PxRigidActor*>(rb->actor);
                if (actor) {
                    glm::vec3 pos = trans->WorldTransformation.position;
                    glm::quat rot = glm::quat(glm::radians(trans->WorldTransformation.rotation));
                    PxTransform pxTrans{ PxVec3{ pos.x, pos.y, pos.z }, PxQuat{ rot.x, rot.y, rot.z, rot.w } };
                    actor->setGlobalPose(pxTrans);
                }
            }
        }

        pm->Update(ecs->deltaTime);

        for (EntityID id : entities) {
            auto* rb = ecs->GetComponent<RigidbodyComponent>(id);
            auto* trans = ecs->GetComponent<TransformComponent>(id);
            auto* name = ecs->GetComponent<NameComponent>(id);

            if (!rb || !trans || trans->scene != scene || name->hide) { continue; }

            PxRigidActor* actor = static_cast<PxRigidActor*>(rb->actor);
            if (actor && !rb->isKinematic) {
                PxTransform pxTrans = actor->getGlobalPose();
                TransformSystem::SetImmediateWorldPosition(trans, glm::vec3{ pxTrans.p.x,pxTrans.p.y,pxTrans.p.z });
                //trans->LocalTransformation.position = glm::vec3{ pxTrans.p.x, pxTrans.p.y, pxTrans.p.z };
                glm::quat q{ pxTrans.q.w, pxTrans.q.x, pxTrans.q.y, pxTrans.q.z };
                //trans->LocalTransformation.rotation = glm::degrees(glm::eulerAngles(q));
                TransformSystem::SetImmediateWorldRotation(trans, glm::degrees(glm::eulerAngles(q)));

            }
        }
    }
}