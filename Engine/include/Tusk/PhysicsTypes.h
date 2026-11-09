#pragma once

namespace Tusk
{
	struct PhysBodyID
	{
		operator size_t() const { return id; }
		size_t id;
	};
}