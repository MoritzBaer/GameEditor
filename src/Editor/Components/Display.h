#pragma once

#include <array>

#include "Components/EditorComponent.h"
#include "Publishable.h"
#include "json-parsing.h"

using namespace Engine;

namespace Editor {

  struct Display : public Core::ComponentT<Display> {
    std::array<char, 64> label;
    Display(Core::Entity entity) : Core::ComponentT<Display>(entity), label("unnamed entity") { }

    inline void AssignLabel(const char * newLabel) {
      std::fill(label.begin(), label.end(), 0);
      strcpy(label.data(), newLabel);
    }

    inline void CopyFrom(Display const & other) override {
      AssignLabel(other.label.data());
    }
  };

  template <> inline constexpr char const * Publishable<Display>::typeLabel = "Display";
  template <> inline Publication Publishable<Display>::Publish(Display & value, const char * label) {
    return { .label = value.label.data(), .type = Publication::Type::TEXT, .referencedPointer = value.label.data() };
  }

} // namespace Editor