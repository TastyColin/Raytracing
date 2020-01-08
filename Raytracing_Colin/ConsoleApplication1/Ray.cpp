#include "pch.h"
#include "Ray.h"

Ray::Ray(const Vector3& C, const Vector3& u, const Vector3& color) : C(C), u(u), color(color)
{
}


Ray::~Ray()
{
}


Vector3 Ray::Get_C(void) const
{
	return C;
}

Vector3 Ray::Get_u(void) const
{
	return u;
}

Vector3 Ray::Get_color(void) const
{
	return color;
}


Vector3 Ray::ComputePoint(double t) const
{
	return C + t * u;
}