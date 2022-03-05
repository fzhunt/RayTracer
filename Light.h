#pragma once
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/half_float.hpp>
#include "Object.h"


using namespace std;

class Light: public Object
{
public:
	Light() {
		pos = glm::dvec3(0, 0, 0);
		diff = glm::dvec3(0, 0, 0);
		spec = glm::dvec3(0, 0, 0);
	}
};
ostream& operator<<(std::ostream& os, Light const& m) {
	return os << "LIGHT\nPosition: " << m.pos.x << ", " << m.pos.y << ", " << m.pos.z << '\n'
		<< "Diffuse: " << m.diff.x << ", " << m.diff.y << ", " << m.diff.z << '\n'
		<< "Specular: " << m.spec.x << ", " << m.spec.y << ", " << m.spec.z << '\n';
}

