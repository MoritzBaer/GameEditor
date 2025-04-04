#include "EntityDetails.h"

#include "Components/EditorComponent.h"
#include "Components/Display.h"
#include "imgui.h"
#include "Components/Script.h"
#include "Publications/NumericTypes.h"

#ifdef SCRIPT_PUBLICATION_SOURCE
#pragma message("SCRIPT_PUBLICATION_SOURCE defined as " SCRIPT_PUBLICATION_SOURCE)
#include SCRIPT_PUBLICATION_SOURCE
#endif

#ifndef SCRIPT_PUBLICATIONS
#define SCRIPT_PUBLICATIONS
#endif

template <> inline constexpr char const * Editor::Publishable<Engine::Core::Script *>::typeLabel = "Script";
template <> inline Editor::Publication Editor::Publishable<Engine::Core::Script *>::Publish(Engine::Core::Script *& value, const char * label) {
  SCRIPT_PUBLICATIONS
    return { .label = "Unknown script", .type = Publication::Type::TEXT };
}

struct TestScript : public Engine::Core::Script {
  float testValue = 0.0f;
  TestScript(Engine::Core::Entity entity) : Engine::Core::Script(entity) { }
  void OnUpdate(Engine::Core::Clock const & clock) override {
    testValue += 0.1f;
  }
  void Clone(Engine::Core::ScriptComponent * targetComponent) override {
    targetComponent->InstantiateScript<TestScript>();
  }
};

void Editor::EntityDetails::DrawContent() {
  if (selectedEntity->IsAlive()) {
    {
      ImGui::Text("Active");
      ImGui::SameLine();
      ImGui::Checkbox("##active", &active);
      if (oldActive != selectedEntity->IsActive()) {
        active = selectedEntity->IsActive();
        oldActive = active;
      } else if (oldActive != active) {
        selectedEntity->SetActive(active);
      }
    }

    if (selectedEntity->HasComponent<Display>()) {
      DrawPublication(Publishable<Display>::Publish(*selectedEntity->GetComponent<Display>()));
    }

    for (auto c : selectedEntity->GetComponents()) {
      if (auto editorComponent = dynamic_cast<EditorComponent *>(c))
        DrawPublication(editorComponent->Publish());
    }

    if (selectedEntity->HasComponent<Engine::Core::ScriptComponent>()) {
      for (auto script : selectedEntity->GetComponent<Engine::Core::ScriptComponent>()->scripts) {
        DrawPublication(Publishable<Engine::Core::Script *>::Publish(script));
      }
    }
  }
}