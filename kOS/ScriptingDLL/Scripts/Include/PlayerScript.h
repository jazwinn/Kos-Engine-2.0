#pragma once

#include "ScriptAdapter/TemplateSC.h"

class PlayerScript : public TemplateSC {
public:
    int health;
    int shield;
    bool healthbool;
    std::string shield2;
    glm::vec3 vec3;
    glm::vec4 vec4;

    void Start() override {
        health = 1;
        shield = 50;

    }

    void Update() override {
        // Example: simple movement / regen
        if (shield < 50) shield += 2;

        if (auto* tc = ecsPtr->GetComponent<ecs::TransformComponent>(entity)) {
            tc->LocalTransformation.position.x += 0.01f;
        }

    }

    REFLECTABLE(PlayerScript, health, shield, healthbool, shield2);
};
