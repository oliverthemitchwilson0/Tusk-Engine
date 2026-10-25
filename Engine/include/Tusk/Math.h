#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>

namespace Tusk::Math
{
	//Vectors.
	using glm::vec2;
	using glm::vec3;
	using glm::vec4;

	//Matrices.
	using glm::mat3;
	using glm::mat4;

	//Quaternions
	using glm::quat;

	//Functions
	using glm::sin;
	using glm::cos;
	using glm::tan;

	using glm::radians;
	using glm::degrees;

	using glm::normalize;
	using glm::identity;
	
	inline mat4 translate(vec3 translation)
	{
		return glm::translate(glm::identity<glm::mat4>(), translation);
	}

	inline mat4 scale(vec3 scale)
	{
		return glm::scale(glm::identity<glm::mat4>(), scale);
	}

	using glm::mat4_cast;
	using glm::angleAxis;

	using glm::lookAt;
	using glm::perspective;
}