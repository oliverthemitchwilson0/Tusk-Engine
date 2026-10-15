#pragma once

namespace Tusk
{
	class MeshComponent : public ComponentBase
	{
	public:
		MeshComponent(GameObject* parentIn) : 
			ComponentBase(parentIn)
		{}
	private:
		size_t meshID;
	};
}
