#pragma once
#include <Tusk/Scene.h>
#include <Tusk/GameObject.h>

//https://en.wikipedia.org/wiki/Non-virtual_interface_pattern

namespace TuskGame
{
	class MainDemoScene : public Tusk::Scene
	{
	public:
		MainDemoScene() = default;
		~MainDemoScene() = default;

	private:
		bool OnStart() final;
		void OnHandleInput(Tusk::Input& input) final;
		void OnFixedUpdate(float fixedDeltaTime) final;
		void OnStop() final;


		Tusk::GameObject* object = nullptr;
		Tusk::GameObject* object2 = nullptr;
		Tusk::GameObject* camera = nullptr;

		float bananaAngle = 0.f;

		bool forward = false, back = false, left = false, right = false, up = false, down = false;
	};
}