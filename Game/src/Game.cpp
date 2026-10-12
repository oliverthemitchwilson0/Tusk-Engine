#include <TuskGame/Game.h>

#include <algorithm>
#include <vector>

namespace TuskGame {

    DemoGame::DemoGame() noexcept {};

    bool DemoGame::Start(Tusk::Engine& engine) 
    {
        auto& renderer = engine.GetRenderer();

        std::vector<Tusk::Vertex> vertices{
            Tusk::Vertex(Tusk::Math::vec3(-0.5f, 0.5f, 0.f), Tusk::Math::vec4(1.f, 0.f, 0.f, 1.f)), //Top Left
            Tusk::Vertex(Tusk::Math::vec3(-0.5f, -0.5f, 0.f), Tusk::Math::vec4(0.f, 1.f, 0.f, 1.f)), //Bottom Left
            Tusk::Vertex(Tusk::Math::vec3(0.5f, -0.5f, 0.f), Tusk::Math::vec4(0.f, 0.f, 1.f, 1.f)), //Bottom Right
            Tusk::Vertex(Tusk::Math::vec3(0.5f, 0.5f, 0.f), Tusk::Math::vec4(0.f, 0.f, 1.f, 1.f)), //Top Right
        };

        std::vector<uint16_t> indices{
            0, 1, 2, 0, 2, 3
        };

        object = renderer.CreateVertexBuffer(vertices);

        objectIndices = renderer.CreateIndexBuffer(indices);

        return true;
    }

    void DemoGame::FixedUpdate(Tusk::Engine&, float seconds) 
    {

    }

    void DemoGame::Render(Tusk::Engine& engine) {
        auto& renderer = engine.GetRenderer();

        renderer.Clear(1, 165.f / 255.f, 0.f);

        renderer.Draw(object, objectIndices, 6);

        renderer.Present();

        //renderer.DrawRectangle(Color(255, 0, 0), 50, 50, 50, 50);*/
    }

    void DemoGame::Stop(Tusk::Engine&) {}

}
