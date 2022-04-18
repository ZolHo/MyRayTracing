#pragma once

#include<cmath>
#include<cstdlib>
#include<limits>
#include<memory>
#include<random>
#include<time.h>

using std::shared_ptr;
using std::make_shared;
using std::sqrt;



//常量
const double infinity = std::numeric_limits<double>::infinity(); //无穷大
const double pi = 3.1415926535897932385;

inline float my_max(float a, float b) {return a > b ? a : b; }
inline float my_min(float a, float b) { return a < b ? a : b; }
const float FLOAT_DIS = 0.00000001;
//inline float fabs(float a) { return a > 0 ? a : -a; }

//函数
inline double degrees_to_radians(double degrees) { //度数到弧度
    return degrees * pi / 180.0;
}

inline double random_double() {
    //[0,1)
    return rand() / (RAND_MAX + 1.0);
}

inline double random_double(double min, double max) {
    //[min, max)s
    return min + random_double() * (max - min);
}

double RadicalInverse(int Base, int i)
{
	double Digit, Radical, Inverse;
	Digit = Radical = 1.0 / (double)Base;
	Inverse = 0.0;
	while (i)
	{
		// i余Base求出i在"Base"进制下的最低位的数
		// 乘以Digit将这个数镜像到小数点右边
		Inverse += Digit * (double)(i % Base);
		Digit *= Radical;

		// i除以Base即可求右一位的数
		i /= Base;
	}
	return Inverse;
}
int NthPrimeNumber(int d) {
	int pri[] = { 2, 3, 5, 7,11 };
	if (d < 5) {
		return pri[d];
	}
	return 0;
}
double Halton(int Dimension, int Index)
{
	// 直接用第Dimension个质数作为底数调用RadicalInverse即可
	return RadicalInverse(NthPrimeNumber(Dimension), Index);
}