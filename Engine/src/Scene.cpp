#include <Tusk/Scene.h>
#include <Tusk/Input.h> //Maybe Scene shouldn't have direct knowledge of the input and rendering system.
#include <Tusk/Renderer.h> 
#include <Tusk/Logging.h>
#include <Tusk/Graphics/CameraData.h>
#include <Tusk/Components/CameraComponent.h>

#include <box3d/box3d.h>

namespace Tusk
{
    Scene::Scene() {}
    Scene::~Scene() {}

	bool Scene::Start()
	{
		return OnStart();
	}

	void Scene::FixedUpdate(Input& input, float fixedDeltaTime)
	{
        physics.FixedUpdate(fixedDeltaTime);
        
        //Update scene objects
        for (auto& object : objects)
        {
            object->FixedUpdate(fixedDeltaTime);
        }

        OnFixedUpdate(input, fixedDeltaTime);
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
        OnStop();
	}

    void Scene::SetStarted()
    {
        sceneStarted = true;
    }

    bool Scene::HasStarted() const
    {
        return sceneStarted;
    }

    Math::vec3 Scene::GetSceneColour()
    {
        return clearColour;
    }

    void Scene::SetSceneColour(const Math::vec3& colour)
    {
        clearColour = colour;
    }

    Physics* Scene::GetPhysicsSystem()
    {
        return &physics;
    }
}