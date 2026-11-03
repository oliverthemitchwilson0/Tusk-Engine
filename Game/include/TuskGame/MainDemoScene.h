#pragma once
#include <Tusk/Scene.h>

namespace TuskGame
{
	class MainDemoScene : public Tusk::Scene
	{
	public:
		MainDemoScene() = default;
		~MainDemoScene() = default;

		bool Start() final;
		void FixedUpdate(Tusk::Input& input, float fixedDeltaTime) final;
		void Render(Tusk::Renderer& renderer) final;
		void Stop() final;
	private:
		//
	};
}