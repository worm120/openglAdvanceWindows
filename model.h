#pragma once

struct VertexData
{
	float position[4];
	float texcoord[4];
	float normal[4];
};

VertexData*LoadObjModel(const char* filePath,unsigned int **indexes,int&vertexCount,int&indexCount);