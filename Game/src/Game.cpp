#include <TuskGame/Game.h>

#include <algorithm>
#include <vector>

namespace TuskGame {

    DemoGame::DemoGame() noexcept {};

    bool DemoGame::Start(Tusk::Engine& engine) 
    {
        auto& renderer = engine.GetRenderer();

        std::vector<Tusk::Vertex> vertices{
            //Front Face
            Tusk::Vertex(Tusk::Math::vec3(-1.f, 1.f, 1.f), Tusk::Math::vec4(1.f, 0.f, 0.f, 1.f)), //Top Left
            Tusk::Vertex(Tusk::Math::vec3(-1.f, -1.f, 1.f), Tusk::Math::vec4(1.f, 0.f, 0.f, 1.f)), //Bottom Left
            Tusk::Vertex(Tusk::Math::vec3(1.f, -1.f, 1.f), Tusk::Math::vec4(1.f, 0.f, 0.f, 1.f)), //Bottom Right
            Tusk::Vertex(Tusk::Math::vec3(1.f, 1.f, 1.f), Tusk::Math::vec4(1.f, 0.f, 0.f, 1.f)), //Top Right

            //Back Face
            Tusk::Vertex(Tusk::Math::vec3(1.f, 1.f, -1.f), Tusk::Math::vec4(0.f, 1.f, 0.f, 1.f)), //Top Left
            Tusk::Vertex(Tusk::Math::vec3(1.f, -1.f, -1.f), Tusk::Math::vec4(0.f, 1.f, 0.f, 1.f)), //Bottom Left
            Tusk::Vertex(Tusk::Math::vec3(-1.f, -1.f, -1.f), Tusk::Math::vec4(0.f, 1.f, 0.f, 1.f)), //Bottom Right
            Tusk::Vertex(Tusk::Math::vec3(-1.f, 1.f, -1.f), Tusk::Math::vec4(0.f, 1.f, 0.f, 1.f)), //Top Right

            //Top Face
            Tusk::Vertex(Tusk::Math::vec3(-1.f, 1.f, -1.f), Tusk::Math::vec4(0.f, 0.f, 1.f, 1.f)), //Top Left
            Tusk::Vertex(Tusk::Math::vec3(-1.f, 1.f, 1.f), Tusk::Math::vec4(0.f, 0.f, 1.f, 1.f)), //Bottom Left
            Tusk::Vertex(Tusk::Math::vec3(1.f, 1.f, 1.f), Tusk::Math::vec4(0.f, 0.f, 1.f, 1.f)), //Bottom Right
            Tusk::Vertex(Tusk::Math::vec3(1.f, 1.f, -1.f), Tusk::Math::vec4(0.f, 0.f, 1.f, 1.f)), //Top Right

            //Bottom Face
            Tusk::Vertex(Tusk::Math::vec3(-1.f, -1.f, 1.f), Tusk::Math::vec4(1.f, 1.f, 0.f, 1.f)), //Top Left
            Tusk::Vertex(Tusk::Math::vec3(-1.f, -1.f, -1.f), Tusk::Math::vec4(1.f, 1.f, 0.f, 1.f)), //Bottom Left
            Tusk::Vertex(Tusk::Math::vec3(1.f, -1.f, -1.f), Tusk::Math::vec4(1.f, 1.f, 0.f, 1.f)), //Bottom Right
            Tusk::Vertex(Tusk::Math::vec3(1.f, -1.f, 1.f), Tusk::Math::vec4(1.f, 1.f, 0.f, 1.f)), //Top Right

            //Right Face
            Tusk::Vertex(Tusk::Math::vec3(1.f, 1.f, 1.f), Tusk::Math::vec4(1.f, 0.f, 1.f, 1.f)), //Top Left
            Tusk::Vertex(Tusk::Math::vec3(1.f, -1.f, 1.f), Tusk::Math::vec4(1.f, 0.f, 1.f, 1.f)), //Bottom Left
            Tusk::Vertex(Tusk::Math::vec3(1.f, -1.f, -1.f), Tusk::Math::vec4(1.f, 0.f, 1.f, 1.f)), //Bottom Right
            Tusk::Vertex(Tusk::Math::vec3(1.f, 1.f, -1.f), Tusk::Math::vec4(1.f, 0.f, 1.f, 1.f)), //Top Right

            //Left Face
            Tusk::Vertex(Tusk::Math::vec3(-1.f, 1.f, -1.f), Tusk::Math::vec4(0.f, 1.f, 1.f, 1.f)), //Top Left
            Tusk::Vertex(Tusk::Math::vec3(-1.f, -1.f, -1.f), Tusk::Math::vec4(0.f, 1.f, 1.f, 1.f)), //Bottom Left
            Tusk::Vertex(Tusk::Math::vec3(-1.f, -1.f, 1.f), Tusk::Math::vec4(0.f, 1.f, 1.f, 1.f)), //Bottom Right
            Tusk::Vertex(Tusk::Math::vec3(-1.f, 1.f, 1.f), Tusk::Math::vec4(0.f, 1.f, 1.f, 1.f)), //Top Right
        };

        std::vector<uint16_t> indices{
            0, 1, 2, 0, 2, 3,
            4, 5, 6, 4, 6, 7,
            8, 9, 10, 8, 10, 11,
            12, 13, 14, 12, 14, 15,
            16, 17, 18, 16, 18, 19,
            20, 21, 22, 20, 22, 23
        };

        object = renderer.CreateVertexBuffer(vertices);

        objectIndices = renderer.CreateIndexBuffer(indices);

        return true;
    }

    void DemoGame::FixedUpdate(Tusk::Engine&, float deltaTime) 
    {
        cubeAngle += glm::radians(90.f * deltaTime);
    }

    void DemoGame::Render(Tusk::Engine& engine) {
        auto& renderer = engine.GetRenderer();

        renderer.Clear(1, 165.f / 255.f, 0.f);

        Tusk::MatrixBuffer cubeMatBuffer
        {
            .world = glm::identity<Tusk::Math::mat4>() * glm::mat4_cast(glm::angleAxis(cubeAngle, Tusk::Math::vec3(0.f, -1.f, 0.f)) * glm::angleAxis(cubeAngle, Tusk::Math::vec3(-1.f, 0.f, 0.f))),
            .view = glm::lookAt(Tusk::Math::vec3(0.f, 0.f, 8.f), Tusk::Math::vec3(0.f, 0.f, 0.f), Tusk::Math::vec3(0.f, 1.f, 0.f)),
            .projection = glm::perspective(glm::radians(90.f), (1040.f / 880.f), 0.1f, 100.f),
        };

        renderer.Draw(object, objectIndices, 36, cubeMatBuffer);

        renderer.Present();

        //renderer.DrawRectangle(Color(255, 0, 0), 50, 50, 50, 50);*/
    }

    void DemoGame::Stop(Tusk::Engine&) {}

}
