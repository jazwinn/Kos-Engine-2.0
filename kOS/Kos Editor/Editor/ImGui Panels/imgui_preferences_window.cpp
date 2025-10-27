/******************************************************************/
/*!
\file      imgui_preferences_window.cpp
\author    Mog Shi Feng
\par       
\date      
\brief     

Copyright (C) 2024 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the
prior written consent of DigiPen Institute of Technology is prohibited.
*/
/******************************************************************/

#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"
#include "Editor.h"

int style_idx = 0;
const std::string editorConfigPath = "../Configs/EditorConfig.txt";

// Will Probably Move this to json_handler.cpp
// Will Probably switch to using rapidJSON or whichever serialization lib 
#include <fstream>
void SerializeProfile(ImGuiStyle* style) {
    if (!style) {
        LOGGING_INFO("No Style Ref");
        return;
    }
    std::ofstream file(editorConfigPath);
    if (!file.is_open()) {
        LOGGING_WARN("Unable to Save Style Profile");
        file.close();
        return;
    }
    file << "Index: " << style_idx << "\n";
    for (int i = 0; i < ImGuiCol_COUNT; i++) {
        file << ImGui::GetStyleColorName(i) << ' ' << style->Colors[i].x << ' ' << style->Colors[i].y << ' ' << style->Colors[i].z <<  ' ' << style->Colors[i].w << '\n';
    }
    file << "WindowPadding " << style->WindowPadding.x << ' ' << style->WindowPadding.y << '\n';
    file << "FramePadding " << style->FramePadding.x << ' ' << style->FramePadding.y << '\n';
    file << "ItemSpacing " << style->ItemSpacing.x << ' ' << style->ItemSpacing.y << '\n';
    file << "ItemInnerSpacing " << style->ItemInnerSpacing.x << ' ' << style->ItemInnerSpacing.y << '\n';
    file << "TouchExtraPadding " << style->TouchExtraPadding.x << ' ' << style->TouchExtraPadding.y << '\n';
    file << "IndentSpacing " << style->IndentSpacing << '\n';
    file << "ScrollbarSize " << style->ScrollbarSize << '\n';
    file << "GrabMinSize " << style->GrabMinSize << '\n';
    file << "WindowBorderSize " << style->WindowBorderSize << '\n';
    file << "ChildBorderSize " << style->ChildBorderSize << '\n';
    file << "PopupBorderSize " << style->PopupBorderSize << '\n';
    file << "FrameBorderSize " << style->FrameBorderSize << '\n';
    file << "TabBorderSize " << style->TabBorderSize << '\n';
    file << "TabBarBorderSize " << style->TabBarBorderSize << '\n';
    file << "TabBarOverlineSize " << style->TabBarOverlineSize << '\n';
    file << "WindowRounding " << style->WindowRounding << '\n';
    file << "ChildRounding " << style->ChildRounding << '\n';
    file << "FrameRounding " << style->FrameRounding << '\n';
    file << "PopupRounding " << style->PopupRounding << '\n';
    file << "ScrollbarRounding " << style->ScrollbarRounding << '\n';
    file << "GrabRounding " << style->GrabRounding << '\n';
    file << "TabRounding " << style->TabRounding << '\n';
    file << "CellPadding " << style->CellPadding.x << ' ' << style->CellPadding.y << '\n';
    file << "TableAngledHeadersAngle " << style->TableAngledHeadersAngle << '\n';
    file << "TableAngledHeadersTextAlign " << style->TableAngledHeadersTextAlign.x << ' ' << style->TableAngledHeadersTextAlign.y << '\n';
    file << "WindowTitleAlign " << style->WindowTitleAlign.x << ' ' << style->WindowTitleAlign.y << '\n';
    file << "WindowMenuButtonPosition " << style->WindowMenuButtonPosition << '\n';
    file << "ColorButtonPosition " << style->ColorButtonPosition << '\n';
    file << "ButtonTextAlign " << style->ButtonTextAlign.x << ' ' << style->ButtonTextAlign.y << '\n';
    file << "SelectableTextAlign " << style->SelectableTextAlign.x << ' ' << style->SelectableTextAlign.y << '\n';
    file << "SeparatorTextBorderSize " << style->SeparatorTextBorderSize << '\n';
    file << "SeparatorTextAlign " << style->SeparatorTextAlign.x << ' ' << style->SelectableTextAlign.y << '\n';
    file << "SeparatorTextPadding " << style->SeparatorTextPadding.x << ' ' << style->SeparatorTextPadding.y << '\n';
    file << "LogSliderDeadzone " << style->LogSliderDeadzone << '\n';
    file << "DockingSplitterSize " << style->DockingSeparatorSize << '\n';
    file << "HoverFlagsForTooltipMouse_ImGuiHoveredFlags_DelayNone " << ((style->HoverFlagsForTooltipMouse & ImGuiHoveredFlags_DelayNone) > 0) << '\n';
    file << "HoverFlagsForTooltipMouse_ImGuiHoveredFlags_DelayShort " << ((style->HoverFlagsForTooltipMouse & ImGuiHoveredFlags_DelayShort) > 0) << '\n';
    file << "HoverFlagsForTooltipMouse_ImGuiHoveredFlags_DelayNormal " << ((style->HoverFlagsForTooltipMouse & ImGuiHoveredFlags_DelayNormal) > 0) << '\n';
    file << "HoverFlagsForTooltipMouse_ImGuiHoveredFlags_Stationary " << ((style->HoverFlagsForTooltipMouse & ImGuiHoveredFlags_Stationary) > 0) << '\n';
    file << "HoverFlagsForTooltipMouse_ImGuiHoveredFlags_NoSharedDelay " << ((style->HoverFlagsForTooltipMouse & ImGuiHoveredFlags_NoSharedDelay) > 0) << '\n';
    file << "HoverFlagsForTooltipNav_ImGuiHoveredFlags_DelayNone " << ((style->HoverFlagsForTooltipNav & ImGuiHoveredFlags_DelayNone) > 0) << '\n';
    file << "HoverFlagsForTooltipNav_ImGuiHoveredFlags_DelayShort " << ((style->HoverFlagsForTooltipNav & ImGuiHoveredFlags_DelayShort) > 0) << '\n';
    file << "HoverFlagsForTooltipNav_ImGuiHoveredFlags_DelayNormal " << ((style->HoverFlagsForTooltipNav & ImGuiHoveredFlags_DelayNormal) > 0) << '\n';
    file << "HoverFlagsForTooltipNav_ImGuiHoveredFlags_Stationary " << ((style->HoverFlagsForTooltipNav & ImGuiHoveredFlags_Stationary) > 0) << '\n';
    file << "HoverFlagsForTooltipNav_ImGuiHoveredFlags_NoSharedDelay " << ((style->HoverFlagsForTooltipNav & ImGuiHoveredFlags_NoSharedDelay) > 0) << '\n';
    file << "DisplayWindowPadding " << style->DisplayWindowPadding.x << ' ' << style->DisplayWindowPadding.y << '\n';
    file << "DisplaySafeAreaPadding " << style->DisplaySafeAreaPadding.x << ' ' << style->DisplaySafeAreaPadding.y << '\n';
    file.close();
}

