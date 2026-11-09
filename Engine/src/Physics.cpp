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
		{
			Logging::Log("Stepping");
			b3World_Step(impl->PhysWorldID, fixedDeltaTime, 4);
		}
	}

	PhysBodyID Physics::CreateBody(const Math::vec3& position, const Math::quat& rotation)
	{
		b3BodyDef bodyDef = b3DefaultBodyDef();
	    bodyDef.position = b3Vec3{ position.x, position.y, position.z };
		bodyDef.rotation = b3Quat{ rotation.x, rotation.y, rotation.z, rotation.w };
		bodyDef.type = b3_dynamicBody;

		auto binID = impl->bodyBin.size();
	    impl->bodyBin.push_back(b3CreateBody(impl->PhysWorldID, &bodyDef));

		//temp

		b3BoxHull groundBox = b3MakeBoxHull(1.f, 1.f, 1.f);

		b3ShapeDef groundShapeDef = b3DefaultShapeDef();
		b3CreateHullShape(impl->bodyBin[binID], &groundShapeDef, &groundBox.base);

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
}