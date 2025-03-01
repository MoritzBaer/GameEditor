#pragma once

#include "Components/EditorComponent.h"
#include "Engine/Graphics/Transform.h"
#include "Publications/NumericTypes.h"

namespace Editor {

    struct Transform : EditorComponentT<Engine::Graphics::Transform> {
        Engine::Maths::Vector3 position;
        Engine::Maths::Vector3 rotation;
        Engine::Maths::Vector3 scale;

        Transform(Engine::Core::Entity entity) : EditorComponentT<Engine::Graphics::Transform>(entity) { }

        inline Publication Publish() override;

        inline void UpdateRealComponent() {
            realComponent->position = position;
            realComponent->rotation = Engine::Maths::Quaternion::FromEulerAngles(rotation);
            realComponent->scale = scale;
        }
    };

    template <> inline constexpr char const * Publishable<Transform>::typeLabel = "Transform";
    template <>
    inline Publication Publishable<Transform>::Publish(Transform & value,
        const char * label) {
        return COMPOSITE(PUBLISH(position),
            PUBLISH_RANGE(rotation, -3.14159265359f, 3.14159265359f, 0.001f),
            PUBLISH_RANGE(scale, 0.01f, 100.0f, 0.01f));
    }

    inline Publication Transform::Publish() {
        position = realComponent->position;
        rotation = realComponent->rotation.EulerAngles();
        scale = realComponent->scale;
        return Publishable<Transform>::Publish(*this);
    }

} // namespace Editor
