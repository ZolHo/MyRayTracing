#pragma once

#include "hitable.h"
#include "vec3.h"
using std::swap;
# define new_sphere 1

class sphere : public hitable {
public:
	point3 ori; //圆心
	double rdu; //半径
    shared_ptr<material> mat_ptr;

public:
	sphere() {}
	sphere(const point3& origin, double radius,const shared_ptr<material>& m) :ori(origin), rdu(radius), mat_ptr(m) {}

	//关于时间t的一元二次方程式求根判别式解是否命中
	bool hit(const ray& the_ray, double time_begin, double time_end, hit_info& hit_result) const {
# if !new_sphere
		vec3 r_p = the_ray.oringin() - ori;    // 光线原点减球原点
		double a = the_ray.dir.dot(the_ray.direction());
		double b = 2 * the_ray.dir.dot(r_p);
		double c = r_p.dot(r_p) - rdu * rdu;
		double rs_discriminant = b * b - 4 * a * c;

		double disr_sqrt = sqrt(rs_discriminant);
		double root1 = (-b - disr_sqrt) / (2 * a);
		double root2 = (-b + disr_sqrt) / (2 * a);
		if (root1 > root2) swap(root1, root2);
		if (rs_discriminant > 0) {
			hit_result.mat_ptr = mat_ptr; // 设置材质
			if (root1 > 0 && root1 > time_begin && root1 < time_end) {
				hit_result.hit_pos = the_ray.at(root1);
				hit_result.distance = root1 * the_ray.dir.lenth();
				hit_result.set_face_normal(the_ray, (hit_result.hit_pos - ori) / rdu);
				return true;
			}
			// 暂时不考虑
			// 从内部往外相交，法向量反转
			//if (root2 > time_begin && root2 > time_end) { 
			//	hit_result.hit_pos = the_ray.at(root2);
			//	hit_result.distance = root1 * the_ray.dir.lenth();
			//	hit_result.set_face_normal(the_ray, (hit_result.hit_pos - ori) / rdu);
			//	return true;
			//}
		}

		return false;
#endif
#if new_sphere
		// 我自己推导的射线与球求交点方法，先计算圆心在射线上的投影t，然后利用勾股定理求出交点相对t点的偏移量来计算交点 !以假设光线的dir为单位向量
		vec3 v_p2o = ori - the_ray.ori;
		//point3 t = the_ray.ori + the_ray.dir.dot(v_p2o) * the_ray.dir;
		float k = (ori - the_ray.ori).dot(the_ray.dir);
		point3 t = the_ray.at(k);
		if (k < 0) return false; // 在内部或在负方向直接返回false

		float distance = (t - ori).lenth();
		if (distance > rdu ) return false;
		hit_result.mat_ptr = mat_ptr;
		float bias = sqrt(rdu * rdu - distance * distance);
		if (bias > k) return false;
		point3 hit_pre = the_ray.at(k-bias);

		hit_result.hit_pos = hit_pre;
		hit_result.distance = (hit_pre - the_ray.ori).lenth();
		if (hit_result.distance < time_begin || hit_result.distance > time_end) return false;
		hit_result.set_face_normal(the_ray, (hit_result.hit_pos - ori) / rdu);
		return true;
#endif
	}
};