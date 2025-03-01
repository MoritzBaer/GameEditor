#pragma once

#include "Engine/Core/ECS.h"
#include "Publishable.h"

namespace Editor {

    struct EditorComponent {
        virtual Publication Publish() = 0;
        virtual void UpdateRealComponent() = 0;
    };

    template <typename T>
    struct EditorComponentT : public EditorComponent, public Engine::Core::ComponentT<EditorComponentT<T>> {
        T * realComponent;
        EditorComponentT(Engine::Core::Entity entity) : EditorComponent(), Engine::Core::ComponentT<EditorComponentT<T>>(entity),
            realComponent(entity.HasComponent<T>() ? entity.GetComponent<T>() : entity.AddComponent<T>()) { }

        inline virtual void CopyFrom(EditorComponentT<T> const & other) override { }
    };

} // namespace Editor
