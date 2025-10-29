/******************************************************************/
/*!
\file      imgui_layer_panel.cpp
\author    Rayner Tan, raynerweichen.tan , 2301449
\par       raynerweichen.tan@digipen.edu
\date      8 Nov 2024
\brief     This file helps with the collision layer matrix panel

Copyright (C) 2024 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the
prior written consent of DigiPen Institute of Technology is prohibited.
*/
/******************************************************************/

#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"
#include "Editor.h"
#include "ECS/Layers.h"
#include "DeSerialization/json_handler.h"
#include "Debugging/Logging.h"
#include "Physics/PhysicsLayer.h"
#include "Configs/ConfigPath.h"
#include "Application/Application.h"
#include "Application/ApplicationData.h"

constexpr int layerCount = layer::MAXLAYER;
layer::LayerStack layers;
physicslayer::PhysicsLayer* physicsLayer = physicslayer::PhysicsLayer::m_GetInstance(); // Get the PhysicsLayer instance

void DrawVerticalLabel(const std::string& text, float x, float y)
{
    ImDrawList* drawList = ImGui::GetWindowDrawList();
    ImVec2 pos = ImGui::GetCursorScreenPos(); // Get the current cursor position in the ImGui window
    pos.x += x; // Shift to the desired X position
    pos.y += y; // Shift to the desired Y position

    ImU32 color = ImGui::GetColorU32(ImGuiCol_Text);
    // Draw each character of the text at the specified position
    for (char c : text)
    {
        std::string character(1, c); // Convert char to a string for drawing
        drawList->AddText(ImVec2(pos.x, pos.y), color, character.c_str());
        pos.y += ImGui::GetFontSize(); // Move down by one character height
    }
}

void gui::ImGuiHandler::DrawLayerWindow() {
    ImGui::Begin("Layer Panel", nullptr, ImGuiWindowFlags_HorizontalScrollbar | ImGuiWindowFlags_MenuBar);

    //static bool flag = false;
    //for (size_t n{}; n < ecs->layersStack.m_layerMap.size(); n++) {

    //    ImGui::Text(ecs->layersStack.m_layerMap[(layer::LAYERS)n].first.c_str());
    //    ImGui::SameLine();
    //    std::string title = "##2" +  ecs->layersStack.m_layerMap[(layer::LAYERS)n].first;
    //    
    //    if (ecs->layersStack.m_layerBitSet.test((layer::LAYERS)n)) {
    //        flag = true;
    //    }
    //    else flag = false;

    //    if (ImGui::Checkbox(title.c_str(), &flag)) {
    //        if (flag == true) {
    //            ecs->layersStack.m_EnableLayer((layer::LAYERS)n);
    //        }
    //        else {
    //            ecs->layersStack.m_DisableLayer((layer::LAYERS)n);
    //        }
    //    }
    //}
	
    //ImGui::NewLine();
    //ImGui::Separator();

    const float cellWidth = 25.0f; // Adjust width as needed
    const float labelWidth = 120.0f; // Width for the row labels
    //const float verticalTextOffsetY = 10.0f; // Adjust as needed for better alignment
    
    // Draw the header row with vertical text
    ImGui::SetCursorPosX(labelWidth); // Align column headers to start after row labels
    ImGui::AlignTextToFramePadding();
    //for (int col = layerCount-1; col >= 0; --col)
    for (int col = 0; col < layerCount; ++col)
    {
        ImGui::SetCursorPosX(labelWidth + col * cellWidth); // Position the vertical text over each column
        auto it = layers.m_layerMap.find(static_cast<layer::LAYERS>(col));
        if (it != layers.m_layerMap.end()) {
            DrawVerticalLabel(it->second.first, 0.0f, 0.0f); // Adjust x and y offsets as needed
        }
        else {
            LOGGING_ERROR_NO_SOURCE_LOCATION("NO SUCH NAME");
        }
    }

    ImGui::NewLine();
    ImGui::NewLine();
    ImGui::NewLine();
    ImGui::NewLine();
    ImGui::NewLine();
    ImGui::NewLine();

    // Draw the matrix rows
    for (int row = 0; row < layerCount; ++row)
    {
        ImGui::SetCursorPosX(0.5f);
        // Draw row label with fixed width

        ImGui::Text("%*s", (int)(labelWidth / ImGui::GetFontSize()), (layers.m_layerMap[static_cast<layer::LAYERS>(row)].first).c_str()); // Adjust label width for alignment

        // Draw checkboxes for this row
        for (int col = 0; col < row + 1; ++col)
        {
            ImGui::SameLine();
            ImGui::SetCursorPosX(labelWidth + col * cellWidth);
            ImGui::PushID(row * layerCount + col);  // Unique ID for each checkbox

            // Get the current value from the collision matrix in PhysicsLayer
            bool isChecked = physicsLayer->m_GetCollide(row, col);
            if (isChecked) {
                ImGui::PushStyleColor(ImGuiCol_FrameBg, IM_COL32(0, 255, 0, 100)); // Green background for checked
                ImGui::PushStyleColor(ImGuiCol_CheckMark, IM_COL32(0, 255, 0, 255)); // Green checkmark color
            }
            else {
                ImGui::PushStyleColor(ImGuiCol_FrameBg, IM_COL32(255, 0, 0, 100)); // Red background for unchecked
                ImGui::PushStyleColor(ImGuiCol_CheckMark, IM_COL32(255, 0, 0, 255)); // Red checkmark color
            }

            if (ImGui::Checkbox("##hidden", &isChecked))
            {
                // Update the PhysicsLayer collision matrix when the checkbox is toggled
                physicsLayer->m_SetCollision(row, col, isChecked);

            }
            ImGui::PopStyleColor(2);
            ImGui::PopID();
        }

    }

    float windowWidth = ImGui::GetContentRegionAvail().x; // Available width inside window
    float buttonWidth = 100.0f; // Width of your button

    ImGui::SetCursorPosX((windowWidth - buttonWidth) * 0.5f); // Center horizontally
    if (ImGui::Button("Save", ImVec2(buttonWidth, 0))) {
		physicsLayer->SaveCollisionLayer();
    }
    ImGui::End();
}