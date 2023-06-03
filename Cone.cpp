#include "Cone.h"
#include <math.h>
#include <stdio.h>

template<class T>
const T& min(const T& a, const T& b)
{
    return (b < a) ? b : a;
}


static void printVec(glm::vec3 c){
	printf("x : %f, y : %f, z : %f\n", c.x, c.y, c.z);
}


static double a_term(
    double dx, 
    double dz, 
    double dy, 
    double r2
    ){return (dx * dx) + (dz * dz) - (r2*(dy * dy));
}

static double b_term(
    double dx, 
    double dz, 
    double dy, 
    double xo, 
    double xc, 
    double zo, 
    double zc,
    double yo,
    double yc,
    double h,
    double r2
    ){return (2*(xo-xc)*dx) + (2*(zo-zc)*dz) + (2*r2*(h-yo+yc)*dy);
}

static double c_term(
    double xo, 
    double xc, 
    double zo, 
    double zc,
    double yo,
    double yc,
    double h,
    double r2
    ){
    return ((xo - xc)*(xo - xc)) + ((zo-zc)*(zo-zc)) - (r2*((h-yo+yc)*(h-yo+yc)));
}

// DONT TOUCH ANYTHING IT WORKS
float Cone::intersect(glm::vec3 p0, glm::vec3 dir){
    double r2 = (radius / hight) * (radius / hight);
    double a = a_term(dir.x, dir.z, dir.y, r2);
    double b = b_term(dir.x, dir.z, dir.y, p0.x, center.x, p0.z, center.z, p0.y, center.y, hight, r2);
    double c = c_term(p0.x, center.x, p0.z, center.z, p0.y, center.y, hight, r2);
    double det = b*b - 4*a*c;
    if (det < 0.001) return -1.0;

    double t1 = (-b-sqrt(det))/(2*a);
    double t2 = (-b+sqrt(det))/(2*a);
    double t;

    if (t1>t2) t = t2;
    else t = t1;
    
    float r = p0.y + t*dir.y;

    if ((r >= center.y) && (r <= center.y+hight_lim)){
        return t;
    }
    if (((p0.y + t2 * dir.y) < center.y + hight_lim) && ((p0.y + t1 * dir.y) > center.y + hight_lim)){
        return t2-t1;
    };
    return -1;

}


glm::vec3 Cone::normal(glm::vec3 p){
    float h = hight_lim > hight? hight:hight_lim; 
    if (p.y >= center.y + hight_lim){
        return glm::vec3(0, 1, 0);
    }
    float r = sqrt((p.x-center.x)*(p.x-center.x) + (p.z-center.z)*(p.z-center.z));
    glm::vec3 n = glm::vec3 (p.x-center.x, r*(radius/hight), p.z-center.z);
    float div_constant = sqrt(n.x*n.x + n.y*n.y + n.z*n.z);
    n /= div_constant;
    return n;
};