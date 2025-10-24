#ifndef PHYSICSEVENTCALLBACK_H
#define PHYSICSEVENTCALLBACK_H

#include "ECS/ECSList.h"
#include "Events/Delegate.h"
#include "PHYSX/PxPhysicsAPI.h"

using namespace ecs;
using namespace physx;

namespace physics {
	struct CollisionInfo {
		EntityID entityA;
		EntityID entityB;
		PxActor* actorA;
		PxActor* actorB;
		PxShape* shapeA;
		PxShape* shapeB;
		std::vector<PxVec3> contactPoints;
		PxVec3 contactNormal;
		float impulse;
	};

	struct CollisionCallbacks {
		Delegate<const CollisionInfo&> OnCollisionEnter;
		Delegate<const CollisionInfo&> OnCollisionStay;
		Delegate<const CollisionInfo&> OnCollisionExit;
	};

	struct TriggerCallbacks {
		Delegate<const CollisionInfo&> OnTriggerEnter;
		Delegate<const CollisionInfo&> OnTriggerStay;
		Delegate<const CollisionInfo&> OnTriggerExit;
	};

    class PhysicsEventCallback : public PxSimulationEventCallback {
    public:
        PhysicsEventCallback() = default;
        virtual ~PhysicsEventCallback() = default;

        void onContact(const PxContactPairHeader& pairHeader, const PxContactPair* pairs, PxU32 nbPairs) override {
            for (PxU32 i = 0; i < nbPairs; i++) {
                const PxContactPair& cp = pairs[i];

                EntityID entityA = reinterpret_cast<EntityID>(pairHeader.actors[0]->userData);
                EntityID entityB = reinterpret_cast<EntityID>(pairHeader.actors[1]->userData);

                CollisionInfo info;
                info.entityA = entityA;
                info.entityB = entityB;
                info.actorA = pairHeader.actors[0];
                info.actorB = pairHeader.actors[1];
                info.shapeA = cp.shapes[0];
                info.shapeB = cp.shapes[1];

                PxContactPairPoint contacts[16];
                PxU32 nbContacts = cp.extractContacts(contacts, 16);
                for (PxU32 j = 0; j < nbContacts; j++) { info.contactPoints.push_back(contacts[j].position); }

                if (nbContacts > 0) {
                    info.contactNormal = contacts[0].normal;
                    info.impulse = contacts[0].impulse.magnitude();
                }

                if (cp.events & PxPairFlag::eNOTIFY_TOUCH_FOUND) {
                    InvokeCollisionEnter(entityA, info);
                    CollisionInfo infoB = info;
                    std::swap(infoB.entityA, infoB.entityB);
                    std::swap(infoB.actorA, infoB.actorB);
                    std::swap(infoB.shapeA, infoB.shapeB);
                    infoB.contactNormal = -infoB.contactNormal;
                    InvokeCollisionEnter(entityB, infoB);
                }

                if (cp.events & PxPairFlag::eNOTIFY_TOUCH_PERSISTS) {
                    InvokeCollisionStay(entityA, info);
                    CollisionInfo infoB = info;
                    std::swap(infoB.entityA, infoB.entityB);
                    std::swap(infoB.actorA, infoB.actorB);
                    std::swap(infoB.shapeA, infoB.shapeB);
                    infoB.contactNormal = -infoB.contactNormal;
                    InvokeCollisionStay(entityB, infoB);
                }

                if (cp.events & PxPairFlag::eNOTIFY_TOUCH_LOST) {
                    InvokeCollisionExit(entityA, info);
                    CollisionInfo infoB = info;
                    std::swap(infoB.entityA, infoB.entityB);
                    std::swap(infoB.actorA, infoB.actorB);
                    std::swap(infoB.shapeA, infoB.shapeB);
                    infoB.contactNormal = -infoB.contactNormal;
                    InvokeCollisionExit(entityB, infoB);
                }
            }
        }

