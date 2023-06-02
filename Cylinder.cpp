#include "Cylinder.h"
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


static double a_term(double dx, double dz){
    return dx*dx + dz*dz;
}

static double b_term(double dx, double dz, double xo, double xc, double zo, double zc){
    return 2*(dx*(xo-xc) + dz*(zo-zc));
}

static double c_term(double xo, double xc, double zo, double zc, double r){
    return ((xo-xc)*(xo-xc))+((zo-zc)*(zo-zc))-r*r;
}

float Cylinder::intersect(glm::vec3 p0, glm::vec3 dir){
    glm::vec3 vdif = p0 - center;
    double a = a_term(dir.x, dir.z);
    double b = b_term(dir.x, dir.z, p0.x, center.x, p0.z, center.z);
    double c = c_term(p0.x, center.x,p0.z, center.z, radius);
    double det = b*b - 4*a*c;

	if(det < 0.001) return -1.0;    //includes zero and negative values


    double t1 = (-b-sqrt(det))/(2*a);
    double t2 = (-b+sqrt(det))/(2*a);
    double t;
    if (t1 < t2) t = t1;
    else t = t2;
    
    glm::vec temp = p0 + (float) t *dir; 
 
    if ((temp.y >= center.y) && (temp.y <= center.y+hight)){
        return t1;
    } else if (temp.y > center.y + hight){
        if ((p0.y + t2 * dir.y) <= center.y + hight){
            return t2-t1;
        }
    }

    return -1;

    // if (((t1*dir.y + p0.y) > center.y+hight) && ((t2*dir.y + p0.y) < center.y+hight)){
    //     t1 = ((center.y+hight-p0.y));
    // } else if (temp.y < center.y || temp.y > center.y+hight){
    //     return -1.0;
    // }

	// if (t1 < 0)
	// {
	// 	return (t2 > 0) ? t2 : -1;
	// }
	// else return t1;
}


glm::vec3 Cylinder::normal(glm::vec3 p){
    glm::vec3 norm = p - center;
    if (p.y >= center.y+hight){
        return glm::vec3(0, 1, 0);
    }
    norm.y = 0;
    norm /= radius;
    return norm;
};