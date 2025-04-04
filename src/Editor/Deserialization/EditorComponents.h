#pragma once

#include "Components/Display.h"
#include "Components/Transform.h"
#include "Components/MeshRenderer.h"
#include "Components/Camera.h"
#include "Components/Script.h"
#include "Engine/Util/AssetParsing/ComponentParsing.h"

#define EDITOR_COMPONENTS                                                     \
  Editor::MeshRendererDSO,                                                    \
  Editor::DisplayDSO,                                                         \
  Editor::CameraDSO,                                                          \
  Editor::TransformDSO                                                      

#ifndef USER_COMPONENTS
#define USER_COMPONENTS EDITOR_COMPONENTS
#endif

namespace Editor {
  struct DisplayDSO : public ComponentDSO_T<Display> {
    std::string label;

    void FillValues(Display * display, AssetManager *) override {
      display->AssignLabel(label.c_str());
    }
  };

  struct TransformDSO : public ComponentDSO_T<Transform> {
    Engine::Maths::Vector3 position, rotation, scale;

    void FillValues(Transform * transform, AssetManager *) override {
      transform->position = position;
      transform->rotation = rotation;
      transform->scale = scale;
      transform->UpdateRealComponent();
    }
  };

  struct MeshRendererDSO : public ComponentDSO_T<MeshRenderer> {
    std::string meshName;
    std::string materialName;

    void FillValues(MeshRenderer * meshRenderer, AssetManager * assetManager) override {
      meshRenderer->meshName = meshName;
      meshRenderer->materialName = materialName;
      meshRenderer->SetAssetManager(assetManager);
      meshRenderer->UpdateRealComponent();
    }
  };

  struct CameraDSO : public ComponentDSO_T<Camera> {
    float fov;
    float nearClip;
    float farClip;
    float aspectRatio;

    void FillValues(Camera * camera, AssetManager *) override {
      camera->fov = fov;
      camera->nearClip = nearClip;
      camera->farClip = farClip;
      camera->aspectRatio = aspectRatio;
      camera->UpdateRealComponent();
    }
  };

} // namespace Engine

JSON(Editor::DisplayDSO, FIELDS(label));
JSON(Editor::TransformDSO, FIELDS(position, rotation, scale));
JSON(Editor::MeshRendererDSO, FIELDS(meshName, materialName));
JSON(Editor::CameraDSO, FIELDS(fov, nearClip, farClip, aspectRatio));