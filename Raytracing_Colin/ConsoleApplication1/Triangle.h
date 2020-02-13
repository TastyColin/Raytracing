#pragma once
#include "Object.h"

class Triangle : public Object
{
public:
	Triangle(const Vector3& A, const Vector3& B, const Vector3& C, Material m);
	~Triangle();

	void GetIntersection(const Ray&, IntersectionObject& intersection) const;
	inline Vector3 GetNormal(const Vector3& P) const { return N; };

protected:
	Vector3 A, B, C;	// sommets
	Vector3 N;			// normale
};

