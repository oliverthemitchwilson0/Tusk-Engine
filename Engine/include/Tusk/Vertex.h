#pragma once

#include <Tusk/Math.h>

namespace Tusk
{
	struct Vertex
	{
		vec3 position;
		vec4 colour;
		Vertex(vec3 position, vec4 colour) : position(position), colour(colour) {};
	};
}