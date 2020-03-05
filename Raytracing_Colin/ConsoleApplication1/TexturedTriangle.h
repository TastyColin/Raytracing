#pragma once
#include "Triangle.h"

class TexturedTriangle : public Triangle
{
public:
	TexturedTriangle(const Vector3& A, const Vector3& B, const Vector3& C, Material m, const double& step);
	~TexturedTriangle();

	void GetIntersection(const Ray&, IntersectionObject& intersection) const;

protected:
	double step;

};

