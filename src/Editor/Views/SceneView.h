#pragma once

#include "Core/SceneHierarchy.h"
#include "ImGUIManager.h"

namespace Editor {
  class EntityDetails;
  class SceneView : public Engine::Graphics::ImGUIView {
    Engine::Core::SceneHierarchy const * sceneHierarchy;
    Engine::Core::Entity * selectedEntity;

    void DrawNode(Engine::Core::SceneHierarchy::TreeNode const & node, ImGuiTreeNodeFlags flags = 0) const;

  public:
    SceneView(Engine::Core::Entity * selectedEntity)
      : ImGUIView("Scene"), sceneHierarchy(nullptr), selectedEntity(selectedEntity) { }
    inline void SetSceneHierarchy(Engine::Core::SceneHierarchy const * sceneHierarchy) {
      this->sceneHierarchy = sceneHierarchy;
    };
    void DrawContent() override;
  };

} // namespace Editor
