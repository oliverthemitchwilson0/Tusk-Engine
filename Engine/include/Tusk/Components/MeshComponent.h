#pragma once

#include <Tusk/Components/TextureComponent.h>
#include <Tusk/Renderer.h> //Probably shouldn't have full renderer access.

namespace Tusk
{
	class MeshComponent : public ComponentBase
	{
	public:
		MeshComponent(GameObject* parentIn) :
			ComponentBase(parentIn)
		{}
		~MeshComponent() final = default;

		void Start() final {};
		void FixedUpdate(float deltaTime) final {};
		void Draw(Renderer& renderer)
		{
			if (!drawing)
				return;

			if(mesh.id == -1)
			{
				mesh = renderer.GetGPUMesh(meshName);
			}

			Math::mat4 world = Math::identity<Math::mat4>();
			std::string textureName;
			if (parent)
			{
				if (auto* transfromComp = parent->GetComponent<TransformComponent>())
					world = transfromComp->GetTransfromMatrix();

				if (auto* textureComp = parent->GetComponent<TextureComponent>())
					textureName = textureComp->GetTexture();
			}

			renderer.Draw(mesh, textureName, world);
		}

		void UseMesh(std::string name)
		{
			meshName = name;
		}

		void SetDrawing(bool draw) { drawing = draw; }
	private:
		std::string meshName;
		MeshID mesh;
		bool drawing = true;
	};
}
