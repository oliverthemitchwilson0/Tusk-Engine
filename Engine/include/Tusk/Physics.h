#pragma once

#include <memory>

namespace Tusk
{
	class Physics
	{
	public:
		Physics();
		~Physics();

		void FixedUpdate(float fixedDeltaTime);

	private:
		struct Impl;
		std::unique_ptr<Impl> impl;
	};
}