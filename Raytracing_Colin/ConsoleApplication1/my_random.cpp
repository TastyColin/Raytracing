#include "pch.h"
#include "my_random.h"

std::default_random_engine generator[8];
std::uniform_real_distribution<double> distribution(0.0, 1.0);


double my_random(void) { return distribution(generator[omp_get_thread_num()]); }
void my_random_cos(double &x, double &y, double &z)
{
	double r1, r2;
	r1 = my_random();
	r2 = my_random();
	x = cos(2 * PI * r1) * sqrt(1 - r2);
	y = sin(2 * PI * r1) * sqrt(1 - r2);
	z = sqrt(r2);
}

Vector3 random_cos(const Vector3& N)
{
	int i_min = 0;
	for (int i = 1; i < 3; ++i)
	{
		if (abs(N[i_min]) > abs(N[i])) { i_min = i; }
	}
	Vector3 T1;
	T1[i_min] = 0;
	T1[(i_min + 1) % 3] = -N[(i_min + 2) % 3];
	T1[(i_min + 2) % 3] = N[(i_min + 1) % 3];
	T1.normalization();
	Vector3 T2 = prod(N, T1);

	double x, y, z;
	my_random_cos(x, y, z);
	return x * T1 + y * T2 + z * N;
}

void my_random_gaussian(double &x, double &y, const double &sigma)
{
	double r1, r2;
	r1 = my_random();
	r2 = my_random();
	double R = sqrt(-log(r2));
	x = sigma * R * cos(2 * PI * r1);
	y = sigma * R * sin(2 * PI * r1);
}