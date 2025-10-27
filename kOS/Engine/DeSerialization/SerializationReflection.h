/******************************************************************/
/*!
\file      serializationReflection.h
\author    Jaz Winn Ng
\par       jazwinn.ng@digipen.edu
\date      Sept 28, 2025
\brief	   Reflection for serialization

Copyright (C) 2025 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the
prior written consent of DigiPen Institute of Technology is prohibited.
*/
/******************************************************************/
#pragma once

#include <RAPIDJSON/document.h>
#include <RAPIDJSON/writer.h>
#include <RAPIDJSON/stringbuffer.h>

template <typename T>
struct SaveComponent {

	T m_Array;
	int count{};



	void operator()(float& _args, rapidjson::Value& value, rapidjson::Document::AllocatorType& allocator) {
		rapidjson::Value key;
		key.SetString(m_Array[count].c_str(), allocator);
		value.AddMember(key, _args, allocator);
		count++;
	}


	void operator()(int& _args, rapidjson::Value& value, rapidjson::Document::AllocatorType& allocator) {
		rapidjson::Value key;
		key.SetString(m_Array[count].c_str(), allocator);

		value.AddMember(key, _args, allocator);
		count++;
	}


	void operator()(glm::vec2& _args, rapidjson::Value& value, rapidjson::Document::AllocatorType& allocator) {
		rapidjson::Value key;
		key.SetString(m_Array[count].c_str(), allocator);

		value.AddMember(key, rapidjson::Value().SetObject()
			.AddMember("x", _args.x, allocator)
			.AddMember("y", _args.y, allocator), allocator);
		count++;
	}

	void operator()(glm::vec3& _args, rapidjson::Value& value, rapidjson::Document::AllocatorType& allocator) {
		rapidjson::Value key;
		key.SetString(m_Array[count].c_str(), allocator);

		value.AddMember(key, rapidjson::Value().SetObject()
			.AddMember("x", _args.x, allocator)
			.AddMember("y", _args.y, allocator)
			.AddMember("z", _args.z, allocator), allocator);
		count++;
	}

	void operator()(glm::vec4& _args, rapidjson::Value& value, rapidjson::Document::AllocatorType& allocator) {
		rapidjson::Value key;
		key.SetString(m_Array[count].c_str(), allocator);

		value.AddMember(key, rapidjson::Value().SetObject()
			.AddMember("x", _args.x, allocator)
			.AddMember("y", _args.y, allocator)
			.AddMember("z", _args.z, allocator)
			.AddMember("w", _args.w, allocator), allocator);
		count++;
	}


	void operator()(bool& _args, rapidjson::Value& value, rapidjson::Document::AllocatorType& allocator) {
		rapidjson::Value key;
		key.SetString(m_Array[count].c_str(), allocator);

		value.AddMember(key, _args, allocator);
		count++;
	}

	void operator()(std::string& _args, rapidjson::Value& value, rapidjson::Document::AllocatorType& allocator) {
		rapidjson::Value jsonString;
		jsonString.SetString(_args.c_str(), allocator); // Set the string
		value.AddMember(rapidjson::Value(m_Array[count].c_str(), allocator), jsonString, allocator); // Add the key-value pair
		count++;
	}


	template <typename EnumType>
	requires std::is_enum_v<EnumType>
	void operator()(EnumType& _args, rapidjson::Value& value, rapidjson::Document::AllocatorType& allocator) {
		// Create a rapidjson::Value key from the string
		rapidjson::Value key;
		key.SetString(m_Array[count].c_str(), allocator);

		// Add the member using the key and integer value
		value.AddMember(key, static_cast<int>(_args), allocator);

		count++;
	}

