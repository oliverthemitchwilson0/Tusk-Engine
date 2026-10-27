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
		void Render(Renderer& renderer)
		{
			if (!drawing)
				return;

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

		void UseMesh(Renderer& renderer, std::string name)
		{
			mesh = renderer.GetGPUMesh(name);
		}

		void SetDrawing(bool draw) { drawing = draw; }
	private:
		MeshID mesh;
		bool drawing = true;
	};
}
