#pragma once

#include <Vector>
#include <memory>
#include <unordered_map>

namespace Tusk
{



	class SceneBase;
	class Engine

	class SceneManager
	{
	public:
		SceneManager() = default;
		~SceneManager() = default;

		void FixedUpdate(Engine& engine, float deltaTime);
		void Render(Engine& engine);

		template<class T>
		void AddScene(std::string sceneName)
		{
			sceneLookup.emplace(sceneName, sceneBin.size());
			sceneBin.push_back(std::make_unique(T()));
		}

		void ActivateScene();

	private:
		std::vector<std::unique_ptr<SceneBase>> sceneBin;
		std::unordered_map<std::string, size_t> sceneLookup;

	};

}