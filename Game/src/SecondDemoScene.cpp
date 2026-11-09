#include <TuskGame/SecondDemoScene.h>

#include <Tusk/Asset.h>
#include <Tusk/Logging.h>
#include <Tusk/Components/CameraComponent.h>
#include <Tusk/Components/MeshComponent.h>
#include <Tusk/Components/TextureComponent.h>
#include <Tusk/Components/TransformComponent.h>
#include <Tusk/Components/RigidBodyComponent.h>
#include <Tusk/Math.h>

namespace TuskGame
{
    bool SecondDemoScene::OnStart()
    {
        SetSceneColour(Tusk::Math::vec3(3.f / 255.f, 252.f / 255.f, 157.f / 255.f));

        Tusk::Asset::LoadModel("Assets/io_food_pine_big.obj", "Pineapple");

        objects.push_back(std::make_unique<Tusk::GameObject>(this)); //Maybe this should be wrapped? Shouldn't expect the user to write the out everytime they need a new object.
        auto* object = objects.at(objects.size() - 1).get();

        auto* objectTransformComp = object->GetComponent<Tusk::TransformComponent>();
        objectTransformComp->SetTranslation(Tusk::Math::vec3(0.1f, 10.f, 8.f));

        auto* objectMeshComp = object->AddComponent<Tusk::MeshComponent>();
        objectMeshComp->UseMesh("Pineapple");

        auto* objectTextureComp = object->AddComponent<Tusk::TextureComponent>();
        objectTextureComp->SetTexture("fruitTex");

        auto* objectRigidBodyComp = object->AddComponent<Tusk::RigidBodyComponent>();
        objectRigidBodyComp->SetTypeToDynamic();

        objects.push_back(std::make_unique<Tusk::GameObject>(this)); //Maybe this should be wrapped? Shouldn't expect the user to write the out everytime they need a new object.
        auto* object2 = objects.at(objects.size() - 1).get();

        auto* object2TransformComp = object2->GetComponent<Tusk::TransformComponent>();
        object2TransformComp->SetTranslation(Tusk::Math::vec3(0.f, -2.f, 8.f));

        auto* object2MeshComp = object2->AddComponent<Tusk::MeshComponent>();
        object2MeshComp->UseMesh("Pineapple");

        auto* object2TextureComp = object2->AddComponent<Tusk::TextureComponent>();
        object2TextureComp->SetTexture("fruitTex");

        auto* object2RigidBodyComp = object2->AddComponent<Tusk::RigidBodyComponent>();

        return true;
    }

    void SecondDemoScene::OnFixedUpdate(Tusk::Input& input, float fixedDeltaTime)
    {

    }

    void SecondDemoScene::OnStop()
    {

    }
}