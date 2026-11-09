#pragma once

#include <Tusk/Components/ComponentBase.h>

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
		void Draw(Renderer& renderer) final;
	private:
		PhysBodyID ID;
	};
}