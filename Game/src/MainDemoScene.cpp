#include <TuskGame/MainDemoScene.h>

#include <Tusk/Asset.h>
#include <Tusk/Logging.h>
#include <Tusk/Components/CameraComponent.h>
#include <Tusk/Components/MeshComponent.h>
#include <Tusk/Components/TextureComponent.h>
#include <Tusk/Components/TransformComponent.h>
#include <Tusk/Math.h>

#include <Tusk/Input.h>
#include <Tusk/Renderer.h>

namespace TuskGame
{
	bool MainDemoScene::Start()
	{
		Tusk::Logging::Log("Main Demo Scene Start");

        Tusk::Asset::LoadModel("Assets/io_food_banana_big.obj", "Banana");
        Tusk::Asset::LoadModel("Assets/d4c.gltf", "D4C");

        Tusk::Asset::LoadImage("Assets/fruit_col_mt.png", "fruitTex");
        Tusk::Asset::LoadImage("Assets/d4c.png", "D4CTex");

        objects.push_back(std::make_unique<Tusk::GameObject>()); //Maybe this should be wrapped? Shouldn't expect the user to write the out everytime they need a new object.
        object = objects.at(objects.size() - 1).get();

        auto* objectTransformComp = object->AddComponent<Tusk::TransformComponent>();
        objectTransformComp->SetTranslation(Tusk::Math::vec3(2.f, 0.f, 3.f));

        auto* objectMeshComp = object->AddComponent<Tusk::MeshComponent>();
        objectMeshComp->UseMesh("Banana");

        auto* objectTextureComp = object->AddComponent<Tusk::TextureComponent>();
        objectTextureComp->SetTexture("fruitTex");

        objects.push_back(std::make_unique<Tusk::GameObject>());
        object2 = objects.at(objects.size() - 1).get();
        auto* object2TransformComp = object2->AddComponent<Tusk::TransformComponent>();
        object2TransformComp->SetTranslation(Tusk::Math::vec3(-2.f, 0.f, 3.f));
        object2TransformComp->SetRotation(Tusk::Math::angleAxis(Tusk::Math::radians(90.f), Tusk::Math::vec3(-1.f, 0.f, 0.f)));

        auto* object2MeshComp = object2->AddComponent<Tusk::MeshComponent>();
        object2MeshComp->UseMesh("D4C");

        auto* object2TextureComp = object2->AddComponent<Tusk::TextureComponent>();
        object2TextureComp->SetTexture("D4CTex");

        objects.push_back(std::make_unique<Tusk::GameObject>());
        camera = objects.at(objects.size() - 1).get();
        auto* camComponent = camera->AddComponent<Tusk::CameraComponent>();
        camComponent->SetActive(true);
        auto* camTransformComp = camera->AddComponent<Tusk::TransformComponent>();

        return true;
	}

	void MainDemoScene::FixedUpdate(Tusk::Input& input, float fixedDeltaTime)
	{
        //Banana Test

        bananaAngle += Tusk::Math::radians(90.f * fixedDeltaTime);

        auto* transformComp = object->GetComponent<Tusk::TransformComponent>();
        transformComp->SetRotation(Tusk::Math::angleAxis(bananaAngle, Tusk::Math::vec3(0.f, -1.f, 0.f)) * Tusk::Math::angleAxis(bananaAngle, Tusk::Math::vec3(-1.f, 0.f, 0.f)));


        //Camera Test

        auto* cameraTranasform = camera->GetComponent<Tusk::TransformComponent>();
        auto newCamTranslation = cameraTranasform->GetTranslation();
        auto cameraForward = cameraTranasform->GetForward();
        auto cameraRight = cameraTranasform->GetRight();
        auto cameraUp = cameraTranasform->GetUp();
        auto rotation = cameraTranasform->GetRotation();

        if (input.GetKeyDown(Tusk::W))
            newCamTranslation += cameraForward * 8.f * fixedDeltaTime;
        else if (input.GetKeyDown(Tusk::S))
            newCamTranslation -= cameraForward * 8.f * fixedDeltaTime;

        if (input.GetKeyDown(Tusk::D))
            newCamTranslation += cameraRight * 8.f * fixedDeltaTime;
        else if (input.GetKeyDown(Tusk::A))
            newCamTranslation -= cameraRight * 8.f * fixedDeltaTime;

        if (input.GetKeyDown(Tusk::SPACE))
            newCamTranslation += cameraUp * 8.f * fixedDeltaTime;
        else if (input.GetKeyDown(Tusk::LSHIFT))
            newCamTranslation -= cameraUp * 8.f * fixedDeltaTime;

        if (input.GetKeyDown(Tusk::X))
            cameraTranasform->SetRotation(rotation * Tusk::Math::angleAxis(glm::radians(90.f * fixedDeltaTime), Tusk::Math::vec3(0.f, 1.f, 0.f)));
        else if (input.GetKeyDown(Tusk::Z))
            cameraTranasform->SetRotation(rotation * Tusk::Math::angleAxis(glm::radians(90.f * fixedDeltaTime), Tusk::Math::vec3(0.f, -1.f, 0.f)));

        cameraTranasform->SetTranslation(newCamTranslation);
	}

	void MainDemoScene::Stop()
	{
		
	}
}