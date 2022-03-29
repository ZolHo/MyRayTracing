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
using std::cin;
using std::cout;
using std::vector;
using std::shared_ptr;
using std::make_shared;

// 俄罗斯轮盘赌算法参数
const float RR_p = 0.8;

bool is_RR = true;

// 画面自定义参数
int samping = 300; // samping per pixel
int pixel_width = 1080; // 屏幕宽
char* file_name = "test2.ppm";

bool muti_hit(const ray& r, const vector<shared_ptr<hitable>>& things, double t_begin, double t_end, hit_info& rec);

template<class T>
T clamp(T x, T min, T max)
{
	if (x > max)
		return max;
	if (x < min)
		return min;
	return x;
}
color ray_color(const ray& r, const vector<shared_ptr<hitable>>& things, int deep) {
	hit_info rec;
	color att;
	ray sca;
	if (!is_RR && deep <= 0) return color(0, 0, 0);
	if (muti_hit(r, things, 0.00001, infinity, rec) && (*(rec.mat_ptr)).scatter(r, rec, att, sca)) {
		if (rec.mat_ptr->is_light) {
			//if(!is_RR)
			return static_cast<light*>(rec.mat_ptr.get())->light_color;
			//else return static_cast<light*>(rec.mat_ptr.get())->light_color *(1./RR_p);
		}
		if (is_RR) {
			bool go_on = random_double() < RR_p;
			if (go_on) {
				return att * ray_color(sca, things, deep - 1) * (1. / RR_p);
			}
			else return vec3(0, 0, 0);
		}
		else {
			return att * ray_color(sca, things, deep - 1);
		}
	}
	return color(0, 0, 0);
}
// 处理光线多个物体的求交，返回在前面的交点的hit info
bool muti_hit(const ray& r, const vector<shared_ptr<hitable>>& things, double t_begin, double t_end, hit_info& rec) {
	bool is_hit = false;
	rec.distance = infinity;
	hit_info rs_temp;
	for (auto thing : things) {
		if (thing->hit(r, t_begin, t_end, rs_temp)) {
			is_hit = true;
			if (rs_temp.distance < rec.distance) rec = rs_temp;
		}
	}
	return is_hit;
}

void write_color(std::ostream& out, color pixel_color, int samping) {
	auto r = pixel_color.x();
	auto g = pixel_color.y();
	auto b = pixel_color.z();
	float scale;
	//根据样本数对颜色取平均值
	if (is_RR) scale = 1.0 / samping;
	else scale = 1.0 / samping;
	r = sqrt(r * scale);
	g = sqrt(g * scale);
	b = sqrt(b * scale);
	out << static_cast<int>(256 * clamp(r, 0.0, 0.999)) << ' '
		<< static_cast<int>(256 * clamp(g, 0.0, 0.999)) << ' '
		<< static_cast<int>(256 * clamp(b, 0.0, 0.999)) << '\n';
	
}

