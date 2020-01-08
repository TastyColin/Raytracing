#pragma once
#include "Sphere.h"
#include <vector>

struct IntersectionScene
{
	bool b_intersect;
	double t;
	Vector3 P;
	int i_sph;
	Vector3 color;
};

struct SphereColor
{
	Sphere sphere;
	Vector3 color = Vector3(1,1,1);
};


class Scene
{
public:
	Scene();
	~Scene();

	void AddSphere(const Sphere&);
	void AddSphere(const SphereColor&);
	bool DoIntersect(const Ray&) const;
	IntersectionScene GetIntersection(const Ray&) const;
	std::vector<SphereColor> v_spheres;

private:
	int nb_sphere;
};

