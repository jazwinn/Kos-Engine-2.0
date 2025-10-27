/******************************************************************/
/*!
\file      WindowFile.cpp
\author    Clarence Boey
\par       c.boey@digipen.edu
\date      Sept 30, 2024
\brief     


Copyright (C) 2024 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the
prior written consent of DigiPen Institute of Technology is prohibited.
*/
/******************************************************************/

#include "WindowFile.h"
#include <string>   
#include <vector>
#include <fstream>
#include "Debugging/Logging.h"


namespace filewindow {
	std::string m_OpenfileDialog( const char* filePath)
	{

        //OPENING THIS CAUSE FPS TO DROP BY HALF?????????
        OPENFILENAMEA ofn;
        CHAR fileSize[256] = { 0 };
        //CHAR directory[256] = { 0 };
        ZeroMemory(&ofn, sizeof(ofn));
        ofn.lpstrInitialDir = filePath;
        ofn.lStructSize = sizeof(ofn);
        ofn.hwndOwner = NULL; // Handle to the owner window (can be NULL)
        ofn.lpstrFile = fileSize; // Buffer to store the file path
        ofn.lpstrFile[0] = '\0';  // Initial null-terminated string
        ofn.nMaxFile = sizeof(fileSize);
        ofn.lpstrFilter = "JSON Files\0*.json\0";
        ofn.nFilterIndex = 1; // Filter index
        ofn.lpstrDefExt = "json"; // Default extension
        ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST | OFN_NOCHANGEDIR;



        if (GetOpenFileNameA(&ofn) == TRUE) {
            return ofn.lpstrFile;
        }

		return std::string();
	}

    // Function to read `editorConfig.txt` and return its contents as a vector of strings
    std::vector<std::string> readEditorConfig(const std::string& filePath) {
        std::vector<std::string> configData;
        std::ifstream file(filePath);

        if (file.is_open()) {
            std::string line;
            while (std::getline(file, line)) {
                // Add non-empty lines to the vector
                if (!line.empty()) {
                    configData.push_back(line);
                }
            }
            file.close();
        }
        else {
            LOGGING_ERROR("FAILED TO OPEN FILE: {} ", filePath);
            //std::cerr << "Failed to open file: " << filePath << std::endl;
        }
        return configData;
    }
}


