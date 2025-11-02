#include "TemplateSC.h"
//#include "Inputs/Input.h"

class PlayerManagerScript : public TemplateSC {
public:
	int playerHealth;
	float playerMovementSpeed;
	float playerJumpForce;

	float playerCameraSpeedX;
	float playerCameraSpeedY;

	float rotationX = 0.f, rotationY = 0.f;

	void Start() override {

	}

	void Update() override {
		if (auto* tc = ecsPtr->GetComponent<ecs::TransformComponent>(entity)) {

			glm::vec3 rotationInDegrees(tc->LocalTransformation.rotation);
			glm::vec3 rotationInRad = glm::radians(rotationInDegrees);
			glm::quat q = glm::quat(rotationInRad);

			glm::vec3 forward = q * glm::vec3(0.f, 0.f, 1.f);
			glm::vec3 right = q * glm::vec3(1.f, 0.f, 0.f);

			// Movement Inputs
			if (Input->IsKeyPressed(keys::W)) {
				tc->LocalTransformation.position += glm::normalize(forward) * playerMovementSpeed * ecsPtr->m_GetDeltaTime();
			}

			if (Input->IsKeyPressed(keys::S)) {
				tc->LocalTransformation.position -= glm::normalize(forward) * playerMovementSpeed * ecsPtr->m_GetDeltaTime();
			}

			if (Input->IsKeyPressed(keys::D)) {
				tc->LocalTransformation.position -= glm::normalize(right) * playerMovementSpeed * ecsPtr->m_GetDeltaTime();
			}

			if (Input->IsKeyPressed(keys::A)) {
				tc->LocalTransformation.position += glm::normalize(right) * playerMovementSpeed * ecsPtr->m_GetDeltaTime();
			}

			// First Person Camera
			// THIS IS SUPER CURSED FOR NOW I SWEAR -> HARDCODING THE CAMERA TO BE FIRST CHILD
			if (auto* cc = ecsPtr->GetComponent<ecs::CameraComponent>(ecsPtr->GetComponent<ecs::TransformComponent>(entity)->m_childID[0])) {				
				//std::cout << "CAMERA EXISTS\n";
				
				float mouseRotationX = Input->GetAxisRaw("Mouse Y") * playerCameraSpeedX;
				float mouseRotationY = Input->GetAxisRaw("Mouse X") * playerCameraSpeedY;
				rotationX += mouseRotationX;
				rotationY += mouseRotationY;
				rotationX = glm::clamp(rotationX, -90.f, 90.f);
				auto* cameraTransform = ecsPtr->GetComponent<ecs::TransformComponent>(ecsPtr->GetComponent<ecs::TransformComponent>(entity)->m_childID[0]);
				cameraTransform->LocalTransformation.rotation = glm::vec3(rotationX, rotationY + 90.f, 0.f);
				//std::cout << "CAMERA: " << cameraTransform->LocalTransformation.rotation.x << ", " << cameraTransform->LocalTransformation.rotation.y << std::endl;
				tc->LocalTransformation.rotation = glm::vec3(0.f, -rotationY, 0.f);
			}

			// Shooting Inputs
			if (Input->IsKeyTriggered(keys::LMB)) {
				ecs::EntityID bullet = ecsPtr->CreateEntity(ecsPtr->GetSceneByEntityID(entity));
				ecsPtr->GetComponent<NameComponent>(bullet)->entityName = "Bullet";
				ecsPtr->AddComponent<BoxColliderComponent>(bullet);
				ecsPtr->AddComponent<RigidbodyComponent>(bullet);
				ecsPtr->AddComponent<BulletLogic>(bullet);

				if (auto* bulletTransform = ecsPtr->GetComponent<TransformComponent>(bullet)) {
					bulletTransform->LocalTransformation.position = tc->LocalTransformation.position;
					bulletTransform->LocalTransformation.rotation = glm::vec3(360.f - rotationX, 360.f - rotationY, 0.f);
				}

				if (auto* bulletCol = ecsPtr->GetComponent<BoxColliderComponent>(bullet)) {
					bulletCol->isTrigger = true;
				}

				if (auto* bulletRb = ecsPtr->GetComponent<RigidbodyComponent>(bullet)) {
					bulletRb->useGravity = false;
				}

				if (auto* bulletScript = ecsPtr->GetComponent<BulletLogic>(bullet)) {
					bulletScript->direction = forward;
				}
			}

			// Interact Inputs
			if (Input->IsKeyTriggered(keys::E)) {

			}
		}
	}

	REFLECTABLE(PlayerManagerScript, playerHealth, playerMovementSpeed, playerJumpForce, playerCameraSpeedX, playerCameraSpeedY);
};