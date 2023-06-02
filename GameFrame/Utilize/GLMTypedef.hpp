#ifndef GAMEFRAME_UTILIZE_GLMTYPEDEF_HPP
#define GAMEFRAME_UTILIZE_GLMTYPEDEF_HPP

/*	
	Alias for commonly used glm types
*/

#include <glm/vec2.hpp>
#include <glm/vec3.hpp>
#include <glm/vec4.hpp>

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
} // namespace HJUIK

#endif