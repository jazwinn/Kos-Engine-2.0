
#include "Config/pch.h"
#include "AssetDatabase.h"
#include "DeSerialization/json_handler.h"

std::string AssetDatabase::ImportAsset(std::filesystem::path filePath, const std::string& type)
{
	if (!std::filesystem::exists(filePath)) {
		LOGGING_ASSERT_WITH_MSG(filePath.string() + " , filepath does not exist");
	}

	//check if file meta exist
	std::filesystem::path metaPath = filePath.string() + ".meta";
	
	std::string returnGUID;
	if (std::filesystem::exists(metaPath)) {
		AssetData data = Serialization::ReadJsonFile<AssetData>(metaPath.string());
		returnGUID = data.GUID;
	}
	else {
		//Generate Meta file
		std::string newGUID = GenerateRandomGUID();

		

		AssetData data;
		data.GUID = newGUID;
		data.Type = type;

		Serialization::WriteJsonFile(metaPath.string(), &data);


		returnGUID = newGUID;
	}



	m_pathToGUID[filePath] = returnGUID;

	return returnGUID;
}

std::string AssetDatabase::GenerateRandomGUID()
{
	static std::random_device dev;
	static std::mt19937 rng(dev());

	std::uniform_int_distribution<int> dist(0, 15);

	const char* v = "0123456789abcdef";
	const bool dash[] = { 0, 0, 0, 0, 1, 0, 1, 0, 1, 0, 1, 0, 0, 0, 0, 0 };

	std::string res;
	for (int i = 0; i < 16; i++) {
		if (dash[i]) res += "-";
		res += v[dist(rng)];
		res += v[dist(rng)];
	}
	return res;
}

