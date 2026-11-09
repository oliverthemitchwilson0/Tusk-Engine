#pragma once

#include <Tusk/Math.h>
#include <Tusk/PhysicsTypes.h>

#include <memory>

namespace Tusk
{
	class Physics
	{
	public:
		Physics();
		~Physics();

		void FixedUpdate(float fixedDeltaTime);

		PhysBodyID CreateBody(const Math::vec3& position, const Math::quat& rotation);
		Math::vec3 GetBodyTranslation(const PhysBodyID& body);
	private:
		struct Impl;
		std::unique_ptr<Impl> impl;
	};
}