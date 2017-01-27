#pragma once
/*
* Color.h
*
*  Created on: Jan 20, 2017
*      Author: socce_000
*/

class MyColor
{
public:
	MyColor(float r, float g, float b);
	MyColor();
	~MyColor();

	static MyColor *RandomColor();
	float r, g, b;
};
