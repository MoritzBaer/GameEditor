#pragma once

#include "Components/EditorComponent.h"
#include "Engine/Core/Script.h"
#include "pp-foreach.h"

#define S_PUBLISH_WITH_LABEL(object, label) Editor::Publishable<decltype(script->object)>::Publish(script->object, label)
#define S_PUBLISH(object) S_PUBLISH_WITH_LABEL(object, #object)
#define S_PUBLISH_RANGE_WITH_LABEL(object, label, min, max, step) RANGE(min, max, step, S_PUBLISH_WITH_LABEL(object, label))
#define S_PUBLISH_RANGE(object, min, max, step) S_PUBLISH_RANGE_WITH_LABEL(object, #object, min, max, step)
#define S_PUBLISH_SLIDER_WITH_LABEL(object, label, min, max, step) SLIDER(min, max, step, S_PUBLISH_WITH_LABEL(object, label))
#define S_PUBLISH_SLIDER(object, min, max, step) S_PUBLISH_SLIDER_WITH_LABEL(object, #object, min, max, step)
#define S_COMPOSITE COMPOSITE
#define S_PUBLISH_NOTHING PUBLISH_NOTHING

#define __APPEND_S(arg) CAT(S_, __PROTECT(arg))

#define FOR_SCRIPT(...) FOR_EACH(__APPEND_S, __PROTECT(__VA_ARGS__))

// TODO: Figure out how to automatically attach S_ to normal PUBLISH macros
#define SCRIPT_PUBLISH(T, ...)                                              \
    if (auto script = dynamic_cast<T *>(value)) {                           \
        if (!strcmp(label, "Script"))                                       \
        label = #T;                                                         \
        return COMPOSITE(__VA_ARGS__);                                      \
    } else
