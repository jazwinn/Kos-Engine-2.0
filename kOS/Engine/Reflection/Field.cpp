#include "Field.h"

std::unordered_map<std::string, std::function<std::shared_ptr<IFieldInvoker>()>> ComponentTypeRegistry::actionFactories;

std::shared_ptr<FieldSingleton> FieldSingleton::m_InstancePtr = nullptr;