#include "Config/pch.h"
#include "PhysicsSystem.h"
#include "Physics/PhysicsManager.h"

using namespace physics;

namespace ecs {
    void PhysicsSystem::Init() {

        m_physicsManager.Init();
    }

    void PhysicsSystem::Update() {


        const auto& entities = m_entities.Data();

        for (EntityID id : entities) {
            auto* rb = m_ecs.GetComponent<RigidbodyComponent>(id);
            auto* trans = m_ecs.GetComponent<TransformComponent>(id);
            auto* name = m_ecs.GetComponent<NameComponent>(id);

            if (name->hide || !rb->actor) { continue; }

            PxRigidDynamic* actor = static_cast<PxRigidDynamic*>(rb->actor);

            glm::vec3 pos = trans->WorldTransformation.position;
            glm::quat rot = glm::quat(glm::radians(trans->WorldTransformation.rotation));
            PxTransform pxTrans{ PxVec3{ pos.x, pos.y, pos.z }, PxQuat{ rot.x, rot.y, rot.z, rot.w } };

            if (rb->isKinematic) { actor->setKinematicTarget(pxTrans); }
            else { actor->setGlobalPose(pxTrans); }
        }

        m_physicsManager.Update(m_ecs.m_GetDeltaTime());

        for (EntityID id : entities) {
            auto* rb = m_ecs.GetComponent<RigidbodyComponent>(id);
            auto* trans = m_ecs.GetComponent<TransformComponent>(id);
            auto* name = m_ecs.GetComponent<NameComponent>(id);

            if (name->hide || !rb->actor) { continue; }

            PxRigidActor* actor = static_cast<PxRigidActor*>(rb->actor);

            if (!rb->isKinematic) {
                PxTransform pxTrans = actor->getGlobalPose();
                TransformSystem::SetImmediateWorldPosition(m_ecs, trans, glm::vec3{ pxTrans.p.x, pxTrans.p.y, pxTrans.p.z });
                glm::quat q{ pxTrans.q.w, pxTrans.q.x, pxTrans.q.y, pxTrans.q.z };
                TransformSystem::SetImmediateWorldRotation(m_ecs, trans, glm::degrees(glm::eulerAngles(q)));
            }
        }
    }
}