int main(int argc, char *argv[]) {
	if (argc > 2) {
		samping = atoi(argv[1]);
		pixel_width = atoi(argv[2]);
		if(argc==4) file_name = argv[3];
	}
	srand((unsigned int)time(NULL));//随机种子
	const double aspect_ratio = 16.0 / 9.0;
	const int pixel_height = static_cast<int> (pixel_width / aspect_ratio);
	const bool is_ssaa = false;   //是否开启4倍超采样（废弃）
    const bool is_random_select = true;
	std::ofstream out_file;
	out_file.open(file_name, std::ios::out | std::ios::trunc);

	cout << "samping: " << samping << "  resolution: " <<pixel_height<<"*"<< pixel_width << "  file_name: " << file_name << std::endl;

	const int ray_deep = 8;
	const int pixel_samping = samping;
    shared_ptr<lambertian> lbt_ptr(new lambertian({1-117./255.,1-49./255.,1- 142./255.}));
	shared_ptr<lambertian> lbt_red(new lambertian({ 0.9,0,0 }));
	shared_ptr<lambertian> lbt_green(new lambertian({ 0,0.9,0 }));
	shared_ptr<lambertian> lbt_white(new lambertian({ 0.9,0.9,0.9 }));
	shared_ptr<lambertian> lbt_ptr2(new lambertian({ 117. / 255., 49. / 255.,  142. / 255. }));
	shared_ptr<lambertian> lbt_ptr3(new lambertian({ random_double(), random_double(), random_double() }));
	shared_ptr<metal> metal_ptr(new metal({ 0.85, 0.83, 0.87 }));
	shared_ptr<light> light_ptr(new light({ 1,1,1 }));

	// world
	vector<shared_ptr<hitable>> world;

	world.push_back(make_shared<box>(point3(0, -4, 0), vec3(20, 4, 20), lbt_white));
	world.push_back(make_shared<box>(point3(0, 4, 0), vec3(20, 4, 20), lbt_white));
	world.push_back(make_shared<box>(point3(0, 0, -5), vec3(8, 8, 2), lbt_white));
	world.push_back(make_shared<box>(point3(-5, 0, 0), vec3(4, 8, 8), lbt_green));
	world.push_back(make_shared<box>(point3(5, 0, 0), vec3(4, 8, 8), lbt_red));
	//world.push_back(make_shared<box_light>(point3(0, 1.98, -2), vec3(1, 0.04, 1), light_ptr));
	world.push_back(make_shared<box_light>(point3(0, 1.98, -1.5), vec3(1, 0.04, 1), light_ptr));
	world.push_back(make_shared<box>(point3(-1, -0.5, -1.3), vec3(1.4, 3, 1.4), lbt_white));
	world.push_back(make_shared<sphere>(point3(0.5, -1, 0), 1, lbt_ptr3));
	world.push_back(make_shared<sphere>(point3(1.4, 1, -0.4), 0.5, metal_ptr));


	// 相机
	camera cam;

	// render
	double u, v;
	ray r;
	//cout << "P3\n" << pixel_width << " " << pixel_height << "\n255\n";
	out_file << "P3\n" << pixel_width << " " << pixel_height << "\n255\n";

	//Tip：这里因为用用了整数，导致unit_sssaa值为0
	double unit_ssaa[2] = { 1 / (2.0 * (pixel_width - 1)), 1 / (2.0 * (pixel_height - 1)) };
	double SSAA[4][2] = { {-1,-1}, {-1, 1}, {1, -1}, {1,1} };
	for (int i = 0; i < 4; i++) {
		SSAA[i][0] *= unit_ssaa[0];
		SSAA[i][1] *= unit_ssaa[1];
	}
	int jin_du_tiao = 0;
	for (int j = pixel_height - 1; j >= 0; --j) {
		for (int i = 0; i < pixel_width; ++i) {
			u = double(i) / (pixel_width - 1);
			v = double(j) / (pixel_height - 1);
			color final_col;

			if (is_ssaa) {
				// 超采样
				color sum_col = color();

				for (int p = 0; p < 4; p++) {
					r = cam.get_ray(u + SSAA[p][0], v + SSAA[p][1]);
					auto temp = ray_color(r, world,ray_deep);
					sum_col += temp;
				}
				final_col = sum_col / 4;
			}
			else if(is_random_select) {
				color sum_col = color();
                for (int p = 0; p < pixel_samping; p++) {
                    double ut = u + random_double(0,2)*unit_ssaa[0];
                    double uv = v + random_double(0, 2) * unit_ssaa[1];
                    r = cam.get_ray(ut, uv);
					//r = cam.get_ray(u, v);
                    auto temp = ray_color(r,world, ray_deep);
                    sum_col+=temp;
                }
                //final_col = sum_col / pixel_samping;
				final_col = sum_col;
			} else {
                final_col = ray_color(cam.get_ray(u, v), world, ray_deep);
            }

			//write_color(cout, final_col, pixel_samping);
			write_color(out_file, final_col, pixel_samping);
		}
		float percent = ((float)(pixel_height - j) / (float)pixel_height) * 100.;
		if (percent > jin_du_tiao * 5) {
			if(jin_du_tiao != 0) cout << " -> ";
			cout << jin_du_tiao * 5<< "%";
			jin_du_tiao++;
		}
			 
	}
	cout << " ->  Done!" << std::endl;
	out_file.close();
    return 0;
}