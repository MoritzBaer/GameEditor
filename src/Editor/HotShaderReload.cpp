#include "HotShaderReload.h"

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
