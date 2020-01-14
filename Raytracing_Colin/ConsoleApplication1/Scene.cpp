#include "pch.h"
#include "Scene.h"


Scene::Scene() :
	nb_sphere(0)
{
	
	//AddSphere(SphereColor { Sphere(Vector3(1000, 0, 0), 940), Vector3(1,0,0) });
	//AddSphere(SphereColor{ Sphere(Vector3(-1000, 0, 0), 940), Vector3(1,0,0) });
	//AddSphere(SphereColor{ Sphere(Vector3(0, 1000, 0), 940), Vector3(0,1,0) });
	AddSphere(SphereColor{ Sphere(Vector3(0, -1000, 0), 940), Vector3(0,1,0) });
	//AddSphere(SphereColor{ Sphere(Vector3(0, 0, 1000), 940), Vector3(0,0,1) });
	//AddSphere(SphereColor{ Sphere(Vector3(0, 0, -1000), 940), Vector3(0,0,1) });
	/*
	AddSphere(SphereColor { Sphere(Vector3(1000, 0, 0), 940), Vector3(1,1,1) });
	AddSphere(SphereColor{ Sphere(Vector3(-1000, 0, 0), 940), Vector3(1,1,1) });
	AddSphere(SphereColor{ Sphere(Vector3(0, 1000, 0), 940), Vector3(1,1,1) });
	AddSphere(SphereColor{ Sphere(Vector3(0, -1000, 0), 940), Vector3(1,1,1) });
	AddSphere(SphereColor{ Sphere(Vector3(0, 0, 1000), 940), Vector3(1,1,1) });
	AddSphere(SphereColor{ Sphere(Vector3(0, 0, -1000), 940), Vector3(1,1,1) });
	*/
}


Scene::~Scene()
{
}


void Scene::AddSphere(const Sphere& Sph)
{
	SphereColor Sph_color { Sph, Vector3(1,1,1) };
	v_spheres.push_back(Sph_color);
	nb_sphere++;
}


void Scene::AddSphere(const SphereColor& Sph_color)
{
	v_spheres.push_back(Sph_color);
	nb_sphere++;
}


bool Scene::DoIntersect(const Ray& ray) const
{
	for (int i_sph = 0; i_sph < nb_sphere; i_sph++)
	{
		if (v_spheres[i_sph].sphere.DoIntersect(ray))
		{
			return true;
		};
	}
	return false;
}



IntersectionScene Scene::GetIntersection(const Ray& ray) const
{
	IntersectionScene res{ false, 0, Vector3(0,0,0), 0 , Vector3(0,0,0)};
	for (int i_sph = 0; i_sph < nb_sphere; i_sph++)
	{
		IntersectionSphere intersection_sphere = v_spheres[i_sph].sphere.GetIntersection(ray);
		if (intersection_sphere.b_intersect)
		{
			res.b_intersect = true;
			double t_sph = intersection_sphere.t;
			if (res.t == 0 || res.t > t_sph) {
				res.i_sph = i_sph;
				res.t = t_sph;
			}
		};
	}
	res.P = ray.ComputePoint(res.t);
	res.color = ray.Get_color() *v_spheres[res.i_sph].color;
	return res;
}

