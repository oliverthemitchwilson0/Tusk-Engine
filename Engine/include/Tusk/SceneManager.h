#pragma once

#include <Tusk/Scene.h>

#include <vector>
#include <string>
#include <unordered_map>
#include <memory>

namespace Tusk
{
	//class Scene;

	class Input;
	class Renderer;

	class SceneManager
	{
	public:
		SceneManager() = default;
		~SceneManager() = default;

		//Set's the desired scene to be queued for switching.
		void SetScene(std::string sceneName);
		void SetScene(size_t value);

		//Processes the scene switching for any queued scene
		void Process();

		template<class T>
		void AddScene(std::string sceneName)
		{
			bool firstScene = sceneBin.empty();
			sceneLookup.emplace(sceneName, sceneBin.size());
			sceneBin.push_back(std::make_unique<T>());
			if (firstScene)
				SetScene(sceneName);
		}

		size_t GetSceneCount();

		void HandleInput(Input& input);
		void FixedUpdate(float fixedDeltaTime);
		void Render(Renderer& renderer);
		//void Stop();
	private:
		Scene* currentScene = nullptr;
		Scene* queuedScene = nullptr;
		std::vector<std::unique_ptr<Scene>> sceneBin;
		std::unordered_map<std::string, size_t> sceneLookup;
	};
}