#include <Tusk/SceneManager.h>
#include <Tusk/Scene.h>
#include <Tusk/Input.h>
#include <Tusk/Renderer.h>
#include <Tusk/Logging.h>

namespace Tusk
{
	void SceneManager::SetScene(std::string sceneName)
	{
		//Find scene within scene bin.
		auto found = sceneLookup.find(sceneName);
		if(found == sceneLookup.end())
		{
			Logging::Log("Failed to find scene with name: %s", sceneName.c_str());
			return;
		}

		//Set current scene ptr to desired scene.
		currentScene = sceneBin[sceneLookup[sceneName]].get();

		//Check if this is scene's first time running.
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