#pragma once
#include <Tusk/Scene.h>

#include <Tusk/GameObject.h>

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
		Tusk::GameObject* object;
		Tusk::GameObject* object2;
		Tusk::GameObject* camera;

		float cubeAngle = 0.f;
	};
}