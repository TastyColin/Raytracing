#include "pch.h"
#include "Ray.h"

Ray::Ray() :
	C(Vector3()),
	u(Vector3(0,0,-1)),
	color(Vector3()),
	b_inside(false)
{
}


Ray::Ray(const Vector3& C, const Vector3& u, const Vector3& color) : C(C), u(u), color(color), b_inside(false), emissivity(0)
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

double Ray::Get_emissivity(void) const
{
	return emissivity;
}

bool Ray::IsInside(void) const
{
	return b_inside;
}


void Ray::Set_C(const Vector3& new_C)
{
	C = new_C;
}

void Ray::Set_u(const Vector3& new_u)
{
	u = new_u;
}

void Ray::Set_inside(bool b)
{
	b_inside = b;
}


void Ray::Set_color(const Vector3& new_color)
{
	color = new_color;
}

void Ray::Color(const Vector3& color_to_apply, const double& e)
{
	color *= color_to_apply;
	emissivity = e;
}


Vector3 Ray::ComputePoint(double t) const
{
	return C + t * u;
}