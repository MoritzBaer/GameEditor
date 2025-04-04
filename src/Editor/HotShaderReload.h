#pragma once

#include "Engine/Util/AssetParsing/MaterialParsing.h"
#include "Engine/Util/AssetParsing/ShaderParsing.h"

using namespace Engine::Graphics;
using namespace Engine;

#ifndef RESOURCE_PATH
#define RESOURCE_PATH "res/"
#endif

namespace Editor {

    class ReloadablePipelineManager {
        PipelineCache pipelineCache;
        PipelineLoader pipelineLoader;
        InstanceManager const * instanceManager;

    public:
        ReloadablePipelineManager(InstanceManager const * instanceManager, AssetManager * assetManager)
            : pipelineLoader(assetManager, instanceManager), pipelineCache(instanceManager), instanceManager(instanceManager) { }

        inline Pipeline * LoadAsset(char const * assetName) {
            if (!pipelineCache.HasAsset(assetName)) {
                pipelineCache.InsertAsset(assetName, pipelineLoader.LoadAsset(assetName));
            }
            return pipelineCache.LoadAsset(assetName);
        }
        inline void Cleanup() { pipelineCache.Clear(); }
        void HotReloadAllPipelines();
    };

    class ReloadableBackgroundEffectManager {
        ComputeBackgroundCache cache;
        ComputeBackgroundLoader loader;

    public:
        ReloadableBackgroundEffectManager(InstanceManager const * instanceManager, AssetManager * assetManager)
            : cache(instanceManager), loader(instanceManager, assetManager) { }
        inline void Cleanup() { cache.Clear(); }
        void HotReloadAllEffects();
        inline RenderingStrategies::ComputeBackground * LoadAsset(const char * assetName) {
            if (!cache.HasAsset(assetName)) {
                cache.InsertAsset(assetName, loader.LoadAsset(assetName));
            }
            return cache.LoadAsset(assetName);
        }
    };

    class ShaderReloader {
        InstanceManager const * instanceManager;
        ShaderManager<ShaderType::VERTEX> * vertexShaderManager;
        ShaderManager<ShaderType::GEOMETRY> * geometryShaderManager;
        ShaderManager<ShaderType::FRAGMENT> * fragmentShaderManager;
        ShaderManager<ShaderType::COMPUTE> * computeShaderManager;
        ReloadablePipelineManager * pipelineManager;

        CompiledEffectManager * compiledEffectManager;
        ReloadableBackgroundEffectManager * backgroundEffectManager;

        static inline const std::string shaderPath = std::string(RESOURCE_PATH) + "shaders/";

    public:
        ShaderReloader() : vertexShaderManager(nullptr), geometryShaderManager(nullptr),
            fragmentShaderManager(nullptr), computeShaderManager(nullptr), pipelineManager(nullptr),
            compiledEffectManager(nullptr), backgroundEffectManager(nullptr) { }
        ShaderReloader(
            InstanceManager const * instanceManager,
            ShaderManager<ShaderType::VERTEX> * vertexShaderManager,
            ShaderManager<ShaderType::GEOMETRY> * geometryShaderManager,
            ShaderManager<ShaderType::FRAGMENT> * fragmentShaderManager,
            ShaderManager<ShaderType::COMPUTE> * computeShaderManager,
            ReloadablePipelineManager * pipelineManager,
            CompiledEffectManager * compiledEffectManager,
            ReloadableBackgroundEffectManager * backgroundEffectManager)
            : vertexShaderManager(vertexShaderManager), geometryShaderManager(geometryShaderManager),
            fragmentShaderManager(fragmentShaderManager), computeShaderManager(computeShaderManager),
            pipelineManager(pipelineManager), instanceManager(instanceManager),
            compiledEffectManager(compiledEffectManager),
            backgroundEffectManager(backgroundEffectManager) { }

        inline void DoHotReload() {
            Util::FileIO::CopyDirectory(shaderPath, "res/shaders/");

            vertexShaderManager->Cleanup();
            geometryShaderManager->Cleanup();
            fragmentShaderManager->Cleanup();
            computeShaderManager->Cleanup();

            instanceManager->WaitUntilDeviceIdle();

            compiledEffectManager->Cleanup();
            pipelineManager->HotReloadAllPipelines();
            backgroundEffectManager->HotReloadAllEffects();
        }
    };

} // namespace Editor
