#pragma once
class Vector3
{

	friend Vector3 operator+(const Vector3 &, const Vector3 &);
	friend Vector3 operator-(const Vector3 &, const Vector3 &);
	friend Vector3 operator*(const Vector3 &, const Vector3 &);
	friend Vector3 operator*(const double &, const Vector3 &);



public:
	Vector3();
	Vector3(double x, double y, double z);
	Vector3(const Vector3 &source);
	~Vector3();

	Vector3 &operator+=(const Vector3 & other);
	Vector3 &operator-=(const Vector3 & other);

	double operator[](int i) const;
	double &operator[](int i);

	double norm_square() const;

	void normalization(void);

protected:
	double coords[3];
};

double dot(const Vector3&, const Vector3&);