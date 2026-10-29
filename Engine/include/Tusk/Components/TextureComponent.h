#pragma once
#include <Tusk/Components/ComponentBase.h>

#include <string>

namespace Tusk
{
	class TextureComponent : public ComponentBase
	{
	public:
		TextureComponent(GameObject* parentIn) :
			ComponentBase(parentIn)
		{}
		~TextureComponent() final = default;
	private:
		std::string texName;
	};
}