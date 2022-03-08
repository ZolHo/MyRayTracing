#pragma once

#include "hitable.h"
#include "vec3.h"

class sphere : public hitable {
public:
	point3 ori; //Բ��
	double rdu;   //�뾶

public:
	sphere() {}
	sphere(const point3& origin, double radius) :ori(origin), rdu(radius) {}

	//����ʱ��t��һԪ���η���ʽ����б�ʽ���Ƿ�����
	bool hit(const ray& light, double time_begin, double time_end, hit_info& hit_result) const {
		vec3 r_p = light.oringin() - ori;    // ����ԭ�����ԭ��
		double a = light.dir.dot(light.direction());
		double b = 2 * light.dir.dot(r_p);
		double c = r_p.dot(r_p) - rdu * rdu;
		double rs_discriminant = b * b - 4 * a * c;
		return rs_discriminant > 0;
	}
};