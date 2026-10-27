#pragma once
#include <vector>
#include <string>

namespace Tusk
{
	class Renderer;

	struct Mesh;
	struct TextureAsset;

	class Asset
	{
	public:
		Asset() = default;
		~Asset() = default;
		
		//Loads single mesh models.
		static void LoadModel(const char* path, std::string name);
		static void PushMeshesToRenderer(Renderer& renderer);
		static void LoadImage(const char* path, std::string name);
	private:
		static std::vector<Mesh> meshes;
		static std::vector<TextureAsset> textureAssetBin;
	};
}