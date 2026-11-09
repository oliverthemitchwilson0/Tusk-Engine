#include <Tusk/Components/RigidBodyComponent.h>
#include <Tusk/Scene.h>
#include <Tusk/GameObject.h>
#include <Tusk/Physics.h>
#include <Tusk/Logging.h>

namespace Tusk
{
	void RigidBodyComponent::Start()
	{
		auto* physicsSystem = GetScene()->GetPhysicsSystem();
		auto* transform = GetParentObject()->GetComponent<TransformComponent>();

		body = physicsSystem->CreateBody(transform->GetTranslation(), transform->GetRotation());
	}

	void RigidBodyComponent::FixedUpdate(float fixedDeltaTime)
	{
		auto* physicsSystem = GetScene()->GetPhysicsSystem();
		auto* objTransform = GetParentObject()->GetComponent<TransformComponent>();

		auto position = physicsSystem->GetBodyTranslation(body);
		//Logging::Log("Phys Position: %.1f, %.1f, %.1f", position.x, position.y, position.z);
		objTransform->SetTranslation(position);
	}
}