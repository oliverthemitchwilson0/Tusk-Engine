#pragma once

#include <Tusk/Components/TextureComponent.h>
#include <Tusk/Renderer.h> //Probably shouldn't have full renderer access. Note: https://www.youtube.com/watch?v=hZ0TGCUcY2g - This has a good solution to interfacing with engine systenms @ 9:24 

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
		void HandleInput(Input& input) final {};
		void FixedUpdate(float deltaTime) final {};
		void Draw(Renderer& renderer) final
		{
			if (!drawing)
				return;

			if(mesh.id == -1)
			{
				mesh = renderer.GetGPUMesh(meshName);
			}

			Math::mat4 world = Math::identity<Math::mat4>();
			std::string textureName;
			if (auto* parent = GetParentObject())
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
