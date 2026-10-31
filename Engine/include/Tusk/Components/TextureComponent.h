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

		void Start() final {};
		void FixedUpdate(float deltaTime) final {};

		void SetTexture(std::string name) { texName = name; }
		std::string GetTexture() { return texName; }
	private:
		std::string texName;
	};
}