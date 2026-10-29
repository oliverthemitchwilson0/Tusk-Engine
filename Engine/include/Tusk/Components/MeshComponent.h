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

			renderer.Draw(mesh, { .id = static_cast<size_t>(-1) }, world);
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
