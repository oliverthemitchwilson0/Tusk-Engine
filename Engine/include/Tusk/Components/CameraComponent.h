#pragma once

#include <Tusk/Components/TransformComponent.h>
#include <Tusk/Graphics/CameraData.h>

namespace Tusk
{
	class CameraComponent : public ComponentBase
	{
	public:
		CameraComponent(GameObject* parentIn) :
			ComponentBase(parentIn)
		{
		}
		~CameraComponent() final = default;

		void Start() final {};
		void HandleInput(Input& input) final {};
		void FixedUpdate(float deltaTime) final {};
		void Draw(Renderer& renderer) final {};

		//Returns a CameraData Struct containing all relative data for supplying to the renderer.
		CameraData GetCameraData();

		void SetActive(bool activeIn);
		bool IsActive();
		void SetFov(float degreesIn);

	private:
		float fovDegrees = 90.f;
		float nearPlane = 0.1f;
		float farPlane = 100.f;
		bool active = false;
	};
}