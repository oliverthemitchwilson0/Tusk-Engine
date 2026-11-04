#include <TuskGame/Game.h>
#include <TuskGame/MainDemoScene.h>

#include <algorithm>
#include <vector>

namespace TuskGame {

    DemoGame::DemoGame() noexcept {};

    bool DemoGame::Start(Tusk::Engine& engine) 
    {
        engine.SetScene<MainDemoScene>();

        return true;
    }

    void DemoGame::FixedUpdate(Tusk::Input& input, float deltaTime) 
    {

    }

    void DemoGame::Render(Tusk::Renderer& renderer) 
    {

    }

    void DemoGame::Stop(Tusk::Engine&) {}

}
