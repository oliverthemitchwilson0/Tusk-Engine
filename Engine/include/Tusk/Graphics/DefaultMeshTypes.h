#pragma once
#include <Tusk/Graphics/Vertex.h>
#include <Tusk/Graphics/Index.h>
#include <Tusk/Math.h>

namespace Tusk
{
	struct CubeMesh
	{
        Vertex vertices[24] = {
            //Front Face
            Vertex(Math::vec3(-1.f, 1.f, 1.f),  Math::vec2(0.f, 0.f), Tusk::Math::vec4(0.f, 0.f, 0.f, 1.f)), //Top Left
            Vertex(Math::vec3(-1.f, -1.f, 1.f), Math::vec2(0.f, 1.f), Tusk::Math::vec4(0.f, 0.f, 0.f, 1.f)), //Bottom Left
            Vertex(Math::vec3(1.f, -1.f, 1.f),  Math::vec2(1.f, 1.f), Tusk::Math::vec4(0.f, 0.f, 0.f, 1.f)), //Bottom Right
            Vertex(Math::vec3(1.f, 1.f, 1.f),   Math::vec2(1.f, 0.f), Tusk::Math::vec4(0.f, 0.f, 0.f, 1.f)), //Top Right

            //Back Face
            Vertex(Math::vec3(1.f, 1.f, -1.f),   Math::vec2(0.f, 0.f), Tusk::Math::vec4(0.f, 0.f, 0.f, 1.f)), //Top Left
            Vertex(Math::vec3(1.f, -1.f, -1.f),  Math::vec2(0.f, 1.f), Tusk::Math::vec4(0.f, 0.f, 0.f, 1.f)), //Bottom Left
            Vertex(Math::vec3(-1.f, -1.f, -1.f), Math::vec2(1.f, 1.f), Tusk::Math::vec4(0.f, 0.f, 0.f, 1.f)), //Bottom Right
            Vertex(Math::vec3(-1.f, 1.f, -1.f),  Math::vec2(1.f, 0.f), Tusk::Math::vec4(0.f, 0.f, 0.f, 1.f)), //Top Right

            //Top Face
            Vertex(Math::vec3(-1.f, 1.f, -1.f), Math::vec2(0.f, 0.f), Tusk::Math::vec4(0.f, 0.f, 0.f, 1.f)), //Top Left
            Vertex(Math::vec3(-1.f, 1.f, 1.f),  Math::vec2(0.f, 1.f), Tusk::Math::vec4(0.f, 0.f, 0.f, 1.f)), //Bottom Left
            Vertex(Math::vec3(1.f, 1.f, 1.f),   Math::vec2(1.f, 1.f), Tusk::Math::vec4(0.f, 0.f, 0.f, 1.f)), //Bottom Right
            Vertex(Math::vec3(1.f, 1.f, -1.f),  Math::vec2(1.f, 0.f), Tusk::Math::vec4(0.f, 0.f, 0.f, 1.f)), //Top Right

            //Bottom Face
            Vertex(Math::vec3(-1.f, -1.f, 1.f),  Math::vec2(0.f, 0.f), Tusk::Math::vec4(0.f, 0.f, 0.f, 1.f)), //Top Left
            Vertex(Math::vec3(-1.f, -1.f, -1.f), Math::vec2(0.f, 1.f), Tusk::Math::vec4(0.f, 0.f, 0.f, 1.f)), //Bottom Left
            Vertex(Math::vec3(1.f, -1.f, -1.f),  Math::vec2(1.f, 1.f), Tusk::Math::vec4(0.f, 0.f, 0.f, 1.f)), //Bottom Right
            Vertex(Math::vec3(1.f, -1.f, 1.f),   Math::vec2(1.f, 0.f), Tusk::Math::vec4(0.f, 0.f, 0.f, 1.f)), //Top Right

            //Right Face
            Vertex(Math::vec3(1.f, 1.f, 1.f),   Math::vec2(0.f, 0.f), Tusk::Math::vec4(0.f, 0.f, 0.f, 1.f)), //Top Left
            Vertex(Math::vec3(1.f, -1.f, 1.f),  Math::vec2(0.f, 1.f), Tusk::Math::vec4(0.f, 0.f, 0.f, 1.f)), //Bottom Left
            Vertex(Math::vec3(1.f, -1.f, -1.f), Math::vec2(1.f, 1.f), Tusk::Math::vec4(0.f, 0.f, 0.f, 1.f)), //Bottom Right
            Vertex(Math::vec3(1.f, 1.f, -1.f),  Math::vec2(1.f, 0.f), Tusk::Math::vec4(0.f, 0.f, 0.f, 1.f)), //Top Right

            //Left Face
            Vertex(Math::vec3(-1.f, 1.f, -1.f),  Math::vec2(0.f, 0.f), Tusk::Math::vec4(0.f, 0.f, 0.f, 1.f)), //Top Left
            Vertex(Math::vec3(-1.f, -1.f, -1.f), Math::vec2(0.f, 1.f), Tusk::Math::vec4(0.f, 0.f, 0.f, 1.f)), //Bottom Left
            Vertex(Math::vec3(-1.f, -1.f, 1.f),  Math::vec2(1.f, 1.f), Tusk::Math::vec4(0.f, 0.f, 0.f, 1.f)), //Bottom Right
            Vertex(Math::vec3(-1.f, 1.f, 1.f),   Math::vec2(1.f, 0.f), Tusk::Math::vec4(0.f, 0.f, 0.f, 1.f)), //Top Right
        };

        Index indices[36] = {
            0, 1, 2, 0, 2, 3,
            4, 5, 6, 4, 6, 7,
            8, 9, 10, 8, 10, 11,
            12, 13, 14, 12, 14, 15,
            16, 17, 18, 16, 18, 19,
            20, 21, 22, 20, 22, 23
        };
	};
}