//#define SDL_MAIN_HANDLED
#include <stdio.h>
#include <string>
#include <vector>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/half_float.hpp>
#include <iostream>
#include <fstream>
#include <SDL2/SDL.h>
#include <SDL_image.h>
#include "SDL.h"

#include "Light.h"
#include "Object.h"
#include "Ray.h"
#include "Camera.h"
#include "Intersection.h"

using namespace std;

vector<Light> lights;
vector<Sphere> spheres;
vector<Plane> planes;
vector<Triangle> triangles;

int resX;
int resY;
Camera camera;
glm::dvec3 back;
SDL_Window* window = NULL;
SDL_Surface* screenSurface = NULL;
SDL_Surface* hello = NULL;
//SDL_Renderer* renderer = NULL;

Ray calculateRay(int i, int j) {
	//make sure you divide the other one by resY, the slides are wrong
	double a = (double)resX / resY;
	glm::dvec3 xFactor = camera.v * (double)i;
	glm::dvec3 yFactor = camera.u * (double)j;
	glm::dvec3 p = camera.ll + xFactor + yFactor;
	glm::dvec3 dir = glm::normalize(p - camera.eye);
	return Ray(camera.eye, dir);
}

bool clearPath(Light light, glm::dvec3 pos) {
	Ray ray = Ray(pos, light.pos - pos);
	double t_val = glm::length(ray.dir);
	ray.dir = glm::normalize(ray.dir);
	ray.pos = ray.pos + ray.dir * 0.5;
	if (spheres.size() > 0) {
		for (int s = 0; s < spheres.size(); s++) {
			Sphere temp = spheres[s];
			glm::dvec3 d = glm::normalize(ray.dir);
			glm::dvec3 hold = ray.pos - temp.pos;
			double a = 1;
			double b = 2 * (d.x * hold.x + d.y * hold.y + d.z * hold.z);
			double c = hold.x * hold.x + hold.y * hold.y + hold.z * hold.z -
				temp.radius * temp.radius;
			//cout << b * b - 4 * c << '\n';
			if (b * b - 4 * c >= 0) {
				double t2 = (-b + sqrt(b * b - 4 * c)) / 2;
				if (t2 > 0.0 && t2 < t_val) {
					return false;
				}
			}
		}
	}
	if (planes.size() > 0) {
		for (unsigned int s = 0; s < planes.size(); s++) {
			Plane temp = planes[s];
			if (glm::dot(ray.dir, temp.norm) != 0) {
				glm::dvec3 q = temp.pos - ray.pos;
				double t = glm::dot(q, temp.norm) / glm::dot(ray.dir, temp.norm);
				if (t > 0 && t <= t_val) {
					return false;
				}
			}
		}
	}
	return true;
}

