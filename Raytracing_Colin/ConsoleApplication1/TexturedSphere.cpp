#include "pch.h"
#include "TexturedSphere.h"


TexturedSphere::TexturedSphere(const Vector3& O, double R, Material m, const char* texture_name) :
	Sphere(O, R, m)
{
	load_texture(texture_name);
}


TexturedSphere::~TexturedSphere()
{
}


void TexturedSphere::load_texture(const char* filename) {

	FILE* f;
	errno_t err;
	err = fopen_s(&f, (std::string("models/") + std::string(filename)).c_str(), "rb");
	unsigned char info[54];
	fread(info, sizeof(unsigned char), 54, f); // read the 54-byte header

	w = *(int*)&info[18]; // extract image height and width from header
	h = *(int*)&info[22];

	int size = 3 * w * h;
	texture.resize(size);
	fread(&texture[0], sizeof(unsigned char), size, f); // read the rest of the data at once
	fclose(f);

	for (int i = 0; i < size; i += 3) {
		std::swap(texture[i], texture[i + 2]);
	}
}


void TexturedSphere::GetIntersection(const Ray& ray, IntersectionObject& intersection) const
{
	Vector3 C = ray.Get_C();
	Vector3 u = ray.Get_u();
	intersection.b_intersect = false;
	intersection.t = 0;
	// Résolution eq degrès 2
	double a, b, c, delta, t1, t2;
	a = 1.;
	b = 2. * dot(u, C - O);
	c = (C - O).norm_square() - R * R;
	delta = b * b - 4 * a*c;
	t1 = (-b - sqrt(delta)) / (2 * a);
	t2 = (-b + sqrt(delta)) / (2 * a);
	if (t1 > 0)
	{
		intersection.b_intersect = true;
		intersection.t = t1;
		Vector3 P = ray.ComputePoint(t1);
		intersection.N = (C + intersection.t * u - O) / R;
		_GetColor((P - O) / R, intersection.Color);
		return;
	}
	if (t2 > 0)
	{
		intersection.b_intersect = true;
		intersection.t = t2;
		Vector3 P = ray.ComputePoint(t2);
		intersection.N = (C + intersection.t * u - O) / R;
		_GetColor((P - O) / R, intersection.Color);
	}
}


void TexturedSphere::_GetColor(const Vector3& P, Vector3& Color) const
{
	double p1 = std::fmax(std::fmin(P[1], 1), -1);
	int y = h * (PI - acos(p1)) / PI;
	int x = w * (PI + atan2(P[0], P[2])) / (2 * PI);
	for (int k = 0; k < 3; ++k)
	{
		Color[k] = (texture[(y * w + x) * 3 + k]) / 255.;
	}
}