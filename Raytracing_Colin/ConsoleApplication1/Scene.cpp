#include "pch.h"
#include "Scene.h"


Scene::Scene() :
	nb_sphere(0)
{
	
	AddSphere(Material { Sphere(Vector3(1000, 0, 0), 940), Vector3(1,0.2,0.8) });
	AddSphere(Material{ Sphere(Vector3(-1000, 0, 0), 940), Vector3(1,0,0) });
	AddSphere(Material{ Sphere(Vector3(0, 1000, 0), 940), Vector3(0.2,0.8,1) });
	AddSphere(Material{ Sphere(Vector3(0, -1000, 0), 940), Vector3(0,0,1) });
	AddSphere(Material{ Sphere(Vector3(0, 0, 1000), 940), Vector3(0.6,0,1) });
	AddSphere(Material{ Sphere(Vector3(0, 0, -1000), 900), Vector3(1,1,0.95) });
	/*
	AddSphere(Material { Sphere(Vector3(1000, 0, 0), 940), Vector3(1,1,1) });
	AddSphere(Material{ Sphere(Vector3(-1000, 0, 0), 940), Vector3(1,1,1) });
	AddSphere(Material{ Sphere(Vector3(0, 1000, 0), 940), Vector3(1,1,1) });
	AddSphere(Material{ Sphere(Vector3(0, -1000, 0), 940), Vector3(1,1,1) });
	AddSphere(Material{ Sphere(Vector3(0, 0, 1000), 940), Vector3(1,1,1) });
	AddSphere(Material{ Sphere(Vector3(0, 0, -1000), 940), Vector3(1,1,1) });
	*/
}


Scene::~Scene()
{
}


void Scene::AddSphere(const Sphere& Sph)
{
	Material Sph_color { Sph, Vector3(1,1,1) };
	v_spheres.push_back(Sph_color);
	nb_sphere++;
}


void Scene::AddSphere(const Material& Sph_color)
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


IntersectionScene Scene::GetIntersectionShadow(const Ray& ray) const
{
	IntersectionScene intersection{ false, 0, Vector3(0,0,0), 0 , Vector3(0,0,0) };
	for (int i_sph = 0; i_sph < nb_sphere; i_sph++)
	{
		IntersectionSphere intersection_sphere = v_spheres[i_sph].sphere.GetIntersection(ray);
		if (intersection_sphere.b_intersect)
		{
			intersection.b_intersect = true;
			double t_sph = intersection_sphere.t;
			if (intersection.t == 0 || intersection.t > t_sph) {
				intersection.i_sph = i_sph;
				intersection.t = t_sph;
			}
		};
	}
	return intersection;
}


IntersectionScene Scene::GetIntersection(Ray& ray) const
{
	IntersectionScene intersection{ false, 0, Vector3(0,0,0), 0 , Vector3(0,0,0) };
	GetIntersectionRec(ray, 5, intersection);
	return intersection;
}


void Scene::GetIntersectionRec(Ray& ray, int i_down, IntersectionScene& intersection) const
{
	if (i_down < 0)
	{ 
		return;
	}
	intersection.t = 0;
	intersection.b_intersect = false;
	for (int i_sph = 0; i_sph < nb_sphere; i_sph++)
	{
		IntersectionSphere intersection_sphere = v_spheres[i_sph].sphere.GetIntersection(ray);
		if (intersection_sphere.b_intersect)
		{
			intersection.b_intersect = true;
			double t_sph = intersection_sphere.t;
			if (intersection.t == 0 || intersection.t > t_sph) {
				intersection.i_sph = i_sph;
				intersection.t = t_sph;
			}
		};
	}
	if (intersection.b_intersect)
	{
		intersection.P = ray.ComputePoint(intersection.t);
		if (v_spheres[intersection.i_sph].b_mirror)
		{
			Vector3 n = (1 / v_spheres[intersection.i_sph].sphere.Get_R()) * (intersection.P - v_spheres[intersection.i_sph].sphere.Get_O());
			ray.Set_C(intersection.P + 1e-8*n);
			ray.Set_u(ray.Get_u() - 2 * dot(ray.Get_u(), n) * n);
			GetIntersectionRec(ray, i_down - 1, intersection);
		}
		else if (v_spheres[intersection.i_sph].b_transparency)
		{
			Vector3 u_I = ray.Get_u();
			Vector3 n;
			if (ray.IsInside()){n = -(1 / v_spheres[intersection.i_sph].sphere.Get_R()) * (intersection.P - v_spheres[intersection.i_sph].sphere.Get_O());}
			else {n= (1 / v_spheres[intersection.i_sph].sphere.Get_R()) * (intersection.P - v_spheres[intersection.i_sph].sphere.Get_O());}
			double r_index;
			if (ray.IsInside())
			{
				r_index = v_spheres[intersection.i_sph].material_index;
			}
			else
			{
				r_index = 1 / v_spheres[intersection.i_sph].material_index;
			}
			double alpha = 1 - pow(r_index, 2) * (1 - pow(dot(n, u_I), 2));
			
			if (alpha > 0)	// Refraction
			{
				Vector3 T_T = r_index * (u_I - dot(u_I, n)*n);
				Vector3 T_N = -sqrt(alpha)*n;
				ray.Set_C(intersection.P - 1e-4*n);
				ray.Set_u(T_T+T_N);
				ray.Set_inside(!ray.IsInside());
				GetIntersectionRec(ray, i_down - 1, intersection);
			}
			else // Réflexion
			{
				ray.Set_C(intersection.P + 1e-8*n);
				ray.Set_u(ray.Get_u() - 2 * dot(ray.Get_u(), n) * n);
				intersection.color = ray.Get_color()*v_spheres[intersection.i_sph].color = Vector3(0,0,0);
			}
		}
		else
		{
			intersection.color = ray.Get_color()*v_spheres[intersection.i_sph].color;
		}
	}
	
}
