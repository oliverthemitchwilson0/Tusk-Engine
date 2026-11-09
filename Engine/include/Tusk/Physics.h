#pragma once

#include <Tusk/Math.h>

#include <memory>

namespace Tusk
{
	struct PhysBodyID;

	class Physics
	{
	public:
		Physics();
		~Physics();

		void FixedUpdate(float fixedDeltaTime);

		PhysBodyID CreateBody(Math::vec3 position, Math::quat rotation);
	private:
		struct Impl;
		std::unique_ptr<Impl> impl;
	};
}