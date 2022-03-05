#pragma once
#define _USE_MATH_DEFINES
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/half_float.hpp>
#include <math.h>

class Camera
{
public:
	glm::dvec3 eye;
	glm::dvec3 up;
	glm::dvec3 lookAt;
	glm::dvec3 l;
	glm::dvec3 v;
	glm::dvec3 u;
	glm::dvec3 ll;
	int resX;
	int resY;
	double aspect;
	Camera() {
		glm::dvec3 zero = glm::dvec3(0, 0, 0);
		eye = zero;
		up = zero;
		lookAt = zero;
		l = zero;
		v = zero;
		u = zero;
	}
	Camera(glm::dvec3 e, glm::dvec3 up2, glm::dvec3 look, int resolX, int resolY) {
		eye = e;
		up = up2;
		resX = resolX;
		resY = resolY;
		aspect = resX / resY;
		lookAt = look;
		//l = (lookAt - eye) / (glm::normalize(lookAt - eye));
		l = glm::normalize(lookAt- eye); //swapped eye and lookAt
		cout << "l: " << l.x << ", " << l.y << ", " << l.z << "\n";
		//v = (glm::cross(l, up) / glm::normalize(glm::cross(l, up)));
		v = glm::normalize(glm::cross(l, up));
		cout << "v: " << v.x << ", " << v.y << ", " << v.z << "\n";
		u = glm::cross(v, l); //swapped l and v
		
		cout << "u: " << u.x << ", " << u.y << ", " << u.z << "\n";
		//fovy = 75, Picked this arbitrarily from what I remember
		
		float fov = 70.0f;
		double d_val = 0.5 * resY / tan(0.5 * fov / 180.0 * M_PI);

		//double d_val = (double)1.0 / tan(M_PI / 360 ); //was M_PI / 180 / 2 / 2
		ll = eye + d_val * l - aspect * v * 0.5 * (double)resX - u * 0.5 * (double)resY;
		cout << "ll: " << ll.x << ", " << ll.y << ", " << ll.z << "\n";
	}
};
