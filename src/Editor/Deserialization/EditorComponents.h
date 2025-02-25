#pragma once

#include "Display.h"
#include "Engine/Util/AssetParsing/Members.h"

#define EDITOR_COMPONENTS                                                                                              \
  INHERITANCE_PARSER(Engine::ComponentDSO, Editor::DisplayDSO)

#ifndef USER_COMPONENTS
#define USER_COMPONENTS EDITOR_COMPONENTS
#endif

namespace Editor {
  struct DisplayDSO : public ComponentDSO_T<Editor::Display> {
    std::string label;

    void FillValues(Editor::Display * display, AssetManager::LoaderMembers<Core::Entity> * loaderMembers) override {
      display->AssignLabel(label.c_str());
    }
  };
} // namespace Engine

OBJECT_PARSER(Editor::DisplayDSO, FIELD_PARSER(label));