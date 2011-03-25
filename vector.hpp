#ifndef _VECTOR_HPP
#define _VECTOR_HPP

#include "common.h"

class Vector {
public:
	double x, y, z;
	
	Vector();
	Vector(double x, double y, double z);
	Vector(const double a[3]);
	/* functors simulate constructors */
	void operator()();
	void operator()(double x, double y, double z);
	void operator()(const double a[3]);
	
	/* copy on assignment */
	Vector &operator=(const Vector &v);
	
	bool operator==(const Vector &v) const;
	bool operator!=(const Vector &v) const;
	
	/* additive operators */
	Vector &operator+=(const Vector &v);
	Vector operator+(const Vector &v) const;
	Vector &operator-=(const Vector &v);
	Vector operator-(const Vector &v) const;
	
	/* multiply by scalar s */
	Vector &operator*=(const double s);
	Vector operator*(const double s) const;
	// TODO: friend function for scalar multiplation commutativity
	Vector &operator/=(const double s);
	Vector operator/(const double s) const;
	
	/* dot product */
	double operator*(const Vector &v) const;
	
	/* cross product */
	Vector operator/(const Vector &v) const;
	
	double norm() const;
	void normalize();
	Vector normal() const;
	void rotate(const double deg);
	Vector rotation(const double deg) const;
	double angleTo(const Vector &v) const;
	void componentwise_scale(const Vector &v);
	
	Vector projection(const Vector &b) const;
	
	Vector v_union(const Vector &b) const;
	Vector intersection(const Vector &b) const;
	bool intersect(const Vector &b) const;
};

class Range {
public:
	Vector a, b;
	
	Range() { a(); b(); }
	Range(Vector a, Vector b) { this->a = a; this->b = b; }
	~Range() {}
	
	Vector minimum() const;
	Vector maximum() const;
	Vector flat() const { return this->b - this->a; }
	Range operator*(const Range &y) const;
	Range operator+(const Range &y) const;
	bool overlaps(const Range &y) const;
	
	static double den(Range a, Range b);
	static double t1(Range a, Range b);
	static double t2(Range a, Range b);
};

#endif
