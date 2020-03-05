#pragma once
#include <string>
#include <vector>
#include <math.h>  
#include "Sphere.h"

class TexturedSphere : public Sphere
{
public:
	TexturedSphere(const Vector3& O, double R, Material m, const char* texture_name);
	~TexturedSphere();

	void GetIntersection(const Ray& ray, IntersectionObject& intersection) const;

protected:
	void load_texture(const char* filename);
	std::vector<unsigned char> texture;
	double w;
	double h;

	void _GetColor(const Vector3&P, Vector3& color) const;
};

