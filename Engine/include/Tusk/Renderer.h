#pragma once

#include <memory>
#include <span>
#include <Tusk/Vertex.h>

struct SDL_Window;
struct SDL_GPUBuffer;

namespace Tusk
{
	struct MatrixBuffer
	{
		Math::mat4 world;
		Math::mat4 view;
		Math::mat4 projection;
	};

	struct MeshBufferData;

	struct MeshID
	{
		size_t id = -1;
	};

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
		//bool IsActive();

		//Renderer Commands
		void Clear(float red, float green, float blue);
		void Draw(MeshID mesh, Math::mat4 transformMat);
		void Present();
		//void DrawRectangle(Color color, float x, float y, float w, float h);
		//void RenderPresent();
		MeshID CreateMeshBuffers(std::span<Vertex> vertices, std::span<uint16_t> indices);

	private:
		struct Impl;
		std::unique_ptr<Impl> impl;

		bool CreatePipeline();
	};
}