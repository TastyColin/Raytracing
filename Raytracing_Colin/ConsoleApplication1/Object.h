#pragma once
#include "Vector3.h"
#include "Ray.h"
#include <math.h>
#include "constantes.h"

struct IntersectionObject
{
	bool b_intersect = false;
	double t = 0;
	Vector3 N = Vector3(0.,0.,0.);
	Vector3 Color = Vector3(0., 0., 0.);
};

struct Material
{
	Vector3 color = Vector3(1, 1, 1);
	double emissivity = 0;
	bool b_mirror = false;
	bool b_transparency = false;
	double material_index = 1;
	double ks = 0.;
	double n = 1000.;
};

class Object
{
public:
	Object(Material material);
	~Object();

	virtual void GetIntersection(const Ray&, IntersectionObject& intersection) const = 0;

	Material material;
};

