#pragma once

namespace Tusk
{
	class Asset
	{
	public:
		Asset() = default;
		~Asset() = default;
	
		static void LoadModel(const char* path);
	};
}