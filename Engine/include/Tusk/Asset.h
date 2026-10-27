#pragma once
#include <vector>
#include <string>

namespace Tusk
{
	class Renderer;

	struct Mesh;

	class Asset
	{
	public:
		Asset() = default;
		~Asset() = default;
		
		//Loads single mesh models.
		static void LoadModel(const char* path, std::string name);
		static void PushMeshesToRenderer(Renderer& renderer);
	private:
		static std::vector<Mesh> meshes;
	};
}