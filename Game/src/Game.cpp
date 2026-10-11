#include <TuskGame/Game.h>

#include <algorithm>
#include <cmath>
#include <vector>

namespace TuskGame {

    DemoGame::DemoGame() noexcept {};

    bool DemoGame::Start(Tusk::Engine& engine) 
    {
        auto& renderer = engine.GetRenderer();

        std::vector<Tusk::Vertex> vertices{
            Tusk::Vertex(1.f, 0.f, 0.f, 1.f, 0.f, 0.f),
            Tusk::Vertex(0.f, 1.f, 0.f, 0.f, 1.f, 0.f),
            Tusk::Vertex(-1.f, 0.f, 0.f, 0.f, 0.f, 1.f),
        };

        object = renderer.CreateVertexBuffer(vertices);

        return true;
    }

    void DemoGame::FixedUpdate(Tusk::Engine&, float seconds) 
    {

    }

    void DemoGame::Render(Tusk::Engine& engine) {
        auto& renderer = engine.GetRenderer();

        renderer.Clear(1, 165.f / 255.f, 0.f);

        renderer.Draw(object, 3);

        renderer.Present();

        //renderer.DrawRectangle(Color(255, 0, 0), 50, 50, 50, 50);*/
    }

    void DemoGame::Stop(Tusk::Engine&) {}

}
