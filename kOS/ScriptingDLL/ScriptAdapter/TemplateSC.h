#pragma once

#include "ECS/Component/Component.h"
#include "Config/pch.h"
#include "ECS/ECS.h"
#include "Scripting/Script.h"


class TemplateSC :public ecs::Component,  public ScriptClass {
public:
	//use raw, shared ptr will destruct exe ecs
	
	static ecs::ECS* ecsPtr;
private:
};


