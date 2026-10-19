#pragma once

#include <Tusk/Math.h>

namespace Tusk
{
	class TransformComponent : public ComponentBase
	{
	public:
		TransformComponent(GameObject* parentIn) : 
			ComponentBase(parentIn),
			translation(0.f, 0.f, 0.f),
			rotation(glm::identity<Math::quat>()),
			scale(1.f, 1.f, 1.f)
		{}
		~TransformComponent() final = default;

		void Start() final {};
		void FixedUpdate(float deltaTime) final {};

		void SetTranslation(Math::vec3 newTranslation) { translation = newTranslation; }
		void SetRotation(Math::quat newRotation) { rotation = newRotation; }
		void SetScale(Math::vec3 newScale) { scale = newScale; }
		Math::vec3 GetTranslation() { return translation; }
		Math::quat GetRotation() { return rotation; }
		Math::vec3 GetScale() { return scale; }

		Math::mat4 GetTransfromMatrix() 
		{
			return glm::translate(glm::identity<Math::mat4>(), translation) * glm::mat4_cast(rotation) * glm::scale(glm::identity<Math::mat4>(), scale);
		}
	private:
		Math::vec3 translation;
		Math::quat rotation;
		Math::vec3 scale;
	};
}