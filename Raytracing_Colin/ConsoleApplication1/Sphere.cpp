#include "pch.h"
#include "Sphere.h"


Sphere::Sphere(const Vector3& O, double R, Material m) : O(O), R(R), Object(m)
{
}


Sphere::~Sphere()
{
}


Vector3 Sphere::Get_O(void) const
{
	return O;
}


double Sphere::Get_R(void) const
{
	return R;
}


// Intersection

bool Sphere::DoIntersect(const Ray& ray) const
{
	Vector3 C = ray.Get_C();
	Vector3 u = ray.Get_u();
	// Résolution eq degrès 2
	double a, b, c, delta;
	a = 1.;
	b = 2. * dot(u, C - O);
	c = (C - O).norm_square() - R * R;
	delta = b * b - 4 * a*c;
	if (delta >= 0)
	{
		return (-b + sqrt(delta)) > 0;
	}
	else
	{
		return false;
	}
}


void Sphere::GetIntersection(const Ray& ray, IntersectionObject& intersection) const
{
	Vector3 C = ray.Get_C();
	Vector3 u = ray.Get_u();
	intersection.b_intersect = false;
	intersection.t = 0;
	// Résolution eq degrès 2
	double a, b, c, delta, t1, t2;
	a = 1.;
	b = 2. * dot(u, C - O);
	c = (C - O).norm_square() - R * R;
	delta = b * b - 4 * a*c;
	t1 = (-b - sqrt(delta)) / (2 * a);
	t2 = (-b + sqrt(delta)) / (2 * a);
	if (t1 > 0)
	{
		intersection.b_intersect = true;
		intersection.t = t1;
		intersection.N = (C + intersection.t * u - O) / R;
		return;
	}
	if (t2 > 0)
	{
		intersection.b_intersect = true;
		intersection.t = t2;
		intersection.N = (C + intersection.t * u - O) / R;
	}
}


double Sphere::GetIntersectionDistance(const Ray& ray) const
{
	Vector3 C = ray.Get_C();
	Vector3 u = ray.Get_u();
	// Résolution eq degrès 2
	double a, b, c, delta, t1, t2, t;
	a = 1.;
	b = 2. * dot(u, C - O);
	c = (C - O).norm_square() - R * R;
	delta = b * b - 4 * a*c;
	t1 = (-b - sqrt(delta)) / (2 * a);
	t2 = (-b + sqrt(delta)) / (2 * a);
	t = fmin(t1, t2);
	if (t <= 0) { t = t1 + t2 - t; }
	return t;
}


Vector3 Sphere::GetNormal(const Vector3& P) const
{
	return (P - O) / R;
}