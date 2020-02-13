#pragma once
#include "Object.h"


class Sphere : public Object
{
public:
	Sphere(const Vector3& O, double R, Material m);
	~Sphere();

	Vector3 Get_O(void) const;
	double Get_R(void) const;
	bool DoIntersect(const Ray&) const;
	void GetIntersection(const Ray&, IntersectionObject& intersection) const;
	double GetIntersectionDistance(const Ray&) const;
	Vector3 GetNormal(const Vector3&) const;

protected:
	Vector3 O;
	double R;
};

