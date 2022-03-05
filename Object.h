#pragma once
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/half_float.hpp>
#include <iostream>
using namespace std;

class Object
{
public:
	glm::dvec3 pos;
	glm::dvec3 diff;
	glm::dvec3 spec;
	double shininess;
	Object() {
		pos = glm::dvec3(0, 0, 0);
		diff = glm::dvec3(0, 0, 0);
		spec = glm::dvec3(0, 0, 0);
	}
	int getType() {
		return 0;
	}
};

class Sphere: public Object
{
public:
	double radius;
	Sphere() {
		radius = 2;
		shininess = 10;
		pos = glm::dvec3(0, 0, 0);
		diff = glm::dvec3(0, 0, 0);
		spec = glm::dvec3(0, 0, 0);
	}
	int getType() {
		return 1;
	}
	
};

class Plane : public Object
{
public:
	glm::dvec3 norm;
	Plane() {
		pos = glm::dvec3(0, 0, 0);
		diff = glm::dvec3(0, 0, 0);
		spec = glm::dvec3(0, 0, 0);
		norm = glm::dvec3(0, 0, 0);
	}
};

class Triangle : public Object {
public:
	glm::dvec3 pos2;
	glm::dvec3 pos3;
	Triangle() {
		pos = glm::dvec3(0, 0, 0);
		pos2 = glm::dvec3(0, 0, 0);
		pos3 = glm::dvec3(0, 0, 0);
		diff = glm::dvec3(0, 0, 0);
		spec = glm::dvec3(0, 0, 0);
	}
};

ostream& operator<<(std::ostream& os, Sphere const& m) {
	return os << "SPHERE\nPosition: " << m.pos.x << ", " << m.pos.y << ", " << m.pos.z << '\n'
		<< "Diffuse: " << m.diff.x << ", " << m.diff.y << ", " << m.diff.z << '\n'
		<< "Specular: " << m.spec.x << ", " << m.spec.y << ", " << m.spec.z << '\n'
		<< "Radius: " << m.radius << "\nShininess: " << m.shininess << '\n';
}

ostream& operator<<(std::ostream& os, Plane const& m) {
	return os << "PLANE\nNorm: " << m.norm.x << ", "  << m.norm.y << ", " << m.norm.z << '\n'
		<< "Position: " << m.pos.x << ", " << m.pos.y << ", " << m.pos.z << '\n'
		<< "Diffuse: " << m.diff.x << ", " << m.diff.y << ", " << m.diff.z << '\n'
		<< "Specular: " << m.spec.x << ", " << m.spec.y << ", " << m.spec.z << '\n'
		<< "Shininess: " << m.shininess << '\n';
}