
#ifndef RayTracer_PhongMaterial_h
#define RayTracer_PhongMaterial_h

class PhongMaterial : public IMaterial {
public:
    
    // Diffuse material
    PhongMaterial(cbh::vec3 color, double kd) : IMaterial(color, kd) {}
    
    // Normal/Specular
    PhongMaterial(cbh::vec3 color, double kd, double ks, double specularPower) : IMaterial(color,kd,ks,specularPower) {}
    
    // With rIndex
    PhongMaterial(cbh::vec3 color, double kd, double ks, double specularPower, double rIndex) : IMaterial(color, kd,ks,specularPower,rIndex) {}

    
	cbh::vec3 brdf(const cbh::vec3 & incident, const cbh::vec3 & outgoing, const cbh::vec3 & normal) const {
       /*
        // Modified Blinn-Phong
        cbh::vec3 halfVec = -incident + outgoing;
        halfVec = halfVec.normalize();
        
        return color * (ks * pow(normal.dot(halfVec), specularPower) + kd);
        */
        
        // Lambertian
        return color * kd;
    }

};

#endif
