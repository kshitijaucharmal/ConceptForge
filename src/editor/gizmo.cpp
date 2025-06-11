#include "gizmo.hpp"

using namespace Editor;

Gizmo::Gizmo(Camera &cam, Projection &pro) : camera(cam), projection(pro){
  glGenFramebuffers(1, &fbo);
  glBindFramebuffer(GL_FRAMEBUFFER, fbo);

  glGenTextures(1, &fboTexture);
  glBindTexture(GL_TEXTURE_2D, fboTexture);
  glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, Const::WIDTH, Const::HEIGHT, 0, GL_RGB,
               GL_UNSIGNED_BYTE, nullptr);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

  glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, fboTexture, 0);

  if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
    std::cout << "Framebuffer not complete!" << std::endl;
  else std::cout << fbo << std::endl;

  glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void Gizmo::Show(SimObject::Entity &entity, WindowManagement::Window &window){
    glBindFramebuffer(GL_FRAMEBUFFER, fbo);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    ImGuizmo::SetOrthographic(false);
    ImGuizmo::SetDrawlist();
    auto pos = window.viewportPos;
    auto size = window.viewportSize;
    ImGuizmo::SetRect(pos.x, pos.y, size.x, size.y);
    ImGuizmo::BeginFrame();

    ImGuizmo::Manipulate(glm::value_ptr(projection.view),
                        glm::value_ptr(projection.projection),
                        gizmoOperation,
                        gizmoMode,
                        glm::value_ptr(entity.model),
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

      glm::decompose(entity.model, entity.scale, rotQuat, entity.position, skew, perspective);
      entity.rotation = glm::degrees(glm::eulerAngles(rotQuat)); // Convert to Euler angles

      // Update the model matrix from the new values
      entity.UpdateModelMatrix();
    }
    glBindFramebuffer(GL_FRAMEBUFFER, 0);

}
