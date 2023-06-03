/*----------------------------------------------------------
* COSC363  Ray Tracer
*
*  The sphere class
*  This is a subclass of Object, and hence implements the
*  methods intersect() and normal().
-------------------------------------------------------------*/

#include "Sphere.h"
#include <math.h>
#include <stdio.h>

/**
* Sphere's intersection method.  The input is a ray. 
*/
float Sphere::intersect(glm::vec3 p0, glm::vec3 dir)
{
    glm::vec3 vdif = p0 - center;   //Vector s (see Slide 28)
    float b = glm::dot(dir, vdif);
    float len = glm::length(vdif);
    float c = len*len - radius*radius;
    float delta = b*b - c;
   
	if(delta < 0.001) return -1.0;    //includes zero and negative values

    float t1 = -b - sqrt(delta);
    float t2 = -b + sqrt(delta);

	if (t1 < 0)
	{
		return (t2 > 0) ? t2 : -1;
	}
	else return t1;
}

/**
* Returns the unit normal vector at a given point.
* Assumption: The input point p lies on the sphere.
*/
glm::vec3 Sphere::normal(glm::vec3 p)
{
    glm::vec3 n = p - center;
    n = glm::normalize(n);
    return n;
}

glm::vec2 Sphere::getTextcoq(glm::vec3 hit){
    glm::vec3 n = glm::normalize(hit - center);
    float u = atan2(n.x, n.z) / (2*M_PI) + 0.5;
    float v = n.y * 0.5 + 0.5;
    return glm::vec2(u, v);
}
