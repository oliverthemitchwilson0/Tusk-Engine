#include <TuskGame/SecondDemoScene.h>

#include <Tusk/Asset.h>
#include <Tusk/Logging.h>
#include <Tusk/Components/CameraComponent.h>
#include <Tusk/Components/MeshComponent.h>
#include <Tusk/Components/TextureComponent.h>
#include <Tusk/Components/TransformComponent.h>
#include <Tusk/Components/RigidBodyComponent.h>
#include <Tusk/Math.h>
#include <Tusk/Input.h>

namespace TuskGame
{
    bool SecondDemoScene::OnStart()
    {
        SetSceneColour(Tusk::Math::vec3(3.f / 255.f, 252.f / 255.f, 157.f / 255.f));

        Tusk::Asset::LoadModel("Assets/io_food_pine_big.obj", "Pineapple");
        
        Tusk::Asset::LoadImage("Assets/container.jpg", "Container");
        Tusk::Asset::LoadImage("Assets/prototype.png", "PrototypeTex");

        //Pineapple

        object = CreateGameObject();

        auto* objectTransformComp = object->GetComponent<Tusk::TransformComponent>();
        objectTransformComp->SetTranslation(Tusk::Math::vec3(0.1f, 10.f, 8.f));
        objectTransformComp->SetScale(Tusk::Math::vec3(1.5f, 1.5f, 1.5f));

        auto* objectMeshComp = object->AddComponent<Tusk::MeshComponent>();
        objectMeshComp->UseMesh("Pineapple");

        auto* objectTextureComp = object->AddComponent<Tusk::TextureComponent>();
        objectTextureComp->SetTexture("fruitTex");

        auto* objectRigidBodyComp = object->AddComponent<Tusk::RigidBodyComponent>();
        objectRigidBodyComp->SetTypeToDynamic();
        objectRigidBodyComp->SetShapeToSphere(0.75f);

        //Floor

        auto* object2 = CreateGameObject();

        auto* object2TransformComp = object2->GetComponent<Tusk::TransformComponent>();
        object2TransformComp->SetTranslation(Tusk::Math::vec3(0.f, -4.f, 25.f));
        object2TransformComp->SetScale(Tusk::Math::vec3(30.f, 1.f, 30.f));

        auto* object2MeshComp = object2->AddComponent<Tusk::MeshComponent>();
        object2MeshComp->UseMesh("Cube");

        auto* object2TextureComp = object2->AddComponent<Tusk::TextureComponent>();
        object2TextureComp->SetTexture("PrototypeTex");

        auto* object2RigidBodyComp = object2->AddComponent<Tusk::RigidBodyComponent>();
        object2RigidBodyComp->SetShapeToCube();

        //Box

        for(int i = 0; i < 10; i++)
        {
            float height = 4.f * (i + 1);

            auto* object3 = CreateGameObject();

            auto* object3TransformComp = object3->GetComponent<Tusk::TransformComponent>();
            object3TransformComp->SetTranslation(Tusk::Math::vec3(0.f, height, 25.f));
            //object3TransformComp->SetScale(Tusk::Math::vec3(10.f, 1.f, 10.f));

            auto* object3MeshComp = object3->AddComponent<Tusk::MeshComponent>();
            object3MeshComp->UseMesh("Cube");

            auto* object3TextureComp = object3->AddComponent<Tusk::TextureComponent>();
            object3TextureComp->SetTexture("Container");

            auto* object3RigidBodyComp = object3->AddComponent<Tusk::RigidBodyComponent>();
            object3RigidBodyComp->SetTypeToDynamic();
            object3RigidBodyComp->SetShapeToCube();
            object3RigidBodyComp->SetDensityModifier(0.01f);
        }

        return true;
    }

    void SecondDemoScene::OnFixedUpdate(Tusk::Input& input, float fixedDeltaTime)
    {
        auto* comp = object->GetComponent<Tusk::RigidBodyComponent>();
        //if(inpu)
        if(input.GetKeyDown(Tusk::W))
            GetPhysicsSystem()->AddVelocity(*comp, Tusk::Math::vec3(0.f, 0.f, 10.f) * fixedDeltaTime);
        else if (input.GetKeyDown(Tusk::S))
            GetPhysicsSystem()->AddVelocity(*comp, Tusk::Math::vec3(0.f, 0.f, -10.f) * fixedDeltaTime);

        if (input.GetKeyDown(Tusk::A))
            GetPhysicsSystem()->AddVelocity(*comp, Tusk::Math::vec3(10.f, 0.f, 0.f) * fixedDeltaTime);
        else if (input.GetKeyDown(Tusk::D))
            GetPhysicsSystem()->AddVelocity(*comp, Tusk::Math::vec3(-10.f, 0.f, 0.f) * fixedDeltaTime);
    }

    void SecondDemoScene::OnStop()
    {

    }
}