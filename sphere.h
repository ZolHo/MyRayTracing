#pragma once

#include "hitable.h"
#include "vec3.h"
using std::swap;

class sphere : public hitable {
public:
	point3 ori; //Բ��
	double rdu; //�뾶
    shared_ptr<material> mat_ptr;

public:
	sphere() {}
	sphere(const point3& origin, double radius,const shared_ptr<material>& m) :ori(origin), rdu(radius), mat_ptr(m) {}

	//����ʱ��t��һԪ���η���ʽ����б�ʽ���Ƿ�����
	bool hit(const ray& the_ray, double time_begin, double time_end, hit_info& hit_result) const {
		vec3 r_p = the_ray.oringin() - ori;    // ����ԭ�����ԭ��
		double a = the_ray.dir.dot(the_ray.direction());
		double b = 2 * the_ray.dir.dot(r_p);
		double c = r_p.dot(r_p) - rdu * rdu;
		double rs_discriminant = b * b - 4 * a * c;

		double disr_sqrt = sqrt(rs_discriminant);
		double root1 = (-b - disr_sqrt) / (2 * a);
		double root2 = (-b + disr_sqrt) / (2 * a);
		if (root1 > root2) swap(root1, root2);
		if (rs_discriminant > 0) {
			hit_result.mat_ptr = mat_ptr; // ���ò���
			if (root1 > 0 && root1 > time_begin && root1 < time_end) {
				hit_result.hit_pos = the_ray.at(root1);
				hit_result.distance = root1 * the_ray.dir.lenth();
				hit_result.set_face_normal(the_ray, (hit_result.hit_pos - ori) / rdu);
				return true;
			}

			// ���ڲ������ཻ����������ת
			if (root2 > time_begin && root2 > time_end) { 
				hit_result.hit_pos = the_ray.at(root2);
				hit_result.distance = root1 * the_ray.dir.lenth();
				hit_result.set_face_normal(the_ray, (hit_result.hit_pos - ori) / rdu);
				return true;
			}
		}

		return false;
	}
};