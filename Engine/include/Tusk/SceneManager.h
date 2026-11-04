#pragma once

namespace Tusk
{
	class Scene;

	class Input;
	class Renderer;

	class SceneManager
	{
	public:
		SceneManager() = default;
		~SceneManager() = default;

		void SetScene(Scene* scene);
		void FixedUpdate(Input& input, float fixedDeltaTime);
		void Render(Renderer& renderer);
		//void Stop();
	private:
		Scene* currentScene = nullptr;
	};
}