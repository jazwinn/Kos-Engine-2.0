#pragma once

#include "ECS/Component/Component.h"
#include "Config/pch.h"
#include "ScriptAdapter/TemplateSC.h"


class EnemyScripts : public TemplateSC {
public:
	int health;
	int shield;
	bool healthbool;
	std::vector<std::string> strings;


	void Start() {
		health = 11131410;
		shield = 501111111;
		healthbool = true;
		std::cout << "start called,  123" << std::endl;
	}

	void Update() {
		// Example logic: Regenerate shield over time
		if (shield < 50) {
			shield += 2; // Regenerate 1 shield per update
		}
		

	}

	REFLECTABLE(EnemyScripts, health, shield, healthbool, strings);
};