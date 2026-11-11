#pragma once

#include <Tusk/Components/ComponentBase.h>
#include <Tusk/Graphics/MeshID.h>

#include <string>

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
		void Draw(Renderer& renderer) final;

		void UseMesh(std::string name);
		void SetDrawing(bool draw);
	private:
		std::string meshName;
		MeshID mesh;
		bool drawing = true;
	};
}
