
#include <Windows.h>
#include <cstdlib>
#include <fstream>
#include <iostream>
#include <filesystem>


std::string getShortPath(const std::string& longPath) {
	char shortPath[MAX_PATH];
	DWORD result = GetShortPathNameA(longPath.c_str(), shortPath, MAX_PATH);
	if (result > 0 && result < MAX_PATH) {
		return std::string(shortPath);
	}
	return longPath; // fallback
}
int main(int argc, char* argv[])
{
	std::string texturePath{ std::filesystem::absolute(argv[1]).string() };
	std::string metaPath{ std::filesystem::absolute(argv[2]).string() };
	std::string outputPath{ std::filesystem::absolute(argv[3]).string()};
	//std::string tempBatch = "temp_run_texturecompiler.bat";
	// Write the batch file
	/*std::ofstream batchFile(tempBatch);
	if (!batchFile) {
		std::cerr << "Failed to create batch file!" << std::endl;
		return 0;
	}*/
	//Manipulate output file path
	//Remove file name 
	//Iterate backwards
	auto endStringIterator{ outputPath.end() };
	endStringIterator--;
	while (*endStringIterator != '\\') {
		endStringIterator--;
	}
	std::string outputFilePath=std::string(outputPath.begin(), endStringIterator);

	//Get file name
	std::filesystem::path p(texturePath);
	std::string fileName = p.stem().string();

	//std::cout<<"Test output: " << outputFilePath<<fileName<<".dds" << '\n';
	//std::string shortPath{ getShortPath(texturePath) };
	//batchFile << "@echo off\n";
	//batchFile << "set EXE=\"" << ".\\Compilers\\Executable\\Texture Compiler\\texconv.exe" << "\"\n";
	//batchFile << "%EXE% -ft DDS -f BC3_UNORM -y -o ";
	//batchFile << "\"" << outputFilePath << "\" ";
	//batchFile << "\"" << texturePath << "\"";
	//batchFile << std::endl;
	//batchFile.close();
	std::cout << "Output file path basrgaregret test" << outputFilePath<< '\n';
	std::cout << "Texture file path" << texturePath << '\n';

	// Run the batch file
	std::string exePath = std::filesystem::absolute(".\\Kos Editor\\Compilers\\Executable\\Texture Compiler\\texconv.exe").string();
	std::string command = "\"\"" + exePath + "\" -ft DDS -f BC3_UNORM -y -o \"" + outputFilePath + "\" \"" + texturePath + "\"\"";
	/*command += outputFilePath + "\" \"" + texturePath + "\"";*/
	std::cout <<"COMMAND: " << command << '\n';
	int result = std::system(command.c_str());
	if (result != 0) {
		std::cerr << "Command failed with code: " << result << std::endl;
	}
	// Delete the temporary batch file
	//std::filesystem::remove(tempBatch);
	//Rename the file
	outputFilePath += '\\';
	outputFilePath += fileName;
	outputFilePath += ".dds";
	std::cout<<"Output file path" << outputFilePath << '\n';
	std::cout<<"New file path" << outputPath << '\n';
	std::filesystem::rename(outputFilePath, outputPath);

	return 0;;
}