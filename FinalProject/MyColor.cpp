/*
* Color.cpp
*
*  Created on: Jan 20, 2017
*      Author: socce_000
*/

#include <GL\glut.h>
#include <stdio.h>
#include <ctime>
#include "MyColor.h"

MyColor::MyColor(float r, float g, float b)
{
	this->r = r;
	this->g = g;
	this->b = b;
}

MyColor::MyColor()
{
	this->r = 1.0;
	this->g = 1.0;
	this->b = 1.0;
}

MyColor::~MyColor()
{

}

MyColor *MyColor::RandomColor()
{
	srand(static_cast <unsigned> (time(0)));
	float r1 = static_cast <float> (rand()) / static_cast <float> (RAND_MAX);
	float r2 = static_cast <float> (rand()) / static_cast <float> (RAND_MAX);
	float r3 = static_cast <float> (rand()) / static_cast <float> (RAND_MAX);
	return new MyColor(r1, r2, r3);
}
