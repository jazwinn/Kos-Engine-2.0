#include "Editor.h"
#include "config/pch.h"
#include "Resources/R_Material.h"
#include "AssetManager/AssetManager.h"

struct DisplayMaterial{
    std::string materialName;
    MaterialData data;

    REFLECTABLE(DisplayMaterial, materialName, data);
};
DisplayMaterial materialData;


void gui::ImGuiHandler::DrawMaterialWindow() {

    ImGui::Begin("Material");

    

    materialData.ApplyFunction(DrawComponents{ materialData.Names() });

    float windowWidth = ImGui::GetContentRegionAvail().x; // Available width inside window
    float buttonWidth = 100.0f; // Width of your button

    ImGui::SetCursorPosX((windowWidth - buttonWidth) * 0.5f); // Center horizontally
    if(ImGui::Button("Create", ImVec2(buttonWidth, 0))){
        if (!materialData.materialName.empty()) {
            std::string fileName = materialData.materialName + ".mat";
            std::string filepath = AssetManager::AssetManager::GetInstance()->GetAssetManagerDirectory() + "/Material/" + fileName;

            Serialization::WriteJsonFile(filepath, &selectedAsset);
            LOGGING_POPUP("Material Successfully Added");
        }
        else {
			LOGGING_WARN("Material Name cannot be empty");
        }


    }


    ImGui::End();

}