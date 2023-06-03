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
    glm::vec3 temp(0); 
    
    if (t1 < t2) temp = p0 + (float) t1 *dir;
    else temp = p0 + (float) t2 *dir;
    
    if ((temp.y >= center.y) && (temp.y <= center.y+hight)) return t1; 
    else if ((temp.y > center.y + hight) && ((p0.y + t2 * dir.y) <= center.y + hight)) return t2-t1;
    
    // if nothing works then it doesnt intersect
    return -1;
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

glm::vec2 Cylinder::getTextcoq(glm::vec3 hit){
    static float max = 0;
    glm::vec3 n = hit - center;
    if (n.y >= hight){
        n = glm::normalize(n);    
        return glm::vec2(1, n.x); 
    }
    float v = (n.y)/hight;
    n = glm::normalize(n);    
    float u = (atan2(n.x, n.z)+M_PI)/(2*M_PI);
    return glm::vec2(u, v);
}
