#pragma once
#include "Object.h"

class Triangle : public Object
{
public:
	Triangle(const Vector3& A, const Vector3& B, const Vector3& C, Material m);
	Triangle(const Vector3& A, const Vector3& B, const Vector3& C, const Vector3& NA, const Vector3& NB, const Vector3& NC, Material m);
	~Triangle();

	void GetIntersection(const Ray&, IntersectionObject& intersection) const;

protected:
	Vector3 A, B, C;			// sommets
	Vector3 NA, NB, NC;			// normales
	Vector3 N;
};

