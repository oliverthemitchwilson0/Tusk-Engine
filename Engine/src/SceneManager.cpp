#include <Tusk/SceneManager.h>
#include <Tusk/Scene.h>
#include <Tusk/Input.h>
#include <Tusk/Renderer.h>
#include <Tusk/Logging.h>

namespace Tusk
{
	void SceneManager::SetScene(Scene* scene)
	{
		currentScene = scene;

		if(!currentScene->HasStarted())
		{
			bool sceneStartResult = currentScene->Start();

			if(!sceneStartResult)
				Logging::Log("Scene failed to start.");
			else
				currentScene->SetStarted();
		}
	}

	void SceneManager::FixedUpdate(Input& input, float fixedDeltaTime)
	{
		currentScene->FixedUpdate(input, fixedDeltaTime);
	}

	void SceneManager::Render(Renderer& renderer)
	{
		currentScene->Render(renderer);
	}
}