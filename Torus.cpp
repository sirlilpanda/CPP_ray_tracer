//  ========================================================================
//  COSC422: Computer Graphics (2022);  University of Canterbury.
//
//  FILE NAME: Torus.cpp
//  This file contains functions to generate mesh data for a torus and to 
//   generate buffer objects. It also contains a render function. 
//  ========================================================================

#include <iostream>
#include "Torus.h"
#include <glm/glm.hpp>
#include "SceneObject.h"
#include <cmath>
using namespace std;

const float Torus:: TWO_PI = 6.2831853;


float Torus::intersect(glm::vec3 p0, glm::vec3 dir){
    
}

glm::vec3 Torus::normal(glm::vec3 p){
    
    /* vertex position */
    vx = cos(j_angle)*(rout+cos(i_angle)*rin);
    vy = sin(j_angle)*(rout+cos(i_angle)*rin);
    vz = sin(i_angle)*rin;
    
    /* tangent vector with respect to big circle */
    tx = -sin(j_angle);
    ty = cos(j_angle);
    tz = 0;
    
    /* tangent vector with respect to little circle */
    sx = cos(j_angle)*(-sin(i_angle));
    sy = sin(j_angle)*(-sin(i_angle));
    sz = cos(i_angle);
    
    /* normal is cross-product of tangents */
    nx = ty*sz - tz*sy;
    ny = tz*sx - tx*sz;
    nz = tx*sy - ty*sx;

    /* normalize normal */
    length = sqrt(nx*nx + ny*ny + nz*nz);
    nx /= length;
    ny /= length;
    nz /= length;
}

