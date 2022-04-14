#pragma once

#include <cmath>
#include <iostream>
#include "tools.h"
using std::sqrt;
using std::cout;

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
	vec3 operator*(const vec3& v) const {
		return vec3(v.e[0] * e[0], v.e[1] * e[1], v.e[2] * e[2]);
	}
	vec3 operator/(double d) const {
		if (d != 0) return *this * (1 / d);
	}
	vec3 operator/(const vec3& v) const {
		vec3 rs;
		if (my_abs(v.e[0]) < FLOAT_DIS) rs.e[0] = infinity;
		else rs.e[0] = e[0] / v.e[0];
		if (my_abs(v.e[1]) < FLOAT_DIS) rs.e[1] = infinity;
		else rs.e[1] = e[1] / v.e[1];
		if (my_abs(v.e[2]) < FLOAT_DIS) rs.e[2] = infinity;
		else rs.e[2] = e[2] / v.e[2];
		return rs;
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

	// 求最值
	double min_v() {
		double rs = infinity;
		for (auto v : e) {
			if (v < infinity && v > -infinity && v < rs) rs = v;
		}
		return rs;
	}
	double max_v() {
		double rs = -infinity;
		for (auto v : e) {
			if (v < infinity && v > -infinity && v > rs) rs = v;
		}
		return rs;
	}

	// 判等
	bool operator == (const vec3& b) {
		return (*this - b).lenth() < FLOAT_DIS;
	}
	bool operator != (const vec3& b) {
		return !(*this == b);
	}


public:
	double e[3];
};
using point3 = vec3;
using color = vec3;

inline vec3 operator*(double d, const vec3& to) {
	return to * d;
}

inline vec3 reflect(const vec3& v, const vec3& n) {
	return v - 2 * v.dot(n) * n;  //sin(a+90) = cos(a)
}

inline vec3 random_unit_vector() {
	auto a = random_double(0, 2 * pi);
	auto z = random_double(-1, 1);
	auto r = sqrt(1 - z * z);
	return vec3(r * cos(a), r * sin(a), z);
}