        void onTrigger(PxTriggerPair* pairs, PxU32 count) override {
            for (PxU32 i = 0; i < count; i++) {
                const PxTriggerPair& tp = pairs[i];

                if (tp.flags & (PxTriggerPairFlag::eREMOVED_SHAPE_TRIGGER |
                    PxTriggerPairFlag::eREMOVED_SHAPE_OTHER)) {
                    continue;
                }

                EntityID triggerEntity = reinterpret_cast<EntityID>(tp.triggerActor->userData);
                EntityID otherEntity = reinterpret_cast<EntityID>(tp.otherActor->userData);

                CollisionInfo info;
                info.entityA = triggerEntity;
                info.entityB = otherEntity;
                info.actorA = tp.triggerActor;
                info.actorB = tp.otherActor;
                info.shapeA = tp.triggerShape;
                info.shapeB = tp.otherShape;

                if (tp.status & PxPairFlag::eNOTIFY_TOUCH_FOUND) {
                    InvokeTriggerEnter(triggerEntity, info);
                    CollisionInfo infoB = info;
                    std::swap(infoB.entityA, infoB.entityB);
                    std::swap(infoB.actorA, infoB.actorB);
                    std::swap(infoB.shapeA, infoB.shapeB);
                    InvokeTriggerEnter(otherEntity, infoB);
                }
                else if (tp.status & PxPairFlag::eNOTIFY_TOUCH_LOST) {
                    InvokeTriggerExit(triggerEntity, info);
                    CollisionInfo infoB = info;
                    std::swap(infoB.entityA, infoB.entityB);
                    std::swap(infoB.actorA, infoB.actorB);
                    std::swap(infoB.shapeA, infoB.shapeB);
                    InvokeTriggerExit(otherEntity, infoB);
                }
            }
        }

        CollisionCallbacks* RegisterCollisionCallbacks(EntityID entity) {
            if (m_collisionCallbacks.find(entity) == m_collisionCallbacks.end()) {
                m_collisionCallbacks[entity] = std::make_unique<CollisionCallbacks>();
            }
            return m_collisionCallbacks[entity].get();
        }

        TriggerCallbacks* RegisterTriggerCallbacks(EntityID entity) {
            if (m_triggerCallbacks.find(entity) == m_triggerCallbacks.end()) {
                m_triggerCallbacks[entity] = std::make_unique<TriggerCallbacks>();
            }
            return m_triggerCallbacks[entity].get();
        }

        void UnregisterCallbacks(EntityID entity) {
            m_collisionCallbacks.erase(entity);
            m_triggerCallbacks.erase(entity);
            m_triggerStayPairs.erase(entity);
        }

        void ProcessTriggerStay() {
            for (auto& [entity, otherEntities] : m_triggerStayPairs) {
                auto it = m_triggerCallbacks.find(entity);
                if (it != m_triggerCallbacks.end()) {
                    for (EntityID other : otherEntities) {
                        CollisionInfo info;
                        info.entityA = entity;
                        info.entityB = other;
                        it->second->OnTriggerStay.Invoke(info);
                    }
                }
            }
        }

        void onConstraintBreak(PxConstraintInfo*, PxU32) override {}
        void onWake(PxActor**, PxU32) override {}
        void onSleep(PxActor**, PxU32) override {}
        void onAdvance(const PxRigidBody* const*, const PxTransform*, const PxU32) override {}
    private:
        void InvokeCollisionEnter(EntityID entity, const CollisionInfo& info) {
            auto it = m_collisionCallbacks.find(entity);
            if (it != m_collisionCallbacks.end()) {
                it->second->OnCollisionEnter.Invoke(info);
            }
        }

        void InvokeCollisionStay(EntityID entity, const CollisionInfo& info) {
            auto it = m_collisionCallbacks.find(entity);
            if (it != m_collisionCallbacks.end()) {
                it->second->OnCollisionStay.Invoke(info);
            }
        }

        void InvokeCollisionExit(EntityID entity, const CollisionInfo& info) {
            auto it = m_collisionCallbacks.find(entity);
            if (it != m_collisionCallbacks.end()) {
                it->second->OnCollisionExit.Invoke(info);
            }
        }

        void InvokeTriggerEnter(EntityID entity, const CollisionInfo& info) {
            auto it = m_triggerCallbacks.find(entity);
            if (it != m_triggerCallbacks.end()) {
                it->second->OnTriggerEnter.Invoke(info);
                m_triggerStayPairs[entity].insert(info.entityB);
            }
        }

        void InvokeTriggerExit(EntityID entity, const CollisionInfo& info) {
            auto it = m_triggerCallbacks.find(entity);
            if (it != m_triggerCallbacks.end()) {
                it->second->OnTriggerExit.Invoke(info);
                m_triggerStayPairs[entity].erase(info.entityB);
            }
        }

        std::unordered_map<EntityID, std::unique_ptr<CollisionCallbacks>> m_collisionCallbacks;
        std::unordered_map<EntityID, std::unique_ptr<TriggerCallbacks>> m_triggerCallbacks;
        std::unordered_map<EntityID, std::set<EntityID>> m_triggerStayPairs;
    };
}

#endif