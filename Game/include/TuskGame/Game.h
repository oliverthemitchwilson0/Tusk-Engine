#pragma once
#include <Tusk/Engine.h>

#include <TuskGame/MainDemoScene.h>

#include <memory>

namespace TuskGame 
{
    //Extends from the Engine's Application class.
    class DemoGame final : public Tusk::Application {
    public:
        DemoGame() noexcept;

        bool Start(Tusk::SceneManager& sceneManager) override;
        void FixedUpdate(Tusk::Input& engine, float seconds) override;
        void Render(Tusk::Renderer& engine) override;
        void Stop(Tusk::SceneManager& sceneManager) override;

    private:
        std::unique_ptr<MainDemoScene> mainDemoScene;
    };

}
