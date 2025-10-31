#ifndef SCRIPTINGSYS_H
#define SCRIPTINGSYS_H


#include "System.h"
#include "ECS/ECSList.h"
#include <glad/glad.h>
#include <GLFW/glfw3.h>

namespace ecs {

	class ScriptingSystem : public ISystem {

	public:


		void Init() override;
		void Update() override;

		REFLECTABLE(ScriptingSystem)
	private:
	};
}


#endif