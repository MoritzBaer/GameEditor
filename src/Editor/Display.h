#pragma once

#include <array>

#include "Core/ECS.h"
#include "Debug/Logging.h"
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

} // namespace Editor