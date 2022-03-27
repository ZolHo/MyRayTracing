#pragma once

#include "ray.h"

class material;

struct hit_info {
	point3 hit_pos; // 击中处
	vec3 normal;    // 击中点法线
	double distance;// 离光源距离
    shared_ptr<material> mat_ptr; // 材质指针
    bool front_face;

    inline void set_face_normal(const ray &r, const vec3& outward_normal) {
        front_face = r.direction().dot(outward_normal) < 0;
        normal = front_face ? outward_normal: -outward_normal;
    }

    inline void print() {
        cout << "hit pos:";
        hit_pos << (cout);
        cout  << "normal:";
        normal << (cout);

    }
};

class hitable {
public:
	virtual bool hit(const ray& r, double time_begin, double time_end, hit_info& hit_result) const = 0;
};