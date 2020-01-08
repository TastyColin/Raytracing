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

const double PI = 3.14159265359;

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



int main()
{
	Vector3 C(0, 0, 55);
	Vector3 O1(0, 0, 0);
	Vector3 O2(10, 0, -40);
	Vector3 L(-10, 20, 40);
	double I_light = 750000000;
	Sphere Sph1(O1, 10);
	Sphere Sph2(O2, 20);
	Scene scene;
	scene.AddSphere(Sph1);
	scene.AddSphere(Sph2);
	double fov = 60. * PI / 180.;
	int W = 1024;
	int H = 1024;
	std::vector<unsigned char> img(W*H * 3, 0);

	for (int i = 0; i < W; i++)
	{
		for (int j = 0; j < H; j++)
		{
			Vector3 v(j - W / 2 + 0.5, -i + H / 2 + 0.5, -H / (2 * tan(fov / 2)));
			v.normalization();
			Ray ray(C, v, Vector3(1,1,1));
			IntersectionScene intersection = scene.GetIntersection(ray);
			if (intersection.b_intersect) {
				Vector3 P = intersection.P;
				Vector3 O = scene.v_spheres[intersection.i_sph].sphere.Get_O();
				Vector3 u_PO = (P - O);
				u_PO.normalization();
				Vector3 u_LP = (L - P);
				u_LP.normalization();
				double n2_PL = (P - L).norm_square();
				double I_pixel = I_light / n2_PL * dot(u_LP, u_PO);
				I_pixel = pow(I_pixel, 0.45);
				I_pixel = fmax(0, I_pixel);
				//I_pixel = pow(I_pixel, 0.45);
				I_pixel = fmin(255, I_pixel);
				for (int k = 0; k < 3; k++) img[(i*W + j ) * 3 + k] = int(I_pixel*intersection.color[k]);
			}
		}
	}

	save_image("test.bmp", &img[0], W, H);

	Vector3 M(1, 2, 3);
	Vector3 N(2, 4, 6);
	double result = M.norm_square();
	std::cout << result;

	return 0;
}