void DeserializeProfile() {
    std::ifstream file(editorConfigPath);
    if (!file.is_open()) {
        LOGGING_WARN("Unable to Open File");
        file.close();
        return;
    }
    std::string tmp;
    std::vector<float> data;
    while (std::getline(file, tmp)) {
        size_t pos = tmp.find_first_of(' ');
        while (pos != std::string::npos) {
            std::string s(tmp, pos + 1, tmp.find_first_of(' ', pos + 1) - pos);
            data.push_back(static_cast<float>(std::atof(s.c_str())));
            pos = tmp.find_first_of(' ', pos + 1);
        }
    }
    ImGuiStyle& style = ImGui::GetStyle();
}

void Style_DefaultWhite(ImGuiStyle* dst = NULL)
{
    ImGuiStyle* style = dst ? dst : &ImGui::GetStyle();
    ImVec4* colors = style->Colors;

    // General window settings
    style->WindowRounding = 5.0f;
    style->FrameRounding = 5.0f;
    style->PopupRounding = 5.0f;
    style->ScrollbarRounding = 5.0f;
    style->GrabRounding = 5.0f;
    style->TabRounding = 5.0f;
    style->WindowBorderSize = 1.0f;
    style->FrameBorderSize = 1.0f;
    style->PopupBorderSize = 1.0f;

    // Setting the colors (Light version)
    colors[ImGuiCol_Text] = ImVec4(0.10f, 0.10f, 0.10f, 1.00f);
    colors[ImGuiCol_TextDisabled] = ImVec4(0.60f, 0.60f, 0.60f, 1.00f);
    colors[ImGuiCol_WindowBg] = ImVec4(0.95f, 0.95f, 0.95f, 1.00f); // Light background
    colors[ImGuiCol_ChildBg] = ImVec4(0.90f, 0.90f, 0.90f, 1.00f);
    colors[ImGuiCol_PopupBg] = ImVec4(0.98f, 0.98f, 0.98f, 1.00f);
    colors[ImGuiCol_Border] = ImVec4(0.70f, 0.70f, 0.70f, 1.00f);
    colors[ImGuiCol_BorderShadow] = ImVec4(0.00f, 0.00f, 0.00f, 0.00f);
    colors[ImGuiCol_FrameBg] = ImVec4(0.85f, 0.85f, 0.85f, 1.00f); // Light frame background
    colors[ImGuiCol_FrameBgHovered] = ImVec4(0.80f, 0.80f, 0.80f, 1.00f);
    colors[ImGuiCol_FrameBgActive] = ImVec4(0.75f, 0.75f, 0.75f, 1.00f);
    colors[ImGuiCol_TitleBg] = ImVec4(0.90f, 0.90f, 0.90f, 1.00f);
    colors[ImGuiCol_TitleBgActive] = ImVec4(0.85f, 0.85f, 0.85f, 1.00f);
    colors[ImGuiCol_TitleBgCollapsed] = ImVec4(0.90f, 0.90f, 0.90f, 1.00f);
    colors[ImGuiCol_MenuBarBg] = ImVec4(0.95f, 0.95f, 0.95f, 1.00f);
    colors[ImGuiCol_ScrollbarBg] = ImVec4(0.90f, 0.90f, 0.90f, 1.00f);
    colors[ImGuiCol_ScrollbarGrab] = ImVec4(0.80f, 0.80f, 0.80f, 1.00f);
    colors[ImGuiCol_ScrollbarGrabHovered] = ImVec4(0.75f, 0.75f, 0.75f, 1.00f);
    colors[ImGuiCol_ScrollbarGrabActive] = ImVec4(0.70f, 0.70f, 0.70f, 1.00f);

    // Accent colors with a soft pastel gray-green
    colors[ImGuiCol_CheckMark] = ImVec4(0.55f, 0.65f, 0.55f, 1.00f); // Soft gray-green for check marks
    colors[ImGuiCol_SliderGrab] = ImVec4(0.55f, 0.65f, 0.55f, 1.00f);
    colors[ImGuiCol_SliderGrabActive] = ImVec4(0.60f, 0.70f, 0.60f, 1.00f);
    colors[ImGuiCol_Button] = ImVec4(0.85f, 0.85f, 0.85f, 1.00f); // Light button background
    colors[ImGuiCol_ButtonHovered] = ImVec4(0.80f, 0.80f, 0.80f, 1.00f);
    colors[ImGuiCol_ButtonActive] = ImVec4(0.75f, 0.75f, 0.75f, 1.00f);
    colors[ImGuiCol_Header] = ImVec4(0.75f, 0.75f, 0.75f, 1.00f);
    colors[ImGuiCol_HeaderHovered] = ImVec4(0.70f, 0.70f, 0.70f, 1.00f);
    colors[ImGuiCol_HeaderActive] = ImVec4(0.65f, 0.65f, 0.65f, 1.00f);
    colors[ImGuiCol_Separator] = ImVec4(0.60f, 0.60f, 0.60f, 1.00f);
    colors[ImGuiCol_SeparatorHovered] = ImVec4(0.65f, 0.65f, 0.65f, 1.00f);
    colors[ImGuiCol_SeparatorActive] = ImVec4(0.70f, 0.70f, 0.70f, 1.00f);
    colors[ImGuiCol_ResizeGrip] = ImVec4(0.55f, 0.65f, 0.55f, 1.00f); // Accent color for resize grips
    colors[ImGuiCol_ResizeGripHovered] = ImVec4(0.60f, 0.70f, 0.60f, 1.00f);
    colors[ImGuiCol_ResizeGripActive] = ImVec4(0.65f, 0.75f, 0.65f, 1.00f);
    colors[ImGuiCol_Tab] = ImVec4(0.85f, 0.85f, 0.85f, 1.00f); // Tabs background
    colors[ImGuiCol_TabHovered] = ImVec4(0.80f, 0.80f, 0.80f, 1.00f);
    colors[ImGuiCol_TabActive] = ImVec4(0.75f, 0.75f, 0.75f, 1.00f);
    colors[ImGuiCol_TabUnfocused] = ImVec4(0.90f, 0.90f, 0.90f, 1.00f);
    colors[ImGuiCol_TabUnfocusedActive] = ImVec4(0.75f, 0.75f, 0.75f, 1.00f);
    colors[ImGuiCol_DockingPreview] = ImVec4(0.55f, 0.65f, 0.55f, 1.00f); // Docking preview in gray-green
    colors[ImGuiCol_DockingEmptyBg] = ImVec4(0.90f, 0.90f, 0.90f, 1.00f);

    // Additional styles
    style->FramePadding = ImVec2(8.0f, 4.0f);
    style->ItemSpacing = ImVec2(8.0f, 4.0f);
    style->IndentSpacing = 20.0f;
    style->ScrollbarSize = 16.0f;
}

