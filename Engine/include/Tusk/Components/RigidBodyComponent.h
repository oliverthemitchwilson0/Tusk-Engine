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

		operator PhysBodyID() const { return body; }

		void Start() final;
		void FixedUpdate(float deltaTime) final;
		void Draw(Renderer& renderer) final {};

		void SetTypeToStatic();
		void SetTypeToDynamic();
		void SetShapeToCube();
		void SetShapeToSphere(float radius);
	private:
		PhysBodyID body;
		BodyType type = StaticBody;
		int shape = -1;
		float radius = 0.f;
	};
}