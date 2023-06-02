#ifndef TORUS_H
#define TORUS_H

#include <glm/glm.hpp>
#include "SceneObject.h"

class Torus : SceneObject
{
private:

    unsigned int vaoID;
	static const float TWOPI;
	int nelms;  //Total number of elements
    float innerRadius = 1;
    float outerRadius = 1;
    glm::vec3 centre;

public:
    Torus(float ir, float outr, glm::vec3 c) : innerRadius(ir), outerRadius(outr), centre(c){}
    float intersect(glm::vec3 p0, glm::vec3 dir);
    glm::vec3 normal(glm::vec3 p);
};

#endif //TORUS_H
