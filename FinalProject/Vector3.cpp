/*
* Vector3.cpp
*
*  Created on: Jan 20, 2017
*      Author: socce_000
*/


#include <GL\glut.h>
#include <math.h>
#include "Vector3.h"

Vector3::Vector3(float x, float y, float z)
{
	this->x = x;
	this->y = y;
	this->z = z;
}

Vector3::Vector3()
{
	this->x = 0;
	this->y = 0;
	this->z = 0;
}

Vector3::~Vector3()
{

}

float Vector3::flerp(float a, float b, float f)
{
	return (a * (1.0f - f)) + (b * f);
}

Vector3 *Vector3::lerp(Vector3 *from, Vector3 *to, float f)
{
	float newX = flerp(from->x, to->x, f);
	float newY = flerp(from->y, to->y, f);
	float newZ = flerp(from->z, to->z, f);

	return new Vector3(newX, newY, newZ);
}

//operators
Vector3 *Vector3::operator+(const Vector3 &other) const
{
	return new Vector3(x + other.x, y + other.y, z + other.z);
}
Vector3 *Vector3::operator-(const Vector3 &other) const
{
	return new Vector3(x - other.x, y - other.y, z - other.z);
}
Vector3 *Vector3::operator*(float scale) const
{
	return new Vector3(x * scale, y * scale, z * scale);
}
Vector3 *Vector3::operator/(float scale) const
{
	return new Vector3(x / scale, y / scale, z / scale);
}

//helper methods
float Vector3::magnitude()
{
	float mag = sqrt(x * x + y * y + z * z);
	return mag;
}
float Vector3::magnitudeSquared()
{
	float mags = x * x + y * y + z * z;
	return mags;
}
void Vector3::norm()
{
	float m = magnitude();
	if (m != 0)
	{
		this->x /= m;
		this->y /= m;
		this->z /= m;
	}
}
Vector3 *Vector3::cross(const Vector3 &other)
{
	float newX = this->y * other.z - this->z * other.y;
	float newY = this->z * other.x - this->x * other.z;
	float newZ = this->x * other.y - this->y * other.x;

	return new Vector3(newX, newY, newZ);
}
float Vector3::dot(const Vector3 &other)
{
	float xs = this->x * other.x;
	float ys = this->y * other.y;
	float zs = this->z * other.z;

	return xs + ys + zs;
}