#pragma once

namespace Tusk
{
	struct MeshID 
	{
		operator size_t() const { return id; }
		size_t id = -1; 
	};
}