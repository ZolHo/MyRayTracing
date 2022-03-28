#pragma once
#include "vec3.h"
#include "hitable.h"
#include "tools.h"
#include "box.h"

class box_light :public box {
public:
	box_light(const point3& ori, const vec3& xyz, const shared_ptr<material>& m) : box (box_ori, xyz_half_len, mat_ptr){
		box_ori = ori;
		xyz_half_len = xyz;
		mat_ptr = m;
	}
};