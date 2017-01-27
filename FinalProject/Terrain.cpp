#include "Terrain.h"

Terrain::Terrain(int w2, int l2, float sz, float hs)
{
	this->width = w2;
	this->length = l2;
	this->size = sz;
	this->heightScale = hs;

	heights = new float *[this->length];
	for (int i = 0; i < this->length; ++i)
	{
		heights[i] = new float[this->width];
	}

	normals = new Vector3*[this->length];
	for (int i = 0; i < this->length; ++i)
	{
		normals[i] = new Vector3[this->width];
	}

	computedNormals = false;

	//set position, rotation
	this->position = new Vector3(0, 0, 0);
	this->rotation = new Vector3(0, 0, 0);
}

Terrain::~Terrain()
{

}

void Terrain::SetHeight(int x, int z, float y)
{
	heights[z][x] = y;
	computedNormals = false;
}
float Terrain::GetHeight(int x, int z)
{
	return heights[x][z];
}
void Terrain::ComputeNormals()
{
	if (computedNormals)
		return;
	//TODO generate normals
	Vector3 ** normals2 = new Vector3*[this->length];
	for (int i = 0; i < this->length; ++i)
	{
		normals2[i] = new Vector3[this->width];
	}

	for (int z = 0; z < this->length; z++)
	{
		for (int x = 0; x < this->width; ++x)
		{
			Vector3 sum(0.0, 0.0, 0.0);

			Vector3 *out = new Vector3(0, 0, 0);
			if (z > 0)
			{
				out = new Vector3(0.0,
					this->heights[z - 1][x] - this->heights[z][x], -1.0);
			}
			Vector3 *in = new Vector3(0, 0, 0);
			if (z < this->length - 1)
			{
				in = new Vector3(0.0, 
					this->heights[z + 1][x] - this->heights[z][x], 1.0);
			}
			Vector3 *left = new Vector3(0, 0, 0);
			if (x > 0)
			{
				left = new Vector3(-1.0,
					this->heights[z][x - 1] - this->heights[z][x], 0.0);
			}
			Vector3 *right = new Vector3(0,0,0);
			if (x < this->width - 1)
			{
				right = new Vector3(1.0,
					this->heights[z][x + 1] - this->heights[z][x], 0.0);
			}

			if (x > 0 && z > 0)
			{
				Vector3 *v = out->cross(*left);
				v->norm();
				//do math now
				sum.x = sum.x + v->x;
				sum.y = sum.y + v->y;
				sum.z = sum.z + v->z;
			}
			if (x > 0 && z < this->length - 1)
			{
				Vector3 *v = left->cross(*in);
				v->norm();
				//do math now
				sum.x = sum.x + v->x;
				sum.y = sum.y + v->y;
				sum.z = sum.z + v->z;
			}
			if (x < this->width - 1 && z < this->length - 1)
			{
				Vector3 *v = in->cross(*right);
				v->norm();
				//do math now
				sum.x = sum.x + v->x;
				sum.y = sum.y + v->y;
				sum.z = sum.z + v->z;
			}
			if (x < this->width - 1 && z > 0)
			{
				Vector3 *v = right->cross(*out);
				v->norm();
				//do math now
				sum.x = sum.x + v->x;
				sum.y = sum.y + v->y;
				sum.z = sum.z + v->z;
			}

			normals2[z][x] = sum;
		}
	}

	const float falloutratio = 0.5;

	for (int z = 0; z < this->length; ++z)
	{
		for (int x = 0; x < this->width; ++x)
		{
			Vector3 sum = normals2[z][x];

			if (x > 0)
			{
				sum.x = sum.x + normals[z][x - 1].x * falloutratio;
				sum.y = sum.y + normals[z][x - 1].y * falloutratio;
				sum.z = sum.z + normals[z][x - 1].z * falloutratio;
			}
			if (x < this->width - 1)
			{
				sum.x = sum.x + normals[z][x+1].x * falloutratio;
				sum.y = sum.y + normals[z][x+1].y * falloutratio;
				sum.z = sum.z + normals[z][x+1].z * falloutratio;
			}
			if (z > 0)
			{
				sum.x = sum.x + normals[z - 1][x].x * falloutratio;
				sum.y = sum.y + normals[z - 1][x].y * falloutratio;
				sum.z = sum.z + normals[z - 1][x].z * falloutratio;
			}
			if (z < this->length - 1)
			{
				sum.x = sum.x + normals[z + 1][x].x * falloutratio;
				sum.y = sum.y + normals[z + 1][x].y * falloutratio;
				sum.z = sum.z + normals[z + 1][x].z * falloutratio;
			}

			if (sum.magnitude() == 0)
			{
				sum = Vector3(0.0, 1.0, 0.0);
			}

			normals[z][x] = sum;
		}
	}

	for (int i = 0; i < this->length; ++i)
	{
		delete[] normals2[i];
	}

	delete[] normals2;

	computedNormals = true;
}

Vector3 Terrain::GetNormal(int x, int z)
{
	if (!computedNormals)
	{
		ComputeNormals();
	}

	return normals[z][x];
}