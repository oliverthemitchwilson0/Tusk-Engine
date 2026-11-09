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

		//temp

		b3BoxHull groundBox = b3MakeBoxHull(.5f, .5f, .5f);

		b3Sphere sphere = { b3Vec3_zero, .5f };

		b3ShapeDef groundShapeDef = b3DefaultShapeDef();
		b3CreateSphereShape(impl->bodyBin[binID], &groundShapeDef, &sphere);

		Logging::Log("Body Created.");

		return PhysBodyID{
			.id = binID,
		};
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