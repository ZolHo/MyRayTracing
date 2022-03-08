#pragma once

#include "ray.h"

struct hit_info {
	point3 hit_pos;
	vec3 normal;
	double distance;
};

class hitable {
public:
	virtual bool hit(const ray& r, double time_begin, double time_end, hit_info& hit_result) const = 0;
};