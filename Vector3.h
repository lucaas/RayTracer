#ifndef VECTOR3
#define VECTOR3

#include <iostream>
#include <cmath>

namespace cbh {

	template <class T>
	class Vector3
	{
	public:
		Vector3<T>() { vec[0] = 0.0; vec[1] = 0.0; vec[2] = 0.0; }
		Vector3<T>(T x, T y, T z) { vec[0] = x; vec[1] = y; vec[2] = z; }
		Vector3<T>(T v) { vec[0] = vec[1] = vec[2] = v; }
		Vector3<T>(const Vector3<T> & v) { vec[0] = v.vec[0]; vec[1] = v.vec[1]; vec[2] = v.vec[2]; }

		T squareNorm() const {return vec[0]*vec[0] + vec[1]*vec[1] + vec[2]*vec[2]; }
		T operator*(const Vector3<T> & rhs) const { return vec[0]*rhs.vec[0]+vec[1]*rhs.vec[1]+vec[2]*rhs.vec[2]; } 
		T dot(const Vector3<T> & rhs) const { return vec[0]*rhs.vec[0]+vec[1]*rhs.vec[1]+vec[2]*rhs.vec[2];}

		//Elementwise multiplication
		Vector3<T> mtimes(const Vector3<T> &rhs) const { return Vector3<T>(vec[0]*rhs.vec[0], vec[1]*rhs.vec[1], vec[2]*rhs.vec[2]);}

		friend Vector3<T> operator*(T lhs, const Vector3<T> &rhs) { return Vector3<T>(lhs*rhs.vec[0], lhs*rhs.vec[1], lhs*rhs.vec[2]);}
		friend Vector3<T> operator*(const Vector3<T> &lhs,T rhs) { return Vector3<T>(rhs*lhs.vec[0], rhs*lhs.vec[1], rhs*lhs.vec[2]);}
		friend Vector3<T> operator*=(T lhs, const Vector3<T> &rhs) { return Vector3<T>(lhs*rhs.vec[0], lhs*rhs.vec[1], lhs*rhs.vec[2]);}

		friend Vector3<T> operator/(T lhs, const Vector3<T> &rhs) { return Vector3<T>(lhs/rhs.vec[0], lhs/rhs.vec[1], lhs/rhs.vec[2]);}
		Vector3<T> operator/(const T rhs) const {return Vector3<T>(vec[0]/rhs, vec[1]/rhs, vec[2]/rhs);}

		Vector3<T> cross(const Vector3<T> &rhs) const {return Vector3<T>(vec[1]*rhs.vec[2]-vec[2]*rhs.vec[1], -(vec[0]*rhs.vec[2]-vec[2]*rhs.vec[0]), vec[0]*rhs.vec[1]-vec[1]*rhs.vec[0] );}
		Vector3<T> operator-(const Vector3<T> &rhs) const {return Vector3<T>(vec[0]-rhs.vec[0], vec[1]-rhs.vec[1], vec[2]-rhs.vec[2]);}
		friend Vector3<T> operator-(const Vector3<T> &rhs) {return Vector3<T>(-rhs.vec[0], -rhs.vec[1], -rhs.vec[2]);}
		Vector3<T> operator+(const Vector3<T> &rhs) const {return Vector3<T>(vec[0]+rhs.vec[0], vec[1]+rhs.vec[1], vec[2]+rhs.vec[2]);}

		Vector3<T> normalize() const;
		Vector3<T> normalizeWithMax() const;
		Vector3<T> clamp(T _min,T _max) const;

		T sum() const { return vec[0]+vec[1]+vec[2]; }

		bool operator== (const Vector3<T> &rhs) const { return (vec[0]==rhs.vec[0] && vec[1]==rhs.vec[1] && vec[2]==rhs.vec[2]); }
		bool operator!= (const Vector3<T> &rhs) const { return !(*this == rhs); }

		bool operator== (const T d) const { return (vec[0]==d && vec[1]==d && vec[2]==d); }
		bool operator!= (const T d) const { return !(*this == d); }

		Vector3<T>& operator= (const Vector3<T> &rhs);
		Vector3<T>& operator= (const T d);
		Vector3<T>& operator*= (const T d);
		Vector3<T>& operator+= (const Vector3<T> &rhs);

		bool operator>=(const T d) const { return (vec[0] >= d && vec[1] >= d && vec[2] >= d); }

		T & operator[](const int & i) { return vec[i]; }
		T operator[](const int & i) const { return vec[i]; }

		T getX() const { return vec[0]; }
		T getY() const { return vec[1]; }
		T getZ() const { return vec[2]; }
		void setX(const T _x) { vec[0] = _x;}
		void setY(const T _y) { vec[1] = _y;}
		void setZ(const T _z) { vec[2] = _z;}
		void set(const T _x, const T _y, const T _z){vec[0] = _x; vec[1] = _y; vec[2] = _z;}

		friend std::ostream& operator<<(std::ostream &os, const Vector3<T> &vec);
		
	private:
		T vec[3];
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

		max = vec[0];

		if (vec[1] > max)
			max = vec[1];

		if (vec[2] > max)
			max = vec[2];

		if (max > 1.0)
			return *this/max;
		else
			return *this;
	}

	template <typename T> Vector3<T> Vector3<T>::clamp(T _min,T _max) const
	{
		Vector3<T> result(*this);
		//if(X < min) -> X = min; else if(X > max) -> X = max; else X = X //Unchanged	
		result.vec[0] = result.vec[0] < _min ? _min : (result.vec[0] > _max ? _max : result.vec[0]);
		result.vec[1] = result.vec[1] < _min ? _min : (result.vec[1] > _max ? _max : result.vec[1]);
		result.vec[2] = result.vec[2] < _min ? _min : (result.vec[2] > _max ? _max : result.vec[2]);
		return result;
	}



	template <typename T> Vector3<T>& Vector3<T>::operator= (const Vector3<T> &rhs)
	{
		// Handle self assignment
		if (this == &rhs)
			return *this;

		vec[0] = rhs.vec[0]; vec[1] = rhs.vec[1]; vec[2] = rhs.vec[2];
		return *this;
	}


	template <typename T> Vector3<T>& Vector3<T>::operator= (const T d)
	{
		vec[0] = d; vec[1] = d; vec[2] = d;
		return *this;
	}

	template <typename T> Vector3<T>& Vector3<T>::operator*= (const T d)
	{
		vec[0] *= d; vec[1] *= d; vec[2] *= d;
		return *this;
	}


	template <typename T> Vector3<T>& Vector3<T>::operator+= (const Vector3<T> &rhs)
	{
		vec[0] += rhs.vec[0]; vec[1] += rhs.vec[1]; vec[2] += rhs.vec[2];
		return *this;
	}


	template <typename T> std::ostream& operator<<(std::ostream &os, const Vector3<T> &vec)
	{
		//os << "(" << vec.vec[0] << ", " << vec.vec[1] << ", " << vec.vec[2] << ")"; 
		os << vec.vec[0] << "," << vec.vec[1] << "," << vec.vec[2];
		return os;
	}

	//Assumes incoming pointing towards the surface
	template <typename T> Vector3<T> reflect(const Vector3<T> &incoming , const Vector3<T> &normal)
	{
		Vector3<T> reflection(2.0*(-incoming.dot(normal))*normal + incoming);
		return reflection.normalize();
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
