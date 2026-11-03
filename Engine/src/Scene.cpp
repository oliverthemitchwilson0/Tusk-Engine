#include <Tusk/Scene.h>
#include <Tusk/Input.h>
#include <Tusk/Renderer.h>
#include <Tusk/Logging.h>

namespace Tusk
{
	bool Scene::Start()
	{
		Logging::Log("Scene Started");
		return true;
	}

	void Scene::FixedUpdate(Input& input, float deltaFixedTime)
	{
		Logging::Log("Scene Updating");
	}

	void Scene::Render(Renderer& renderer)
	{
		Logging::Log("Scene Renderering");
	}

	void Scene::Stop()
	{
		
	}
}