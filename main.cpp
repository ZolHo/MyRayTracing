#include <iostream>

#include "camera.h"
#include "vec3.h"
#include "hitable.h"
#include "sphere.h"
#include <vector>
#include "tools.h"
#include "material.h"
#include <cmath>
#include <algorithm>
#include "box.h"
#include "light_source.h"
#include <stdio.h>
#include <fstream>
#include <chrono>
using std::cin;
using std::cout;
using std::vector;
using std::shared_ptr;
using std::make_shared;

// ����˹���̶��㷨����
const float RR_p = 0.8;

// �����Զ������
int samping = 300; // samping per pixel
int pixel_width = 1080; // ��Ļ��
char* file_name = "test.ppm";
bool is_low_discrepancy_seq = true; // ���õͲ������д������
// ��������
//float blur_ind = 1.;

bool muti_hit(const ray& r, const vector<shared_ptr<hitable>>& things, double t_begin, double t_end, hit_info& rec);

//�Զ��� clampģ�庯��
template<class T>
T clamp(T x, T min, T max)
{
	if (x > max)
		return max;
	if (x < min)
		return min;
	return x;
}

// ·��׷�ٺ��ĵݹ��㷨
color ray_color(const ray& r, const vector<shared_ptr<hitable>>& things) {
	hit_info rec; // ��¼������Ϣ
	color att;    // ��¼���㴦��ķ�����
	ray sca;      // ��¼��һ������
	// ��������������ཻ��
	if (muti_hit(r, things, 0.00001, infinity, rec)) {
		// ����ǹ�Դ�򷵻�
		if (rec.mat_ptr->is_light) {
			return static_cast<light*>(rec.mat_ptr.get())->light_color;
		}
		// �������������һ��
		bool go_on = random_double() < RR_p;
		if (go_on) {
			(*(rec.mat_ptr)).scatter(r, rec, att, sca);
			return att * ray_color(sca, things) * (1. / RR_p);
		}
		else return color(0, 0, 0);
		
	}
	return color(0, 0, 0);
}

// �������ȫ��������󽻣�������ǰ��Ľ����hit info����ʹ��BVH�Ż�
bool muti_hit(const ray& r, const vector<shared_ptr<hitable>>& things, double t_begin, double t_end, hit_info& rec) {
	bool is_hit = false;
	rec.distance = infinity;
	hit_info rs_temp;
	// ��������
	for (auto thing : things) {
		if (thing->hit(r, t_begin, t_end, rs_temp)) {
			is_hit = true;
			if (rs_temp.distance < rec.distance) rec = rs_temp;
		}
	}
	return is_hit;
}

// ����PPM��ʽ��һ�����ص���������������
void write_color(std::ostream& out, color pixel_color, int samping) {
	auto r = pixel_color.x();
	auto g = pixel_color.y();
	auto b = pixel_color.z();
	float scale;
	//��������������ɫȡƽ��ֵ
	scale = 1.0 / samping;
	r = sqrt(r * scale);
	g = sqrt(g * scale);
	b = sqrt(b * scale);
	out << static_cast<int>(256 * clamp(r, 0.0, 0.999)) << ' '
		<< static_cast<int>(256 * clamp(g, 0.0, 0.999)) << ' '
		<< static_cast<int>(256 * clamp(b, 0.0, 0.999)) << '\n';
}


