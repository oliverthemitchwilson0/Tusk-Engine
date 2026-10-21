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

        object = new Tusk::GameObject();

        auto* transformComp = object->AddComponent<Tusk::TransformComponent>();
        transformComp->SetScale(Tusk::Math::vec3(1.f, 1.f, 1.f));

        auto* meshComp = object->AddComponent<Tusk::MeshComponent>();
        meshComp->LoadMesh(engine.GetRenderer(), vertices, indices);
        /*meshComp->SetDrawing(false);*/

        return true;
    }

    void DemoGame::FixedUpdate(Tusk::Engine& engine, float deltaTime) 
    {
        auto& input = engine.GetInput();

        cubeAngle += glm::radians(90.f * deltaTime);
        accumulator += deltaTime;

        /*if(accumulator >= 3)
        {
            auto* meshComp = object->GetComponent<Tusk::MeshComponent>();
            meshComp->SetDrawing(true);
        }*/

        auto* transformComp = object->GetComponent<Tusk::TransformComponent>();
        transformComp->SetRotation(glm::angleAxis(cubeAngle, glm::vec3(0.f, -1.f, 0.f)) * glm::angleAxis(cubeAngle, glm::vec3(-1.f, 0.f, 0.f)));

        auto position = transformComp->GetTranslation();
        if(input.GetKeyDown(Tusk::D))
        {
            position.x += 10.f * deltaTime;
        }
        if (input.GetKeyDown(Tusk::A))
        {
            position.x -= 10.f * deltaTime;
        }


        float value = glm::sin(3.f * accumulator) * 4.f;
        position.y = value;
        transformComp->SetTranslation(position);

    }

    void DemoGame::Render(Tusk::Engine& engine) {
        auto& renderer = engine.GetRenderer();

        renderer.Clear(1, 165.f / 255.f, 0.f);

        object->GetComponent<Tusk::MeshComponent>()->Render(renderer);

        renderer.Present();

        //renderer.DrawRectangle(Color(255, 0, 0), 50, 50, 50, 50);*/
    }

    void DemoGame::Stop(Tusk::Engine&) {}

}
