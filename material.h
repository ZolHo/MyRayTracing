#pragma once

#include "vec3.h"
#include "hitable.h"
#include "ray.h"
struct hit_info;

class material {
public:
    bool is_light = false;
public:
	virtual bool scatter(const ray& r_in, const hit_info& rec, color& attenuation, ray& scattered) const = 0;
};

// 兰伯特散射
class lambertian : public material {
public:
    lambertian(const color& a) : albedo(a){};
    bool scatter(const ray &r_in, const hit_info &rec, color &attenuation, ray &scattered) const override {
        vec3 scatter_direction = rec.normal + random_unit_vector();
        scattered = ray(rec.hit_pos, scatter_direction);
        attenuation = albedo;
        return true;
    }
public:
    color albedo; //反射率
};

// 镜面反射 
class metal : public material {
public :
    color albedo;
public:
    metal(const color& c) :albedo(c) {}

    virtual bool scatter(const ray& r_in, const hit_info& rec, color& attenuation, ray& scattered) const override {
        scattered.dir = reflect(r_in.direction().unit(), rec.normal);
        scattered.ori = rec.hit_pos;
        attenuation = albedo;
        return (scattered.direction().dot(rec.normal) > 0);
    }
};

// 光源
class light : public material {
public:
    color light_color;
    double indensity;
public:
    light(const color& c) : light_color(c) {
        is_light = true;
    }
    virtual bool scatter(const ray& r_in, const hit_info& rec, color& attenuation, ray& scattered) const override {
        return true;
    }
};

// 我设想的glossy算法
class glossy : public material {
public :
    color albedo;
    unsigned int glossy_intensity = 0; // 该值越大越接近镜面，越小越接近漫反射
public :
    glossy(const color& c, int intensity) : albedo (c), glossy_intensity(intensity) {}

    virtual bool scatter(const ray& r_in, const hit_info& rec, color& attenuation, ray& scattered) const override {
        vec3 scatter_direction = rec.normal * (glossy_intensity + 1) + random_unit_vector();
        scattered = ray(rec.hit_pos, scatter_direction);
        attenuation = albedo;
        return true;
    }
};