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

        //Pineapple

        objects.push_back(std::make_unique<Tusk::GameObject>(this)); //Maybe this should be wrapped? Shouldn't expect the user to write the out everytime they need a new object.
        object = objects.at(objects.size() - 1).get();

        auto* objectTransformComp = object->GetComponent<Tusk::TransformComponent>();
        objectTransformComp->SetTranslation(Tusk::Math::vec3(0.1f, 10.f, 8.f));
        objectTransformComp->SetScale(Tusk::Math::vec3(2.f, 2.f, 2.f));

        auto* objectMeshComp = object->AddComponent<Tusk::MeshComponent>();
        objectMeshComp->UseMesh("Pineapple");

        auto* objectTextureComp = object->AddComponent<Tusk::TextureComponent>();
        objectTextureComp->SetTexture("fruitTex");

        auto* objectRigidBodyComp = object->AddComponent<Tusk::RigidBodyComponent>();
        objectRigidBodyComp->SetTypeToDynamic();
        objectRigidBodyComp->SetShapeToSphere(1.f);

        objects.push_back(std::make_unique<Tusk::GameObject>(this)); //Maybe this should be wrapped? Shouldn't expect the user to write the out everytime they need a new object.
        auto* object2 = objects.at(objects.size() - 1).get();

        auto* object2TransformComp = object2->GetComponent<Tusk::TransformComponent>();
        object2TransformComp->SetTranslation(Tusk::Math::vec3(0.f, -4.f, 15.f));
        object2TransformComp->SetScale(Tusk::Math::vec3(10.f, 1.f, 10.f));

        //Floor

        auto* object2MeshComp = object2->AddComponent<Tusk::MeshComponent>();
        object2MeshComp->UseMesh("Cube");

        auto* object2TextureComp = object2->AddComponent<Tusk::TextureComponent>();
        object2TextureComp->SetTexture("Container");

        auto* object2RigidBodyComp = object2->AddComponent<Tusk::RigidBodyComponent>();
        object2RigidBodyComp->SetShapeToCube();

        //Box

        for(int i = 0; i < 4; i++)
        {
            float height = 4.f * (i + 1);

            objects.push_back(std::make_unique<Tusk::GameObject>(this)); //Maybe this should be wrapped? Shouldn't expect the user to write the out everytime they need a new object.
            auto* object3 = objects.at(objects.size() - 1).get();

            auto* object3TransformComp = object3->GetComponent<Tusk::TransformComponent>();
            object3TransformComp->SetTranslation(Tusk::Math::vec3(0.f, height, 16.f));
            //object3TransformComp->SetScale(Tusk::Math::vec3(10.f, 1.f, 10.f));

            auto* object3MeshComp = object3->AddComponent<Tusk::MeshComponent>();
            object3MeshComp->UseMesh("Cube");

            auto* object3TextureComp = object3->AddComponent<Tusk::TextureComponent>();
            object3TextureComp->SetTexture("Container");

            auto* object3RigidBodyComp = object3->AddComponent<Tusk::RigidBodyComponent>();
            object3RigidBodyComp->SetTypeToDynamic();
            object3RigidBodyComp->SetShapeToCube();
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