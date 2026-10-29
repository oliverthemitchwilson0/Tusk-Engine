#pragma once
#include <Tusk/Math.h>
#include <Tusk/Graphics/Vertex.h>

#include <memory>
#include <span>
#include <string>

struct SDL_Window;

namespace Tusk
{
	struct MeshBufferData;
	struct CameraData;

	struct CameraBufferType;
	struct TransformBufferType;

	struct MeshAsset;
	struct TextureAsset;

	struct MeshID {	size_t id = -1; };
	struct TextureID { size_t id = -1; };

	class Renderer
	{
	public:
		Renderer();
		~Renderer();

		Renderer(const Renderer&) = delete;
		Renderer& operator=(const Renderer&) = delete;
		Renderer(Renderer&&) = delete;
		Renderer& operator=(Renderer&&) = delete;

		bool Initilise(SDL_Window* window, bool debugActive = false);

		//Renderer Commands

		void Clear(float red, float green, float blue);
		void Draw(MeshID mesh, TextureID texture, Math::mat4 transformMat);
		void Present();
		
		//GPU Resource Creation

		MeshID CreateGPUMeshBuffers(const MeshAsset& asset);
		void CreateGPUTexture(const TextureAsset& asset);

		MeshID GetGPUMesh(const std::string& name);
		void SetCameraData(const CameraData& data);
		void ResizeDepthTexture();

	private:
		struct Impl;
		std::unique_ptr<Impl> impl;

		bool CreateDepthTexture();
		bool CreatePipeline();
		void CreateTextureSampler();
		void PushCameraBuffer(const CameraBufferType& buff);
		void PushTransformBuffer(const TransformBufferType& buff);
	};
}