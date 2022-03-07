#pragma once
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/half_float.hpp>
#include <vector>
class Intersection
{
public:
	glm::dvec3 pos;
	glm::dvec3 norm;
	double t;
	glm::dvec3 spec;
	glm::dvec3 diff;
	vector<Light> lightVec;

	Intersection() {
		pos = glm::dvec3(0);
		norm = glm::dvec3(0);
		spec = glm::dvec3(0);
		diff = glm::dvec3(0);
		t = DBL_MAX; 
	}
};
