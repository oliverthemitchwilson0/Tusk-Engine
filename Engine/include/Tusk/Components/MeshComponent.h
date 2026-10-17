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

			Math::mat4 world;
			if (parent)
			{
				if (auto* transfromComp = parent->GetComponent<TransformComponent>())
					world = transfromComp->GetTransfromMatrix();
			}
			else
				world = glm::identity<Math::mat4>();

			renderer.Draw(mesh, world);
		}

		void LoadMesh(Renderer& renderer, std::span<Vertex> vertices, std::span<uint16_t> indices)
		{
			mesh = renderer.CreateMeshBuffers(vertices, indices);
		}

		void SetDrawing(bool draw) { drawing = draw; }
	private:
		MeshID mesh;
		bool drawing = true;
	};
}
