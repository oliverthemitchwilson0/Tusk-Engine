#include <Tusk/Components/CameraComponent.h>
#include <Tusk/GameObject.h>

namespace Tusk
{
	CameraData CameraComponent::GetCameraData()
	{
		Math::vec3 position(0.f, 0.f, 0.f);
		Math::vec3 forward(0.f, 0.f, -1.f);
		if (auto* transformComp = GetParentObject()->GetComponent<TransformComponent>())
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

	void CameraComponent::SetActive(bool activeIn)
	{
		active = activeIn;
	}

	bool CameraComponent::IsActive()
	{
		return active;
	};

	void CameraComponent::SetFov(float degreesIn)
	{
		fovDegrees = degreesIn;
	}
}