/*
* BMPLoader.cpp
*
*  Created on: Jan 23, 2017
*      Author: socce_000
*/
#define _CRT_SECURE_NO_DEPRECATE

#include <stdio.h>
#include "BMPLoader.h"

using namespace std;

BMPLoader::BMPLoader()
{

}

BMPLoader::~BMPLoader()
{

}

GLuint BMPLoader::loadBMP_custom(const char * imagepath)
{
	unsigned char header[54]; // Each BMP file begins by a 54-bytes header
	unsigned int dataPos;     // Position in the file where the actual data begins
	unsigned int width, height;
	unsigned int imageSize;   // = width*height*3

							  //open the fiel
	FILE * file = fopen(imagepath, "rb");
	if (!file)
	{
		printf("Image could not be opened\n"); return 0;
	}

	//BMP files have a 54 byte header, so lets see if the file we have is 54 bytes in the header
	if (fread(header, 1, 54, file) != 54)
	{
		printf("Not a correct BMP file\n");
		return false;
	}

	//check header for correct values
	if (header[0] != 'B' || header[1] != 'M')
	{
		printf("Not a correct BMP file\n");
		return 0;
	}

	//we can now read some of the info
	dataPos = *(int*)&(header[0x0A]);
	imageSize = *(int*)&(header[0x22]);
	width = *(int*)&(header[0x12]);
	height = *(int*)&(header[0x16]);

	//sometimes shit is missing, we can sort of guess some of it though:
	if (imageSize == 0)    imageSize = width*height * 3; // 3 : one byte for each Red, Green and Blue component
	if (dataPos == 0)      dataPos = 54; // The BMP header is done that way

										 //actual RGB data
	unsigned char * data = new unsigned char[imageSize];

	// Read the actual data from the file into the buffer
	fread(data, 1, imageSize, file);

	//Everything is in memory now, the file can be closed
	fclose(file);


	//NOW we have to actually use this data in openGL
	// Create one OpenGL texture
	GLuint textureID;
	glGenTextures(1, &textureID);

	// "Bind" the newly created texture : all future texture functions will modify this texture
	glBindTexture(GL_TEXTURE_2D, textureID);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	//glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
	//glTexImage2D(GL_TEXTURE_2D, 0, 3, 256, 256, 0, GL_BGR_EXT, GL_UNSIGNED_BYTE, data);

	//for (int i = 0; i < width * height * 3; ++i)
	//{
	//	printf("%c", data[i]);
	//}
	//
	// Give the image to OpenGL
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_BGR_EXT, GL_UNSIGNED_BYTE, data);

	return textureID;
}