int main(int argc, char *argv[]) {

	// -----------------------     ��ʼ��    -----------------------

	//���ο��Ʋ��������ֱ��ʣ� �ļ���
	if (argc > 2) {
		samping = atoi(argv[1]);
		pixel_width = atoi(argv[2]);
		if(argc==4) file_name = argv[3];
	}
	srand((unsigned int)time(NULL));       // �������
	const double aspect_ratio = 16.0 / 9.0;// FOV
	const int pixel_height = static_cast<int> (pixel_width / aspect_ratio);
    // const bool is_random_select = true; // ���� 
	const int pixel_samping = samping;     // ������
	std::ofstream out_file;
	// PPM�ļ�ͷ
	out_file.open(file_name, std::ios::out | std::ios::trunc);
	out_file << "P3\n" << pixel_width << " " << pixel_height << "\n255\n";

	// ����̨��Ϣ
	cout << "samping: " << samping << "  resolution: " <<pixel_height<<"*"<< pixel_width << "  file_name: " << file_name << std::endl;

	// ��ʼ��ʹ�õ��Ĳ�������
    shared_ptr<lambertian> lbt_ptr(new lambertian({1-117./255.,1-49./255.,1- 142./255.}));
	shared_ptr<lambertian> lbt_red(new lambertian({ 0.5,0,0 }));
	shared_ptr<lambertian> lbt_green(new lambertian({ 0,0.5,0 }));
	shared_ptr<lambertian> lbt_white(new lambertian({ 0.6,0.6,0.6 }));
	shared_ptr<lambertian> lbt_ptr2(new lambertian({ 117. / 255., 49. / 255.,  142. / 255. }));
	shared_ptr<lambertian> lbt_ptr3(new lambertian({ random_double(), random_double(), random_double() }));
	shared_ptr<metal> metal_ptr(new metal({ 0.85, 0.83, 0.87 }));
	shared_ptr<light> light_ptr(new light({ 10,10,10 }));

	shared_ptr<glossy> glossy_2(new glossy({ 0.9, 0.4, 0.4 }, 2));
	shared_ptr<glossy> glossy_4(new glossy({ 0.4, 0.9, 0.4 }, 4));
	shared_ptr<glossy> glossy_8(new glossy({ 0.9, 0.9, 0.9 }, 8));
	shared_ptr<glossy> glossy_16(new glossy({ 0.8, 0.9, 0.8 }, 16));

	// world
	vector<shared_ptr<hitable>> world;
	// ����������Ӽ����壬���������ǲ���
	world.push_back(make_shared<box>(point3(0, -4, 0), vec3(20, 4, 20), lbt_white));
	world.push_back(make_shared<box>(point3(0, 4, 0), vec3(20, 4, 20), lbt_white));
	world.push_back(make_shared<box>(point3(0, 0, -5), vec3(8, 8, 2), lbt_white));
	world.push_back(make_shared<box>(point3(-5, 0, 0), vec3(4, 8, 8), lbt_green));
	world.push_back(make_shared<box>(point3(5, 0, 0), vec3(4, 8, 8), lbt_red));
	world.push_back(make_shared<box_light>(point3(0, 1.98, -1.5), vec3(1, 0.04, 1), light_ptr));
	world.push_back(make_shared<box>(point3(-1, -0.5, -1.3), vec3(1.4, 3, 1.4), lbt_white));
	world.push_back(make_shared<sphere>(point3(0.5, -1, 0), 1, lbt_ptr3));
	//world.push_back(make_shared<box>(point3(1.5, -1.5, 0), vec3(1,1,1), glossy_16));
	world.push_back(make_shared<sphere>(point3(1.4, 1, -0.4), 0.5, metal_ptr));

	//world.push_back(make_shared<sphere>(point3(0, -4, 0), 2, lbt_white));
	//world.push_back(make_shared<sphere>(point3(0, 4, 0), 2, lbt_white));
	//world.push_back(make_shared<sphere>(point3(0, 0, -5), 2, lbt_white));
	//world.push_back(make_shared<sphere>(point3(-5, 0, 0), 2, lbt_green));
	//world.push_back(make_shared<sphere>(point3(5, 0, 0), 1, lbt_red));
	//world.push_back(make_shared<box_light>(point3(0, 1.98, -1.5), vec3(1, 0.04, 1), light_ptr));
	//world.push_back(make_shared<sphere>(point3(-1, -0.5, -1.3), 2, lbt_white));
	//world.push_back(make_shared<sphere>(point3(0.5, -1, 0), 1, lbt_ptr3));
	////world.push_back(make_shared<box>(point3(1.5, -1.5, 0), vec3(1,1,1), glossy_16));
	//world.push_back(make_shared<sphere>(point3(1.4, 1, -0.4), 0.5, metal_ptr));


	// ��ʼ�������ʹ��Ĭ�ϲ���
	camera cam;

	// ��ʼ��ʱ�����ڲ鿴��Ⱦʱ��
	auto startTime = std::chrono::system_clock::now();
	
	
	// -----------------------    ��ʼ����    -----------------------

	// ��ʱ����
	double u, v;
	ray r;

	// Tip��������bug��Ϊ����������������unit_sssaaֵΪ0
	double unit_pixel[2] = { 1. / pixel_width , 1. / pixel_height }; // ��λ���ض�Ӧ��λuv
	//cout << unit_pixel[0] << " " << unit_pixel[1] << std::endl;
	int idx_low_dis_seq = 0;// �Ͳ������в���
	int jin_du_tiao = 0;    // ����̨����������
	for (int j = pixel_height - 1; j >= 0; --j) {
		for (int i = 0; i < pixel_width; ++i) {
			// �����������������
			//TODO: ���Թ����õȾ�����������ʲô��ͬ (�Ͳ�������)

			// �������ض�Ӧuv
			u = double(i) / (pixel_width - 1);
			v = double(j) / (pixel_height - 1);
			color final_col;
			color sum_col = color();

			for (int p = 0; p < pixel_samping; p++) {
				// ut,vt��ʾԭuv���� ���/�Ͳ������� ƫ��
				double ut = u ;
				double vt = v ;
				if (is_low_discrepancy_seq) {
					ut += Halton(0, idx_low_dis_seq) * unit_pixel[0];
					vt += Halton(1, idx_low_dis_seq) * unit_pixel[1];
					idx_low_dis_seq++;
				}
				else {
					ut += random_double(0, 1) * unit_pixel[0];
					vt += random_double(0, 1) * unit_pixel[1];
				}
				
				// ��������Ͷ�����
				r = cam.get_ray(ut, vt);
				//r = cam.get_ray(u, v);
				auto temp = ray_color(r, world);
				sum_col += temp;
			}
			final_col = sum_col;
			auto tp_vec = final_col / pixel_samping;
			
			write_color(out_file, final_col, pixel_samping);
		}

		//-----------------------    ��������    -----------------------

		//�����н����� windows only
		float percent = ((float)(pixel_height - j) / (float)pixel_height) * 100.;
		if (percent > jin_du_tiao * 5) {
			if(jin_du_tiao != 0) cout << " -> ";
			cout << jin_du_tiao * 5<< "%";
			jin_du_tiao++;
		}
			 
	}
	cout << " ->  Done!" << std::endl;
	// ͳ������ʱ��
	auto endTime = std::chrono::system_clock::now();
	std::cout << "time:" << std::chrono::duration_cast<std::chrono::seconds>(endTime - startTime).count() <<" s"<< std::endl;
	out_file.close();
    return 0;
}