	template <typename U>
	void operator()(std::vector<U>& _args, rapidjson::Value& value, rapidjson::Document::AllocatorType& allocator) {
		if constexpr (std::is_class_v<U> && requires { U::Names(); }) {
			rapidjson::Value Array(rapidjson::kArrayType);
			for (U& x : _args) {
				rapidjson::Value name(rapidjson::kObjectType);
				SaveComponent<decltype(x.Names())> saver{ x.Names() };

				x.ApplyFunction([&](auto& member) {
					saver(member, name, allocator);  // Apply SaveEntity to each member
					});

				Array.PushBack(name, allocator);
			}

			rapidjson::Value key;
			key.SetString(m_Array[count].c_str(), allocator);

			value.AddMember(key, Array, allocator);
		}
		else {
			rapidjson::Value Array(rapidjson::kArrayType);
			for (U& x : _args) {

				rapidjson::Value name(rapidjson::kObjectType);
				(*this)(x, name, allocator); // Handle non-class types
				Array.PushBack(name, allocator);
				count--;// minus so no subsciprt error

			}

			rapidjson::Value key;
			key.SetString(m_Array[count].c_str(), allocator);

			value.AddMember(key, Array, allocator);
		}
		count++;
	}

	template <typename K>
	void operator()(K& _args, rapidjson::Value& value, rapidjson::Document::AllocatorType& allocator) {
		if constexpr (std::is_class_v<K>) {
			rapidjson::Value name(rapidjson::kObjectType);
			SaveComponent<decltype(_args.Names())> saver{ _args.Names() };

			_args.ApplyFunction([&](auto& member) {
				saver(member, name, allocator);  // Apply SaveEntity to each member
				});

			value.AddMember(rapidjson::Value(m_Array[count].c_str(), allocator), name, allocator);
			//_args.ApplyFunction(DrawComponents<decltype(_args.Names())>{_args.Names()});
			count++;
		}

	}

};

template <typename T>
struct LoadComponent {

	T m_Array;
	int count{};



	void operator()(float& _args, const rapidjson::Value& value) {

		if (value.HasMember(m_Array[count].c_str()) && value[m_Array[count].c_str()].IsFloat()) {
			_args = value[m_Array[count].c_str()].GetFloat();
		}

		count++;
	}


	void operator()(int& _args, const rapidjson::Value& value) {

		if (value.HasMember(m_Array[count].c_str()) && value[m_Array[count].c_str()].IsInt()) {
			_args = value[m_Array[count].c_str()].GetInt();
		}

		count++;
	}

	void operator()(glm::vec2& _args, const rapidjson::Value& value) {

		if (value.HasMember(m_Array[count].c_str()) && value[m_Array[count].c_str()].IsObject()) {
			const rapidjson::Value& vector = value[m_Array[count].c_str()];
			if (vector.HasMember("x") && vector["x"].IsFloat()) {
				_args.x = vector["x"].GetFloat();
			}
			if (vector.HasMember("y") && vector["y"].IsFloat()) {
				_args.y = vector["y"].GetFloat();
			}
		}

		count++;
	}

	void operator()(glm::vec3& _args, const rapidjson::Value& value) {

		if (value.HasMember(m_Array[count].c_str()) && value[m_Array[count].c_str()].IsObject()) {
			const rapidjson::Value& vector = value[m_Array[count].c_str()];
			if (vector.HasMember("x") && vector["x"].IsFloat()) {
				_args.x = vector["x"].GetFloat();
			}
			if (vector.HasMember("y") && vector["y"].IsFloat()) {
				_args.y = vector["y"].GetFloat();
			}
			if (vector.HasMember("z") && vector["z"].IsFloat()) {
				_args.z = vector["z"].GetFloat();
			}
		}

		count++;
	}
	void operator()(glm::vec4& _args, const rapidjson::Value& value) {

		if (value.HasMember(m_Array[count].c_str()) && value[m_Array[count].c_str()].IsObject()) {
			const rapidjson::Value& vector = value[m_Array[count].c_str()];
			if (vector.HasMember("x") && vector["x"].IsFloat()) {
				_args.x = vector["x"].GetFloat();
			}
			if (vector.HasMember("y") && vector["y"].IsFloat()) {
				_args.y = vector["y"].GetFloat();
			}
			if (vector.HasMember("z") && vector["z"].IsFloat()) {
				_args.z = vector["z"].GetFloat();
			}
			if (vector.HasMember("w") && vector["w"].IsFloat()) {
				_args.w = vector["w"].GetFloat();
			}
		}

		count++;
	}

