/*
* Camera.cpp
*
*  Created on: Jan 22, 2017
*      Author: socce_000
*/

#include "Camera.h"
#include <GL\glut.h>

Camera::Camera(Vector3 *pos, Vector3 *view, Vector3 *up)
{
	this->position = pos;
	this->viewing = view;
	this->up = up;
}

Camera::Camera()
{
	this->viewing = new Vector3(0, 0, 0);
	this->up = new Vector3(0, 1, 0);
	this->position = new Vector3(0, 0, -1);
}

Camera::~Camera()
{

}

void Camera::update()
{
	gluLookAt(this->position->x, this->position->y, this->position->z,
		this->viewing->x, this->viewing->y, this->viewing->z,
		this->up->x, this->up->y, this->up->z);
}

void Camera::UpdatePerspective(float FOV, float WINDOWWIDTH, float WINDOWHEIGHT, float cNEAR, float cFAR)
{
	gluPerspective(FOV, WINDOWWIDTH / WINDOWHEIGHT, cNEAR, cFAR);
}