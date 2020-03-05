#include "pch.h"
#include "Scene.h"

const Vector3 PI_VECTOR(1 / PI, 1 / PI, 1 / PI);

Scene::Scene() :
	nb_sphere(0),
	nb_material(0)
{
	AddMaterial(Material{ Vector3(1, 1, 1) });
	Sphere sp = Sphere(Vector3(0, 0, 0), 0, (v_material[nb_material - 1]));
	AddObject(sp);
}


Scene::~Scene()
{
}


void Scene::AddObject(const Sphere& s)
{
	v_objects.push_back(&s);
	nb_sphere++;
}

void Scene::AddObject(const Triangle& s)
{
	v_objects.push_back(&s);
	nb_sphere++;
}

void Scene::AddObject(const Mesh& s)
{
	v_objects.push_back(&s);
	nb_sphere++;
}

void Scene::AddMaterial(const Material& mat)
{
	v_material.push_back(mat);
	++nb_material;
}

void Scene::SetLight(Sphere& sph)
{
	v_objects[0] = &sph;
}




void Scene::GetIntersection(Ray& ray, IntersectionScene& intersection)
{
	intersection.ColorPixel.Reset();
	GetIntersectionRec(ray, 5, 10, intersection);
	return;
}


void Scene::GetIntersectionSimple(const Ray& ray, IntersectionScene& intersection)
{
	intersection.t = 0;
	intersection.b_intersect = false;
	IntersectionObject intersection_sphere;
	for (int i_sph = 0; i_sph < nb_sphere; i_sph++)
	{
		v_objects[i_sph]->GetIntersection(ray, intersection_sphere);
		if (intersection_sphere.b_intersect)
		{
			if (!intersection.b_intersect || intersection.t > intersection_sphere.t) {
				intersection.i_sph = i_sph;
				intersection.t = intersection_sphere.t;
				intersection.N = intersection_sphere.N;
				intersection.Color = intersection_sphere.Color;
				intersection.b_intersect = true;
			}
		};
	}
}

double compute_phong_brdf(const Vector3& wr, const Vector3& wo, const double& n)
{

	return std::pow(dot(wr, wo), n) * (n + 2) / (2*PI);
}


