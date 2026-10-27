#pragma once

#include <SDL3/SDL_surface.h>

//CPU-side Texture Asset.
namespace Tusk
{
	struct TextureAsset
	{
		std::string name;
		SDL_Surface* surface = nullptr;
	};
}