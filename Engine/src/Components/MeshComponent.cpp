#include <Tusk/Components/MeshComponent.h>
#include <Tusk/Components/TransformComponent.h>
#include <Tusk/Components/TextureComponent.h>
#include <Tusk/GameObject.h>
#include <Tusk/Renderer.h>

namespace Tusk
{
	void MeshComponent::Draw(Renderer& renderer)
	{
		if (!drawing)
			return;

		if (mesh.id == -1)
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

	void MeshComponent::UseMesh(std::string name)
	{
		meshName = name;
	}

	void MeshComponent::SetDrawing(bool draw)
	{
		drawing = draw; 
	}
}