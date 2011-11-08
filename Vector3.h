#ifndef VECTOR3
#define VECTOR3

#include <iostream>
#include <cmath>

namespace cbh {

	template <class T>
	class Vector3
	{
	public:
		Vector3<T>() : x(0.0), y(0.0),z(0.0) {}
		Vector3<T>(T x, T y, T z) : x(x),y(y),z(z) {}
		Vector3<T>(T v) : x(v),y(v),z(v) {}
		Vector3<T>(const Vector3<T> & v) : x(v.x),y(v.y),z(v.z) {}

		T squareNorm() const {return x*x + y*y + z*z; }
		T operator*(const Vector3<T> & rhs) const { return x*rhs.x+y*rhs.y+z*rhs.z; } 
		T dot(const Vector3<T> & rhs) const { return x*rhs.x+y*rhs.y+z*rhs.z;}

		//Elementwise multiplication
		Vector3<T> mtimes(const Vector3<T> &rhs) const { return Vector3<T>(x*rhs.x, y*rhs.y, z*rhs.z);}

		friend Vector3<T> operator*(T lhs, const Vector3<T> &rhs) { return Vector3<T>(lhs*rhs.x, lhs*rhs.y, lhs*rhs.z);}
		friend Vector3<T> operator*(const Vector3<T> &lhs,T rhs) { return Vector3<T>(rhs*lhs.x, rhs*lhs.y, rhs*lhs.z);}
		friend Vector3<T> operator*=(T lhs, const Vector3<T> &rhs) { return Vector3<T>(lhs*rhs.x, lhs*rhs.y, lhs*rhs.z);}

		friend Vector3<T> operator/(T lhs, const Vector3<T> &rhs) { return Vector3<T>(lhs/rhs.x, lhs/rhs.y, lhs/rhs.z);}
		Vector3<T> operator/(const T rhs) const {return Vector3<T>(x/rhs, y/rhs, z/rhs);}

		Vector3<T> cross(const Vector3<T> &rhs) const {return Vector3<T>(y*rhs.z-z*rhs.y, -(x*rhs.z-z*rhs.x), x*rhs.y-y*rhs.x );}
		Vector3<T> operator-(const Vector3<T> &rhs) const {return Vector3<T>(x-rhs.x, y-rhs.y, z-rhs.z);}
		friend Vector3<T> operator-(const Vector3<T> &rhs) {return Vector3<T>(-rhs.x, -rhs.y, -rhs.z);}
		Vector3<T> operator+(const Vector3<T> &rhs) const {return Vector3<T>(x+rhs.x, y+rhs.y, z+rhs.z);}

		Vector3<T> normalize() const;
		Vector3<T> normalizeWithMax() const;
		Vector3<T> clamp(T _min,T _max) const;

		T sum() const { return x+y+z; }

		bool operator== (const Vector3<T> &rhs) const { return (x==rhs.x && y==rhs.y && z==rhs.z); }
		bool operator!= (const Vector3<T> &rhs) const { return !(*this == rhs); }

		bool operator== (const T d) const { return (x==d && y==d && z==d); }
		bool operator!= (const T d) const { return !(*this == d); }

		Vector3<T>& operator= (const Vector3<T> &rhs);
		Vector3<T>& operator= (const T d);
		Vector3<T>& operator+= (const Vector3<T> &rhs);

		bool operator>=(const T d) const { return (x >= d && y >= d && z >= d); }

		T getX() const { return x; }
		T getY() const { return y; }
		T getZ() const { return z; }
		void setX(const T _x) { x = _x;}
		void setY(const T _y) { y = _y;}
		void setZ(const T _z) { z = _z;}
		void set(const T _x, const T _y, const T _z){x = _x; y = _y; z = _z;}

		friend std::ostream& operator<<(std::ostream &os, const Vector3<T> &vec);

	private:
		T x,y,z;
	};


	template <typename T> Vector3<T> Vector3<T>::normalize() const
	{
		double sq = squareNorm();
		Vector3<T> ret = *this/sqrt(sq);
		return ret;
	}


	template <typename T> Vector3<T> Vector3<T>::normalizeWithMax() const
	{
		double max = -1.0;

		max = x;

		if (y > max)
			max = y;

		if (z > max)
			max = z;

		if (max > 1.0)
			return *this/max;
		else
			return *this;
	}

	template <typename T> Vector3<T> Vector3<T>::clamp(T _min,T _max) const
	{
		Vector3<T> result(*this);
		//if(X < min) -> X = min; else if(X > max) -> X = max; else X = X //Unchanged	
		result.x = result.x < _min ? _min : (result.x > _max ? _max : result.x);
		result.y = result.y < _min ? _min : (result.y > _max ? _max : result.y);
		result.z = result.z < _min ? _min : (result.z > _max ? _max : result.z);
		return result;
	}



	template <typename T> Vector3<T>& Vector3<T>::operator= (const Vector3<T> &rhs)
	{
		// Handle self assignment
		if (this == &rhs)
			return *this;

		x = rhs.x; y = rhs.y; z = rhs.z;
		return *this;
	}


	template <typename T> Vector3<T>& Vector3<T>::operator= (const T d)
	{
		x = d; y = d; z = d;
		return *this;
	}


	template <typename T> Vector3<T>& Vector3<T>::operator+= (const Vector3<T> &rhs)
	{
		x += rhs.x; y += rhs.y; z += rhs.z;
		return *this;
	}


	template <typename T> std::ostream& operator<<(std::ostream &os, const Vector3<T> &vec)
	{
		os << "(" << vec.x << ", " << vec.y << ", " << vec.z << ")"; 
		return os;
	}

	/******* Typdefs ********/

	//Cheesy way of choosing what precision we need for aaaaalll vectors
	typedef Vector3<double> vec3;

	//These should be used instead
	typedef Vector3<double> vec3d;
	typedef Vector3<float> vec3f;
	typedef Vector3<unsigned char> vec3uc;


}


#endif
