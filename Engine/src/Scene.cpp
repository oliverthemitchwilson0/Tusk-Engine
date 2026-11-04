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

        //Set the camera data in renderer.
        renderer.SetCameraData(activeCameraData);

        //Draw all objects (Only with meshes)
        for (auto& object : objects)
        {
            object->Draw(renderer);
        }
	}

	void Scene::Stop()
	{
		
	}
}