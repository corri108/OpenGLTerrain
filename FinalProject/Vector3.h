#pragma once
/*
* Vector3.h
*
*  Created on: Jan 20, 2017
*      Author: socce_000
*/

class Vector3
{
public:
	Vector3(float x, float y, float z);
	Vector3();
	~Vector3();
	Vector3 *lerp(Vector3 *from, Vector3 *to, float value);
	float flerp(float from, float to, float value);

	float x, y, z;

	Vector3 *operator+(const Vector3 &other) const;
	Vector3 *operator-(const Vector3 &other) const;
	Vector3 *operator*(float scale) const;
	Vector3 *operator/(float scale) const;

	float magnitude();
	float magnitudeSquared();
	void norm();
	Vector3 *cross(const Vector3 &other);
	float dot(const Vector3 &other);
};
