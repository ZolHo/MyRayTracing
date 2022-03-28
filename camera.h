#pragma once
#include "vec3.h"
#include "ray.h"

class camera {
public:
	point3 ori;         //ԭ��
	vec3 dir;           //�������
	double focal_lenth; //��Ļ����
	double height;      //�ӿڸ߶�
	double aspect_ratio;//��Ұ�����

	double weight;      //��   
	point3 low_left_corner;//��Ļ�����½Ƕ�Ӧ�ĵ�����
	vec3 horizontal;    //�ӿ�ˮƽ����
	vec3 vertical;      //�ӿڴ�ֱ����
public:
	camera(double ratio = 16.0/9.0,const point3& oringe=point3(0,0,5), double focal=2, double hei=2.0, const vec3& direct=vec3(0.0,0.0,-1.0)):
		ori(oringe), dir(direct), focal_lenth(focal), height(hei), aspect_ratio(ratio) {
		
		weight = aspect_ratio * height;

		horizontal = vec3(weight, 0., 0.);
		vertical = vec3(0., height, 0.);

		low_left_corner = point3(-weight / 2, -height / 2, ori.e[2]-focal_lenth);
	}

	ray get_ray(double u, double v) const {
		return ray(ori, low_left_corner + u * horizontal + v * vertical - ori);
	}

};