void Scene::GetIntersectionRec(Ray& ray, const int i_down, const int i_reflexion, IntersectionScene& intersection)
{
	if ( i_reflexion < 0 || i_down < 0) {
		return; 
	}
	GetIntersectionSimple(ray, intersection);
	if (intersection.i_sph == 0)
		return;
	if (intersection.b_intersect)
	{
		intersection.P = ray.ComputePoint(intersection.t);
		// Mirroir
		if (v_objects[intersection.i_sph]->material.b_mirror)
		{
			Vector3& N = intersection.N;
			ray.Set_C(intersection.P + EPSILON *N);
			ray.Set_u(reflect(ray.Get_u(), N));
			ray.Color(intersection.Color, v_objects[intersection.i_sph]->material.emissivity);
			GetIntersectionRec(ray, i_down, i_reflexion - 1, intersection);
		}
		// Transparence
		else if (v_objects[intersection.i_sph]->material.b_transparency)
		{
			Vector3 u_I = ray.Get_u();
			Vector3& N = intersection.N;
			if (ray.IsInside()) { N *= -1; }
			double r_index;
			if (ray.IsInside())
			{
				r_index = v_objects[intersection.i_sph]->material.material_index;
			}
			else
			{
				r_index = 1 / v_objects[intersection.i_sph]->material.material_index;
			}
			double alpha = 1 - pow(r_index, 2) * (1 - pow(dot(N, u_I), 2));
			
			if (alpha > 0)	// Refraction
			{
				Vector3 T_T = r_index * (u_I - dot(u_I, N)*N);
				Vector3 T_N = -sqrt(alpha)*N;
				ray.Set_C(intersection.P - EPSILON *N);
				ray.Set_u(T_T+T_N);
				ray.Set_inside(!ray.IsInside());
				ray.Color(intersection.Color, v_objects[intersection.i_sph]->material.emissivity);
				GetIntersectionRec(ray, i_down, i_reflexion - 1, intersection);
			}
			else // Réflexion
			{
				ray.Set_C(intersection.P + EPSILON *N);
				ray.Set_u(reflect(ray.Get_u(), N));
				ray.Color(intersection.Color, v_objects[intersection.i_sph]->material.emissivity);
			}
		}
		else
		{
			ray.Color(intersection.Color);
			AddDirectComponent(ray, intersection);
			Vector3& N = intersection.N;
			intersection.N.normalization();
			double ks = v_objects[intersection.i_sph]->material.ks;
			ray.Set_C(intersection.P + EPSILON * N);
			double n = v_objects[intersection.i_sph]->material.n;
			Vector3 R = reflect(ray.Get_u(), N);
			Vector3 direction_aleatoire;
			double p = 1-ks;
			bool sample_diffuse = my_random() < p;
			if (sample_diffuse)
			{
				direction_aleatoire = random_cos(N);
			}
			else
			{
				direction_aleatoire = random_lob(R, n);
				if (dot(N, direction_aleatoire) < 0 || dot(R, direction_aleatoire)<0)
					return;
			}
			ray.Set_u(direction_aleatoire);

			double proba_phong = (n + 1) / (2 * PI) * std::pow(dot(R, direction_aleatoire), n);
			double proba_diffuse = dot(N, direction_aleatoire) / PI;
			double proba_globale = p * proba_diffuse + (1 - p) * proba_phong;
	
			if (sample_diffuse)
				ray.Color(dot(N, direction_aleatoire) / PI / proba_globale);
			else
				ray.Color(ks * compute_phong_brdf(R, direction_aleatoire, n) / proba_globale);
			GetIntersectionRec(ray, i_down-1, i_reflexion, intersection);
		}
	}
	
}

void Scene::AddDirectComponent(Ray& ray, IntersectionScene& intersection)
{
	if (intersection.b_intersect && intersection.i_sph != 0) {
		const Sphere* p_light_sphere = dynamic_cast<const Sphere*>(v_objects[0]);
		Vector3 L = p_light_sphere->Get_O();
		double R_light = p_light_sphere->Get_R();
		Vector3& u_OP = intersection.N;
		Vector3 P = intersection.P + EPSILON*u_OP;
		Vector3 u_LP = (P - L);
		u_LP.normalization();
		Vector3 u_random_light = random_cos(u_LP);
		Vector3 X = L + R_light * u_random_light; // point sur la sphère de lumière
		Vector3 u_PX = X - P; // wi
		u_PX.normalization();
		double costheta = std::max(0., dot(u_OP, u_PX));
		double costhetaprime = std::max(-dot(u_random_light, u_PX),0.);
		double costhetaseconde = dot(u_random_light, u_LP);

		double d2 = (X - P).norm_square();

		// Test de l'ombre pour la visibilité
		Ray shadow_ray;
		IntersectionScene shadow_intersection;
		shadow_ray.Set_C(P);
		shadow_ray.Set_u(u_PX);
		GetIntersectionSimple(shadow_ray, shadow_intersection);
		bool b_visible = (!shadow_intersection.b_intersect) || 
			(dot(X - P, u_PX) - 2 * EPSILON) < shadow_intersection.t;
		const Vector3 R = reflect(ray.Get_u(), u_OP);
		double brdf_fong = compute_phong_brdf(u_PX, R, v_objects[intersection.i_sph]->material.n);
		Vector3 brdf = ((1 - v_objects[intersection.i_sph]->material.ks) / PI +
			v_objects[intersection.i_sph]->material.ks * 0) * ray.Get_color();

		if (b_visible)
		{
			intersection.ColorPixel += 1 / d2 * std::max(0., costheta)*costhetaprime / costhetaseconde * brdf;
		}
	}
}
