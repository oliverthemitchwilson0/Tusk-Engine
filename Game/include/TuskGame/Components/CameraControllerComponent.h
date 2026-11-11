#pragma once

#include <Tusk/Components/ComponentBase.h>

namespace TuskGame
{
	class CameraControllerComponent : public Tusk::ComponentBase
	{
	public:
		CameraControllerComponent(Tusk::GameObject* parentIn) :
			ComponentBase(parentIn)
		{
		}
		~CameraControllerComponent() final = default;

		void Start() final {};
		void HandleInput(Tusk::Input& input) final;
		void FixedUpdate(float fixedDeltaTime) final;
		void Draw(Tusk::Renderer& renderer) final {};

	private:
		bool moveForward = false, moveBackward = false, moveLeft = false, moveRight = false, moveUp = false, moveDown = false, rotate = false;
		float x = 0.f, y = 0.f, yaw = 0.f, pitch = 0.f;
	};
}