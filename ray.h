#pragma once

#include "vec3.h"

class ray {
public:
	vec3 dir;
	point3 ori;
public:
	ray() {}
	ray(const point3& origin, const vec3& direction) : ori(origin), dir(direction) {}
	point3 oringin() const { return ori; }
	vec3 direction() const { return dir; }

	point3 at(double time) const {
		return ori + time * dir;
	}

};