#pragma once
#include <Tusk/Scene.h>
#include <Tusk/GameObject.h>

namespace TuskGame
{
	class SecondDemoScene : public Tusk::Scene
	{
	public:
		SecondDemoScene() = default;
		~SecondDemoScene() = default;

	private:
		bool OnStart() final;
		void OnHandleInput(Tusk::Input& input) final;
		void OnFixedUpdate(float fixedDeltaTime) final;
		void OnStop() final;

		Tusk::GameObject* object = nullptr;

		bool forward = false, back = false, left = false, right = false, up = false;
	};
}