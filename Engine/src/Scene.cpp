#include <Tusk/Scene.h>
#include <Tusk/Input.h> //Maybe Scene shouldn't have direct knowledge of the input and rendering system.
#include <Tusk/Renderer.h> 
#include <Tusk/Logging.h>
#include <Tusk/Graphics/CameraData.h>
#include <Tusk/Components/CameraComponent.h>

#include <box3d/box3d.h>

namespace Tusk
{
    struct Scene::Impl
    {
        bool sceneStarted = false;
        Math::vec3 clearColour = Math::vec3(1, 165.f / 255.f, 0.f);

        b3WorldId physWorldID;
    };

    Scene::Scene() : impl(std::make_unique<Impl>()) {}
    Scene::~Scene() {}

	bool Scene::Start()
	{
		//Logging::Log("Scene Started");
		return true;
	}

	void Scene::FixedUpdate(Input& input, float deltaFixedTime)
	{
		//Logging::Log("Scene Updating");
	}

	void Scene::Render(Renderer& renderer)
	{
        //Find active camera.
        Tusk::CameraData activeCameraData;
        for (auto& object : objects)
        {
            if (auto* camera = object->GetComponent<Tusk::CameraComponent>())
            {
                if (camera->IsActive())
                {
                    activeCameraData = camera->GetCameraData();
                    break;
                }
            }
        }

        //Logging::Log("Camera Pos: %f, %f, %f", activeCameraData.position.x, activeCameraData.position.y, activeCameraData.position.z);
        //Logging::Log("Camera Dir: %f, %f, %f", activeCameraData.forward.x, activeCameraData.forward.y, activeCameraData.forward.z);

        //Set the camera data in renderer.
        renderer.SetCameraData(activeCameraData);
        renderer.SetClearColour(GetSceneColour());

        //Draw all objects (Only with meshes)
        for (auto& object : objects)
        {
            object->Draw(renderer);
        }
	}

	void Scene::Stop()
	{
		
	}

    void Scene::SetStarted()
    {
        impl->sceneStarted = true;
    }

    bool Scene::HasStarted() const
    {
        return impl->sceneStarted;
    }

    Math::vec3 Scene::GetSceneColour()
    {
        return impl->clearColour;
    }

    void Scene::SetSceneColour(const Math::vec3& colour)
    {
        impl->clearColour = colour;
    }
}