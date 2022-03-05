#pragma once
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/half_float.hpp>
class Ray
{
public:
	glm::dvec3 dir;
	glm::dvec3 pos;
	Ray() {
		dir = glm::dvec3(0, 0, 0);
		pos = glm::dvec3(0, 0, 0);
	}
	Ray(glm::dvec3 p, glm::dvec3 d) {
		dir = d;
		pos = p;
	}
};

