#include "pch.h"
#include "Triangle.h"


Triangle::Triangle(const Vector3& A, const Vector3& B, const Vector3& C, Material m) : A(A), B(B), C(C), Object(m)
{
	N = prod(B - A, C - A);
	N.normalization();
}


Triangle::~Triangle()
{
}



void Triangle::GetIntersection(const Ray& ray, IntersectionObject& intersection) const
{
	double denom = dot(ray.Get_u(), N);
	if (fabs(denom) < EPSILON)
	{
		intersection.b_intersect = false;
		return;
	}

	double t = dot(A - ray.Get_C(), N) / denom;
	if (t < 0)
	{
		intersection.b_intersect = false;
		return;
	}

	Vector3 P = ray.ComputePoint(t);

	double delta, beta, gamma;
	delta = dot(B - A, B - A) * dot(C - A, C - A) - dot(B - A, C - A)*dot(B - A, C - A);
	beta = (dot(P - A, B - A)*dot(C - A, C - A) - dot(P - A, C - A)*dot(B - A, C - A)) / delta;
	gamma = (dot(B - A, B - A)*dot(P - A, C - A) - dot(B - A, C - A)*dot(P - A, B - A)) / delta;
	
	intersection.t = t;
	if (dot(N, ray.Get_u()) > 0)
		intersection.N = -N;
	else
		intersection.N = N;
	intersection.b_intersect = (beta > 0 && gamma > 0 && (beta + gamma) < 1);
}
