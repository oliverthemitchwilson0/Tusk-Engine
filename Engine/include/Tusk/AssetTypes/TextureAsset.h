#pragma once

#include <SDL3/SDL_surface.h>
#include <memory>

namespace Tusk
{
	struct SurfaceDeleter
	{
		void operator()(SDL_Surface* surface) { SDL_DestroySurface(surface); }
	};

	//CPU-side Texture Asset.
	struct TextureAsset
	{
		std::string name;
		std::unique_ptr<SDL_Surface, SurfaceDeleter> surface;
	};
}