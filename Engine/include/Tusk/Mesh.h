#pragma once
#include <Tusk/Vertex.h>

#include <vector>
#include <cstdint>
#include <string>

namespace Tusk
{
	struct Mesh
	{
		std::string name;
		std::vector<Vertex> vertices;
		std::vector<uint16_t> indices;
	};
}