	void operator()(bool& _args, const rapidjson::Value& value) {

		if (value.HasMember(m_Array[count].c_str()) && value[m_Array[count].c_str()].IsBool()) {
			_args = value[m_Array[count].c_str()].GetBool();
		}

		count++;
	}

	void operator()(std::string& _args, const rapidjson::Value& value) {
		if (value.HasMember(m_Array[count].c_str()) && value[m_Array[count].c_str()].IsString()) {
			_args = value[m_Array[count].c_str()].GetString();
		}

		count++;
	}


	template <typename EnumType>
		requires std::is_enum_v<EnumType>
	void operator()(EnumType& _args, const rapidjson::Value& value) {
		if (value.HasMember(m_Array[count].c_str()) && value[m_Array[count].c_str()].IsInt()) {
			_args = static_cast<EnumType>(value[m_Array[count].c_str()].GetInt());
		}
		count++;
	}

	template <typename U>
	void operator()(std::vector<U>& _args, const rapidjson::Value& value) {
		if (value.HasMember(m_Array[count].c_str()) && value[m_Array[count].c_str()].IsArray()) {

			if constexpr (std::is_class_v<U> && requires { U::Names(); }) {

				const rapidjson::Value& Array = value[m_Array[count].c_str()];

				for (rapidjson::SizeType i = 0; i < Array.Size(); i++) {
					const rapidjson::Value& Object = Array[i];

					LoadComponent<decltype(U::Names())> loader{ U::Names() };

					U temp;

					temp.ApplyFunction([&](auto& member) {
						loader(member, Object);
						});

					_args.push_back(temp);
				}
			}
			else {

				const rapidjson::Value& Array = value[m_Array[count].c_str()];

				for (rapidjson::SizeType i = 0; i < Array.Size(); i++) {
					const rapidjson::Value& name = Array[i];

					U temp;
					(*this)(temp, name); // Handle non-class types

					_args.push_back(temp);
					count--; //offset 
				}
			}



		}

		count++;
	}

	template <typename K>
	void operator()(K& _args, const rapidjson::Value& value) {

		if (value.HasMember(m_Array[count].c_str())) {
			if constexpr (std::is_class_v<K>) {
				const rapidjson::Value& name = value[m_Array[count].c_str()];
				LoadComponent<decltype(_args.Names())> loader{ _args.Names() };

				_args.ApplyFunction([&](auto& member) {
					loader(member, name);
					});

					
			}
		}

		count++;
	}


};

template <typename T>
struct CompareComponent {
	T m_Array;
	void* comp1{};
	void* comp2{};
	bool result{ true };  // Added missing semicolon

	template <typename U>
	void SetComponents(U* c1, U* c2) {
		comp1 = static_cast<void*>(c1);
		comp2 = static_cast<void*>(c2);
		result = true;
	}

	void operator()(float& _args1, float& _args2) {
		result = result && (_args1 == _args2);  // Accumulate with AND
	}

	void operator()(int& _args1, int& _args2) {
		result = result && (_args1 == _args2);
	}

	void operator()(glm::vec2& _args1, glm::vec2& _args2) {
		result = result && (_args1 == _args2);
	}

	void operator()(glm::vec3& _args1, glm::vec3& _args2) {
		result = result && (_args1 == _args2);
	}

	void operator()(glm::vec4& _args1, glm::vec4& _args2) {
		result = result && (_args1 == _args2);
	}

