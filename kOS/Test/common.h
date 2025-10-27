/******************************************************************/
/*!
\file      Common.h
\author    Ng Jaz winn, jazwinn.ng , 2301502
\par       jazwinn.ng@digipen.edu
\date      Oct 02, 2025
\brief     Macros and common functions for testing.

Copyright (C) 2024 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the
prior written consent of DigiPen Institute of Technology is prohibited.
*/
/********************************************************************/

#include "ECS/Component/ComponentHeader.h"
#include "Config/pch.h"
#include "DeSerialization/json_handler.h"

#define SERIALIZE_DESERIALIZE_COMPARE_TEST(ComponentType) \
TEST(DeSerializeTest, ComponentType##Test) { \
    ComponentType comp; \
    comp.ApplyFunction(RandomizeComponents<decltype(comp.Names())>{comp.Names()}); \
    const std::string file = #ComponentType ".json"; \
    Serialization::JsonFileValidation(file); \
    Serialization::WriteJsonFile(file, &comp, false); \
    ComponentType comp2 = Serialization::ReadJsonFile<ComponentType>(file); \
    CompareComponents<ComponentType> comparer; \
    EXPECT_NO_THROW(comp.ApplyFunctionPairwise(comparer, comp2)); \
}

template <typename T>
struct RandomizeComponents{

    T m_Array;
    int count{};

    std::random_device rd;
    std::mt19937 gen{ rd() };

    float RandomFloat(float min = 0.0f, float max = 1.0f) {
        std::uniform_real_distribution<float> dist(min, max);
        return dist(gen);
    }

    int RandomInt(int min = 0, int max = 100) {
        std::uniform_int_distribution<int> dist(min, max);
        return dist(gen);
    }

    bool RandomBool() {
        std::bernoulli_distribution dist(0.5);
        return dist(gen);
    }

    std::string RandomString(size_t length = 8) {
        static const std::string chars = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789";
        std::uniform_int_distribution<size_t> dist(0, chars.size() - 1);
        std::string str;
        for (size_t i = 0; i < length; ++i) {
            str += chars[dist(gen)];
        }
        return str;
    }


    void operator()(float& _args) {
        _args = RandomFloat();
        count++;

    }

    void operator()(int& _args) {
        _args = RandomInt();
        count++;

    }

    template <typename EnumType>
    requires std::is_enum_v<EnumType>
    void operator()(EnumType& _args) {
		int enumcount = static_cast<int>(magic_enum::enum_count<EnumType>());
        _args = static_cast<EnumType>(RandomInt(0, enumcount - 1));

        count++;
    }

    void operator()(glm::vec2& _args) {

        _args = glm::vec2(RandomFloat(), RandomFloat());

        count++;

    }

    void operator()(glm::vec3& _args) {
 
        _args = glm::vec3(RandomFloat(), RandomFloat(), RandomFloat());
        count++;

    }

    void operator()(glm::vec4& _args) {
  
        _args = glm::vec4(RandomFloat(), RandomFloat(), RandomFloat(), RandomFloat());
        count++;

    }


    void operator()(bool& _args) {

        _args = RandomBool();
        count++;

    }

    void operator()(std::string& _args) {
 

        _args = RandomString(8);
        count++;
    }

    template <typename U>
    void operator()(std::vector<U>& _args) {

        if constexpr (std::is_class_v<U> && requires { U::Names(); }) {
            for (U& x : _args) {
                x.ApplyFunction(RandomizeComponents<decltype(x.Names())>{x.Names()});
            }
        }
        else {
            int _count{};
            for (U& x : _args) {
                (*this)(x); // Handle non-class types
                count--;// minus so no subsciprt error
            }
        }


        count++;

    }

    template <typename K>
    void operator()(K& _args) {
        if constexpr (std::is_class_v<K>) {

            _args.ApplyFunction(RandomizeComponents<decltype(_args.Names())>{_args.Names()});


        }
        count++;
    }

};

template <typename T>
struct CompareComponents {
    float epsilon = 0.0001f;
    int count = 0;

    // Float
    void operator()(float& a, float& b) {
        if (!glm::epsilonEqual(a, b, epsilon))
            throw std::runtime_error("Float values are not equal");
        count++;
    }

    // Int
    void operator()(int& a, int& b) {
        if (a != b)
            throw std::runtime_error("Int values are not equal");
        count++;
    }

    // Bool
    void operator()(bool& a, bool& b) {
        if (a != b)
            throw std::runtime_error("Bool values are not equal");
        count++;
    }

    // String
    void operator()(std::string& a, std::string& b) {
        if (a != b)
            throw std::runtime_error("String values are not equal");
        count++;
    }

    // Enum
    template <typename EnumType>
        requires std::is_enum_v<EnumType>
    void operator()(EnumType& a, EnumType& b) {
        if (static_cast<int>(a) != static_cast<int>(b))
            throw std::runtime_error("Enum values are not equal");
        count++;
    }

    // glm::vec2
    void operator()(glm::vec2& a, glm::vec2& b) {
        if (!glm::all(glm::epsilonEqual(a, b, epsilon)))
            throw std::runtime_error("vec2 values are not equal");
        count++;
    }

    // glm::vec3
    void operator()(glm::vec3& a, glm::vec3& b) {
        if (!glm::all(glm::epsilonEqual(a, b, epsilon)))
            throw std::runtime_error("vec3 values are not equal");
        count++;
    }

    // glm::vec4
    void operator()(glm::vec4& a, glm::vec4& b) {
        if (!glm::all(glm::epsilonEqual(a, b, epsilon)))
            throw std::runtime_error("vec4 values are not equal");
        count++;
    }

    // Vectors of arbitrary type
    template <typename U>
    void operator()(std::vector<U>& a, std::vector<U>& b) {
        if (a.size() != b.size())
            throw std::runtime_error("Vector sizes differ");
        for (size_t i = 0; i < a.size(); ++i) {
            (*this)(a[i], b[i]); // recursively compare elements
        }
        count++;
    }

    // Reflected classes
    template <typename K>
    void operator()(K& a, K& b) {
        if constexpr (std::is_class_v<K> && requires { a.Names(); }) {
            a.ApplyFunctionPairwise(*this, b); // recursively compares all fields
        }
        count++;
    }
};