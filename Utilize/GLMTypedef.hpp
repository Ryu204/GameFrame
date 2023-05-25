#ifndef __UTILIZE_GLMTYPEDEF_HPP__
#define __UTILIZE_GLMTYPEDEF_HPP__

#include "glm/vec2.hpp"
#include "glm/vec3.hpp"
#include "glm/vec4.hpp"

namespace HJUIK
{
	using Vector4f = glm::vec4;
	using Vector3f = glm::vec3;
	using Vector2f = glm::vec2;

	using Vector2u = glm::uvec2;
	using Vector3u = glm::uvec3;
	using Vector4u = glm::uvec4;
	
	using Vector2i = glm::ivec2;
	using Vector3i = glm::ivec3;
	using Vector4i = glm::ivec4;
}

#endif