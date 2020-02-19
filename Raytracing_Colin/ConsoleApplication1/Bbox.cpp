#include "pch.h"
#include "Bbox.h"

Bbox::Bbox(const Vector3& bmin, const Vector3& bmax) :
	bmin(bmin), bmax(bmax)
{
}


Bbox::~Bbox()
{
}


bool Bbox::DoIntersect(const Ray& ray, double &t) const
{
	double t_1, t_2, t_min, t_max;
	t_1 = (bmin[0] - ray.Get_C()[0]) / ray.Get_u()[0];
	t_2 = (bmax[0] - ray.Get_C()[0]) / ray.Get_u()[0];
	t_min = fmin(t_1, t_2);
	t_max = fmax(t_1, t_2);
	for (int k = 1; k < 3; ++k)
	{
		t_1 = (bmin[k] - ray.Get_C()[k]) / ray.Get_u()[k];
		t_2 = (bmax[k] - ray.Get_C()[k]) / ray.Get_u()[k];
		t_min = fmax(t_min, fmin(t_1, t_2));
		t_max = fmin(t_max, fmax(t_1, t_2));
	}
	t = t_min;
	return t_min < t_max;
}


void Bbox::GetCenterMaxDim(int &k_max, double &center) const
{
	k_max = 0;
	for (int k = 1; k < 3; ++k)
	{
		if ((bmax[k] - bmin[k]) > (bmax[k_max] - bmin[k_max]))
			k_max = k;
	}
	center = (bmax[k_max] + bmin[k_max]) / 2.;
}