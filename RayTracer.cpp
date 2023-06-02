/*==================================================================================
* COSC 363  Computer Graphics (2023)
* Department of Computer Science and Software Engineering, University of Canterbury.
*
* A basic ray tracer
* See Lab06.pdf   for details.
*===================================================================================
*/
#include <iostream>
#include <cmath>
#include <vector>
#include <stdio.h>
#include <glm/glm.hpp>
#include "Sphere.h"
#include "SceneObject.h"
#include "Ray.h"
#include "Plane.h"
#include "Cone.h"
#include "Cylinder.h"
#include <GL/freeglut.h>
#include <glm/gtc/random.hpp>
#include <thread>
using namespace std;

const float EDIST = 40.0;
const int NUMDIV = 500;
const int MAX_STEPS = 6;
const float XMIN = -10.0;
const float XMAX = 10.0;
const float YMIN = -10.0;
const float YMAX = 10.0;
const float AMBIENT_SCALE_FACTOR = 0.2;
const float REFACTIVE_INDEX = 1.0;
// const int MAX_SMAPLES = 2;

vector<SceneObject*> sceneObjects;

bool flag_ran = 0;

void printVec(glm::vec3 c){
	printf("x : %f, y : %f, z : %f\n", c.x, c.y, c.z);
}

template <typename T> int sgn(T val) {
    return (T(0) < val) - (val < T(0));
}

//---The most important function in a ray tracer! ---------------------------------- 
//   Computes the colour value obtained by tracing a ray and finding its 
//     closest point of intersection with objects in the scene.
//----------------------------------------------------------------------------------
glm::vec3 trace(Ray ray, int step){
	glm::vec3 backgroundCol(1,1,1);						//Background colour = (0,0,0)
	// glm::vec3 lightPos(-0, 29.0, 10.0);					//Light's position
	glm::vec3 lightPos(-0, 10, -10.0);					//Light's position
	glm::vec3 color(0);
	SceneObject* obj;

    ray.closestPt(sceneObjects);					//Compare the ray with all objects in the scene
    if(ray.index == -1) return backgroundCol;		//no intersection
	obj = sceneObjects[ray.index];					//object on which the closest point of intersection is found

	// if (ray.index == 0)
	// {
	// 	//Stripe pattern
	// 	int stripeWidth = 5;
	// 	int stripeLength = 5;
	// 	int iz = (ray.hit.z) / stripeWidth;
	// 	int ix = (ray.hit.x) / stripeLength;
	// 	int kz = iz % 2; //inversion factor
	// 	int kx = ix % 2; //2 colors
	// 	if (ray.hit.x <= 0){
	// 		if (kx == 0){
	// 			if (kz == 0) color = glm::vec3(0, 1, 0);
	// 			else color = glm::vec3(1, 1, 0.5);
	// 		} else {
	// 			if (kz == 0) color = glm::vec3(1, 1, 0.5);
	// 			else color = glm::vec3(0, 1, 0);
	// 		}
	// 	}else{
	// 		if (kx == 0){
	// 			if (kz == 0) color = glm::vec3(1, 1, 0.5);
	// 			else color = glm::vec3(0, 1, 0);
	// 		} else {
	// 			if (kz == 0) color = glm::vec3(0, 1, 0);
	// 			else color = glm::vec3(1, 1, 0.5);
	// 		}
	// 	}
		
	// // 	// if (kx == 0) color = glm::vec3(0, 1, 0);
	// 	obj->setColor(color);
	// }
	// if this is just colour you see the objs colour
	// lighting give it the dark and light shading
	color = obj->lighting(lightPos, -ray.dir, ray.hit);						//Object's colour



	if (obj->isReflective() && step < MAX_STEPS) {
		float rho = obj->getReflectionCoeff();
		glm::vec3 normalVec = obj->normal(ray.hit);
		glm::vec3 reflectedDir = glm::reflect(ray.dir, normalVec);
		Ray reflectedRay(ray.hit, reflectedDir);
		glm::vec3 reflectedColor = trace(reflectedRay, step + 1);
		color += (rho * reflectedColor);
	}

	if (obj->isRefractive() && step < MAX_STEPS) {
		float eta = REFACTIVE_INDEX/obj->getRefractiveIndex();
		glm::vec3 n = obj->normal(ray.hit);
		glm::vec3 g = glm::refract(ray.dir, n, eta);
		Ray internal_ray = Ray(ray.hit, g);
		internal_ray.closestPt(sceneObjects);
		glm::vec3 m = obj->normal(internal_ray.hit);
		Ray new_ray = Ray(internal_ray.hit, glm::refract(g, -m, 1/eta));
		glm::vec3 refractiveColor = trace(new_ray, step + 1);
		color += (refractiveColor * obj->getRefractionCoeff());
	}

	// shading
	glm::vec3 lightVec = lightPos - ray.hit; //dir of light
	Ray shadowRay(ray.hit, lightVec);
	shadowRay.closestPt(sceneObjects);
	if ((shadowRay.index > -1) && (shadowRay.dist < glm::length(lightVec) && step < MAX_STEPS) ){
		bool flag = 1;
		if (sceneObjects[shadowRay.index]->isRefractive() && sceneObjects[ray.index] != sceneObjects[shadowRay.index]){
			color *= sceneObjects[shadowRay.index]->getRefractionCoeff()*sceneObjects[shadowRay.index]->getColor();
		}
		if (sceneObjects[shadowRay.index]->isRefractive()){
			flag = 0;
		}

		if (flag){
			color *= AMBIENT_SCALE_FACTOR;
		}
	}
	return color;
}

