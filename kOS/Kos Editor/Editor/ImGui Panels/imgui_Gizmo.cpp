#include "Editor.h"
#include "imgui_impl_opengl3.h"
#include "Editor/EditorCamera.h"
#include "ECS/ECS.h"
#include "ECS/Hierachy.h"

    namespace gui {
    void ImGuiHandler::DrawGizmo(float renderPosX, float renderPosY, float renderWidth, float renderHeight)
    {
        ecs::TransformComponent* transcom = m_ecs->GetComponent<ecs::TransformComponent>(m_clickedEntityId);
        if (m_clickedEntityId < 0 || !transcom) return;

        ImGuizmo::SetDrawlist();
        ImGuizmo::SetRect(renderPosX, renderPosY, renderWidth, renderHeight);

        static ImGuizmo::OPERATION mCurrentGizmoOperation(ImGuizmo::TRANSLATE);
        static bool useSnap{ false }, focusMode{ false };
        static float snap[3] = { 1.f, 1.f, 1.f };

        if (ImGui::IsWindowHovered() && ImGui::IsKeyPressed(ImGuiKey_W) && !ImGuizmo::IsUsing())
            mCurrentGizmoOperation = ImGuizmo::TRANSLATE;
        if (ImGui::IsWindowHovered() && ImGui::IsKeyPressed(ImGuiKey_E) && !ImGuizmo::IsUsing())
            mCurrentGizmoOperation = ImGuizmo::ROTATE;
        if (ImGui::IsWindowHovered() && ImGui::IsKeyPressed(ImGuiKey_R) && !ImGuizmo::IsUsing())
            mCurrentGizmoOperation = ImGuizmo::SCALE;
        if (ImGui::IsWindowHovered() && ImGui::IsKeyDown(ImGuiKey_LeftCtrl)) {
            useSnap = true;
        }
        else if (ImGui::IsKeyReleased(ImGuiKey_LeftCtrl)) {
            useSnap = false;
        }

 
        glm::mat4 cameraView = m_isUi ? glm::mat4{ 1.f } : EditorCamera::editorCamera.GetViewMtx();

        //Change projection based on type... how ah
        glm::mat4 projection = m_isUi ? glm::mat4{ EditorCamera::editorCamera.GetUIOrthoMtx() } : EditorCamera::editorCamera.GetPerspMtx();
        glm::mat4 transformation = hierachy::GetParent(m_clickedEntityId).has_value() ? transcom->localTransform : transcom->transformation;
        if (m_isUi) { ImGuizmo::SetOrthographic(true); }
        else{ ImGuizmo::SetOrthographic(false); }
        // Not need for now.
        glm::mat4 deltaMtx(1.0f);

        ImGuizmo::Manipulate(
            glm::value_ptr(cameraView), glm::value_ptr(projection),
            mCurrentGizmoOperation, ImGuizmo::WORLD,
            glm::value_ptr(transformation),
            glm::value_ptr(deltaMtx),
            useSnap ? &snap[0] : NULL);

        //ImGuizmo::DrawGrid(glm::value_ptr(cameraView), glm::value_ptr(projection), glm::value_ptr(transformation), 200.f);

        if (ImGuizmo::IsUsing()) {
            glm::vec3 newPosition, newRotation, newScale;

            ImGuizmo::DecomposeMatrixToComponents(glm::value_ptr(transformation), glm::value_ptr(newPosition), glm::value_ptr(newRotation), glm::value_ptr(newScale));
            transcom->LocalTransformation.position = newPosition;
            transcom->LocalTransformation.rotation = newRotation;
            transcom->LocalTransformation.scale = newScale;

            // If this is used for zooming into the selected GO, it should probably in the camera code.
            if (ImGui::IsKeyDown(ImGuiKey_LeftShift) && ImGui::IsKeyPressed(ImGuiKey_F)) {
                focusMode = focusMode ? false : true;
            }
            if (focusMode) {

            }
        }
    }
}