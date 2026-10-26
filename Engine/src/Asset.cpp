#include <Tusk/Asset.h>
#include <Tusk/Logging.h>

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

namespace Tusk
{
	//https://learnopengl.com/Model-Loading/Model
	void Asset::LoadModel(const char* path)
	{
		Assimp::Importer importer; //Assimp importer interface.
		const aiScene* scene = importer.ReadFile(path, aiProcess_Triangulate | aiProcess_FlipUVs);

		if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode)
		{
			Logging::Log("Failed to load model with path: %s %s", path, importer.GetErrorString());
			return;
		}
	}

}