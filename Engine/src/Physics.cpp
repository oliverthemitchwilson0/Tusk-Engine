#include <Tusk/Physics.h>

#include <box3d/box3d.h>
#include <thread>

namespace Tusk
{
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

	const b3WorldId& Physics::GetWorldID()
	{
		return impl->PhysWorldID;
	}
}