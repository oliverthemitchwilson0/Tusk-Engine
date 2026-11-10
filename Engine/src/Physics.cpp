#include <Tusk/Physics.h>
#include <Tusk/Math.h>
#include <Tusk/Logging.h>

#include <box3d/box3d.h>
#include <thread>

namespace Tusk
{
	struct Physics::Impl
	{
		b3WorldId PhysWorldID;
		std::vector<b3BodyId> bodyBin; //What a name.
	};

	Physics::Physics() : impl(std::make_unique<Impl>())
	{
		b3WorldDef worldDef = b3DefaultWorldDef();
		worldDef.gravity = b3Vec3{ 0.0f, -10.0f, 0.0f };
		worldDef.workerCount = std::thread::hardware_concurrency() - 1u;
		impl->PhysWorldID = b3CreateWorld(&worldDef);
	}

	Physics::~Physics()
	{
		if(b3World_IsValid(impl->PhysWorldID))
			b3DestroyWorld(impl->PhysWorldID);
	}

	void Physics::FixedUpdate(float fixedDeltaTime)
	{
		if (b3World_IsValid(impl->PhysWorldID) && !impl->bodyBin.empty())
			b3World_Step(impl->PhysWorldID, fixedDeltaTime, 4);
	}

	PhysBodyID Physics::CreateBody(BodyType type, const Math::vec3& position, const Math::quat& rotation)
	{
		b3BodyDef bodyDef = b3DefaultBodyDef();
	    bodyDef.position = b3Vec3{ position.x, position.y, position.z };
		bodyDef.rotation = b3Quat{ rotation.x, rotation.y, rotation.z, rotation.w };

		switch(type)
		{
			case(StaticBody):
				bodyDef.type = b3_staticBody;
				break;
			case(KinematicBody):
				bodyDef.type = b3_kinematicBody;
				break;
			case(DynamicBody):
				bodyDef.type = b3_dynamicBody;
				break;
		}

		auto binID = impl->bodyBin.size();
	    impl->bodyBin.push_back(b3CreateBody(impl->PhysWorldID, &bodyDef));

		return PhysBodyID{
			.id = binID,
		};
	}

	void Physics::CreateBoxShape(const PhysBodyID& body, const PhysShapeProperties& properties, const Math::vec3& scale)
	{
		b3BoxHull box = b3MakeBoxHull(scale.x, scale.y, scale.z);
		b3ShapeDef shapeDef = b3DefaultShapeDef();
		shapeDef.density *= properties.densityModifier;
		b3CreateHullShape(impl->bodyBin[body], &shapeDef, &box.base);
	}

	void Physics::CreateSphereShape(const PhysBodyID& body, const PhysShapeProperties& properties, float radius)
	{
		b3Sphere sphere{
			.center = b3Vec3_zero,
			.radius = radius,
		};
		b3ShapeDef shapeDef = b3DefaultShapeDef();
		shapeDef.density *= properties.densityModifier;
		b3CreateSphereShape(impl->bodyBin[body], &shapeDef, &sphere);
	}

	void Physics::AddVelocity(const PhysBodyID& body, const Math::vec3& vel)
	{
		auto& b3body = impl->bodyBin[body];
		auto currentVel = b3Body_GetLinearVelocity(b3body);
		
		b3Vec3 newVelocity{
			currentVel.x + vel.x,
			currentVel.y + vel.y,
			currentVel.z + vel.z,
		};

		b3Body_SetLinearVelocity(b3body, newVelocity);
	}

	void Physics::AddImpulse(const PhysBodyID& body, const Math::vec3& imp)
	{
		auto& b3body = impl->bodyBin[body];

		b3Vec3 newImpulse{
			imp.x,
			imp.y,
			imp.z,
		};

		b3Body_SetLinearVelocity(b3body, newImpulse);
	}

	Math::vec3 Physics::GetBodyTranslation(const PhysBodyID& body)
	{
		auto& b3ID = impl->bodyBin[body];
		auto position = b3Body_GetPosition(b3ID);
		return Math::vec3(position.x, position.y, position.z);
	}

	Math::quat Physics::GetBodyRotation(const PhysBodyID& body)
	{
		auto& b3ID = impl->bodyBin[body];
		auto rotation = b3Body_GetRotation(b3ID);
		return Math::quat(rotation.s, rotation.v.x, rotation.v.y, rotation.v.z);
	}
}