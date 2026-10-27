#pragma once

#include <Tusk/Math.h>

namespace Tusk
{
	//Struct containing relative data for creating camera view and perspective matrices.
	struct CameraData
	{
		Math::vec3 position, forward;
		float fovDegrees, nearPlane, farPlane;
	};
}