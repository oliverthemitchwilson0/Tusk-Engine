#pragma once

#include <Tusk/Math.h>

namespace Tusk
{
	struct Vertex
	{
		Math::vec3 position;
		Math::vec4 colour;
		Vertex(Math::vec3 position, Math::vec4 colour) : position(position), colour(colour) {};
	};
}