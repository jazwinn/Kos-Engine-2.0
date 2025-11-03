#include "Field.h"

std::unordered_map<std::string, std::function<std::shared_ptr<IFieldInvoker>()>> FieldComponentTypeRegistry::actionFactories;