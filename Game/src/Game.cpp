#include <TuskGame/Game.h>
#include <TuskGame/MainDemoScene.h>
#include <TuskGame/SecondDemoScene.h>

#include <algorithm>
#include <vector>


namespace TuskGame {

    DemoGame::DemoGame() noexcept {};

    bool DemoGame::Start(Tusk::SceneManager& sceneManager) 
    {
        //mainDemoScene = std::make_unique<MainDemoScene>();

        sceneManager.AddScene<MainDemoScene>("Main");
        sceneManager.AddScene<SecondDemoScene>("Second");
        //sceneManager.SetScene(mainDemoScene.get());

        return true;
    }

    void TuskGame::DemoGame::Stop(Tusk::SceneManager& sceneManager)
    {
    }
}
