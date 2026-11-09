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
    bool SecondDemoScene::Start()
    {
        SetSceneColour(Tusk::Math::vec3(3.f / 255.f, 252.f / 255.f, 157.f / 255.f));

        Tusk::Asset::LoadModel("Assets/io_food_pine_big.obj", "Pineapple");

        objects.push_back(std::make_unique<Tusk::GameObject>(this)); //Maybe this should be wrapped? Shouldn't expect the user to write the out everytime they need a new object.
        auto* object = objects.at(objects.size() - 1).get();

        auto* objectTransformComp = object->GetComponent<Tusk::TransformComponent>();
        objectTransformComp->SetTranslation(Tusk::Math::vec3(0.f, 0.f, 8.f));

        auto* objectMeshComp = object->AddComponent<Tusk::MeshComponent>();
        objectMeshComp->UseMesh("Pineapple");

        auto* objectTextureComp = object->AddComponent<Tusk::TextureComponent>();
        objectTextureComp->SetTexture("fruitTex");

        auto* objectRigidBodyComp = object->AddComponent<Tusk::RigidBodyComponent>();
        objectRigidBodyComp->Start();

        return true;
    }

    void SecondDemoScene::FixedUpdate(Tusk::Input& input, float fixedDeltaTime)
    {
        Scene::FixedUpdate(input, fixedDeltaTime);
        for (auto& object : objects)
        {
            object->FixedUpdate(fixedDeltaTime);
        }
    }

    void SecondDemoScene::Stop()
    {

    }
}