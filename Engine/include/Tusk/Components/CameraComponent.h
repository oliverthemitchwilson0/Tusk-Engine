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
				.view = Tusk::Math::lookAt(position, position + forward, Math::vec3(0.f, 1.f, 0.f)),
				.fov = fov,
				.nearPlane = nearPlane,
				.farPlane = farPlane,
			};
		}
	private:
		float fov = 60.f;
		float nearPlane = 0.1f;
		float farPlane = 1000.f;
	};
}