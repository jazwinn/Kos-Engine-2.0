#include "Config/pch.h"
#include "Physics/PhysicsEventCallback.h"
#include "Physics/PhysicsManager.h"

namespace physics {
    void PhysicsEventCallback::onContact(const PxContactPairHeader& pairHeader, const PxContactPair* pairs, PxU32 nbPairs) {
        auto pm = PhysicsManager::GetInstance();

        if (pairHeader.flags & (PxContactPairHeaderFlag::eREMOVED_ACTOR_0 | PxContactPairHeaderFlag::eREMOVED_ACTOR_1)) { return; }

        for (PxU32 i = 0; i < nbPairs; i++) {
            const PxContactPair& cp = pairs[i];

            if (cp.flags & (PxContactPairFlag::eREMOVED_SHAPE_0 | PxContactPairFlag::eREMOVED_SHAPE_1)) { continue; }

            CollisionPair pair{ pairHeader.actors[0], pairHeader.actors[1] };

            EntityID entityA = static_cast<EntityID>(reinterpret_cast<uintptr_t>(pairHeader.actors[0]->userData));
            EntityID entityB = static_cast<EntityID>(reinterpret_cast<uintptr_t>(pairHeader.actors[1]->userData));

            Collision collisionA{};
            Collision collisionB{};
            collisionA.thisEntityID = entityA;
            collisionA.otherEntityID = entityB;

            collisionB.thisEntityID = entityB;
            collisionB.otherEntityID = entityA;

            PxContactPairPoint contacts[16];
            PxU32 contactCount = cp.extractContacts(contacts, 16);
            collisionA.contacts.reserve(contactCount);
            collisionB.contacts.reserve(contactCount);

            glm::vec3 totalImpulse{ 0.0f };
            for (PxU32 j = 0; j < contactCount; ++j) {
                const PxContactPairPoint& p = contacts[j];
                ContactPoint ptA;
                ptA.point = { p.position.x, p.position.y, p.position.z };
                ptA.normal = { p.normal.x, p.normal.y, p.normal.z };
                ptA.impulse = p.impulse.magnitude();
                ptA.separation = p.separation;
                collisionA.contacts.push_back(ptA);
                ContactPoint ptB = ptA;
                ptB.normal = -ptB.normal;
                collisionB.contacts.push_back(ptB);
                totalImpulse += glm::vec3(p.impulse.x, p.impulse.y, p.impulse.z);
            }

            collisionA.impulse = glm::length(totalImpulse);
            collisionB.impulse = glm::length(totalImpulse);

            glm::vec3 velA = GetLinearVelocity(pairHeader.actors[0]);
            glm::vec3 velB = GetLinearVelocity(pairHeader.actors[1]);
            glm::vec3 relVel = velB - velA;
            collisionA.relativeVelocity = relVel;
            collisionB.relativeVelocity = -relVel;

            if (cp.events & PxPairFlag::eNOTIFY_TOUCH_FOUND) {
                pm->OnCollisionEnter.Invoke(collisionA);
                pm->OnCollisionEnter.Invoke(collisionB);
                m_activeCollisions.insert(pair);
            }
            if (cp.events & PxPairFlag::eNOTIFY_TOUCH_PERSISTS) {
                m_activeCollisions.insert(pair);
            }
            if (cp.events & PxPairFlag::eNOTIFY_TOUCH_LOST) {
                pm->OnCollisionExit.Invoke(collisionA);
                pm->OnCollisionExit.Invoke(collisionB);
                m_activeCollisions.erase(pair);
            }
        }
    }

    void PhysicsEventCallback::onTrigger(PxTriggerPair* pairs, PxU32 count) {
        auto pm = PhysicsManager::GetInstance();    

        for (PxU32 i = 0; i < count; ++i) {
            const PxTriggerPair& tp = pairs[i];
            if (tp.flags & (PxTriggerPairFlag::eREMOVED_SHAPE_TRIGGER | PxTriggerPairFlag::eREMOVED_SHAPE_OTHER)) { continue; }

            EntityID entityA = static_cast<EntityID>(reinterpret_cast<uintptr_t>(tp.triggerActor->userData));
            EntityID entityB = static_cast<EntityID>(reinterpret_cast<uintptr_t>(tp.otherActor->userData));

            Collision triggerA{}, triggerB{};
            triggerA.otherEntityID = entityB;
            triggerB.otherEntityID = entityA;

            TriggerPair pair{ tp.triggerActor, tp.otherActor };

            if (tp.status & PxPairFlag::eNOTIFY_TOUCH_FOUND) {
                pm->OnTriggerEnter.Invoke(triggerA);
                pm->OnTriggerEnter.Invoke(triggerB);
                m_activeTriggers.insert(pair);
            }
            else if (tp.status & PxPairFlag::eNOTIFY_TOUCH_LOST) {
                pm->OnTriggerExit.Invoke(triggerA);
                pm->OnTriggerExit.Invoke(triggerB);
                m_activeTriggers.erase(pair);
            }
        }
    }

    void PhysicsEventCallback::ProcessCollisionStay() {
        auto pm = PhysicsManager::GetInstance();

        for (const auto& pair : m_activeCollisions) {
            EntityID entityA = static_cast<EntityID>(reinterpret_cast<uintptr_t>(pair.collision->userData));
            EntityID entityB = static_cast<EntityID>(reinterpret_cast<uintptr_t>(pair.other->userData));

            Collision collisionA{};
            Collision collisionB{};

            collisionA.otherEntityID = entityB;
            collisionB.otherEntityID = entityA;

            pm->OnCollisionStay.Invoke(collisionA);
            pm->OnCollisionStay.Invoke(collisionB);
        }
    }

    void PhysicsEventCallback::ProcessTriggerStay() {
        auto pm = PhysicsManager::GetInstance();

        for (const auto& pair : m_activeTriggers) {
            EntityID entityA = static_cast<EntityID>(reinterpret_cast<uintptr_t>(pair.trigger->userData));
            EntityID entityB = static_cast<EntityID>(reinterpret_cast<uintptr_t>(pair.other->userData));

            Collision triggerA{};
            Collision triggerB{};

            triggerA.otherEntityID = entityB;
            triggerB.otherEntityID = entityA;

            pm->OnTriggerStay.Invoke(triggerA);
            pm->OnTriggerStay.Invoke(triggerB);
        }
    }
}