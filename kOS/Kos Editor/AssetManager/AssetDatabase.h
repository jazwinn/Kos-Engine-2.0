#pragma once
#include "Config/pch.h"
#include "Reflection/Reflection.h"

enum class Arguments {
	None,
	Animation,
	Mesh
};
struct AssetData
{
	std::string GUID;
	std::string Type;
	Arguments Arg1 = Arguments::None;

	REFLECTABLE(AssetData, GUID, Type, Arg1);
};

class AssetDatabase {
	//return GUID
public:
	std::string ImportAsset(std::filesystem::path filePath, const std::string& type);

	std::string GenerateRandomGUID();

	std::string GetGUID(std::filesystem::path filePath) {
		if (m_pathToGUID.find(filePath) == m_pathToGUID.end()) {
			throw std::runtime_error("Asset not imported to Database");
		}

		return m_pathToGUID.at(filePath);
	}

private:
	std::unordered_map<std::filesystem::path, std::string> m_pathToGUID;

};


