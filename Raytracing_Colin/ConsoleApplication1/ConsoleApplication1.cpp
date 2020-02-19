#include "pch.h"

#define _CRT_SECURE_NO_WARNINGS 1
#include <vector>
#include <iostream>
#include <math.h>

#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb_image_write.h"

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

#include "Vector3.h"
#include "Ray.h"
#include "Sphere.h"
#include "Scene.h"
#include "Mesh.h"
#include "constantes.h"


void save_image(const char* filename, const unsigned char* tableau, int w, int h);


int main()
{
	// Variables de tracé
	const int NB_RAY = 1000;					// Nb de rayons par pixel
	const bool b_anti_aliasing = true;		// Anti pixelisation

	// Inialisation de la scène
	Scene scene;
		// Couleur
	const Vector3 Bleu(0, 0.7, 1);
	const Vector3 Blanc(1, 1, 1);
	const Vector3 Gris90(0.9, 0.9, 0.9);
	const Vector3 Rouge(1, 0, 0.3);
	const Vector3 Vert(0, 1, 0.3);


	const Material RightMaterial{ Vector3(1,0.2,0.8) };
	const Material LeftMaterial{ Vector3(1, 0, 0) };
	const Material UpMaterial{ Vector3(0.2,0.8,1) };
	const Material BottomMaterial{ Vector3(0,0,1) };
	const Material FrontMaterial{ Vector3(1,1,0.95) };
	const Material BackMaterial = FrontMaterial;

	const Sphere WallRight(Vector3(1000, 0, 0), 940, RightMaterial);
	const Sphere WallLeft(Vector3(-1000, 0, 0), 940, LeftMaterial);
	const Sphere WallTop(Vector3(0, 1000, 0), 940, UpMaterial);
	const Sphere WallBottom(Vector3(0, -1000, 0), 940, BottomMaterial);
	const Sphere WallFront(Vector3(0, 0, 1000), 940, FrontMaterial);
	const Sphere WallBack(Vector3(0, 0, -1000), 900, BackMaterial);

	scene.AddObject(WallRight);
	scene.AddObject(WallLeft);
	scene.AddObject(WallTop);
	scene.AddObject(WallBottom);
	scene.AddObject(WallFront);
	scene.AddObject(WallBack);
	

		// Caméra
	const Vector3 O_camera(0, 0, 55);		// Position de la caméra
	double fov = 60. * PI / 180.;			// Champs de vue (60°)
	const int W = 512;						// Largeur (en pixels)
	const int H = 512;						// Hauteur (en pixels)
	const double ouverture = 5.;				// largeur de l'ouverture du diaphragme
	const double focus_distance = 115;		// distance de mise au point
	std::vector<unsigned char> img(W*H * 3, 0);	// initialisation du tableau de l'image

		// Lumière
	const Vector3 L(-15, 10, 40);			// Position de la lumière
	const Vector3 color_light = Blanc;		// Couleur
	const double R_light = 1;
	const double I_light = powf(2, 29);		// Puissance de la lumière (2^28)
	Material LightMaterial{ Blanc, 1 };
	Sphere light_sph(L, R_light, LightMaterial);
	scene.SetLight(light_sph);					// Ajout des sphères à la scène
		// Sphères
	const Vector3 O1(0, 0, 0);
	const Vector3 O2(40, 0, -50);
	Material BleuMaterial{ Bleu, 0, false, false, 1.5 };
	Material RougeMaterial{ Rouge, 0, false, false, 1.5 };
	Material VertMaterial{ Vert, 0, false, false, 1.5 };
	Material BlancMaterial{ Blanc, 0, false, false, 1.5 };
	Material MirrorMaterial{ Blanc, 0, true, false, 1.5 };

	/*Sphere Sph1(O1, 10, BleuMaterial);
	Sphere Sph2 = Sphere(O2, 5, RougeMaterial);
	scene.AddObject(Sph1);					// Ajout des sphères à la scène
	scene.AddObject(Sph2);
	Triangle triangle(O1, O2, Vector3(20, 40, -25), VertMaterial);
	scene.AddObject(triangle);*/

	Mesh g1("girl.obj", 50, Vector3(0, -60, -60), BlancMaterial);
	//Mesh g2("girl.obj", 30, Vector3(40, -60, -80), RougeMaterial);
	scene.AddObject(g1);
	//scene.AddObject(g2);

	// Déclaration des variables
	double dx = 0;
	double dy = 0;
	

#pragma omp parallel for
	for (int i = 0; i < W; i++)
	{
		Vector3 ray_direction, I_pixel, O_ray, destination;
		Ray light_ray;
		Ray shadow_ray;
		IntersectionScene light_intersection;
		IntersectionScene shadow_intersection;
		for (int j = 0; j < H; j++)
		{
			I_pixel.Reset();
			
			for (int k_ray = 0; k_ray < NB_RAY; ++k_ray)
			{
				if (b_anti_aliasing) my_random_gaussian(dx, dy, 0.25);
				ray_direction[0] = j - W / 2 + 0.5 + dx;
				ray_direction[1] = -i + H / 2 + 0.5 + dy;
				ray_direction[2] = -H / (2 * tan(fov / 2));
				ray_direction.normalization();

				// Initialisation du rayon
				if (ouverture)
				{
					dx = (my_random() - 0.5)*ouverture;
					dy = (my_random() - 0.5)*ouverture;
					destination = O_camera + focus_distance * ray_direction;
					O_ray = O_camera + Vector3(dx, dy, 0);
					ray_direction = destination - O_ray;
					ray_direction.normalization();
				}
				else
				{
					O_ray = O_camera;
				}
				light_ray.Set_C(O_ray);
				light_ray.Set_u(ray_direction);
				light_ray.Set_color(color_light);
				light_ray.Set_inside(false);

				scene.GetIntersection(light_ray, light_intersection);
				I_pixel += light_intersection.Color;
			}
			for (int k = 0; k < 3; ++k)
			{
				I_pixel[k] = pow(I_pixel[k]* I_light / NB_RAY, 0.45);
				I_pixel[k] = fmin(255, I_pixel[k]);
			}

			for (int k = 0; k < 3; k++)
			{
				img[(i*W + j) * 3 + k] = int(I_pixel[k]);
			}

		}
	}
	save_image("image.bmp", &img[0], W, H);
	return 0;
}


