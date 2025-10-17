
#include "Config/pch.h"
#include "ECS/ECS.h"
#include "Scripting/ScriptManager.h"
#include "Config/ComponentRegistry.h"
#include "Scripts/Include/ScriptHeader.h"

static std::vector<std::string>* scriptNames;

template <typename T>
void RegisterScript() {
	ComponentTypeRegistry::RegisterComponentType<T>();
	ComponentRegistry::GetECSInstance()->RegisterComponent<T>();
	scriptNames->push_back(T::classname());
}

extern "C"  __declspec(dllexport) void UpdateStatic(StaticVariableManager* svm) {
	//ScriptManager::m_GetInstance();
	//ecs::ECS::m_in
	//Link the ECS system together
	TemplateSC::ecsPtr = std::shared_ptr<ecs::ECS>(reinterpret_cast<ecs::ECS*>(svm->ECSSystem), [](ecs::ECS*) { /* no delete, managed elsewhere */ }).get();
	scriptNames = svm->scriptNames;
	ComponentRegistry::SetECSInstance(TemplateSC::ecsPtr);
	ComponentRegistry::SetFieldInstance(static_cast<FieldSingleton*>(svm->field));


	RegisterScript<PlayerScript>();
	RegisterScript<EnemyScripts>();
	RegisterScript<AudioScript>();

	ComponentTypeRegistry::CreateAllDrawers((static_cast<FieldSingleton*>(svm->field)->GetAction()));
}