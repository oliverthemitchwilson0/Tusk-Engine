#pragma once
#include <Tusk/Vertex.h>

#include <vector>
#include <cstdint>

namespace Tusk
{
	struct Mesh
	{
		std::vector<Vertex> vertices;
		std::vector<uint16_t> indices;
	};
}