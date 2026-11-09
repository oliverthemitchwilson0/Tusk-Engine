#include <Tusk/Physics.h>
#include <Tusk/Math.h>

#include <box3d/box3d.h>
#include <thread>

namespace Tusk
{
	struct PhysBodyID
	{
		b3BodyId mID;
	};

	struct Physics::Impl
	{
		b3WorldId PhysWorldID;
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
		if(b3World_IsValid(impl->PhysWorldID))
			b3World_Step(impl->PhysWorldID, fixedDeltaTime, 4);
	}

	PhysBodyID Physics::CreateBody(Math::vec3 position, Math::quat rotation)
	{
		b3BodyDef bodyDef = b3DefaultBodyDef();
	    bodyDef.position = b3Vec3{ position.x, position.y, position.z };
		bodyDef.rotation = b3Quat{ rotation.x, rotation.y, rotation.z, rotation.w };

	    b3BodyId id = b3CreateBody(impl->PhysWorldID, &bodyDef);

		PhysBodyID id_{ .mID = id };
		return id_;
	}
}