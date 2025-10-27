#ifndef SCRIPTCOMP_H
#define SCRIPTCOMP_H
#include "Component.h"

namespace ecs {

    class ScriptComponent : public Component {
    public:
        class ScriptWrapper {
        public:
            std::string scriptName;
            std::shared_ptr<void> script;
        };
        std::vector <std::string> scriptNames;
        std::vector<ScriptWrapper>scriptFunctions;
        REFLECTABLE(ScriptComponent, scriptNames);

    };

}
#endif