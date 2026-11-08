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
	bool MainDemoScene::Start()
	{
		Tusk::Logging::Log("Main Demo Scene Start");

        Tusk::Asset::LoadModel("Assets/io_food_banana_big.obj", "Banana");
        Tusk::Asset::LoadModel("Assets/d4c.gltf", "D4C");

        Tusk::Asset::LoadImage("Assets/fruit_col_mt.png", "fruitTex");
        Tusk::Asset::LoadImage("Assets/d4c.png", "D4CTex");

        objects.push_back(std::make_unique<Tusk::GameObject>()); //Maybe this should be wrapped? Shouldn't expect the user to write the out everytime they need a new object.
        object = objects.at(objects.size() - 1).get();

        auto* objectTransformComp = object->GetComponent<Tusk::TransformComponent>();
        objectTransformComp->SetTranslation(Tusk::Math::vec3(2.f, 0.f, 3.f));

        auto* objectMeshComp = object->AddComponent<Tusk::MeshComponent>();
        objectMeshComp->UseMesh("Banana");

        auto* objectTextureComp = object->AddComponent<Tusk::TextureComponent>();
        objectTextureComp->SetTexture("fruitTex");

        objects.push_back(std::make_unique<Tusk::GameObject>());
        object2 = objects.at(objects.size() - 1).get();

        auto* object2TransformComp = object2->GetComponent<Tusk::TransformComponent>();
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
        //auto* camTransformComp = camera->AddComponent<Tusk::TransformComponent>();

        //Physics test

       b3BodyDef groundBodyDef = b3DefaultBodyDef();
       groundBodyDef.position = b3Vec3{ 0.0f, 0.0f, 0.0f };
       b3BodyId groundId = b3CreateBody(physics.GetWorldID(), &groundBodyDef);

       b3BoxHull groundBox = b3MakeBoxHull(50.0f, 0.0f, 50.0f);

       b3ShapeDef groundShapeDef = b3DefaultShapeDef();
       b3CreateHullShape(groundId, &groundShapeDef, &groundBox.base);


       b3BodyDef bodyDef = b3DefaultBodyDef();
       bodyDef.type = b3_dynamicBody;
       bodyDef.position = b3Vec3{ -2.0f, 10.0f, 3.0f };
       bodyID = b3CreateBody(physics.GetWorldID(), &bodyDef);

       //b3DestroyWorld(worldId);

       b3BoxHull dynamicBox = b3MakeCubeHull(0.5f);

       b3ShapeDef shapeDef = b3DefaultShapeDef();
       shapeDef.density = 1.0f;
       shapeDef.baseMaterial.friction = 0.3f;

       b3CreateHullShape(bodyID, &shapeDef, &dynamicBox.base);

        return true;
	}

	void MainDemoScene::FixedUpdate(Tusk::Input& input, float fixedDeltaTime)
	{
        Scene::FixedUpdate(input, fixedDeltaTime);

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

            if (input.GetMouseBtnDown(Tusk::LEFT))
                cameraTranasform->SetRotation(rotation * Tusk::Math::angleAxis(glm::radians(90.f * fixedDeltaTime), Tusk::Math::vec3(0.f, -1.f, 0.f)));
            else if (input.GetMouseBtnDown(Tusk::RIGHT))
                cameraTranasform->SetRotation(rotation * Tusk::Math::angleAxis(glm::radians(90.f * fixedDeltaTime), Tusk::Math::vec3(0.f, 1.f, 0.f)));



            cameraTranasform->SetTranslation(newCamTranslation);
        }

        //Test
        auto d4cTransform = object2->GetComponent<Tusk::TransformComponent>();

        b3Vec3 position = b3Body_GetPosition(bodyID);

        d4cTransform->SetTranslation(Tusk::Math::vec3(position.x, position.y, position.z));

        b3Quat rotation = b3Body_GetRotation(bodyID);

        printf("%4.2f %4.2f %4.2f %4.2f %4.2f %4.2f %4.2f\n",
            position.x, position.y, position.z,
            rotation.v.x, rotation.v.y, rotation.v.z, rotation.s);
	}

	void MainDemoScene::Stop()
	{
		
	}
}