#ifndef _VECTOR_HPP
#define _VECTOR_HPP

#include "common.h"

class Vector {
public:
	double x, y, z;
	
	Vector();
	Vector(double x, double y, double z);
	/* functors simulate constructors */
	void operator()();
	void operator()(double x, double y, double z);
	
	/* copy on assignment */
	Vector &operator=(const Vector &v);
	
	bool operator==(const Vector &v) const;
	bool operator!=(const Vector &v) const;
	
	/* additive operators */
	Vector &operator+=(const Vector &v);
	const Vector operator+(const Vector &v) const;
	Vector &operator-=(const Vector &v);
	const Vector operator-(const Vector &v) const;
	
	/* multiply by scalar s */
	Vector &operator*=(const double s);
	const Vector operator*(const double s) const;
	Vector &operator/=(const double s);
	const Vector operator/(const double s) const;
	
	/* dot product */
	double operator*(const Vector &v);
	
	double norm() const;
	void normalize();
	void rotate(double deg);
	double angleTo(const Vector &v);
};

#endif
