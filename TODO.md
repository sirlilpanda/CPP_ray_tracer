
## worth 10 marks
- [x] make box around balls
- [ ] make balls and shit look pretty
- [x] make transpartent object
- [x] cast shadows
    - [x] cast hard shadows
    - [ ] cast soft shadows

- [x] mirror like ball #make colour 1, 1, 1 and relectivity 1
- [x] checker floor
- [x] specular refecltion working

## work 4-5
- [x] add cone : 1.5
- [x] add cylinder : 1
- [x] add cylinder cap : 0.5
- [ ] add donut : 2/3
- [x] mirror : 1
- [x] refraction through an object
- [ ] texture map sphere: dont know if this will give me anything



# today
- add soft shadows
- add cone 1
- add trous ~

	// if (obj->isSpecular() && step < MAX_STEPS){
	// 	glm::vec3 totalSum = glm::vec3(1);
	// 	for(int i = 0; i < MAX_SMAPLES; i++){
	// 		glm::vec3 ran = glm::ballRand(1.0);
	// 		glm::vec3 norm = obj->normal(ray.hit);
	// 		ran *= sgn(glm::dot(norm, ran));
	// 		glm::vec3 c = trace(Ray(ray.hit, ran), step+1); 
	// 		totalSum *= glm::vec3(
	// 			(c.r), 
	// 			(c.b), 
	// 			(c.g)
	// 		);
	// 	}
	// 	color += totalSum/((float) MAX_SMAPLES);
	// }