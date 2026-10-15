#pragma once

class GameObject;

namespace Tusk
{
	class ComponentBase
	{
	public:
		explicit ComponentBase(GameObject* creator) : parent(creator) {}
		virtual ~ComponentBase() = default;
	
		virtual void Start() = 0;
		virtual void FixedUpdate(float deltaTime) = 0;
	protected:
		GameObject* parent;
	};
}