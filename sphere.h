#pragma once

#include "hitable.h"
#include "vec3.h"

class sphere : public hitable {
public:
	point3 ori; //圆心
	double rdu;   //半径

public:
	sphere() {}
	sphere(const point3& origin, double radius) :ori(origin), rdu(radius) {}

	//关于时间t的一元二次方程式求根判别式解是否命中
	bool hit(const ray& light, double time_begin, double time_end, hit_info& hit_result) const {
		vec3 r_p = light.oringin() - ori;    // 光线原点减球原点
		double a = light.dir.dot(light.direction());
		double b = 2 * light.dir.dot(r_p);
		double c = r_p.dot(r_p) - rdu * rdu;
		double rs_discriminant = b * b - 4 * a * c;
		return rs_discriminant > 0;
	}
};