#include <Tusk/Components/ComponentBase.h>
#include <Tusk/GameObject.h>

namespace Tusk
{
	GameObject* ComponentBase::GetParentObject()
	{
		return parent;
	}

	Scene* ComponentBase::GetScene()
	{
		return GetParentObject()->GetParentScene();
	}
}