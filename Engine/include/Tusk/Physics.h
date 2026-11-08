#pragma once

#include <memory>

class b3WorldId;

namespace Tusk
{
	class Physics
	{
	public:
		Physics();
		~Physics();

		void FixedUpdate(float fixedDeltaTime);
		const b3WorldId& GetWorldID();
	private:
		struct Impl;
		std::unique_ptr<Impl> impl;
	};
}