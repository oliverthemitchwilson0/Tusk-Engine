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

		//Set queue scene ptr to desired scene for processing later.
		//queuedScene = sceneBin[found->second].get();
		SetScene(found->second);
	}

	void SceneManager::SetScene(size_t value)
	{
		if(value >= sceneBin.size())
		{
			Logging::Log("Scene value out of bounds: %d", value);
			return;
		}

		//Set queue scene ptr to desired scene for processing later.
		queuedScene = sceneBin[value].get();
	}

	void SceneManager::Process()
	{
		if(queuedScene)
		{
			currentScene = queuedScene;
			queuedScene = nullptr;
		}

		//Check if this is scene's first time running.
		if (currentScene && !currentScene->HasStarted())
		{
			bool sceneStartResult = currentScene->Start();

			if (!sceneStartResult)
				Logging::Log("Scene failed to start.");
			else
				currentScene->SetStarted();
		}
	}

	size_t SceneManager::GetSceneCount()
	{
		return sceneBin.size();
	}

	void SceneManager::FixedUpdate(Input& input, float fixedDeltaTime)
	{
		if(currentScene)
			currentScene->FixedUpdate(input, fixedDeltaTime);
	}

	void SceneManager::Render(Renderer& renderer)
	{
		if(currentScene)
			currentScene->Render(renderer);
	}
}