#ifndef MATERIAL
#define MATERIAL

#include "glm.hpp"

class Material
{
public:
	Material() : ka(1), kd(1), ks(1),acolor(glm::vec3(1)),scolor(glm::vec3(1)),dcolor(glm::vec3(1)),specular(10.0f), reflection(0), refraction(0) {}
	Material(float ka, float kd, float ks, glm::vec3 color) : ka(ka),ks(ks),kd(kd),acolor(color),scolor(color),dcolor(color),specular(10.0f), reflection(0), refraction(0) {}
	Material(float ka, float kd, float ks, glm::vec3 acolor,glm::vec3 dcolor,glm::vec3 scolor, float specular, float reflection, float refraction) : ka(ka),ks(ks),kd(kd),acolor(acolor),scolor(scolor),dcolor(dcolor),specular(specular), reflection(reflection), refraction(refraction) {}
	glm::vec3 acolor,scolor,dcolor;
	float ka,ks,kd,specular;
    float reflection, refraction;
};


#endif