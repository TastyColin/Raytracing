#pragma once
#include <math.h>
#include <iostream>

class Vector3
{

	friend Vector3 operator+(const Vector3 &, const Vector3 &);
	friend Vector3 operator-(const Vector3 &, const Vector3 &);
	friend Vector3 operator*(const Vector3 &, const Vector3 &);
	friend Vector3 operator*(const double &, const Vector3 &);
	friend Vector3 operator/(const Vector3 &, const double &);



public:
	Vector3(const double& x = 0, const double& y = 0, const double& z = 0);
	Vector3(const Vector3 &source);
	~Vector3();

	Vector3 &operator+=(const Vector3 & other);
	Vector3 &operator-=(const Vector3 & other);
	Vector3 &operator*=(const Vector3 & other);
	Vector3 &operator*=(const double &);
	Vector3 &operator/=(const double &);
	Vector3 &operator=(const Vector3& other);
	Vector3 operator-(void) const;

	void Reset(void);

	double operator[](int i) const;
	double &operator[](int i);

	double norm_square() const;

	void normalization(void);

	void print(void) const;

protected:
	double coords[3];
};

double dot(const Vector3&, const Vector3&);
Vector3 prod(const Vector3& U, const Vector3& V);
Vector3 reflect(const Vector3& wi, const Vector3& N);


