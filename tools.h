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

inline float my_max(float a, float b) { return a > b ? a : b; }
inline float my_min(float a, float b) { return a < b ? a : b; }
const float FLOAT_DIS = 0.000000005;
//inline float fabs(float a) { return a > 0 ? a : -a; }

//常量
const double infinity = std::numeric_limits<double>::infinity(); //无穷大
const double pi = 3.1415926535897932385;

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