#pragma once

#include "Components/EditorComponent.h"
#include "Engine/Core/Script.h"

#define SCRIPT_ARGS(Arg1, ...)              \
    realScript.Arg1                      \
    __VA_OPT__(, SCRIPT_ARGS(__VA_ARGS__))

#define SCRIPT_CLONE(T, ...)                                                                                \
    inline void ScriptT<T>::Clone(Engine::Core::ScriptComponent * targetComponent) {                \
        targetComponent->InstantiateScript<ScriptT<T>>(__VA_OPT__(SCRIPT_ARGS(__VA_ARGS__)));       \
    }

#define S_PUBLISH_WITH_LABEL(object, label) Editor::Publishable<decltype(tScript->object)>::Publish(tScript->object, label)
#define S_PUBLISH(object) S_PUBLISH_WITH_LABEL(object, #object)
#define S_PUBLISH_RANGE_WITH_LABEL(object, label, min, max, step) RANGE(min, max, step, S_PUBLISH_WITH_LABEL(object, label))
#define S_PUBLISH_RANGE(object, min, max, step) S_PUBLISH_RANGE_WITH_LABEL(object, #object, min, max, step)
#define S_PUBLISH_SLIDER_WITH_LABEL(object, label, min, max, step) SLIDER(min, max, step, S_PUBLISH_WITH_LABEL(object, label))
#define S_PUBLISH_SLIDER(object, min, max, step) S_PUBLISH_SLIDER_WITH_LABEL(object, #object, min, max, step)

#define FOR_SCRIPT(Arg1, ...) Arg1 __VA_OPT__(, FOR_SCRIPT(__VA_ARGS__))

#define SCRIPT_PUBLISH(T, ...)                                  \
    if (auto tScript = dynamic_cast<T *>(script)) {             \
        const char * label = #T;                                \
        DrawPublication(COMPOSITE(__VA_ARGS__));                \
    } else

#define PUBLISH_ARGS(Arg1, ...) PUBLISH(Arg1) __VA_OPT__(, PUBLISH_ARGS(__VA_ARGS__))

#define REGISTER_SCRIPT(T, ...)                                                                                          \
    SCRIPT_PUBLISH(T __VA_OPT__(, __OPT_ARGS__(PUBLISH_ARGS(__VA_ARGS__))))                                                           \
    SCRIPT_CLONE(T __VA_OPT__(, __VA_ARGS__))