#pragma once

#include <Tusk/GameObject.h>
#include <Tusk/Math.h>

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
		Math::vec3 GetSceneColour();
	protected:
		void SetSceneColour(Math::vec3 colour);
		std::vector<std::unique_ptr<GameObject>> objects;
	private:
		bool sceneStarted = false;
		Math::vec3 clearColour = Math::vec3(1, 165.f / 255.f, 0.f);
	};
}