#pragma once

#include <memory>

struct SDL_Window;

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
		//void Clear(std::uint8_t red, std::uint8_t green, std::uint8_t blue);
		//void DrawRectangle(Color color, float x, float y, float w, float h);
		//void RenderPresent();

	private:
		struct Impl;
		std::unique_ptr<Impl> impl;
	};
}