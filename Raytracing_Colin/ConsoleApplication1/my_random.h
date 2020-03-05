#pragma once

#include "constantes.h"
#include "Vector3.h"
#include <random>
#include <omp.h>
#include <math.h>


double my_random(void);
void my_random_cos(double &x, double &y, double &z);
Vector3 random_cos(const Vector3& N);
Vector3 random_lob(const Vector3& R, const double& n);
void my_random_gaussian(double &x, double &y, const double &sigma);
