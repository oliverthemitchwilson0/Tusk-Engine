#include <Tusk/Components/TextureComponent.h>

namespace Tusk
{
	void TextureComponent::SetTexture(std::string name)
	{
		texName = name;
	}

	std::string TextureComponent::GetTexture() 
	{
		return texName;
	}
}