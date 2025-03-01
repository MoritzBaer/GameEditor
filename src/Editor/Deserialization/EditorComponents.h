#pragma once

#include "Components/Display.h"
#include "Components/Transform.h"
#include "Components/MeshRenderer.h"
#include "Components/Camera.h"
#include "Engine/Util/AssetParsing/Members.h"

#define EDITOR_COMPONENTS                                                                                              \
  INHERITANCE_PARSER(Engine::ComponentDSO, Editor::MeshRendererDSO)                                                    \
  INHERITANCE_PARSER(Engine::ComponentDSO, Editor::DisplayDSO)                                                         \
  INHERITANCE_PARSER(Engine::ComponentDSO, Editor::CameraDSO)                                                          \
  INHERITANCE_PARSER(Engine::ComponentDSO, Editor::TransformDSO)

#ifndef USER_COMPONENTS
#define USER_COMPONENTS EDITOR_COMPONENTS
#endif

namespace Editor {
  struct DisplayDSO : public ComponentDSO_T<Display> {
    std::string label;

    void FillValues(Display * display, AssetManager::LoaderMembers<Core::Entity> * loaderMembers) override {
      display->AssignLabel(label.c_str());
    }
  };

  struct TransformDSO : public ComponentDSO_T<Transform> {
    Engine::Maths::Vector3 position, rotation, scale;

    void FillValues(Transform * transform, AssetManager::LoaderMembers<Core::Entity> * loaderMembers) override {
      transform->position = position;
      transform->rotation = rotation;
      transform->scale = scale;
      transform->UpdateRealComponent();
    }
  };

  struct MeshRendererDSO : public ComponentDSO_T<MeshRenderer> {
    std::string meshName;
    std::string materialName;

    void FillValues(MeshRenderer * meshRenderer, AssetManager::LoaderMembers<Core::Entity> * loaderMembers) override {
      meshRenderer->meshName = meshName;
      meshRenderer->materialName = materialName;
      meshRenderer->SetAssetManager(loaderMembers->assetManager);
      meshRenderer->UpdateRealComponent();
    }
  };

  struct CameraDSO : public ComponentDSO_T<Camera> {
    float fov;
    float nearClip;
    float farClip;
    float aspectRatio;

    void FillValues(Camera * camera, AssetManager::LoaderMembers<Core::Entity> * loaderMembers) override {
      camera->fov = fov;
      camera->nearClip = nearClip;
      camera->farClip = farClip;
      camera->aspectRatio = aspectRatio;
      camera->UpdateRealComponent();
    }
  };

} // namespace Engine

OBJECT_PARSER(Editor::DisplayDSO, FIELD_PARSER(label));
OBJECT_PARSER(Editor::TransformDSO, FIELD_PARSER(position) FIELD_PARSER(rotation) FIELD_PARSER(scale));
OBJECT_PARSER(Editor::MeshRendererDSO, FIELD_PARSER(meshName) FIELD_PARSER(materialName));
OBJECT_PARSER(Editor::CameraDSO, FIELD_PARSER(fov) FIELD_PARSER(nearClip) FIELD_PARSER(farClip) FIELD_PARSER(aspectRatio));