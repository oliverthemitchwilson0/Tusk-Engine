#pragma once
#include <Tusk/Graphics/Vertex.h>

#include <vector>
#include <cstdint>
#include <string>

namespace Tusk
{
	//CPU-side Mesh Asset.
	struct MeshAsset
	{
		std::string name;
		std::vector<Vertex> vertices;
		std::vector<uint16_t> indices;
	};
}