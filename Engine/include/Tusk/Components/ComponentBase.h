#pragma once

namespace Tusk
{
	class GameObject;

	class ComponentBase
	{
	public:
		ComponentBase(GameObject* parentIn) : parent(parentIn) {}
		virtual ~ComponentBase() = default;
	
		virtual void Start() = 0;
		virtual void FixedUpdate(float deltaTime) = 0;
	protected:
		GameObject* parent;
	};
}