#pragma once

#include <Tusk/Math.h>

namespace Tusk
{
	//Struct containing relative data for creating camera view and perspective matrices.
	struct CameraData
	{
		Math::vec3 position, forward;
		float fovDegrees = 60.f, nearPlane = 0.1f, farPlane = 100.f;
	};
}