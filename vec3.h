#pragma once

#include <cmath>
#include <iostream>
using std::sqrt;

class vec3 {
public:
	//构造
	vec3() :e{ 0,0,0 } {};
	vec3(double e0, double e1, double e2) {
		e[0] = e0; e[1] = e1; e[2] = e2;
	}
	//返回
	double x() const { return e[0]; }
	double y() const { return e[1]; }
	double z() const { return e[2]; }
	//运算
	vec3 operator+(const vec3& to) const {
		return vec3(e[0] + to.e[0], e[1] + to.e[1], e[2] + to.e[2]);
	}
	vec3 operator-(const vec3& to) const {
		return vec3(e[0]-to.e[0], e[1]-to.e[1], e[2]-to.e[2]);
	}
	vec3 operator*(double d) const {
		return vec3(d*e[0], d*e[1], d*e[2]);
	}
	vec3 operator/(double d) const {
		if (d != 0) return *this * (1 / d);
	}
	vec3& operator+=(const vec3& to) {
		e[0] += to.e[0];
		e[1] += to.e[1];
		e[2] += to.e[2];
		return *this;
	}
	vec3& operator-=(const vec3& to) {
		e[0] -= to.e[0];
		e[1] -= to.e[1];
		e[2] -= to.e[2];
		return *this;
	}
	vec3& operator*=(double d) {
		e[0] *= d;
		e[1] *= d;
		e[2] *= d;
		return *this;
	}
	vec3& operator/=(double d) {
		e[0] /= d;
		e[1] /= d;
		e[2] /= d;
		return *this;
	}
	vec3 operator-() const {
		return vec3(-e[0], -e[1], -e[2]);
	}
	double operator[] (int i) const {
		return e[i];
	}
	double& operator[] (int i) {
		return e[i];
	}

	double dot(const vec3& to) const {
		return e[0] * to.e[0] + e[1] * to.e[1] + e[2] * to.e[2];
	}
	vec3 cross(const vec3& to) const {
		return vec3(e[1] * to.e[2] - e[2] * to.e[1],
			e[2] * to.e[0] - e[0] * to.e[2],
			e[0] * to.e[1] - e[1] * to.e[0]);
	}

	//常用函数
	double lenth() const {
		return sqrt(e[0] * e[0] + e[1] * e[1] + e[2] * e[2]);
	}

	std::ostream& operator<<(std::ostream& out) {
		return out << e[0] << " " << e[1] << " " << e[2] << std::endl;
	}
	//单位化
	vec3 unit() const { 
		return *this / lenth();
	}

public:
	double e[3];
};
using point3 = vec3;
using color = vec3;

inline vec3 operator*(double d, const vec3& to) {
	return to * d;
}