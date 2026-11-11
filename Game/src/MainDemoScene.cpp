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

#include <box3d/box3d.h>

namespace TuskGame
{
	bool MainDemoScene::OnStart()
	{
		Tusk::Logging::Log("Main Demo Scene Start");

        Tusk::Asset::LoadModel("Assets/io_food_banana_big.obj", "Banana");
        Tusk::Asset::LoadModel("Assets/d4c.gltf", "D4C");

        Tusk::Asset::LoadImage("Assets/fruit_col_mt.png", "fruitTex");
        Tusk::Asset::LoadImage("Assets/d4c.png", "D4CTex");

        object = CreateGameObject();

        auto* objectTransformComp = object->GetComponent<Tusk::TransformComponent>();
        objectTransformComp->SetTranslation(Tusk::Math::vec3(2.f, 0.f, 3.f));

        auto* objectMeshComp = object->AddComponent<Tusk::MeshComponent>();
        objectMeshComp->UseMesh("Banana");

        auto* objectTextureComp = object->AddComponent<Tusk::TextureComponent>();
        objectTextureComp->SetTexture("fruitTex");

        object2 = CreateGameObject();

        auto* object2TransformComp = object2->GetComponent<Tusk::TransformComponent>();
        object2TransformComp->SetTranslation(Tusk::Math::vec3(-2.f, 0.f, 3.f));
        object2TransformComp->SetRotation(Tusk::Math::angleAxis(Tusk::Math::radians(90.f), Tusk::Math::vec3(-1.f, 0.f, 0.f)) * Tusk::Math::angleAxis(Tusk::Math::radians(180.f), Tusk::Math::vec3(0.f, 0.f, -1.f)));

        auto* object2MeshComp = object2->AddComponent<Tusk::MeshComponent>();
        object2MeshComp->UseMesh("D4C");

        auto* object2TextureComp = object2->AddComponent<Tusk::TextureComponent>();
        object2TextureComp->SetTexture("D4CTex");

        camera = CreateGameObject();

        auto* camComponent = camera->AddComponent<Tusk::CameraComponent>();
        camComponent->SetActive(true);

        return true;
	}

    void MainDemoScene::OnHandleInput(Tusk::Input& input)
    {
        forward = back = right = left = up = down = false; //This is nasty

        if (input.GetKeyDown(Tusk::W))
            forward = true;
        else if (input.GetKeyDown(Tusk::S))
            back = true;

        if (input.GetKeyDown(Tusk::D))
            right = true;
        else if (input.GetKeyDown(Tusk::A))
            left = true;


        if (input.GetKeyDown(Tusk::SPACE))
            up = true;
        else if (input.GetKeyDown(Tusk::LSHIFT))
            down = true;

            /*if (input.GetMouseBtnDown(Tusk::LEFT))
                cameraTranasform->SetRotation(rotation * Tusk::Math::angleAxis(glm::radians(90.f * fixedDeltaTime), Tusk::Math::vec3(0.f, -1.f, 0.f)));
            else if (input.GetMouseBtnDown(Tusk::RIGHT))
                cameraTranasform->SetRotation(rotation * Tusk::Math::angleAxis(glm::radians(90.f * fixedDeltaTime), Tusk::Math::vec3(0.f, 1.f, 0.f)));*/
    }

	void MainDemoScene::OnFixedUpdate(float fixedDeltaTime)
	{
        //Banana Test

        bananaAngle += Tusk::Math::radians(90.f * fixedDeltaTime);

        auto* transformComp = object->GetComponent<Tusk::TransformComponent>();
        transformComp->SetRotation(Tusk::Math::angleAxis(bananaAngle, Tusk::Math::vec3(0.f, -1.f, 0.f)) * Tusk::Math::angleAxis(bananaAngle, Tusk::Math::vec3(-1.f, 0.f, 0.f)));


        //Camera Test
        {
            auto* cameraTranasform = camera->GetComponent<Tusk::TransformComponent>();
            auto newCamTranslation = cameraTranasform->GetTranslation();
            auto cameraForward = cameraTranasform->GetForward();
            auto cameraRight = cameraTranasform->GetRight();
            auto cameraUp = cameraTranasform->GetUp();
            auto rotation = cameraTranasform->GetRotation();

            if (forward)
                newCamTranslation += cameraForward * 8.f * fixedDeltaTime;
            else if (back)
                newCamTranslation -= cameraForward * 8.f * fixedDeltaTime;

            if (right)
                newCamTranslation += cameraRight * 8.f * fixedDeltaTime;
            else if (left)
                newCamTranslation -= cameraRight * 8.f * fixedDeltaTime;

            if (up)
                newCamTranslation += cameraUp * 8.f * fixedDeltaTime;
            else if (down)
                newCamTranslation -= cameraUp * 8.f * fixedDeltaTime;

            /*if (input.GetMouseBtnDown(Tusk::LEFT))
                cameraTranasform->SetRotation(rotation * Tusk::Math::angleAxis(glm::radians(90.f * fixedDeltaTime), Tusk::Math::vec3(0.f, -1.f, 0.f)));
            else if (input.GetMouseBtnDown(Tusk::RIGHT))
                cameraTranasform->SetRotation(rotation * Tusk::Math::angleAxis(glm::radians(90.f * fixedDeltaTime), Tusk::Math::vec3(0.f, 1.f, 0.f)));*/



            cameraTranasform->SetTranslation(newCamTranslation);
        }
	}

	void MainDemoScene::OnStop()
	{
		
	}
}