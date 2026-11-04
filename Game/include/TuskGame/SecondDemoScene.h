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

		bool Start() final;
		void FixedUpdate(Tusk::Input& input, float fixedDeltaTime) final;
		void Stop() final;
	private:
		/*Tusk::GameObject* object = nullptr;
		Tusk::GameObject* object2 = nullptr;
		Tusk::GameObject* camera = nullptr;*/
	};
}