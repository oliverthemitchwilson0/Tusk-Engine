#pragma once

#include <SDL3/SDL_surface.h>

namespace Tusk
{
	//CPU-side Texture Asset.
	struct TextureAsset
	{
		std::string name;
		SDL_Surface* surface = nullptr;
	};
}