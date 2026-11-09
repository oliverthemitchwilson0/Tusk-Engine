#pragma once

#include <Tusk/GameObject.h>
#include <Tusk/Math.h>
#include <Tusk/Physics.h>

#include <vector>
#include <memory>

namespace Tusk 
{
	class Input;
	class Renderer;

	class Scene
	{
	public:
		Scene();
		virtual ~Scene();

		virtual bool Start();
		virtual void FixedUpdate(Input& input, float fixedDeltaTime);
		void Render(Renderer& renderer);
		virtual void Stop();

		void SetStarted();
		bool HasStarted() const;
		Math::vec3 GetSceneColour();
		Physics* GetPhysicsSystem();
	protected:
		void SetSceneColour(const Math::vec3& colour);
		std::vector<std::unique_ptr<GameObject>> objects;
	private:
		bool sceneStarted = false;
		Math::vec3 clearColour = Math::vec3(1, 165.f / 255.f, 0.f);
		Physics physics;
	};
}