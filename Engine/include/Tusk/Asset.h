#pragma once

namespace Tusk
{
	struct Mesh;

	class Asset
	{
	public:
		Asset() = default;
		~Asset() = default;
	
		static void LoadModel(const char* path, Mesh& mesh);
	};
}