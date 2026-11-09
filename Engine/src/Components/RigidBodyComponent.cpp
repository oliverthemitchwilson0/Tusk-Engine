#include <Tusk/Components/RigidBodyComponent.h>
#include <Tusk/Scene.h>
#include <Tusk/GameObject.h>
#include <Tusk/Physics.h>

namespace Tusk
{
	void RigidBodyComponent::Start()
	{
		auto* physicsSystem = GetScene()->GetPhysicsSystem();

		//b3BodyDef bodyDef = b3DefaultBodyDef();
		//groundBodyDef.position = b3Vec3{ 0.0f, 0.0f, 0.0f };
		//b3BodyId bodyID = b3CreateBody(physics.GetWorldID(), &groundBodyDef);
	}
}