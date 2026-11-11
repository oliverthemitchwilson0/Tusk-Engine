#pragma once

#include <Tusk/Components/ComponentBase.h>
#include <Tusk/PhysicsTypes.h>

namespace Tusk
{
	class Input;

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
		void HandleInput(Input& input) final {};
		void FixedUpdate(float deltaTime) final;
		void Draw(Renderer& renderer) final {};

		void SetTypeToStatic();
		void SetTypeToDynamic();
		void SetShapeToCube();
		void SetShapeToSphere(float radius);
		void SetDensityModifier(float modifier);
	private:
		PhysBodyID body;
		BodyType type = StaticBody;
		PhysShapeProperties properties;
		int shape = -1;
		float radius = 0.f;
	};
}