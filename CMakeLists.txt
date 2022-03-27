cmake_minimum_required(VERSION 3.21)
project(MyRayTracing)

set(CMAKE_CXX_STANDARD 14)

include_directories(.)

add_executable(MyRayTracing
        main.cpp
        camera.h
        hitable.h
        material.h
        ray.h
        sphere.h
        tools.h
        vec3.h
        )
