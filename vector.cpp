#include <stdio.h>
#include <math.h>
#include "vector.hpp"

/*
 * Constructors.
 */
Vector::Vector(double x, double y, double z)
{
	this->x = x;
	this->y = y;
	this->z = z;
}

Vector::Vector()
{
	Vector(0, 0, 0);
}

void Vector::operator()(double x, double y, double z)
{
	this->x = x;
	this->y = y;
	this->z = z;
}

void Vector::operator()()
{
	this->x = 0;
	this->y = 0;
	this->z = 0;
}

/* Assignment. */
Vector &Vector::operator=(const Vector &v)
{
	if (this == &v)
		return *this;
	this->x = v.x;
	this->y = v.y;
	this->z = v.z;
	return *this;
}

/*
 * Comparative operators.
 */
bool Vector::operator==(const Vector &v) const
{
	return (
		this->x == v.x &&
		this->y == v.y &&
		this->z == v.z
	);
}

bool Vector::operator!=(const Vector &v) const
{
	return !(*this == v);
}

/*
 * Vector addition.
 */
Vector &Vector::operator+=(const Vector &v)
{
	this->x += v.x;
	this->y += v.y;
	this->z += v.z;
	return *this;
}

const Vector Vector::operator+(const Vector &v) const
{
	Vector u = *this;
	u += v;
	return u;
}

Vector &Vector::operator-=(const Vector &v)
{
	this->x -= v.x;
	this->y -= v.y;
	this->z -= v.z;
	return *this;
}

const Vector Vector::operator-(const Vector &v) const
{
	Vector u = *this;
	u -= v;
	return u;
}

/*
 * Scalar multiplication.
 */
Vector &Vector::operator*=(const double s)
{
	this->x *= s;
	this->y *= s;
	this->z *= s;
	return *this;
}

const Vector Vector::operator*(const double s) const
{
	Vector u = *this;
	u *= s;
	return u;
}

Vector &Vector::operator/=(const double s)
{
	this->x /= s;
	this->y /= s;
	this->z /= s;
	return *this;
}

const Vector Vector::operator/(const double s) const
{
	Vector u = *this;
	u /= s;
	return u;
}


/* TODO:
 * Cross product. (/)
 */

/*
 * Dot product. (*)
 */
double Vector::operator*(const Vector &v)
{
	return (this->x * v.x + this->y * v.y + this->z * v.z);
}

/*
 * Norms and normalization.
 */
inline double Vector::norm() const
{
	return sqrt(x*x + y*y + z*z);
}

void Vector::normalize()
{
	*this /= norm();
}

void Vector::rotate(double deg)
{
	double x, y;
	double rad;
	
	x = this->x;
	y = this->y;
	
	rad = deg * M_PI / 180;
	this->x = x * cos(rad) - y * sin(rad);
	this->y = x * sin(rad) + y * cos(rad);
}

double Vector::angleTo(const Vector &v)
{
	double rad;
	Vector u = *this;
	double dot;
	double x, y;
	
	dot = (u * v) / (u.norm() * v.norm());
	
//	printf("u: %f, %f, %f ; v: %f, %f, %f\n", u.x, u.y, u.z, v.x, v.y, v.z);
	rad = acos(dot);
	if (v.y > u.y)
		rad = 2*M_PI - rad;
/*	
	x = fabs(v.x);
	y = fabs(v.y);
	if (v.x > y)
		rad -= M_PI/2;
*/
	return rad * 180 / M_PI;
}
