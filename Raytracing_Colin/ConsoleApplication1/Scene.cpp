#include "pch.h"
#include "Scene.h"


Scene::Scene() :
	nb_sphere(0)
{
	AddSphere(Material{ Sphere(Vector3(0,0,0),0), Vector3(1, 1, 1) });
	AddSphere(Material{ Sphere(Vector3(1000, 0, 0), 940), Vector3(1,0.2,0.8), false });
	AddSphere(Material{ Sphere(Vector3(-1000, 0, 0), 940), Vector3(1,0,0), false });
	AddSphere(Material{ Sphere(Vector3(0, 1000, 0), 940), Vector3(0.2,0.8,1), false });
	AddSphere(Material{ Sphere(Vector3(0, -1000, 0), 940), Vector3(0,0,1), false });
	AddSphere(Material{ Sphere(Vector3(0, 0, 1000), 940), Vector3(1,1,0.95), false });
	AddSphere(Material{ Sphere(Vector3(0, 0, -1000), 900), Vector3(1,1,0.95), false });
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

void Scene::SetLight(const Material& sph)
{
	v_spheres[0] = sph;
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



void Scene::GetIntersection(Ray& ray, IntersectionScene& intersection)
{
	intersection.Color.Reset();
	GetIntersectionRec(ray, 4, 1, intersection);
	return;
}


void Scene::GetIntersectionSimple(const Ray& ray, IntersectionScene& intersection)
{
	// Appel récursif
	intersection.t = 0;
	intersection.b_intersect = false;
	for (int i_sph = 0; i_sph < nb_sphere; i_sph++)
	{
		v_spheres[i_sph].sphere.GetIntersection(ray, _intersection_sphere);
		if (_intersection_sphere.b_intersect)
		{
			intersection.b_intersect = true;
			if (intersection.t < EPSILON || intersection.t > _intersection_sphere.t) {
				intersection.i_sph = i_sph;
				intersection.t = _intersection_sphere.t;
			}
		};
	}
}


void Scene::GetIntersectionRec(Ray& ray, const int i_down, const int i_reflexion, IntersectionScene& intersection)
{
	if ( i_reflexion < 0 || i_down < 0) {
		return; 
	}
	GetIntersectionSimple(ray, intersection);
	
	if (intersection.b_intersect)
	{
		intersection.P = ray.ComputePoint(intersection.t);
		// Mirroir
		if (v_spheres[intersection.i_sph].b_mirror)
		{
			Vector3 n = v_spheres[intersection.i_sph].sphere.GetNormal(intersection.P);
			ray.Set_C(intersection.P + EPSILON *n);
			ray.Set_u(ray.Get_u() - 2 * dot(ray.Get_u(), n) * n);
			ray.Color(v_spheres[intersection.i_sph].color, v_spheres[intersection.i_sph].emissivity);
			GetIntersectionRec(ray, i_down, i_reflexion - 1, intersection);
		}
		// Transparence
		else if (v_spheres[intersection.i_sph].b_transparency)
		{
			Vector3 u_I = ray.Get_u();
			Vector3 n = v_spheres[intersection.i_sph].sphere.GetNormal(intersection.P);
			if (ray.IsInside()) { n *= -1; }
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
				ray.Set_C(intersection.P - EPSILON *n);
				ray.Set_u(T_T+T_N);
				ray.Set_inside(!ray.IsInside());
				ray.Color(v_spheres[intersection.i_sph].color, v_spheres[intersection.i_sph].emissivity);
				GetIntersectionRec(ray, i_down, i_reflexion - 1, intersection);
			}
			else // Réflexion
			{
				ray.Set_C(intersection.P + EPSILON *n);
				ray.Set_u(ray.Get_u() - 2 * dot(ray.Get_u(), n) * n);
				ray.Color(v_spheres[intersection.i_sph].color, v_spheres[intersection.i_sph].emissivity);
			}
		}
		else
		{
			ray.Color(v_spheres[intersection.i_sph].color);
			AddDirectComponent(ray, intersection);
			Vector3 n = v_spheres[intersection.i_sph].sphere.GetNormal(intersection.P);
			ray.Set_C(intersection.P + EPSILON *n);
			ray.Set_u(random_cos(n));
			GetIntersectionRec(ray, i_down-1, i_reflexion, intersection);
		}
	}
	
}

void Scene::AddDirectComponent(Ray& ray, IntersectionScene& intersection)
{
	bool b_visible = false;
	if (intersection.b_intersect && intersection.i_sph != 0) {
		Vector3 P = intersection.P;
		Vector3 L = v_spheres[0].sphere.Get_O();
		double R_light = v_spheres[0].sphere.Get_R();
		Vector3 O = v_spheres[intersection.i_sph].sphere.Get_O();
		Vector3 u_OP = (P - O);
		u_OP.normalization();
		Vector3 u_LP = (P - L);
		u_LP.normalization();
		Vector3 u_random_light = random_cos(u_LP);
		Vector3 X = L + R_light * u_random_light; // point sur la sphère de lumière
		Vector3 u_PX = X - P;
		u_PX.normalization();
		double costheta = std::max(0., dot(u_OP, u_PX));
		double costhetaprime = -dot(u_random_light, u_PX);
		double costhetaseconde = dot(u_random_light, u_LP);

		double d2 = (X - P).norm_square();

		// Test de l'ombre pour la visibilité
		Ray shadow_ray;
		IntersectionScene shadow_intersection;
		shadow_ray.Set_C(P + EPSILON*u_PX);
		shadow_ray.Set_u(u_PX);
		GetIntersectionSimple(shadow_ray, shadow_intersection);
		b_visible = (!shadow_intersection.b_intersect) || (dot(X - P, u_PX) < shadow_intersection.t + EPSILON);

		if (b_visible)
		{
			intersection.Color += 1 / d2 * std::max(0., costheta)*ray.Get_color();//*costhetaprime/costhetaseconde
		}
	}
}