Intersection firstInter(Ray ray, bool* collided) {
	Intersection inter = Intersection();
	if (spheres.size() > 0) {
		for (int s = 0; s < spheres.size(); s++) {
			Sphere temp = spheres[s];
			glm::dvec3 d = glm::normalize(ray.dir);
			glm::dvec3 hold = ray.pos - temp.pos;
			double a = 1;
			double b = 2 * (d.x * hold.x + d.y * hold.y + d.z * hold.z);
			double c = hold.x * hold.x + hold.y * hold.y + hold.z * hold.z -
				temp.radius * temp.radius;
			if (b * b - 4 * c >= 0) {
				double t1 = (-b - sqrt(b * b - 4 * c)) / 2;
				double t2 = (-b + sqrt(b * b - 4 * c)) / 2;
				if (t2 > 0.0) {
					*collided = true;
					inter.diff = temp.diff;
					inter.spec = temp.spec;
					if (t1 > 0) {
						inter.t = t1;
						inter.pos = ray.pos + ray.dir * t1;
						inter.norm = glm::normalize(temp.pos - inter.pos);
						for (unsigned int i = 0; i < lights.size(); i++) {
							if (clearPath(lights[i], inter.pos)) {
								inter.lightVec.push_back(lights[i]);
							}
						}
					}
					else {
						inter.t = t2;
						inter.pos = ray.pos + ray.dir * t2;
						for (unsigned int i = 0; i < lights.size(); i++) {
							if (clearPath(lights[i], inter.pos)) {
								inter.lightVec.push_back(lights[i]);
							}
						}
					}
				}
			}
		}
	}
	if (planes.size() > 0) {
		for (unsigned int s = 0; s < planes.size(); s++) {
			Plane temp = planes[s];
			if (glm::dot(ray.dir, temp.norm) != 0) {
				glm::dvec3 q = temp.pos - ray.pos;
				double t = glm::dot(q, temp.norm) / glm::dot(ray.dir,temp.norm);
				if (t > 0 && inter.t > t) {
					inter.t = t;
					inter.diff = temp.diff;
					inter.spec = temp.spec;
					inter.pos = ray.pos + ray.dir * t;
					*collided = true;
					for (unsigned int i = 0; i < lights.size(); i++) {
						if (clearPath(lights[i], inter.pos)) {
							inter.lightVec.push_back(lights[i]);
						}
					}
				}
			}
		}
	}
	if (triangles.size() > 0) {
		for (unsigned int tri = 0; tri < triangles.size(); tri++) {
			Triangle temp = triangles[tri];
			glm::dvec3 p = temp.pos2 - temp.pos;
			glm::dvec3 q = temp.pos3 - temp.pos;
			glm::dvec3 s = ray.pos - temp.pos;
		}
	}
	return inter;
}

glm::dvec3 TraceRay(Ray ray, int depth) {
	bool collided = false;
	Intersection p = firstInter(ray,&collided);
	glm::dvec3 diff = glm::dvec3(0);
	glm::dvec3 spec = glm::dvec3(0);
	if (collided) {
		diff += p.diff;
		for (unsigned int i = 0; i < p.lightVec.size(); i++) {
			Light tempLight = p.lightVec[i];
			diff = diff * tempLight.diff;
		}
		if (p.lightVec.size() == 0) {
			diff = glm::dvec3(0);
		}
		return diff + spec;
	}
	return glm::dvec3(back.x, back.y, back.z);
}

void setPixel(int i, int j, glm::dvec3 color) {
	unsigned int r = (unsigned int)(min(max(color.x, 0.0), 1.0) * 255);
	unsigned int g = (unsigned int)(min(max(color.y, 0.0), 1.0) * 255);
	unsigned int b = (unsigned int)(min(max(color.z, 0.0), 1.0) * 255);
	((uint32_t*)hello->pixels)[j * hello->w + i] = (uint32_t)SDL_MapRGB(hello->format, r, g, b);
}

