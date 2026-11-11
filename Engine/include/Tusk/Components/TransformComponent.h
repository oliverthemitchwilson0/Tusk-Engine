#pragma once

#include <Tusk/Components/ComponentBase.h>
#include <Tusk/Math.h>

namespace Tusk
{
	class TransformComponent : public ComponentBase
	{
	public:
		TransformComponent(GameObject* parentIn) : 
			ComponentBase(parentIn),
			translation(0.f, 0.f, 0.f),
			rotation(Math::identity<Math::quat>()),
			scale(1.f, 1.f, 1.f)
		{}
		~TransformComponent() final = default;

		void Start() final {};
		void HandleInput(Input& input) final {};
		void FixedUpdate(float deltaTime) final {};
		void Draw(Renderer& renderer) final {};

		void SetTranslation(const Math::vec3& newTranslation);
		void SetRotation(const Math::quat& newRotation);
		void SetScale(const Math::vec3& newScale);
		Math::vec3 GetTranslation();
		Math::quat GetRotation();
		Math::vec3 GetScale();

		//Returns a matrix of the transform.
		Math::mat4 GetTransfromMatrix();
		//Returns a normalised forward vector of the transformation.
		Math::vec3 GetForward();
		//Returns a normalised right vector of the transformation.
		Math::vec3 GetRight();
		//Returns a normalised up vector of the transformation.
		Math::vec3 GetUp();
	private:
		Math::vec3 translation;
		Math::quat rotation;
		Math::vec3 scale;
	};
}