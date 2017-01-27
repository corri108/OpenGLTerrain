#pragma once

#ifndef VECTOR3_H
#define VECTOR3_H

#include "Vector3.h"

#endif

#include <GL\freeglut.h>
class Terrain
{
public :
	Terrain(int w2, int l2, float sz, float hs);
	~Terrain();

	Vector3 *position;
	Vector3 *rotation;
	float heightScale = 1;
	float size = 1;
	Vector3 **normals;
	float **heights;
	int width, length;
	bool computedNormals;

	void SetHeight(int x, int z, float y);
	float GetHeight(int x, int z);
	void ComputeNormals();
	Vector3 GetNormal(int x, int z);
};