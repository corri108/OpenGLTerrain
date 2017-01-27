#pragma once

#ifndef VECTOR3_H
#define VECTOR3_H

#include "Vector3.h"

#endif

class Camera
{
public:
	Camera(Vector3 *pos, Vector3 *view, Vector3 *up);
	Camera();
	~Camera();
	void update();
	void UpdatePerspective(float FOV,float WINDOWWIDTH, float WINDOWHEIGHT, float cNEAR, float cFAR);

	Vector3 *viewing;
	Vector3 *up;
	Vector3 *position;
};
