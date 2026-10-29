#pragma once
#include <vector>
#include <string>

namespace Tusk
{
	class Renderer;

	struct MeshAsset;
	struct TextureAsset;

	class Asset
	{
	public:
		Asset() = default;
		~Asset() = default;
		
		//Loads single mesh models.
		static void LoadModel(const char* path, std::string name);
		static void PushAssetsToRenderer(Renderer& renderer);
		static void LoadImage(const char* path, std::string name);
	private:
		static std::vector<MeshAsset> meshAssetBin;
		static std::vector<TextureAsset> textureAssetBin;
	};
}