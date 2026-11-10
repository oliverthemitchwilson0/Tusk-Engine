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

		bool Start();
		void FixedUpdate(Input& input, float fixedDeltaTime);
		void Render(Renderer& renderer);
		void Stop();

		void SetStarted();
		bool HasStarted() const;
		Math::vec3 GetSceneColour();
		Physics* GetPhysicsSystem();

	protected:
		void SetSceneColour(const Math::vec3& colour);
		//Creates a new Game Object (with TransformComponent) within the scene.
		GameObject* CreateGameObject();

	private:
		virtual bool OnStart() = 0;
		virtual void OnFixedUpdate(Input& input, float fixedDeltaTime) = 0;
		virtual void OnStop() = 0;

		bool sceneStarted = false;
		Math::vec3 clearColour = Math::vec3(1, 165.f / 255.f, 0.f);
		std::vector<std::unique_ptr<GameObject>> objects;
		Physics physicsSystem;
	};
}