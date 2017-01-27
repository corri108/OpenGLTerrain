#include <GL\glut.h>
#include <cmath>
#include "NewCamera.h"

void vectorSet(float* vector, float x, float y, float z)
{
	vector[0] = x;
	vector[1] = y;
	vector[2] = z;
}
void vectorAdd(float* vector1, float* vector2)
{
	vector1[0] += vector2[0];
	vector1[1] += vector2[1];
	vector1[2] += vector2[2];
}
void vectorCopy(float* vector1, float* vector2)
{
	vector1[0] = vector2[0];
	vector1[1] = vector2[1];
	vector1[2] = vector2[2];
}
void vectorMultiply(float* vector, float scalar)
{
	vector[0] *= scalar;
	vector[1] *= scalar;
	vector[2] *= scalar;
}
float lengthOfVector(float* vector)
{
	return sqrtf(vector[0] * vector[0] + vector[1] * vector[1] + vector[2] * vector[2]);
}
void normaliseVector(float* vector)
{
	vectorMultiply(vector, 1 / lengthOfVector(vector));
}
void rotationMatrix(float* matrix, float* axis, float angle)
{
	float c1 = cos(angle);
	float c2 = 1 - c1;
	float s = sin(angle);

	matrix[0] = c1 + axis[0] * axis[0] * c2;
	matrix[1] = axis[0] * axis[1] * c2 - axis[2] * s;
	matrix[2] = axis[0] * axis[2] * c2 + axis[1] * s;

	matrix[3] = axis[1] * axis[0] * c2 + axis[2] * s;
	matrix[4] = c1 + axis[1] * axis[1] * c2;
	matrix[5] = axis[1] * axis[2] * c2 - axis[0] * s;

	matrix[6] = axis[2] * axis[0] * c2 - axis[1] * s;
	matrix[7] = axis[2] * axis[1] * c2 + axis[0] * s;
	matrix[8] = c1 + axis[2] * axis[2] * c2;
}
void mulVectorBy(float* vector1, float* matrix, float* vector2)
{
	vector2[0] = vector1[0] * matrix[0] + vector1[1] * matrix[1] + vector1[2] * matrix[2];
	vector2[1] = vector1[0] * matrix[3] + vector1[1] * matrix[4] + vector1[2] * matrix[5];
	vector2[2] = vector1[0] * matrix[6] + vector1[1] * matrix[7] + vector1[2] * matrix[8];
}
void rotateAroundVector(float* vector1, float* vector2, float angle, float* vector3)
{
	float matrix[16];
	rotationMatrix(matrix, vector2, angle);
	mulVectorBy(vector1, matrix, vector3);
}


NewCamera::NewCamera(void)
{

}

NewCamera::NewCamera(float fov, float ww, float wh, float n, float f)
{
	//debug these values
	speed = 2.5f;
	turnSpeed = 0.075f;
	vectorSet(position, 400, 700, 1000);
	vectorSet(forwardVector, 0, -0.5, -0.5);
	vectorSet(rightVector, 1, 0, 0);
	vectorSet(upVector, 0, 0.5, -0.5);
	this->FOV = fov;
	this->WINDOWWIDTH = ww;
	this->WINDOWHEIGHT = wh;
	this->cNEAR = n;
	this->cFAR = f;
}

void NewCamera::UpdatePerspective(void)
{
	gluPerspective(FOV, WINDOWWIDTH / WINDOWHEIGHT, cNEAR, cFAR);
}

void NewCamera::UpdateView(void)
{
	
}

void NewCamera::transformOrientation(void)
{
	gluLookAt(0, 0, 0, forwardVector[0], forwardVector[1], forwardVector[2], upVector[0], upVector[1], upVector[2]);
}

void NewCamera::transformTranslation(void)
{
	glTranslatef(-position[0], -position[1], -position[2]);
}

void NewCamera::pointAt(float* targetVector)
{
	float tempVector[3];
	float up[3] = { 0.0f, 0.0f, 1.0f };
	forwardVector[0] = targetVector[0] - position[0];
	forwardVector[1] = targetVector[1] - position[1];
	forwardVector[2] = targetVector[2] - position[2];

	normaliseVector(forwardVector);
	rotateAroundVector(forwardVector, up, -1.57079632679f, tempVector);
	tempVector[2] = 0;
	normaliseVector(tempVector);
	vectorCopy(rightVector, tempVector);
	rotateAroundVector(forwardVector, rightVector, 1.57079632679f, tempVector);
	vectorCopy(upVector, tempVector);
}

void NewCamera::speedUp(void)
{
	if (speed < 5.0f)
		speed *= 2;
}

void NewCamera::slowDown(void)
{
	if (speed > 0.000001f)
		speed /= 2;
}

void NewCamera::forward(void)
{
	float vector[3];
	vectorCopy(vector, forwardVector);
	vectorMultiply(vector, speed);
	vectorAdd(position, vector);
}

void NewCamera::backward(void)
{
	float vector[3];
	vectorCopy(vector, forwardVector);
	vectorMultiply(vector, -speed);
	vectorAdd(position, vector);
}

void NewCamera::left(void)
{
	float vector[3];
	vectorCopy(vector, rightVector);
	vectorMultiply(vector, -speed);
	vectorAdd(position, vector);
}

void NewCamera::right(void)
{
	float vector[3];
	vectorCopy(vector, rightVector);
	vectorMultiply(vector, speed);
	vectorAdd(position, vector);
}

void NewCamera::pitchUp(void)
{
	float vector[3];
	rotateAroundVector(forwardVector, rightVector, turnSpeed, vector);
	vectorCopy(forwardVector, vector);
	rotateAroundVector(upVector, rightVector, turnSpeed, vector);
	vectorCopy(upVector, vector);
}

void NewCamera::pitchDown(void)
{
	float vector[3];
	rotateAroundVector(forwardVector, rightVector, -turnSpeed, vector);
	vectorCopy(forwardVector, vector);
	rotateAroundVector(upVector, rightVector, -turnSpeed, vector);
	vectorCopy(upVector, vector);
}

void NewCamera::yawLeft(void)
{
	float vector[3];
	rotateAroundVector(forwardVector, upVector, turnSpeed, vector);
	vectorCopy(forwardVector, vector);
	rotateAroundVector(rightVector, upVector, turnSpeed, vector);
	vectorCopy(rightVector, vector);
}

void NewCamera::yawRight(void)
{
	float vector[3];
	rotateAroundVector(forwardVector, upVector, -turnSpeed, vector);
	vectorCopy(forwardVector, vector);
	rotateAroundVector(rightVector, upVector, -turnSpeed, vector);
	vectorCopy(rightVector, vector);
}