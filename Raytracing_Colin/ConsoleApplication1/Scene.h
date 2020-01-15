#pragma once
#include "Sphere.h"
#include <vector>
#include <math.h>

struct IntersectionScene
{
	bool b_intersect = false;
	double t = 0;
	Vector3 P = Vector3(0, 0, 0);
	int i_sph = 0;
	Vector3 color = Vector3(0, 0, 0);
};

struct Material
{
	Sphere sphere;
	Vector3 color = Vector3(1,1,1);
	bool b_mirror = false;
	bool b_transparency = false;
	double material_index = 1;
};


class Scene
{
public:
	Scene();
	~Scene();

	void AddSphere(const Sphere&);
	void AddSphere(const Material&);
	bool DoIntersect(const Ray&) const;
	IntersectionScene GetIntersectionShadow(const Ray &) const;
	IntersectionScene GetIntersection(Ray&) const;
	std::vector<Material> v_spheres;

private:
	int nb_sphere;
	void GetIntersectionRec(Ray& ray, int i_down, IntersectionScene& intersection) const;
};

