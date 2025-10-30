/******************************************************************/
/*!
\file      json_handler.h
\author    Jaz Winn Ng
\par       jazwinn.ng@digipen.edu
\date      Sept 28, 2025
\brief	   Loading and Saving of scene data to and from JSON files using RapidJSON library.

Copyright (C) 2025 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the
prior written consent of DigiPen Institute of Technology is prohibited.
*/
/******************************************************************/
#ifndef JSONHANDLER_H
#define JSONHANDLER_H


#include <RAPIDJSON/document.h>
#include <RAPIDJSON/writer.h>
#include <RAPIDJSON/stringbuffer.h>

#include "ECS/ECSList.h"
#include "SerializationReflection.h"

namespace Serialization {
		void LoadScene(const std::filesystem::path& jsonFilePath, const std::string sceneName = "");
		void SaveScene(const std::filesystem::path& filePath, const std::filesystem::path& targetFilePath = "");

		void SaveEntity(ecs::EntityID entityId, rapidjson::Value& parentArray, rapidjson::Document::AllocatorType& allocator, std::unordered_set<ecs::EntityID>& savedEntities);
		void LoadEntity(const rapidjson::Value& entityData, std::optional<ecs::EntityID> parentID, const std::string& sceneName);

		void JsonFileValidation(const std::string& filePath);


		template <typename T>
		T ReadJsonFile(const std::string& filepath)
		{
			std::ifstream inputFile(filepath);

			if (!inputFile) {
				LOGGING_WARN("Failed to open JSON file for reading: {}", filepath.c_str());
				throw std::runtime_error("Failed to open JSON file: " + filepath);
			}

			std::string fileContent((std::istreambuf_iterator<char>(inputFile)), std::istreambuf_iterator<char>());
			inputFile.close();

			// Parse the JSON content
			rapidjson::Document doc;
			doc.Parse(fileContent.c_str());

			T data;

			for (rapidjson::SizeType i = 0; i < doc.Size(); i++) {
				const rapidjson::Value& entry = doc[i];

				if (entry.HasMember(T::classname())) {
					LoadComponentreflect(&data, entry);
				}
				

			}

			return data;
		}

		template <typename T>
		bool WriteJsonFile(const std::string& filepath, T* object, bool update = false) {
			Serialization::JsonFileValidation(filepath);

			rapidjson::Document doc;


			if (update) {
				std::ifstream inputFile(filepath);

				if (!inputFile) {
					LOGGING_ERROR("Failed to open JSON file for reading: {}", filepath.c_str());
					return false;
				}

				std::string fileContent((std::istreambuf_iterator<char>(inputFile)), std::istreambuf_iterator<char>());
				inputFile.close();

				doc.Parse(fileContent.c_str());

				// Remove existing entries with the same component name
				for (rapidjson::SizeType i = doc.Size(); i > 0; --i) {
					auto& obj = doc[i - 1];
					if (obj.HasMember(T::classname())) {
						doc.Erase(doc.Begin() + (i - 1));
					}
				}
			}


			if (!doc.IsArray()) {
				doc.SetArray();  // Initialize as an empty array
			}



			rapidjson::Document::AllocatorType& allocator = doc.GetAllocator();
			rapidjson::Value entityData(rapidjson::kObjectType);



			saveComponentreflect(object, entityData, allocator);

			doc.PushBack(entityData, allocator);

			// Write to file
			rapidjson::StringBuffer buffer;
			rapidjson::Writer<rapidjson::StringBuffer> writer(buffer);
			doc.Accept(writer);

			std::ofstream ofs(filepath);
			ofs << buffer.GetString();

			return true;
		}
}


#endif JSONHANDLER_H

