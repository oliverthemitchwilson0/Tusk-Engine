#include <Tusk/Components/TransformComponent.h>
#include <Tusk/Math.h>

namespace Tusk
{
	void TransformComponent::SetTranslation(const Math::vec3& newTranslation) { translation = newTranslation; }

	void TransformComponent::SetRotation(const Math::quat& newRotation) { rotation = newRotation; }

	void TransformComponent::SetScale(const Math::vec3& newScale) { scale = newScale; }

	Math::vec3 TransformComponent::GetTranslation() { return translation; }

	Math::quat TransformComponent::GetRotation() { return rotation; }

	Math::vec3 TransformComponent::GetScale() { return scale; }

	Math::mat4 TransformComponent::GetTransfromMatrix()
	{
		return Math::translate(translation) * Math::mat4_cast(rotation) * Math::scale(scale);
	}

	Math::vec3 TransformComponent::GetForward()
	{
		return Math::normalize(Math::vec3(0.f, 0.f, 1.f) * rotation);
	}

	Math::vec3 TransformComponent::GetRight()
	{
		return Math::normalize(Math::vec3(-1.f, 0.f, 0.f) * rotation);
	}

	Math::vec3 TransformComponent::GetUp()
	{
		return Math::normalize(Math::vec3(0.f, 1.f, 0.f) * rotation);
	}
}