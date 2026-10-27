#include <Tusk/Asset.h>
#include <Tusk/Logging.h>
#include <Tusk/Graphics/Mesh.h>
#include <Tusk/Math.h>
#include <Tusk/Renderer.h>

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include <SDL3_image/SDL_image.h>

//https://learnopengl.com/Model-Loading/Model
namespace Tusk
{
	//CPU-side Texture Asset.
	struct TextureAsset
	{
		std::string name;
		SDL_Surface* surface;
	};

	std::vector<Mesh> Asset::meshes;
	std::vector<TextureAsset> Asset::textureAssetBin;

	void Asset::LoadModel(const char* path, std::string name)
	{
		//Assimp Importing

		Assimp::Importer importer; //Assimp importer interface.
		const aiScene* scene = importer.ReadFile(path, aiProcess_Triangulate | aiProcess_FlipUVs); //The entire imported model.

		if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode)
		{
			Logging::Log("Failed to load model with path: %s", importer.GetErrorString());
			return;
		}

		//Model Data Conversion

		auto& impVertices = scene->mMeshes[0]->mVertices; //Imported Vertices.
		auto vertexCount = scene->mMeshes[0]->mNumVertices;
		auto& impFaces = scene->mMeshes[0]->mFaces; //Imported Faces.
		auto faceCount = scene->mMeshes[0]->mNumFaces;

		std::vector<Math::vec3> vertexPosition;
		std::vector<uint16_t> indices;

		//Get each vertex position.
		for(int i = 0; i < vertexCount; i++)
		{
			vertexPosition.push_back(Math::vec3(impVertices[i].x, impVertices[i].y, impVertices[i].z));
		}

		//Get each index that creates a face.
		for(int i = 0; i < faceCount; i++)
		{
			indices.push_back(impFaces[i].mIndices[0]);
			indices.push_back(impFaces[i].mIndices[1]);
			indices.push_back(impFaces[i].mIndices[2]);
		}

		//Build Tusk:Vertex vertices.
		std::vector<Vertex> tuskVertices;

		//Create each vertex.
		for(int i = 0; i < vertexPosition.size(); i++)
		{
			tuskVertices.push_back(Vertex(
				vertexPosition[i],
				Math::vec4(1.f, 1.f, 0.f, 0.f)
			));
		}

		//Push the mesh into Asset meshes.
		meshes.push_back(
			Mesh{
				.name = name,
				.vertices = tuskVertices,
				.indices = indices,
			}
			);
	}

	void Asset::PushMeshesToRenderer(Renderer& renderer)
	{
		for(auto& mesh : meshes)
		{
			renderer.CreateGPUMeshBuffers(mesh.name, mesh.vertices, mesh.indices);
		}
	}

	void Asset::LoadImage(const char* path, std::string name)
	{
		auto* surface = IMG_Load(path);

		if(!surface)
		{
			Logging::Log("Failed to load image: %s", path);
		}
		
		textureAssetBin.push_back(
			TextureAsset{
				.name = name,
				.surface = surface,
			}
		);

		auto binSize = textureAssetBin.size();
		Logging::Log("Texture assets: %d", binSize);
	}
}