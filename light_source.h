#pragma once
#include "vec3.h"
#include "hitable.h"
class sq_area_light :public hitable {
public:
	vec3 ori;
	float len;
};