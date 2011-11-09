#ifndef IMPLICIT_SPHERE
#define IMPLICIT_SPHERE


class ImplicitSphere : public ImplicitObject
{
public: 
	ImplicitSphere(float radius,cbh::vec3 position) : position(position), radius(radius) {
		material = new SimpleMaterial(0.1, 1.0, 1.0, cbh::vec3(1.0, 0.0, 0.0));
	}
	
	bool intersects(Ray &ray) const
	{
		double a = ray.direction.squareNorm();
		cbh::vec3 dist = ray.origin - position;
		double b = 2.0*dist.dot(ray.direction);
		double c = dist.squareNorm() - radius * radius;
		
		//Find discriminant
		double disc = b * b - 4 * a * c;

		// if discriminant is negative there are no real roots, so return 
		// false as ray misses sphere
		if (disc < 0)
			return false;

		// compute q as described above
		double distSqrt = sqrt(disc);
		double q;
		if (b < 0)
			q = (-b - distSqrt)/2.0;
		else
			q = (-b + distSqrt)/2.0;

		// compute t0 and t1
		double t0 = q / a;
		double t1 = c / q;

		// make sure t0 is smaller than t1
		if (t0 > t1)
		{
			// if t0 is bigger than t1 swap them around
			double temp = t0;
			t0 = t1;
			t1 = temp;
		}

		// if t1 is less than zero, the object is in the ray's negative direction
		// and consequently the ray misses the sphere
		if (t1 < 0)
			return false;

		// if t0 is less than zero, the intersection point is at t1
		// camera is inside the sphere
		if (t0 < 0)
		{
			ray.t = t1;
			return true;
		}
		// else the intersection point is at t0
		else
		{
			ray.t = t0;
			return true;
		}
	}

	cbh::vec3 getNormal(cbh::vec3 intersection) const
	{
		return (intersection - position).normalize();
	}

	const SimpleMaterial & getMaterial() const
	{
		return *material;
	}


private:
	cbh::vec3 position;
	double radius;
};





#endif