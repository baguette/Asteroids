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

// Zero vector.
Vector::Vector()
{
	Vector(0, 0, 0);
}

Vector::Vector(const double a[3])
{
	this->x = a[0];
	this->y = a[1];
	this->z = a[2];
}

// Set the components.
void Vector::operator()(double x, double y, double z)
{
	this->x = x;
	this->y = y;
	this->z = z;
}

// Convert to zero vector.
void Vector::operator()()
{
	this->x = 0;
	this->y = 0;
	this->z = 0;
}

void Vector::operator()(const double a[3])
{
	this->x = a[0];
	this->y = a[1];
	this->z = a[2];
}

/* Assignment (deep copy of v). */
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

Vector Vector::operator+(const Vector &v) const
{
	Vector u = *this;
	u += v;
	return u;
}

// In-place vector subtraction.
Vector &Vector::operator-=(const Vector &v)
{
	this->x -= v.x;
	this->y -= v.y;
	this->z -= v.z;
	return *this;
}

// Vector subtraction.
Vector Vector::operator-(const Vector &v) const
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

// scalar multiplication
Vector Vector::operator*(const double s) const
{
	Vector u = *this;
	u *= s;
	return u;
}

// In-place scalar division
Vector &Vector::operator/=(const double s)
{
	this->x /= s;
	this->y /= s;
	this->z /= s;
	return *this;
}

// Scalar division. (/)
Vector Vector::operator/(const double s) const
{
	Vector u = *this;
	u /= s;
	return u;
}

/*
 * Cross product. (/)
 */
Vector Vector::operator/(const Vector &b) const
{
	// Laplace expansion
	Vector i(1, 0, 0), j(0, 1, 0), k(0, 0, 1);
	Vector a = *this;
	return (i*(a.y*b.z - a.z*b.y) - j*(a.x*b.z - a.z*b.x) + k*(a.x*b.y - a.y*b.x));
}

/*
 * Dot product. (*)
 */
inline double Vector::operator*(const Vector &v) const
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

// Normalize this vector.
inline void Vector::normalize()
{
	*this /= norm();
}

// Get the normal (unit length vector with same direction) of this vector.
Vector Vector::normal() const
{
	Vector u = *this;
	u.normalize();
	return u;
}

// Rotate this vector by deg degrees.
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

Vector Vector::rotation(const double deg) const
{
	Vector v = *this;
	v.rotate(deg);
	return v;
}

// Get the angle between this vector and v in degrees.
double Vector::angleTo(const Vector &v) const
{
	double rad;
	Vector u = *this;
	double dot;
	
	dot = (u * v) / (u.norm() * v.norm());
	rad = acos(dot);
	if (v.y > u.y)
		rad = 2*M_PI - rad;

	return rad * 180 / M_PI;
}

// Project a onto b
Vector Vector::projection(const Vector &b) const
{
	Vector a = *this;
	return (b.normal() * (a * b.normal()));
}

void Vector::componentwise_scale(const Vector &v)
{
	this->x *= v.x; this->y *= v.y; this->z *= v.z;
}

Vector Vector::v_union(const Vector &b) const
{
	Vector a = *this;
	if (a.normal() == b.normal())
		return (a.norm() >= b.norm()) ? a : b;
	return Vector();
}

inline Vector Vector::intersection(const Vector &b) const
{
	Vector a = *this;
	if (a.normal() == b.normal())
		return (a.norm() <= b.norm()) ? a : b;
	return Vector();
}

inline bool Vector::intersect(const Vector &b) const
{
	Vector a = *this;
	if (a.normal() == b.normal())
		return (a.norm() <= b.norm()) ? true : false;
}

///////////////////////////////////

Range Range::operator*(const Range &y) const
{
	Range x = *this;
	Vector u = x.b - x.a, v = y.b - y.a;

	Vector xmin = (x.minimum().norm() > y.minimum().norm()) ? x.minimum() : y.minimum();
	Vector xmax = (x.maximum().norm() < y.maximum().norm()) ? x.maximum() : y.maximum();
	return Range(xmin, xmax);
	return Range();
}

Range Range::operator+(const Range &y) const
{
	Range x = *this;
	Vector xmin = (x.minimum().norm() < y.minimum().norm()) ? x.minimum() : y.minimum();
	Vector xmax = (x.maximum().norm() > y.maximum().norm()) ? x.maximum() : y.maximum();
	return Range(xmin, xmax);
}

bool Range::overlaps(const Range &y) const
{
	Range x = *this;
	
	double xmin = x.minimum().norm();
	double xmax = x.maximum().norm();
	double ymin = y.minimum().norm();
	double ymax = y.maximum().norm();

	return (
			((xmin <= ymin) && (ymin <= xmax))
		|| ((ymin <= xmin) && (xmin <= ymax))
		);
}

Vector Range::minimum() const
{
	return (a.norm() < b.norm()) ? a : b;
}

Vector Range::maximum() const
{
	return (a.norm() > b.norm()) ? a : b;
}
