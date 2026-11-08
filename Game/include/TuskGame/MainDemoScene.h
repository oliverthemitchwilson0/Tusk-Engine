#pragma once
#include <Tusk/Scene.h>
#include <Tusk/GameObject.h>

#include <box3d/box3d.h>

namespace TuskGame
{
	class MainDemoScene : public Tusk::Scene
	{
	public:
		MainDemoScene() = default;
		~MainDemoScene() = default;

		bool Start() final;
		void FixedUpdate(Tusk::Input& input, float fixedDeltaTime) final;
		void Stop() final;
	private:
		Tusk::GameObject* object = nullptr;
		Tusk::GameObject* object2 = nullptr;
		Tusk::GameObject* camera = nullptr;

		float bananaAngle = 0.f;

		b3BodyId bodyID;
	};
}