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

		PhysBodyID CreateBody(BodyType type, const Math::vec3& position, const Math::quat& rotation);
		void CreateBoxShape(const PhysBodyID& body, const PhysShapeProperties& properties, const Math::vec3& scale);
		void CreateSphereShape(const PhysBodyID& body, const PhysShapeProperties& properties, float radius);

		void AddVelocity(const PhysBodyID& body, const Math::vec3& vel);
		void AddImpulse(const PhysBodyID& body, const Math::vec3& imp);

		Math::vec3 GetBodyTranslation(const PhysBodyID& body);
		Math::quat GetBodyRotation(const PhysBodyID& body);
	private:
		struct Impl;
		std::unique_ptr<Impl> impl;
	};
}