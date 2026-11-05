#include <TuskGame/SecondDemoScene.h>

#include <Tusk/Asset.h>
#include <Tusk/Logging.h>
#include <Tusk/Components/CameraComponent.h>
#include <Tusk/Components/MeshComponent.h>
#include <Tusk/Components/TextureComponent.h>
#include <Tusk/Components/TransformComponent.h>
#include <Tusk/Math.h>

namespace TuskGame
{
    bool SecondDemoScene::Start()
    {
        objects.push_back(std::make_unique<Tusk::GameObject>()); //Maybe this should be wrapped? Shouldn't expect the user to write the out everytime they need a new object.
        auto* object = objects.at(objects.size() - 1).get();

        auto* objectTransformComp = object->AddComponent<Tusk::TransformComponent>();
        objectTransformComp->SetTranslation(Tusk::Math::vec3(0.f, 0.f, 4.f));

        auto* objectMeshComp = object->AddComponent<Tusk::MeshComponent>();
        objectMeshComp->UseMesh("Pineapple");

        auto* objectTextureComp = object->AddComponent<Tusk::TextureComponent>();
        objectTextureComp->SetTexture("fruitTex");

        return true;
    }

    void SecondDemoScene::FixedUpdate(Tusk::Input& input, float fixedDeltaTime)
    {

    }

    void SecondDemoScene::Stop()
    {

    }
}