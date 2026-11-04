#pragma once

#include <Tusk/GameObject.h>

namespace Tusk 
{
	class Input;
	class Renderer;

	class Scene
	{
	public:
		Scene() = default;
		virtual ~Scene() = default;

		virtual bool Start();
		virtual void FixedUpdate(Input& input, float deltaFixedTime);
		void Render(Renderer& renderer);
		virtual void Stop();

		void SetStarted() { sceneStarted = true; };
		bool HasStarted() { return sceneStarted; };
	protected:
		std::vector<std::unique_ptr<GameObject>> objects;
	private:
		bool sceneStarted = false;
	};
}