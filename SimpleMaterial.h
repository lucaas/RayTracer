#ifndef SIMPLE_MATERIAL
#define SIMPLE_MATERIAL

#include "Vector3.h"

class SimpleMaterial
{
public:
	SimpleMaterial() : ka(1), kd(1), ks(1),acolor(cbh::vec3(1.0)),scolor(cbh::vec3(1.0)),dcolor(cbh::vec3(1.0)),specular(10.0f), reflection(0), refraction(0) {}
	SimpleMaterial(double ka, double kd, double ks, cbh::vec3 color) : ka(ka),ks(ks),kd(kd),acolor(color),scolor(color),dcolor(color),specular(10.0f), reflection(0), refraction(0) {}
	SimpleMaterial(double ka, double kd, double ks, cbh::vec3 acolor,cbh::vec3 dcolor,cbh::vec3 scolor, double specular, double reflection, double refraction) : ka(ka),ks(ks),kd(kd),acolor(acolor),scolor(scolor),dcolor(dcolor),specular(specular), reflection(reflection), refraction(refraction) {}

	cbh::vec3 acolor,scolor,dcolor;
	double ka,ks,kd,specular;
	double reflection, refraction;
};


#endif