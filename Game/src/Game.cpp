#include <TuskGame/Game.h>

#include <algorithm>
#include <vector>

namespace TuskGame {

    DemoGame::DemoGame() noexcept {};

    bool DemoGame::Start(Tusk::Engine& engine) 
    {
        auto& renderer = engine.GetRenderer();

        Tusk::Asset::LoadModel("Assets/io_food_banana_big.obj", "Banana");
        Tusk::Asset::LoadModel("Assets/io_food_pine_big.obj", "Pineapple");
        Tusk::Asset::LoadModel("Assets/d4c.gltf", "D4C");

        Tusk::Asset::LoadImage("Assets/d4c.png", "fruitTex");

        object = new Tusk::GameObject();

        auto* objectTransformComp = object->AddComponent<Tusk::TransformComponent>();
        objectTransformComp->SetTranslation(Tusk::Math::vec3(2.f, 0.f, 3.f));

       auto* objectMeshComp = object->AddComponent<Tusk::MeshComponent>();
       objectMeshComp->UseMesh("Banana");

        object2 = new Tusk::GameObject();
        auto* object2TransformComp = object2->AddComponent<Tusk::TransformComponent>();
        object2TransformComp->SetTranslation(Tusk::Math::vec3(-2.f, 0.f, 3.f));

        auto* object2MeshComp = object2->AddComponent<Tusk::MeshComponent>();
        object2MeshComp->UseMesh("D4C");

        camera = new Tusk::GameObject();
        camera->AddComponent<Tusk::CameraComponent>();
        auto* camTransformComp = camera->AddComponent<Tusk::TransformComponent>();

        return true;
    }

    void DemoGame::FixedUpdate(Tusk::Engine& engine, float deltaTime) 
    {
        auto& input = engine.GetInput();

        //Banana Test

        cubeAngle += Tusk::Math::radians(90.f * deltaTime);
        //accumulator += deltaTime;

        auto* transformComp = object->GetComponent<Tusk::TransformComponent>();
        transformComp->SetRotation(Tusk::Math::angleAxis(cubeAngle, Tusk::Math::vec3(0.f, -1.f, 0.f)) * Tusk::Math::angleAxis(cubeAngle, Tusk::Math::vec3(-1.f, 0.f, 0.f)));

       /* auto position = transformComp->GetTranslation();

        float value = Tusk::Math::sin(3.f * accumulator) * 4.f;
        position.y = value;
        transformComp->SetTranslation(position);*/

        //Camera Test

        auto* cameraTranasform = camera->GetComponent<Tusk::TransformComponent>();
        auto newCamTranslation = cameraTranasform->GetTranslation();
        auto cameraForward = cameraTranasform->GetForward();
        auto cameraRight = cameraTranasform->GetRight();
        auto cameraUp = cameraTranasform->GetUp();
        auto rotation = cameraTranasform->GetRotation();

        if(input.GetKeyDown(Tusk::W))
            newCamTranslation += cameraForward * 8.f * deltaTime;
        else if(input.GetKeyDown(Tusk::S))
            newCamTranslation -= cameraForward * 8.f * deltaTime;

        if(input.GetKeyDown(Tusk::D))
            newCamTranslation += cameraRight * 8.f * deltaTime;
        else if(input.GetKeyDown(Tusk::A))
            newCamTranslation -= cameraRight * 8.f * deltaTime;

        if (input.GetKeyDown(Tusk::SPACE))
            newCamTranslation += cameraUp * 8.f * deltaTime;
        else if (input.GetKeyDown(Tusk::LSHIFT))
            newCamTranslation -= cameraUp * 8.f * deltaTime;

        if (input.GetKeyDown(Tusk::X))
            cameraTranasform->SetRotation(rotation * Tusk::Math::angleAxis(glm::radians(90.f * deltaTime), Tusk::Math::vec3(0.f, 1.f, 0.f)));
        else if (input.GetKeyDown(Tusk::Z))
            cameraTranasform->SetRotation(rotation * Tusk::Math::angleAxis(glm::radians(90.f * deltaTime), Tusk::Math::vec3(0.f, -1.f, 0.f)));

        cameraTranasform->SetTranslation(newCamTranslation);
    }

    void DemoGame::Render(Tusk::Engine& engine) {
        auto& renderer = engine.GetRenderer();

        auto cameraData = camera->GetComponent<Tusk::CameraComponent>()->GetCameraData();
        renderer.SetCameraData(cameraData);

        renderer.Clear(1, 165.f / 255.f, 0.f);

        object->GetComponent<Tusk::MeshComponent>()->Draw(renderer);
        object2->GetComponent<Tusk::MeshComponent>()->Draw(renderer);

        renderer.Present();
    }

    void DemoGame::Stop(Tusk::Engine&) {}

}
