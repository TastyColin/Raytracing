#pragma once
#include "Vector3.h"

class Ray
{
public:
	Ray(const Vector3& C, const Vector3& u, const Vector3& color);
	~Ray();

	//bool b_intersection(const Sphere& Sph) const;
	//Vector3 Get_intersection(const Sphere& Sph) const;

	Vector3 Get_C(void) const;
	Vector3 Get_u(void) const;
	Vector3 Get_color(void) const;
	bool IsInside(void) const;
	void Set_C(Vector3);
	void Set_u(Vector3);
	void Set_inside(bool);


	Vector3 ComputePoint(double t) const;

protected:
	Vector3 C;
	Vector3 u;
	Vector3 color;
	bool b_inside;
};

