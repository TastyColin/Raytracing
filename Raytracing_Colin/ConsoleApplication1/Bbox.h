#pragma once
#include "Vector3.h"
#include "Ray.h"

class Bbox
{
public:
	Bbox(const Vector3& bmin = Vector3(0.,0.,0.), const Vector3& bmax = Vector3(0., 0., 0.));
	~Bbox();

	bool DoIntersect(const Ray& ray, double& t) const;
	void GetCenterMaxDim(int &k_max, double &center) const;


protected:
	Vector3 bmin;
	Vector3 bmax;
};

