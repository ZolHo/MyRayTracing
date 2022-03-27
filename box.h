#pragma once

#include "hitable.h"
#include "vec3.h"
using std::swap;
#define max(a,b) (((a) > (b)) ? (a) : (b))
#define min(a,b) (((a) < (b)) ? (a) : (b))
#define FLOAT_DIS 0.000001
#define abs(a) a>0? a : -a
// 轴对齐长方体
class box : public hitable {
public:
	vec3 box_ori;
	// x, y, z三轴的半长
	float x_len;
	float y_len;
	float z_len;
	shared_ptr<material> mat_ptr;

public: 
	box(const point3& ori,const vec3& xyz, const shared_ptr<material>& m) : box_ori(ori), mat_ptr(m) {
		x_len = xyz.x()/2;
		y_len = xyz.y()/2;
		z_len = xyz.z()/2;
	}

	virtual bool hit(const ray& r, double time_begin, double time_end, hit_info& hit_result) const {
		// 射线与AABB包围盒求交过程
		float x_min, x_max, y_min, y_max, z_min, z_max;
		if (abs(r.dir.x()) > FLOAT_DIS) {
			x_min = (box_ori.x() - x_len - r.ori.x()) / r.dir.x();
			x_max = (box_ori.x() + x_len - r.ori.x()) / r.dir.x();
			if (x_min > x_max) swap(x_min, x_max);
		} 
		else {
			x_min = -INFINITY; x_max = INFINITY;
		}

		if (abs(r.dir.y()) > FLOAT_DIS) {
			y_min = (box_ori.y() - y_len - r.ori.y()) / r.dir.y();
			y_max = (box_ori.y() + y_len - r.ori.y()) / r.dir.y();
			if (y_min > y_max) swap(y_min, y_max);
		}
		else {
			y_min = -INFINITY; y_max = INFINITY;
		}

		if (abs(r.dir.z()) > FLOAT_DIS) {
			z_min = (box_ori.z() - z_len - r.ori.z()) / r.dir.z();
			z_max = (box_ori.z() + z_len - r.ori.z()) / r.dir.z();
			if (z_min > z_max) swap(z_min, z_max);
		}
		else {
			z_min = -INFINITY; z_max = INFINITY;
		}

		float in_max = max(max(x_min, y_min), z_min);
		float out_min = min(min(x_max, y_max), z_max);

		if (in_max > out_min || out_min < 0) return false; // 不存在交点
	

		float time_list[] = { x_min, x_max, y_min, y_max, z_min, z_max };

		float hit_time = in_max > 0 ? in_max  : out_min;

		hit_result.hit_pos =  r.at(hit_time);
		hit_result.distance = (hit_result.hit_pos - r.ori).lenth();
		hit_result.mat_ptr = mat_ptr;
		vec3 normal;
		// 求法线
		for (int i = 0; i < 6; i++) {
			float f = time_list[i];
			if (abs(f - hit_time) < FLOAT_DIS) {
				i /= 2;
				switch (i) {
				case(0): normal = vec3(hit_result.hit_pos.x() > box_ori.x()?-1:1, 0, 0); break;
				case(1): normal = vec3(0, hit_result.hit_pos.y() > box_ori.y()?-1:1,0); break;
				case(2): normal = vec3(0, 0, hit_result.hit_pos.z() > box_ori.z()?-1:1); break;
				}
				break;
			}
		}

		hit_result.set_face_normal(r, normal.unit());
		//hit_result.print();
		return true;
	}
};