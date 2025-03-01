#pragma once

#include "Core/ECS.h"
#include "Components/Display.h"
#include "Components/Transform.h"
#include "Components/MeshRenderer.h"
#include "Components/Camera.h"
#include "EditorGUIRenderingStrategy.h"
#include "ImGUIManager.h"
#include "EntityDetails.h"
#include "Game.h"
#include "Graphics/RenderingStrategies/ComputeBackground.h"
#include "Graphics/RenderingStrategies/ForwardRendering.h"
#include "RenderView.h"
#include "SceneView.h"
#include "WindowedApplication.h"
#include "Components/EditorComponent.h"

namespace Editor {
  struct GameControl : public Engine::Graphics::ImGUIView {
    bool * runGame;
    GameControl(bool * runGame) : ImGUIView("Game Control"), runGame(runGame) { }

    void DrawContent() override {
      if (ImGui::Button("Start Game")) {
        *runGame = true;
      }
    }
  };

  struct DebugInfo : public Engine::Graphics::ImGUIView {
    bool showImGuiDemo = false;
    Engine::Core::Clock const * clock;
    DebugInfo(Engine::Core::Clock const * clock) : ImGUIView("Debug Info"), clock(clock) { }

    void DrawContent() override {
      ImGui::Text("FPS: %.1f", 1.0f / clock->deltaTime);
      ImGui::SameLine();
      if (showImGuiDemo) {
        if (ImGui::Button("Hide demo window")) {
          showImGuiDemo = false;
        }
      } else if (ImGui::Button("Show demo window")) {
        showImGuiDemo = true;
      }
      if (showImGuiDemo) {
        ImGui::ShowDemoWindow();
      }
      ImGui::Text("Time: %.2f", clock->time);
      ImGui::SameLine();
      ImGui::Text("Delta Time: %.8f", clock->deltaTime);
    }
  };

  template <class GameInstance> struct Editor : public Game {
    GameInstance game;
    bool runGame;
    bool gameInitialized;
    Engine::Graphics::ImGUIManager * imGuiManager;
    GameControl gameControl;
    DebugInfo debugInfo;
    SceneView sceneView;
    RenderView gameView;
    RenderView viewport;

    Engine::Core::Entity selectedEntity;
    EntityDetails entityDetails;

    template <typename... GameArgs>
    Editor(Engine::Graphics::VulkanSuite
#ifdef NDEBUG
      const
#endif
      * vulkan,
      Engine::Graphics::ImGUIManager * imGuiManager, GameArgs &&...gameArgs)
      : Game("Editor", vulkan), game(vulkan, std::forward<GameArgs>(gameArgs)...), gameControl(&runGame),
      runGame(false), gameInitialized(false), imGuiManager(imGuiManager), sceneView(&selectedEntity),
      entityDetails(&selectedEntity), gameView(&vulkan->gpuObjectManager, &vulkan->instanceManager),
      viewport(&vulkan->gpuObjectManager, &vulkan->instanceManager), debugInfo(&clock) { }

    ~Editor() { vulkan->instanceManager.WaitUntilDeviceIdle(); }

    inline void Init() override {
      imGuiManager->RegisterView(&debugInfo);
      imGuiManager->RegisterView(&gameControl);
      imGuiManager->RegisterView(&sceneView);
      imGuiManager->RegisterView(&entityDetails);
      imGuiManager->RegisterView(&gameView);
      imGuiManager->RegisterView(&viewport);

      Core::ECS::RegisterComponent<Display>();    // Should be registered first so it appears at the top of the details page
      Game::Init();
      Core::ECS::RegisterComponent<Transform>();  // EditorComponents need to be registered after their engine counterparts, otherwise there's an issue with copying
      Core::ECS::RegisterComponent<MeshRenderer>();
      Core::ECS::RegisterComponent<Camera>();

      activeScene = assetManager.LoadAsset<Engine::Core::Scene *>("editor");
      game.Init();
      sceneView.SetSceneHierarchy(&game.activeScene->sceneHierarchy);
      game.renderer.SetRenderResourceProvider(&gameView);
      delete renderingStrategy;
      renderingStrategy = new EditorGUIRenderingStrategy(
        &vulkan->gpuObjectManager,
        assetManager.LoadAsset<Engine::Graphics::RenderingStrategies::ComputeBackground *>("editor"));
      renderer.SetRenderingStrategy(renderingStrategy);
    }

    inline void CalculateFrame() override {
      for (auto comp : selectedEntity.GetComponents()) {
        if (auto editorComp = dynamic_cast<EditorComponent *>(comp)) {
          editorComp->UpdateRealComponent();
        }
      }
      if (runGame) {
        if (!gameInitialized) {
          game.Start();
          gameInitialized = true;
        }
        game.CalculateFrame();
      }
      imGuiManager->BeginFrame();
      Game::CalculateFrame();
    }
  };

  template <class GameInstance> class EditorApp : public GameApp<Editor<GameInstance>> {
  private:
    Engine::Graphics::ImGUIManager imGuiManager;

  public:
    template <typename... GameArgs>
    EditorApp(const char * name, Engine::Maths::Dimension2 const & windowSize, GameArgs &&...gameArgs)
      : GameApp<Editor<GameInstance>>(name, windowSize, &imGuiManager, std::forward<GameArgs>(gameArgs)...),
      imGuiManager(&this->windowedApplication.GetVulkan()->instanceManager) {
      imGuiManager.InitImGUIOnWindow(this->windowedApplication.GetWindow(),
        this->windowedApplication.GetSwapChainProvider()->GetSwapchainFormat());
    }
  };

} // namespace Editor