void save_image(const char* filename, const unsigned char* tableau, int w, int h) { // (0,0) is top-left corner

	FILE *f;

	int filesize = 54 + 3 * w*h;

	unsigned char bmpfileheader[14] = { 'B','M', 0,0,0,0, 0,0, 0,0, 54,0,0,0 };
	unsigned char bmpinfoheader[40] = { 40,0,0,0, 0,0,0,0, 0,0,0,0, 1,0, 24,0 };
	unsigned char bmppad[3] = { 0,0,0 };

	bmpfileheader[2] = (unsigned char)(filesize);
	bmpfileheader[3] = (unsigned char)(filesize >> 8);
	bmpfileheader[4] = (unsigned char)(filesize >> 16);
	bmpfileheader[5] = (unsigned char)(filesize >> 24);

	bmpinfoheader[4] = (unsigned char)(w);
	bmpinfoheader[5] = (unsigned char)(w >> 8);
	bmpinfoheader[6] = (unsigned char)(w >> 16);
	bmpinfoheader[7] = (unsigned char)(w >> 24);
	bmpinfoheader[8] = (unsigned char)(h);
	bmpinfoheader[9] = (unsigned char)(h >> 8);
	bmpinfoheader[10] = (unsigned char)(h >> 16);
	bmpinfoheader[11] = (unsigned char)(h >> 24);

	f = fopen(filename, "wb");
	fwrite(bmpfileheader, 1, 14, f);
	fwrite(bmpinfoheader, 1, 40, f);
	unsigned char *row = new unsigned char[w * 3];
	for (int i = 0; i < h; i++)
	{
		for (int j = 0; j < w; j++) {
			row[j * 3] = tableau[(w*(h - i - 1) * 3) + j * 3 + 2];
			row[j * 3 + 1] = tableau[(w*(h - i - 1) * 3) + j * 3 + 1];
			row[j * 3 + 2] = tableau[(w*(h - i - 1) * 3) + j * 3];
		}
		fwrite(row, 3, w, f);
		fwrite(bmppad, 1, (4 - (w * 3) % 4) % 4, f);
	}
	fclose(f);
	delete[] row;
}