void CustomStyleProfile(ImGuiStyle* dst = NULL) {
    ImGuiStyle& style = ImGui::GetStyle();
    // Check for current custom profile
    // Will need to serialize color profile myself, ImGui does not provide inate saving of custom color profile.
    // If not using custom, will need to know which color profile is choosen.
    if(ImGui::Button("Save Profile")) {
        SerializeProfile(&style);
    }
    ImGui::SameLine();
    if (ImGui::Button("Get Profile")) {
        DeserializeProfile();
    }
    ImGui::SeparatorText("Custom Profile");
    if (ImGui::BeginTabBar("##tabs", ImGuiTabBarFlags_DrawSelectedOverline)) {
        if (ImGui::BeginTabItem("Color")) {
            static int style_idx = 0;
            if (ImGui::Combo("Colors##Selector", &style_idx, "White\0Classic\0Dark\0Light")) // \0Custom+
            {
                switch (style_idx)
                {
                case 0: Style_DefaultWhite();       break;
                case 1: ImGui::StyleColorsClassic(); break;
                case 2: ImGui::StyleColorsDark();   break;
                case 3: ImGui::StyleColorsLight();  break;
                }
            }
            ImGui::Separator();
            ImGui::SetNextWindowSizeConstraints(ImVec2(0.0f, ImGui::GetTextLineHeightWithSpacing() * 10), ImVec2(FLT_MAX, FLT_MAX));
            ImGui::BeginChild("##colors", ImVec2(0, 0), ImGuiChildFlags_Borders | ImGuiChildFlags_NavFlattened, ImGuiWindowFlags_AlwaysVerticalScrollbar | ImGuiWindowFlags_AlwaysHorizontalScrollbar);
            ImGui::PushItemWidth(ImGui::GetFontSize() * -12);
            for (int i = 0; i < ImGuiCol_COUNT; i++) {
                const char* name = ImGui::GetStyleColorName(i);
                ImGui::PushID(i);
                if (ImGui::Button("?")) ImGui::DebugFlashStyleColor((ImGuiCol)i);
                ImGui::SetItemTooltip("Flash given color to identify places where it is used.");
                ImGui::SameLine();
                ImGui::ColorEdit4("##color", (float*)&style.Colors[i], ImGuiColorEditFlags_AlphaBar);
                //if (memcmp(&style.Colors[i], &dst->Colors[i], sizeof(ImVec4)) != 0)
                //{
                //    // Tips: in a real user application, you may want to merge and use an icon font into the main font,
                //    // so instead of "Save"/"Revert" you'd use icons!
                //    // Read the FAQ and docs/FONTS.md about using icon fonts. It's really easy and super convenient!
                //    ImGui::SameLine(0.0f, style.ItemInnerSpacing.x); if (ImGui::Button("Save")) { dst->Colors[i] = style.Colors[i]; }
                //    ImGui::SameLine(0.0f, style.ItemInnerSpacing.x); if (ImGui::Button("Revert")) { style.Colors[i] = dst->Colors[i]; }
                //}
                ImGui::SameLine(0.0f, style.ItemInnerSpacing.x);
                ImGui::TextUnformatted(name);
                ImGui::PopID();
            }
            ImGui::PopItemWidth();
            ImGui::EndChild();
            ImGui::EndTabItem();
        }
        if (ImGui::BeginTabItem("Sizes")) {
            ImGui::SetNextWindowSizeConstraints(ImVec2(0.0f, ImGui::GetTextLineHeightWithSpacing() * 10), ImVec2(FLT_MAX, FLT_MAX));
            ImGui::BeginChild("##sizes", ImVec2(0, 0), ImGuiChildFlags_Borders | ImGuiChildFlags_NavFlattened, ImGuiWindowFlags_AlwaysVerticalScrollbar | ImGuiWindowFlags_AlwaysHorizontalScrollbar);
            ImGui::PushItemWidth(ImGui::GetFontSize() * -12);

            ImGui::SeparatorText("Main");
            ImGui::SliderFloat2("WindowPadding", (float*)&style.WindowPadding, 0.0f, 20.0f, "%.0f");
            ImGui::SliderFloat2("FramePadding", (float*)&style.FramePadding, 0.0f, 20.0f, "%.0f");
            ImGui::SliderFloat2("ItemSpacing", (float*)&style.ItemSpacing, 0.0f, 20.0f, "%.0f");
            ImGui::SliderFloat2("ItemInnerSpacing", (float*)&style.ItemInnerSpacing, 0.0f, 20.0f, "%.0f");
            ImGui::SliderFloat2("TouchExtraPadding", (float*)&style.TouchExtraPadding, 0.0f, 10.0f, "%.0f");
            ImGui::SliderFloat("IndentSpacing", &style.IndentSpacing, 0.0f, 30.0f, "%.0f");
            ImGui::SliderFloat("ScrollbarSize", &style.ScrollbarSize, 1.0f, 20.0f, "%.0f");
            ImGui::SliderFloat("GrabMinSize", &style.GrabMinSize, 1.0f, 20.0f, "%.0f");

            ImGui::SeparatorText("Borders");
            ImGui::SliderFloat("WindowBorderSize", &style.WindowBorderSize, 0.0f, 1.0f, "%.0f");
            ImGui::SliderFloat("ChildBorderSize", &style.ChildBorderSize, 0.0f, 1.0f, "%.0f");
            ImGui::SliderFloat("PopupBorderSize", &style.PopupBorderSize, 0.0f, 1.0f, "%.0f");
            ImGui::SliderFloat("FrameBorderSize", &style.FrameBorderSize, 0.0f, 1.0f, "%.0f");
            ImGui::SliderFloat("TabBorderSize", &style.TabBorderSize, 0.0f, 1.0f, "%.0f");
            ImGui::SliderFloat("TabBarBorderSize", &style.TabBarBorderSize, 0.0f, 2.0f, "%.0f");
            ImGui::SliderFloat("TabBarOverlineSize", &style.TabBarOverlineSize, 0.0f, 2.0f, "%.0f");

            ImGui::SeparatorText("Rounding");
            ImGui::SliderFloat("WindowRounding", &style.WindowRounding, 0.0f, 12.0f, "%.0f");
            ImGui::SliderFloat("ChildRounding", &style.ChildRounding, 0.0f, 12.0f, "%.0f");
            ImGui::SliderFloat("FrameRounding", &style.FrameRounding, 0.0f, 12.0f, "%.0f");
            ImGui::SliderFloat("PopupRounding", &style.PopupRounding, 0.0f, 12.0f, "%.0f");
            ImGui::SliderFloat("ScrollbarRounding", &style.ScrollbarRounding, 0.0f, 12.0f, "%.0f");
            ImGui::SliderFloat("GrabRounding", &style.GrabRounding, 0.0f, 12.0f, "%.0f");
            ImGui::SliderFloat("TabRounding", &style.TabRounding, 0.0f, 12.0f, "%.0f");

            ImGui::SeparatorText("Tables");
            ImGui::SliderFloat2("CellPadding", (float*)&style.CellPadding, 0.0f, 20.0f, "%.0f");
            ImGui::SliderAngle("TableAngledHeadersAngle", &style.TableAngledHeadersAngle, -50.0f, +50.0f);
            ImGui::SliderFloat2("TableAngledHeadersTextAlign", (float*)&style.TableAngledHeadersTextAlign, 0.0f, 1.0f, "%.2f");

            ImGui::SeparatorText("Widgets");
            ImGui::SliderFloat2("WindowTitleAlign", (float*)&style.WindowTitleAlign, 0.0f, 1.0f, "%.2f");
            int window_menu_button_position = style.WindowMenuButtonPosition + 1;
            if (ImGui::Combo("WindowMenuButtonPosition", (int*)&window_menu_button_position, "None\0Left\0Right\0"))
                style.WindowMenuButtonPosition = (ImGuiDir)(window_menu_button_position - 1);
            ImGui::Combo("ColorButtonPosition", (int*)&style.ColorButtonPosition, "Left\0Right\0");
            ImGui::SliderFloat2("ButtonTextAlign", (float*)&style.ButtonTextAlign, 0.0f, 1.0f, "%.2f");
            ImGui::SliderFloat2("SelectableTextAlign", (float*)&style.SelectableTextAlign, 0.0f, 1.0f, "%.2f");
            ImGui::SliderFloat("SeparatorTextBorderSize", &style.SeparatorTextBorderSize, 0.0f, 10.0f, "%.0f");
            ImGui::SliderFloat2("SeparatorTextAlign", (float*)&style.SeparatorTextAlign, 0.0f, 1.0f, "%.2f");
            ImGui::SliderFloat2("SeparatorTextPadding", (float*)&style.SeparatorTextPadding, 0.0f, 40.0f, "%.0f");
            ImGui::SliderFloat("LogSliderDeadzone", &style.LogSliderDeadzone, 0.0f, 12.0f, "%.0f");

            ImGui::SeparatorText("Docking");
            ImGui::SliderFloat("DockingSplitterSize", &style.DockingSeparatorSize, 0.0f, 12.0f, "%.0f");

            ImGui::SeparatorText("Tooltips");
            for (int n = 0; n < 2; n++) {
                if (ImGui::TreeNodeEx(n == 0 ? "HoverFlagsForTooltipMouse" : "HoverFlagsForTooltipNav")) {
                    ImGuiHoveredFlags* p = (n == 0) ? &style.HoverFlagsForTooltipMouse : &style.HoverFlagsForTooltipNav;
                    ImGui::CheckboxFlags("ImGuiHoveredFlags_DelayNone", p, ImGuiHoveredFlags_DelayNone);
                    ImGui::CheckboxFlags("ImGuiHoveredFlags_DelayShort", p, ImGuiHoveredFlags_DelayShort);
                    ImGui::CheckboxFlags("ImGuiHoveredFlags_DelayNormal", p, ImGuiHoveredFlags_DelayNormal);
                    ImGui::CheckboxFlags("ImGuiHoveredFlags_Stationary", p, ImGuiHoveredFlags_Stationary);
                    ImGui::CheckboxFlags("ImGuiHoveredFlags_NoSharedDelay", p, ImGuiHoveredFlags_NoSharedDelay);
                    ImGui::TreePop();
                }
            }
            ImGui::SeparatorText("Misc");
            ImGui::SliderFloat2("DisplayWindowPadding", (float*)&style.DisplayWindowPadding, 0.0f, 30.0f, "%.0f");
            ImGui::SliderFloat2("DisplaySafeAreaPadding", (float*)&style.DisplaySafeAreaPadding, 0.0f, 30.0f, "%.0f");
            
            ImGui::PopItemWidth();
            ImGui::EndChild();
            ImGui::EndTabItem();
        }
        ImGui::EndTabBar();
    }
}

void gui::ImGuiHandler::DrawPreferencesWindow() {
    if (openPreferencesTab) {
        ImGui::SetNextWindowSize(ImVec2(ImGui::GetIO().DisplaySize.x * 0.3f, ImGui::GetIO().DisplaySize.y * 0.8f), ImGuiCond_FirstUseEver);
        if (ImGui::Begin("Preferences", &openPreferencesTab, ImGuiWindowFlags_NoDocking)) {
            CustomStyleProfile();
            ImGui::End();
        }
    }
}