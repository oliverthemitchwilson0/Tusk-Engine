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
		void OnFixedUpdate(Tusk::Input& input, float fixedDeltaTime) final;
		void OnStop() final;

		Tusk::GameObject* object = nullptr;
	};
}