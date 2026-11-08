#pragma once

#include <Tusk/GameObject.h>
#include <Tusk/Math.h>

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
		virtual void FixedUpdate(Input& input, float deltaFixedTime);
		void Render(Renderer& renderer);
		virtual void Stop();

		void SetStarted();
		bool HasStarted() const;
		Math::vec3 GetSceneColour();
	protected:
		void SetSceneColour(const Math::vec3& colour);
		std::vector<std::unique_ptr<GameObject>> objects;
	private:
		struct Impl;
		std::unique_ptr<Impl> impl;
	};
}