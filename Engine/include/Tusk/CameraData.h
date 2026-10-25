#pragma once

#include <Tusk/Math.h>

namespace Tusk
{
	struct CameraData
	{
		Math::mat4 view;
		float fov, nearPlane, farPlane;
	};
}