#pragma once

namespace Tusk
{
	enum BodyType
	{
		//Ripped straight from b3d.

		/// zero mass, zero velocity, may be manually moved
		StaticBody = 0,

		/// zero mass, velocity set by user, moved by solver
		KinematicBody = 1,

		/// positive mass, velocity determined by forces, moved by solver
		DynamicBody = 2,
	};

	struct PhysBodyID
	{
		operator size_t() const { return id; }
		size_t id;
	};

	struct PhysShapeProperties
	{
		//Value that gets multiplied by the default density.
		float densityModifier = 1.f;
	};
}