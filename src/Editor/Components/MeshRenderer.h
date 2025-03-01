#pragma once

#include "Components/EditorComponent.h"
#include "Engine/Graphics/MeshRenderer.h"
#include "Engine/AssetManager.h"
#include "Engine/Debug/Logging.h"

namespace Editor {
    struct MeshRenderer : public EditorComponentT<Engine::Graphics::MeshRenderer> {
        std::string meshName;
        std::string materialName;   // TODO: Add material instance

        Engine::AssetManager * assetManager;

        MeshRenderer(Engine::Core::Entity entity) : EditorComponentT<Engine::Graphics::MeshRenderer>(entity), assetManager(nullptr) { }

        inline void SetAssetManager(Engine::AssetManager * assetManager) {
            this->assetManager = assetManager;
        }

        inline Publication Publish() override;

        inline void UpdateRealComponent() override {
            if (!assetManager) {
                return;
            }
            realComponent->mesh = assetManager->LoadAsset<Engine::Graphics::AllocatedMesh *>(meshName);
            realComponent->material = assetManager->LoadAsset<Engine::Graphics::Material *>(materialName);
        }

        inline void CopyFrom(EditorComponentT<Engine::Graphics::MeshRenderer> const & other) override {
            if (auto otherMeshRenderer = dynamic_cast<MeshRenderer const *>(&other)) {
                meshName = otherMeshRenderer->meshName;
                materialName = otherMeshRenderer->materialName;
            }
        }
    };

    template <> inline constexpr char const * Publishable<std::string>::typeLabel = "String";
    template <>
    inline Publication Publishable<std::string>::Publish(std::string & value,
        const char * label) {
        return Publication{
            .label = value.c_str(),
            .type = Publication::Type::TEXT
        };
    }

    template <> inline constexpr char const * Publishable<MeshRenderer>::typeLabel = "Mesh Renderer";
    template <>
    inline Publication Publishable<MeshRenderer>::Publish(MeshRenderer & value,
        const char * label) {
        return COMPOSITE(PUBLISH(meshName),
            PUBLISH(materialName));
    }

    inline Publication MeshRenderer::Publish() {
        return Publishable<MeshRenderer>::Publish(*this);
    }
} // namespace Editor
