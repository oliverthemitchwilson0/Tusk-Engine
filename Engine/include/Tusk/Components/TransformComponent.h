#pragma once

#include <ComponentBase.h>
#include <Tusk/Math.h>

namespace Tusk
{
	class TransformComponent : public ComponentBase
	{
	public:
		TransformComponent() :
			translation(0.f, 0.f, 0.f),
			rotation(glm::identity<glm::quat>()),
			scale(1.f, 1.f, 1.f)
		{ }
		TransformComponent(Math::vec3 translationIn, Math::vec3 scaleIn) : 
			translation(translationIn), 
			rotation(glm::identity<glm::quat>()),
			scale(scaleIn)
		{ }

		//void Start() final;
		//void FixedUpdate(float deltaTime) final;
	private:
		Math::vec3 translation;
		Math::quat rotation;
		Math::vec3 scale;
	};
}