//---The main display module -----------------------------------------------------------
// In a ray tracing application, it just displays the ray traced image by drawing
// each cell as a quad.
//---------------------------------------------------------------------------------------
void display()
{
	float xp, yp;  //grid point
	float cellX = (XMAX - XMIN) / NUMDIV;  //cell width
	float cellY = (YMAX - YMIN) / NUMDIV;  //cell height
	glm::vec3 eye(0., 0., 0.);

	glClear(GL_COLOR_BUFFER_BIT);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();


	for (int i = 0; i < NUMDIV; i++)	//Scan every cell of the image plane
	{	
		printf("%.2f%%\r\n", ((float) i / (float) NUMDIV) * 100);
	
		xp = XMIN + i * cellX;
		for (int j = 0; j < NUMDIV; j++)
		{
			
			yp = YMIN + j * cellY;

			glm::vec3 dir(xp + 0.5 * cellX, yp + 0.5 * cellY, -EDIST);	//direction of the primary ray

			Ray ray = Ray(eye, dir);

			glm::vec3 col = trace(ray, 0); //Trace the primary ray and get the colour value
			// this is so its a real
			glBegin(GL_QUADS);  //Each cell is a tiny quad.
			glColor3f(col.r, col.g, col.b);
			glVertex2f(xp, yp);				//Draw each cell with its color value
			glVertex2f(xp + cellX, yp);
			glVertex2f(xp + cellX, yp + cellY);
			glVertex2f(xp, yp + cellY);
		    glEnd();	
		    glFlush();
		}
	}
}

Plane* createWall(glm::vec3 a,glm::vec3 b,glm::vec3 c,glm::vec3 d, glm::vec3 colour){
	Plane *wall = new Plane (
		a, b, c, d
	);
	wall->setSpecularity(true);
	wall->setColor(colour);
	sceneObjects.push_back(wall);
	return wall;
}



//---This function initializes the scene ------------------------------------------- 
//   Specifically, it creates scene objects (spheres, planes, cones, cylinders etc)
//     and add them to the list of scene objects.
//   It also initializes the OpenGL 2D orthographc projection matrix for drawing the
//     the ray traced image.
//----------------------------------------------------------------------------------
void initialize()
{
    glMatrixMode(GL_PROJECTION);
    gluOrtho2D(XMIN, XMAX, YMIN, YMAX);
    glClearColor(0, 0, 0, 1);

	glm::vec3 A(-20., -15, 0); 
	glm::vec3 B(20., -15, 0); 
	glm::vec3 C(20., -15, -200); 
	glm::vec3 D(-20., -15, -200); 
	glm::vec3 E(-20., 30, -200); 
	glm::vec3 F(20., 30, -200); 
	glm::vec3 G(20., 30, 0); 
	glm::vec3 H(-20., 30, 0); 

	Sphere *sphere1 = new Sphere(glm::vec3(-0, 0, -100.0), 9.0);
	Sphere *sphere2 = new Sphere(glm::vec3(5, -10, -120.0), 5.0);
	Sphere *sphere3 = new Sphere(glm::vec3(7.0, 10.0, -90.0), 6.0);
	Cone *Cone1 = new Cone(glm::vec3(-0, -15, -80.0), 5, 5, 3);
	Cylinder *cylinder1 = new Cylinder(glm::vec3(-0, -15, -100.0), 5, 5);

	Plane *floor = createWall(A, B, C, D, glm::vec3(0.5, 0.5, 0.5)); //Point D
	Plane *roof = createWall(
		H, 
		G, 
		F, 
		E, 
		glm::vec3(1, 0, 1)); //Point D
	Plane *leftWall = createWall( H, A, D, E, glm::vec3(1, 0, 0)); //Point D
	Plane *rightWall = createWall(
		F,
		C,
		B, //Point B
		G, //Point A
		glm::vec3(0, 0, 1)
	); //Point D
	Plane *backWall = createWall(
		D, //Point A
		C,
		F, //Point C
		E,
		glm::vec3(0, 0, 0)
	); //Point D
	Plane *frontWall = createWall(
		glm::vec3(-20., -15, -0), //Point A
		glm::vec3(20., -15, -0), //Point B
		glm::vec3(20., 30, -0), //Point C
		glm::vec3(-20., 30, -0),
		glm::vec3(0, 0, 0)
	); //Point D


	sphere1->setColor(glm::vec3(0, 0, 0));   //Set colour to blue
	sphere2->setColor(glm::vec3(0, 0, 0));   //Set colour to blue
	sphere3->setColor(glm::vec3(1, 1, 0));   //Set colour to blue
	Cone1->setColor(glm::vec3(0	, 0, 0));   //Set colour to blue
	cylinder1->setColor(glm::vec3(1, 1, 0));   //Set colour to blue
	Cone1->setSpecularity(true);
	cylinder1->setSpecularity(true);
	backWall->setReflectivity(1, 0.9);
	backWall->setSpecularity(false);
	frontWall->setReflectivity(1, 0.9);
	frontWall->setSpecularity(false);
	frontWall->setReflectivity(1, 0.8);
	sphere1->setRefractivity(1, 0.5, 1.5);   //Set colour to blue
	Cone1->setReflectivity(1, 0.5);   //Set colour to blue
	sceneObjects.push_back(sphere1);		 //Add sphere to scene objects
	sceneObjects.push_back(sphere2);		 //Add sphere to scene objects
	sceneObjects.push_back(Cone1);		 //Add sphere to scene objects
	sceneObjects.push_back(cylinder1);		 //Add sphere to scene objects
	sceneObjects.push_back(sphere3);		 //Add sphere to scene objects
}


int main(int argc, char *argv[]) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB );
    glutInitWindowSize(1000, 1000);
    glutInitWindowPosition(2000, 3000);
    glutCreateWindow("Raytracing");

    glutDisplayFunc(display);
    initialize();

    glutMainLoop();
    return 0;
}
