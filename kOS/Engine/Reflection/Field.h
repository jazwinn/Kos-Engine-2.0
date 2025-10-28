/******************************************************************/
/*!
\file      Field.h
\author    Jaz Winn Ng
\par       jazwinn.ng@digipen.edu
\date      Sept 28, 2025
\brief     Field class for reflecting script componetns and their fields.

Copyright (C) 2025 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the
prior written consent of DigiPen Institute of Technology is prohibited.
*/
/******************************************************************/

#pragma once

#include "Config/pch.h"

struct FieldReference {
    std::string name;
    std::type_index type;
    void* address = nullptr;

    template <typename T>
    bool IsType() const { return type == typeid(T); }
    
    template <typename T>
    T& AsType() const { return *static_cast<T*>(address); }


 //   // Type checks
 //   bool is_int() const { return type_name == typeid(int).name(); }
 //   bool is_float() const { return type_name == typeid(float).name(); }
 //   bool is_bool() const { return type_name == typeid(bool).name(); }
 //   bool is_string() const { return type_name == typeid(std::string).name(); }
 //   bool is_vec3() const { return type_name == typeid(glm::vec3).name(); }
	//bool is_vec2() const { return type_name == typeid(glm::vec2).name(); }
	//bool is_vec4() const { return type_name == typeid(glm::vec4).name(); }

 //   // Get typed references
 //   int& as_int() const { return *static_cast<int*>(address); }
 //   float& as_float() const { return *static_cast<float*>(address); }
 //   bool& as_bool() const { return *static_cast<bool*>(address); }
 //   std::string& as_string() const { return *static_cast<std::string*>(address); }
	//glm::vec3& as_vec3() const { return *static_cast<glm::vec3*>(address); }
	//glm::vec2& as_vec2() const { return *static_cast<glm::vec2*>(address); }
	//glm::vec4& as_vec4() const { return *static_cast<glm::vec4*>(address); }
};

class FieldReferenceCollector {
private:
    std::vector<FieldReference> m_fields;
    const std::vector<std::string>& m_fieldNames;
    size_t m_currentIndex = 0;

public:
    FieldReferenceCollector(const std::vector<std::string>& fieldNames)
        : m_fieldNames(fieldNames) {
    }

    std::vector<FieldReference> GetFieldReferences() const { return m_fields; }

    template<typename T>
    void operator()(T& value) {
        m_fields.emplace_back(m_fieldNames[m_currentIndex], typeid(T), &value);
        m_currentIndex++;
    }
};


class IFieldInvoker {
public:
    virtual ~IFieldInvoker() = default;
    virtual std::string GetFieldName() = 0;
    virtual std::vector<FieldReference> GetFieldReference(void*) = 0;
    virtual std::vector<std::string> GetNames() = 0;

};


template <typename T>
class FieldInvoker : public IFieldInvoker {
public:
    std::string GetFieldName() override {
        return T::classname();
    }

    std::vector<std::string> GetNames() override {
		return T::NamesV();
    }

    std::vector<FieldReference> GetFieldReference(void* _component) override {
            T* component = static_cast<T*>(_component);
            auto fieldNames = T::NamesV();
            FieldReferenceCollector collector(fieldNames);
            component->ApplyFunction(collector);
            return collector.GetFieldReferences();
     }

};

class FieldComponentTypeRegistry {
private:
    static std::unordered_map<std::string, std::function<std::shared_ptr<IFieldInvoker>()>> actionFactories;

public:
    template <typename T>
    static void RegisterComponentType() {
        actionFactories[T::classname()] = []() {
            return std::make_shared<FieldInvoker<T>>();
            };

    }

    static void CreateAllDrawers(std::unordered_map<std::string, std::shared_ptr<IFieldInvoker>>& drawers) {
        for (const auto& [className, factory] : actionFactories) {
            drawers[className] = factory();
        }
    }
};

class FieldSingleton {
public:
    //singleton
    static FieldSingleton* GetInstance() {
        if (!m_InstancePtr) {
            m_InstancePtr.reset(new FieldSingleton{});
        }
        return m_InstancePtr.get();
    }

    std::unordered_map<std::string, std::shared_ptr<IFieldInvoker>>& GetAction() {
        return componentAction;
    }
private:

    std::unordered_map<std::string, std::shared_ptr<IFieldInvoker>> componentAction;

    FieldSingleton() = default;

    static std::shared_ptr<FieldSingleton> m_InstancePtr;

};