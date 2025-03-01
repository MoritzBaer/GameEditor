#pragma once

#include "Engine/Graphics/Camera.h"
#include "Components/EditorComponent.h"
#include "Publications/NumericTypes.h"

namespace Editor {
    struct Camera : public EditorComponentT<Engine::Graphics::Camera> {
        float fov;
        float nearClip;
        float farClip;
        float aspectRatio;

        Camera(Engine::Core::Entity entity) : EditorComponentT<Engine::Graphics::Camera>(entity) { }

        inline Publication Publish() override;

        inline void UpdateRealComponent() override {
            realComponent->projection = Maths::Transformations::Perspective(nearClip, farClip, fov, aspectRatio);
        }

        inline void CopyFrom(EditorComponentT<Engine::Graphics::Camera> const & other) override {
            if (auto otherCamera = dynamic_cast<Camera const *>(&other)) {
                fov = otherCamera->fov;
                nearClip = otherCamera->nearClip;
                farClip = otherCamera->farClip;
                aspectRatio = otherCamera->aspectRatio;
            }
        }
    };

    template <> inline constexpr char const * Publishable<Camera>::typeLabel = "Camera";
    template <>
    inline Publication Publishable<Camera>::Publish(Camera & value,
        const char * label) {
        return COMPOSITE(PUBLISH(fov),  // TODO: Set reasonable ranges
            PUBLISH(nearClip),
            PUBLISH(farClip),
            PUBLISH(aspectRatio));
    }

    inline Publication Camera::Publish() {
        return Publishable<Camera>::Publish(*this);
    }
} // namespace Editor
