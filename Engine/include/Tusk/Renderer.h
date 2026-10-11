#pragma once

#include <memory>
#include <span>
#include <Tusk/Vertex.h>


struct SDL_Window;
struct SDL_GPUBuffer;

namespace Tusk
{

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
		void Draw(SDL_GPUBuffer* vertexBuffer, int vertexCount);
		void Present();
		//void DrawRectangle(Color color, float x, float y, float w, float h);
		//void RenderPresent();
		SDL_GPUBuffer* CreateVertexBuffer(std::span<Vertex> vertices);

	private:
		struct Impl;
		std::unique_ptr<Impl> impl;

		bool CreatePipeline();
	};
}