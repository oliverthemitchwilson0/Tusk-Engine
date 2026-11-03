#pragma once

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
		virtual void Render(Renderer& renderer);
		virtual void Stop();
	private:
		//
	};
}