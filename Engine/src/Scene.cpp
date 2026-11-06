#include <Tusk/Scene.h>
#include <Tusk/Input.h> //Maybe Scene shouldn't have direct knowledge of the 
#include <Tusk/Renderer.h> 
#include <Tusk/Logging.h>
#include <Tusk/Graphics/CameraData.h>
#include <Tusk/Components/CameraComponent.h>

namespace Tusk
{
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

    Math::vec3 Scene::GetSceneColour()
    {
        return clearColour;
    }

    void Scene::SetSceneColour(Math::vec3 colour)
    {
        clearColour = colour;
    }
}