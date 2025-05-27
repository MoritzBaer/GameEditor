#include "HotShaderReload.h"

#ifdef RESOURCE_PATH
#pragma message("RESOURCE_PATH defined as '" RESOURCE_PATH "'.")
#else
#pragma message("RESOURCE_PATH not defined, using default 'res/'")
#define RESOURCE_PATH "res/"
#endif

static inline const std::string shaderPath = std::string(RESOURCE_PATH) + "shaders/";

void HotReloadPipeline(std::string pipelineName, Pipeline * currentPipeline) {

}

void Editor::ReloadablePipelineManager::HotReloadAllPipelines() {
    for (auto & [name, oldPipeline] : pipelineCache) {
        auto newPipeline = pipelineLoader.LoadAsset(name.c_str());
        std::swap(*oldPipeline, *newPipeline);
        PipelineBuilder::DestroyPipeline(*newPipeline, instanceManager);
        delete newPipeline;
    }
}

void Editor::ReloadableBackgroundEffectManager::HotReloadAllEffects() {
    for (auto & [name, oldEffect] : cache) {
        auto newEffect = loader.LoadAsset(name.c_str());
        std::swap(*oldEffect, *newEffect);
        newEffect->Cleanup();
        delete newEffect;
    }
}

void Editor::ShaderReloader::DoHotReload() { {
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
 }
