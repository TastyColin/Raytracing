#include "pch.h"
#include "Ray.h"

Ray::Ray(const Vector3& C, const Vector3& u, const Vector3& color) : C(C), u(u), color(color), b_inside(false)
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

bool Ray::IsInside(void) const
{
	return b_inside;
}


void Ray::Set_C(Vector3 new_C)
{
	C = new_C;
}

void Ray::Set_u(Vector3 new_u)
{
	u = new_u;
}

void Ray::Set_inside(bool b)
{
	b_inside = b;
}


Vector3 Ray::ComputePoint(double t) const
{
	return C + t * u;
}