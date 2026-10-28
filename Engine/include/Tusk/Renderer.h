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

	struct TextureAsset;
	struct MeshID {	size_t id = -1; };

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
		void Draw(MeshID mesh, Math::mat4 transformMat);
		void Present();
		
		//GPU Resource Creation

		MeshID CreateGPUMeshBuffers(std::string name, std::span<Vertex> vertices, std::span<uint16_t> indices);
		void CreateGPUTexture(const TextureAsset& asset);

		MeshID GetGPUMesh(std::string name);
		void SetCameraData(const CameraData& data);

	private:
		struct Impl;
		std::unique_ptr<Impl> impl;

		bool CreatePipeline();
		void CreateTextureSampler();
		void PushCameraBuffer(const CameraBufferType& buff);
		void PushTransformBuffer(const TransformBufferType& buff);
	};
}