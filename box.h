#pragma once

#include "hitable.h"
#include "vec3.h"
using std::swap;
#include "tools.h"
// 轴对齐长方体
class box : public hitable {
public:
	vec3 box_ori;
	// x, y, z三轴的半长
	vec3 xyz_half_len;
	shared_ptr<material> mat_ptr;

public: 
	box(const point3& ori,const vec3& xyz, const shared_ptr<material>& m) : box_ori(ori), mat_ptr(m) {
		xyz_half_len = xyz / 2;
	}

	virtual bool hit(const ray& r, double time_begin, double time_end, hit_info& hit_result) const {
		// 射线与AABB包围盒求交过程
		//float x_min, x_max, y_min, y_max, z_min, z_max;
		vec3 times_in, times_out, temp;
		times_in = (box_ori - xyz_half_len - r.ori) / (r.direction());
		times_out = (box_ori + xyz_half_len - r.ori) / (r.direction());

		temp = vec3(my_min(times_in.x(), times_out.x()), my_min(times_in.y(), times_out.y()), my_min(times_in.z(), times_out.z()));
		times_out = vec3(my_max(times_in.x(), times_out.x()), my_max(times_in.y(), times_out.y()), my_max(times_in.z(), times_out.z()));
		times_in = temp;

		float in_max = times_in.max_v();
		float out_min = times_out.min_v();
		float hit_time = in_max > 0?in_max:out_min;

		if (in_max <= 0 || out_min <= 0 || in_max >= out_min || hit_time<time_begin || hit_time>time_end ) return false;

		hit_result.hit_pos = r.at(hit_time);
		hit_result.distance = (hit_result.hit_pos - r.ori).lenth();
		hit_result.mat_ptr = mat_ptr;
		int temp_i = 0;
		for (; temp_i < 3; temp_i++) {
			if (my_abs(hit_time - times_in.e[temp_i]) < FLOAT_DIS ) break;
			//if (hit_time == times_in.e[temp_i]) break;
		}
		vec3 normal;
		switch (temp_i) {
			case(0): normal = vec3((hit_result.hit_pos.x() > box_ori.x() ? 1 : -1), 0, 0); break;
			case(1): normal = vec3(0, (hit_result.hit_pos.y() > box_ori.y() ? 1 : -1), 0); break;
			case(2): normal = vec3(0, 0,( hit_result.hit_pos.z() > box_ori.z() ? 1 : -1)); break;
			//default: times_in << (cout); cout << hit_time <<" "<< hit_time - times_in.e[2] <<" "<< abs(hit_time - times_in.e[2])<< std::endl;
			default: normal = vec3();
		}
		hit_result.set_face_normal(r, normal);
		//(hit_result.hit_pos) << (cout);
		//normal << (cout);
		//cout << std::endl;
		//hit_result.normal = normal;
		return true;
		
	}
};