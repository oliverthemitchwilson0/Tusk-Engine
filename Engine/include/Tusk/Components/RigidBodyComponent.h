#pragma once

#include <Tusk/Components/ComponentBase.h>
#include <Tusk/PhysicsTypes.h>

namespace Tusk
{
	class RigidBodyComponent : public ComponentBase
	{
	public:
		RigidBodyComponent(GameObject* parentIn) :
			ComponentBase(parentIn)
		{
		}
		~RigidBodyComponent() final = default;

		void Start() final;
		void FixedUpdate(float deltaTime) final;
		void Draw(Renderer& renderer) final {};

		void SetTypeToStatic();
		void SetTypeToDynamic();
	private:
		PhysBodyID body;
		BodyType type = StaticBody;
	};
}