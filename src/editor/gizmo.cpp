#include "gizmo.hpp"

using namespace Editor;

Gizmo::Gizmo(){
    // Empty
}

void Gizmo::Show(SimObject::Entity *entity, Projection &projection, Camera &camera){
    if(entity == nullptr) return;
    ImGui::Begin("Scene");
    ImGuizmo::SetOrthographic(false);
    ImGuizmo::SetDrawlist();
    ImGuizmo::SetRect(0, 0, (float)Const::WIDTH, (float)Const::HEIGHT);
    ImGuizmo::BeginFrame();

    ImGuizmo::Manipulate(glm::value_ptr(projection.view),
                        glm::value_ptr(projection.projection),
                        gizmoOperation,
                        gizmoMode,
                        (float*)entity->GetModelPointer(),
                        nullptr,
                        nullptr);

    // Modify camera
    glm::mat4 cameraMatrix = glm::inverse(projection.view);
    glm::vec3 position = glm::vec3(cameraMatrix[3]);
    glm::vec3 forward  = -glm::vec3(cameraMatrix[2]);
    glm::vec3 up       = glm::vec3(cameraMatrix[1]);

    camera.SetTransform(position, up);
    camera.LookAt(position + forward);
    projection.view = camera.GetViewMatrix();

    // If the gizmo was used, decompose the result and update
    if (ImGuizmo::IsUsing()) {
      glm::vec3 skew;
      glm::vec4 perspective;
      glm::quat rotQuat;

      glm::decompose(entity->GetModel(), entity->scale, rotQuat, entity->position, skew, perspective);
      entity->SetRotation(glm::degrees(glm::eulerAngles(rotQuat))); // Convert to Euler angles

      // Update the model matrix from the new values
      entity->UpdateModelMatrix();
    }
    ImGui::End();
}
