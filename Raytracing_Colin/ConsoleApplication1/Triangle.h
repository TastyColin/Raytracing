#pragma once
#include "Object.h"

class Triangle : public Object
{
public:
	Triangle(const Vector3& A, const Vector3& B, const Vector3& C, Material m);
	Triangle(const Vector3& A, const Vector3& B, const Vector3& C, const Vector3& NA, const Vector3& NB, const Vector3& NC, Material m);
	Triangle(const Vector3& A, const Vector3& B, const Vector3& C,
		const Vector3& NA, const Vector3& NB, const Vector3& NC,
		const Vector3& uvA, const Vector3& uvB, const Vector3& uvC,
		Material m);
	~Triangle();

	void GetIntersection(const Ray&, IntersectionObject& intersection) const;
	void GetIntersectionUV(const Ray&, IntersectionObject& intersection, Vector3& UV) const;

protected:
	Vector3 A, B, C;			// sommets
	Vector3 NA, NB, NC;			// normales
	Vector3 uvA, uvB, uvC;		// coordonnées UV
	Vector3 N;
};

