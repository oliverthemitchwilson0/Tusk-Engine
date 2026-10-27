#pragma once
#include <Tusk/Math.h>
#include <Tusk/Vertex.h>

#include <memory>
#include <span>
#include <string>

struct SDL_Window;
struct SDL_GPUBuffer;

namespace Tusk
{
	struct MeshBufferData;
	struct CameraData;

	struct CameraBufferType;
	struct TransformBufferType;

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
		
		MeshID CreateGPUMeshBuffers(std::string name, std::span<Vertex> vertices, std::span<uint16_t> indices);
		MeshID GetGPUMesh(std::string name);
		void SetCameraData(const CameraData& data);
		//void PushCamera

	private:
		struct Impl;
		std::unique_ptr<Impl> impl;

		bool CreatePipeline();
		void PushCameraBuffer(const CameraBufferType& buff);
		void PushTransformBuffer(const TransformBufferType& buff);
	};
}