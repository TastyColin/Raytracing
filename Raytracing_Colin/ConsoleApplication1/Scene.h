#pragma once
#include "Triangle.h"
#include"Mesh.h"
#include "Sphere.h"
#include "my_random.h"
#include <vector>
#include <math.h>

struct IntersectionScene
{
	bool b_intersect = false;
	double t = 0;
	Vector3 P = Vector3(0, 0, 0);
	Vector3 N = Vector3(0, 0, 0);
	int i_sph = 0;
	Vector3 Color = Vector3(0, 0, 0);
};



class Scene
{
public:
	Scene();
	~Scene();

	void AddObject(const Sphere&);
	void AddObject(const Triangle&);
	void AddObject(const Mesh&);
	void AddMaterial(const Material&);
	void SetLight(Sphere& Sph);
	void GetIntersection(Ray&, IntersectionScene& intersection);
	std::vector<Material> v_material;
	int nb_material;


protected:
	std::vector<const Object*> v_objects;
	int nb_sphere;
	void GetIntersectionSimple(const Ray& ray, IntersectionScene& intersection);
	void GetIntersectionRec(Ray& ray, const int i_down, const int i_reflexion, IntersectionScene& intersection);
	void AddDirectComponent(Ray& ray, IntersectionScene& intersection);
};

