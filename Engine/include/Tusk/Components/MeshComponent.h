#pragma once

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
			if (parent)
			{
				if (auto* transfromComp = parent->GetComponent<TransformComponent>())
					world = transfromComp->GetTransfromMatrix();
			}

			renderer.Draw(mesh, world);
		}

		void LoadMesh(Renderer& renderer, std::string name, std::span<Vertex> vertices, std::span<uint16_t> indices)
		{
			mesh = renderer.CreateGPUMeshBuffers(name, vertices, indices);
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
