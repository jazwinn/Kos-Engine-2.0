#include "ScriptAdapter/TemplateSC.h"

class EnemyManagerScript : public TemplateSC {
public:
	int enemyHealth;
	float enemyMovementSpeed;

	void Start() override {

	}

	void Update() override {
		if (auto* tc = ecsPtr->GetComponent<ecs::TransformComponent>(entity)) {

		}
	}

	REFLECTABLE(EnemyManagerScript, enemyHealth, enemyMovementSpeed);
};