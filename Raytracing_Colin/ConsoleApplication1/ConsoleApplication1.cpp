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
#include "constantes.h"


void save_image(const char* filename, const unsigned char* tableau, int w, int h);


int main()
{
	// Variables de tracé
	const int NB_RAY = 100;					// Nb de rayons par pixel
	const bool b_shadowing = true;			// Ombrage
	const bool b_anti_aliasing = true;		// Anti pixelisation

	// Inialisation de la scène
	Scene scene;
		// Couleur
	const Vector3 Bleu(0, 0.7, 1);
	const Vector3 Blanc(1, 1, 1);
	const Vector3 Gris90(0.9, 0.9, 0.9);
	const Vector3 Rouge(1, 0, 0.3);
		// Caméra
	const Vector3 C(0, 0, 55);				// Position de la caméra
	double fov = 60. * PI / 180.;			// Champs de vue (60°)
	const int W = 512;						// Largeur (en pixels)
	const int H = 512;						// Hauteur (en pixels)
	std::vector<unsigned char> img(W*H * 3, 0);	// initialisation du tableau de l'image
		// Lumière
	const Vector3 L(-15, 10, 40);			// Position de la lumière
	const Vector3 color_light = Blanc;		// Couleur
	const double R_light =1;
	const double I_light = powf(2, 28);		// Puissance de la lumière (2^28)

	const Material Light{ Sphere(L,R_light), Blanc, 1 };
	scene.SetLight(Light);					// Ajout des sphères à la scène

		// Sphères
	const Vector3 O1(0, 0, 0);
	const Vector3 O2(40, 0, -50);
	const Sphere Sph1(O1, 10);
	const Sphere Sph2(O2, 5);
	const Material S1{ Sph1, Bleu, 0, false, false, 1.5 };
	const Material S2{ Sph2, Rouge, 0, false, false, 1.5 };
	scene.AddSphere(S1);					// Ajout des sphères à la scène
	scene.AddSphere(S2);

	// Déclaration des variables
	double dx = 0;
	double dy = 0;
	Vector3 X, P, O, u_OP, u_LP,v, u_random_light, u_PX, I_pixel, I_pixel_aux;
	Ray light_ray;
	Ray shadow_ray;
	IntersectionScene light_intersection;
	IntersectionScene shadow_intersection;

#pragma omp parallel for
	for (int i = 0; i < W; i++)
	{
		for (int j = 0; j < H; j++)
		{
			I_pixel.Reset();
			
			for (int k_ray = 0; k_ray < NB_RAY; ++k_ray)
			{
				if (b_anti_aliasing) my_random_gaussian(dx, dy, 0.25);
				v[0] = j - W / 2 + 0.5 + dx;
				v[1] = -i + H / 2 + 0.5 + dy;
				v[2] = -H / (2 * tan(fov / 2));
				v.normalization();

				// Initialisation du rayon
				light_ray.Set_C(C);
				light_ray.Set_u(v);
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
	int i = 371;
	int j = 377;
	for (int k = 0; k < 3; k++)
	{
		img[(i*W + j) * 3 + k] = int(255);
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