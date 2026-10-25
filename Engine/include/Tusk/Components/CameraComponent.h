#pragma once

#include <Tusk/CameraData.h>

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
		void FixedUpdate(float deltaTime) final {};

		//Returns a CameraData Struct containing all relative data for supplying to the renderer.
		CameraData GetCameraData()
		{
			Math::vec3 position(0.f, 0.f, 0.f);
			Math::vec3 forward(0.f, 0.f, -1.f);
			if(auto* transformComp = parent->GetComponent<TransformComponent>())
			{
				position = transformComp->GetTranslation();
				forward = transformComp->GetForward();
			}
			
			return CameraData{
				.position = position,
				.forward = forward,
				.fovDegrees = fovDegrees,
				.nearPlane = nearPlane,
				.farPlane = farPlane,
			};
		}
	private:
		float fovDegrees = 90.f;
		float nearPlane = 0.1f;
		float farPlane = 100.f;
	};
}