	void operator()(bool& _args1, bool& _args2) {
		result = result && (_args1 == _args2);
	}

	void operator()(std::string& _args1, std::string& _args2) {
		result = result && (_args1 == _args2);
	}

	template <typename EnumType>
		requires std::is_enum_v<EnumType>
	void operator()(EnumType& _args1, EnumType& _args2) {
		result = result && (static_cast<int>(_args1) == static_cast<int>(_args2));
	}

	template <typename U>
	void operator()(std::vector<U>& _args1, std::vector<U>& _args2) {
		if (_args1.size() != _args2.size()) {
			result = false;
			return;
		}

		if constexpr (std::is_class_v<U> && requires { U::Names(); }) {
			// Compare reflectable objects
			for (size_t i = 0; i < _args1.size(); ++i) {
				CompareComponent<decltype(U::Names())> compare{ U::Names() };
				compare.SetComponents(&_args1[i], &_args2[i]);

				auto m1 = _args1[i].member();
				auto m2 = _args2[i].member();

				[&] <std::size_t... Is>(std::index_sequence<Is...>) {
					((compare(std::get<Is>(m1), std::get<Is>(m2))), ...);
				}(std::make_index_sequence<std::tuple_size_v<decltype(m1)>>{});

				result = result && compare.result;
				if (!result) break;  // Early exit if any comparison fails
			}
		}
		else {
			// Compare simple types
			result = result && (_args1 == _args2);
		}
	}

	template <typename K>
	void operator()(K& _args1, K& _args2) {
		if constexpr (std::is_class_v<K> && requires { K::Names(); }) {
			// Compare reflectable nested objects
			CompareComponent<decltype(K::Names())> compare{ K::Names() };
			compare.SetComponents(&_args1, &_args2);

			auto m1 = _args1.member();
			auto m2 = _args2.member();

			[&] <std::size_t... Is>(std::index_sequence<Is...>) {
				((compare(std::get<Is>(m1), std::get<Is>(m2))), ...);
			}(std::make_index_sequence<std::tuple_size_v<decltype(m1)>>{});

			result = result && compare.result;
		}
		else if constexpr (requires { _args1 == _args2; }) {
			// Has equality operator
			result = result && (_args1 == _args2);
		}
		else {
			// Cannot compare, assume different
			result = false;
		}
	}
};

template<typename T>
inline void saveComponentreflect(T* component, rapidjson::Value& entityData, rapidjson::Document::AllocatorType& allocator)
{
	if (component == nullptr) return;
	rapidjson::Value name(rapidjson::kObjectType);
	SaveComponent<decltype(component->Names())> saver{ component->Names() };  // Create SaveEntity with member names

	component->ApplyFunction([&](auto& member) {
		saver(member, name, allocator);  // Apply SaveEntity to each member
		});

	rapidjson::Value key(component->classname(), allocator);



	entityData.AddMember(key, name, allocator);
}

template<typename T>
inline void LoadComponentreflect(T* component, const rapidjson::Value& entityData)
{
	if (component == nullptr) return;

	const rapidjson::Value& name = entityData[T::classname()];
	LoadComponent<decltype(T::Names())> loader{ T::Names() };

	component->ApplyFunction([&](auto& member) {
		loader(member, name);
		});

}

template<typename T>
inline bool CompareComponentReflect(T* component1, T* component2)
{
	if (component1 == nullptr || component2 == nullptr) return false;

	CompareComponent<decltype(T::Names())> compare{ T::Names() };
	compare.SetComponents(component1, component2);

	std::tuple m1 = component1->member();
	std::tuple m2 = component2->member();

	[&] <std::size_t... Is>(std::index_sequence<Is...>) {
		((compare(std::get<Is>(m1), std::get<Is>(m2))), ...);
	}(std::make_index_sequence<std::tuple_size_v<decltype(m1)>>{});

	return compare.result;
}
