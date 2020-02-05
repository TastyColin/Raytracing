#pragma once
#include "Vector3.h"
#include "Ray.h"
#include <math.h>


struct IntersectionSphere
{
	bool b_intersect = false;
	double t = 0;
};

class Sphere
{
public:
	Sphere(const Vector3& O, double R);
	~Sphere();

	Vector3 Get_O(void) const;
	double Get_R(void) const;
	bool DoIntersect(const Ray&) const;
	void GetIntersection(const Ray&, IntersectionSphere& intersection) const;
	double GetIntersectionDistance(const Ray&) const;
	Vector3 GetNormal(const Vector3&) const;

protected:
	Vector3 O;
	double R;
};

