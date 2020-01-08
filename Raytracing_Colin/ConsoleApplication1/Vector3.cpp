#include "pch.h"
#include "Vector3.h"
#include <math.h>


Vector3::Vector3(double x = 0, double y = 0, double z = 0)
{
	coords[0] = x;
	coords[1] = y;
	coords[2] = z;
}


Vector3::Vector3(const Vector3 &source)
{
	for (int i = 0; i < 3; i++) { coords[i] = source.coords[i]; }
}



Vector3::~Vector3()
{
}


double Vector3::operator[](int i) const
{
	return coords[i];
}

double &Vector3::operator[](int i)
{
	return coords[i];
}

// Opérateurs de bases

Vector3& Vector3::operator+=(const Vector3 &other)
{
	for (int i = 0; i < 3; i++) { coords[i] += other.coords[i]; }
	return *this;
}

Vector3& Vector3::operator-=(const Vector3 &other)
{
	for (int i = 0; i < 3; i++) { coords[i] -= other.coords[i]; }
	return *this;
}




// Opérateurs standards

Vector3 operator+(const Vector3 &V1, const Vector3 &V2)
{
	Vector3 result = V1;
	return result += V2;
}

Vector3 operator-(const Vector3 &V1, const Vector3 &V2)
{
	Vector3 result = V1;
	return result -= V2;
}

Vector3 operator*(const Vector3 &V1, const Vector3 &V2)
{
	Vector3 res(0,0,0);
	for (int i = 0; i < 3; i++) { res[i] = V1[i] * V2[i]; }
	return res;
}

Vector3 operator*(const double &scal, const Vector3 &V)
{
	Vector3 result = V;
	for (int i = 0; i < 3; i++) { result[i] *= scal; }
	return result;
}

// Autres fonctions

double Vector3::norm_square() const
{
	double result = 0;
	for (int i = 0; i < 3; i++) { result += coords[i]*coords[i]; }
	return result;
}

void Vector3::normalization()
{
	double n = sqrt(norm_square());
	if (n > 0)
	{
		for (int i = 0; i < 3; i++) { coords[i] /= n; }
	}
}

double dot(const Vector3& v1, const Vector3& v2)
{
	double result = 0;
	for (int i = 0; i < 3; i++)
	{
		result += v1[i] * v2[i];
	}
	return result;
}