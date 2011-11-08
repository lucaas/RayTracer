#ifndef IMATERIAL
#define IMATERIAL

#include "Vector3.h"

class IMaterial
{
public:
	virtual Brdf(const cbh::vec3 & incident, const cbh::vec3 & outgoing) const = 0;



};


#endif	IMATERIAL