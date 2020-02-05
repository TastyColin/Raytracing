#pragma once
#include "Sphere.h"
#include "my_random.h"
#include <vector>
#include <math.h>

struct IntersectionScene
{
	bool b_intersect = false;
	double t = 0;
	Vector3 P = Vector3(0, 0, 0);
	int i_sph = 0;
	Vector3 Color = Vector3(0, 0, 0);
};

struct Material
{
	Sphere sphere;
	Vector3 color = Vector3(1,1,1);
	double emissivity = 0;
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
	void SetLight(const Material& Sph);
	bool DoIntersect(const Ray&) const;
	void GetIntersection(Ray&, IntersectionScene& intersection);
	std::vector<Material> v_spheres;

protected:
	int nb_sphere;
	IntersectionSphere _intersection_sphere;
	void GetIntersectionSimple(const Ray& ray, IntersectionScene& intersection);
	void GetIntersectionRec(Ray& ray, const int i_down, const int i_reflexion, IntersectionScene& intersection);
	void AddDirectComponent(Ray& ray, IntersectionScene& intersection);
};