int main(int argc, char* args[] ) {
	ifstream scene("scene.txt");
	string holder;
	scene >> holder >> back.x >> back.y >> back.z;
	//cout << holder + '\n';
	int max_depth;
	scene >> holder >> max_depth;
	scene >> holder;
	//cout << holder + '\n';
	while (strcmp(holder.c_str(), "RESOLUTION") != 0) {
		if (strcmp(holder.c_str(), "LIGHT") == 0) {
			Light light = Light();
			glm::dvec3 temp;
			scene >> holder >> temp.x >> temp.y >> temp.z;
			light.pos = temp;
			scene >> holder >> temp.x >> temp.y >> temp.z;
			light.diff = temp;
			scene >> holder >> temp.x >> temp.y >> temp.z;
			light.spec = temp;
			cout << light;
			lights.push_back(light);
		}
		else if (strcmp(holder.c_str(), "SPHERE") == 0) {
			Sphere sphere = Sphere();
			glm::dvec3 temp;
			scene >> holder >> temp.x >> temp.y >> temp.z >> holder >> sphere.radius;
			sphere.pos = temp;
			scene >> holder >> temp.x >> temp.y >> temp.z;
			sphere.diff = temp;
			scene >> holder >> temp.x >> temp.y >> temp.z;
			sphere.spec = temp;
			scene >> holder >> sphere.shininess;
			cout << sphere;
			spheres.push_back(sphere);
		}
		else if (strcmp(holder.c_str(), "PLANE") == 0) {
			cout << "I'm adding a plane!";
			Plane plane = Plane();
			glm::dvec3 temp;
			scene >> holder >> temp.x >> temp.y >> temp.z;
			plane.norm = temp;
			scene >> holder >> temp.x >> temp.y >> temp.z;
			plane.pos = temp;
			scene >> holder >> temp.x >> temp.y >> temp.z;
			plane.diff = temp;
			scene >> holder >> temp.x >> temp.y >> temp.z;
			plane.spec = temp;
			scene >> holder >> plane.shininess;
			cout << plane;
			planes.push_back(plane);
		}
		scene >> holder;
		//cout << holder + '\n';
	}
	scene >> resX >> resY;
	cout << "Resolution: " << resX << ", " << resY << '\n';

	glm::dvec3 eye = glm::dvec3(0, 0, 70);//set to 250 later
	glm::dvec3 up = glm::dvec3(0, 1, 0);
	glm::dvec3 look = glm::dvec3(0, 0, 0);
	camera = Camera(eye, up, look, resX, resY);
	
	cout << "Hello I'm working here\n";
	if (SDL_Init(SDL_INIT_EVERYTHING) < 0)
	{
		printf("SDL could not initialize! SDL_Error: %s\n", SDL_GetError());
		return -1;
	}
	window = SDL_CreateWindow("RayTracer", SDL_WINDOWPOS_UNDEFINED,
		SDL_WINDOWPOS_UNDEFINED, resX, resY,SDL_WINDOW_SHOWN);
	if (window == NULL)
	{
		printf("Window could not be created! SDL_Error: %s\n", SDL_GetError());
		return -1;
	}
	screenSurface = SDL_GetWindowSurface(window);
	hello = SDL_CreateRGBSurface(0, resX, resY, 32, 0, 0, 0, 0);
	if (hello == NULL) {
		SDL_Log("SDL_CreateRGBSurface() failed: %s", SDL_GetError());
		exit(1);
	}
	
	//beginning of the overall loop
	for (int x = 0; x < resX; x++) {
	#pragma omp parallel for
		for (int y = 0; y < resY; y++) {
			Ray ray = calculateRay(x, y);
			//Ray ray = Ray(glm::dvec3(0, 0, -5), glm::dvec3(0, 0, 1));
			if ((x==0 && y ==0) || (x==resX-1) && (y==resY-1)) {
				cout << "x: " << x << ", y: " << y << "\n";
				cout << ray.dir.x << ", " << ray.dir.y << ", " << ray.dir.z << "\n";
			}
			glm::dvec3 color = TraceRay(ray, max_depth);
			setPixel(x, y, color);
		}
		SDL_BlitSurface(hello, 0, screenSurface, 0);
		SDL_UpdateWindowSurface(window);
	}
	cout << "Loop is done\n";
	cout << "----------------------------------------\n";
	for (int i = 0; i < size(spheres); i++) {
		cout << spheres[i];
	}
	cout << "sizeof planes: " << size(planes) << '\n';
	for (int i = 0; i < size(planes); i++) {
		cout << planes[i];
	}

	
	SDL_SaveBMP(screenSurface, "endResult.bmp");
	SDL_Delay(2000);
	planes.clear();
	spheres.clear();
	lights.clear();

	SDL_FreeSurface(hello);
	hello = NULL;
	SDL_FreeSurface(screenSurface);
	screenSurface = NULL;
	SDL_DestroyWindow(window);
	window = NULL;
	SDL_Quit();

	return 0;
}