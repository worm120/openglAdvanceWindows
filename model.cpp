#include "model.h"
#include "misc.h"
#include <stdio.h>
#include <string>
#include <sstream>
#include <vector>

VertexData*LoadObjModel(const char* filePath, unsigned int **indexes, int&vertexCount, int&indexCount)
{
	char*fileContent = LoadFileContent(filePath);
	if (fileContent!=nullptr)
	{
		//obj model decode
		struct VertexInfo
		{
			float v[3];
		};

		struct VertexDefine
		{
			int positionIndex;
			int texcoordIndex;
			int normalIndex;
		};
		std::vector<VertexInfo> positions;
		std::vector<VertexInfo> texcoords;
		std::vector<VertexInfo> normals;

		std::vector<unsigned int> objIndexes;// -> opengl indexes
		std::vector<VertexDefine> vertices;// -> opengl vertexes

		std::stringstream ssObjFile(fileContent);
		char szOneLine[256];
		std::string temp;
		while (!ssObjFile.eof())
		{
			memset(szOneLine, 0, 256);
			ssObjFile.getline(szOneLine,256);
			if (strlen(szOneLine)>0)
			{
				std::stringstream ssOneLine(szOneLine);

				if (szOneLine[0]=='v')
				{
					if (szOneLine[1]=='t')
					{
						//vertex coord
						ssOneLine >> temp;
						VertexInfo vi;
						ssOneLine >> vi.v[0];
						ssOneLine >> vi.v[1];
						texcoords.push_back(vi);
						//printf("%s %f,%f\n", temp.c_str(), vi.v[0], vi.v[1]);
					}
					else if(szOneLine[1]=='n')
					{
						//normal
						ssOneLine >> temp;
						VertexInfo vi;
						ssOneLine >> vi.v[0];
						ssOneLine >> vi.v[1];
						ssOneLine >> vi.v[2];
						normals.push_back(vi);
						//printf("%s %f,%f,%f\n", temp.c_str(), vi.v[0], vi.v[1], vi.v[2]);
					}
					else
					{
						//position
						ssOneLine >> temp;
						VertexInfo vi;
						ssOneLine >> vi.v[0];
						ssOneLine >> vi.v[1];
						ssOneLine >> vi.v[2];
						positions.push_back(vi);
						//printf("%s %f,%f,%f\n",temp.c_str(), vi.v[0], vi.v[1], vi.v[2]);
					}
				}
				else if (szOneLine[0] == 'f')
				{
					//face
					ssOneLine >> temp;// 'f'
					std::string vertexStr;
					for (int i=0;i<3;i++)
					{
						ssOneLine >> vertexStr;
						size_t pos = vertexStr.find_first_of('/');
						std::string positionIndexStr = vertexStr.substr(0, pos);
						size_t pos2 = vertexStr.find_first_of('/', pos + 1);
						std::string texcoordIndexStr = vertexStr.substr(pos + 1, pos2 - pos - 1);
						std::string normalIndexStr = vertexStr.substr(pos2 + 1, vertexStr.length() - pos2 - 1);
						VertexDefine vd;
						vd.positionIndex = atoi(positionIndexStr.c_str())-1;
						vd.texcoordIndex = atoi(texcoordIndexStr.c_str()) - 1;
						vd.normalIndex = atoi(normalIndexStr.c_str()) - 1;

						int nCurrentIndex = -1;//indexes
						//check if exist
						size_t nCurrentVerticeCount = vertices.size();
						for (size_t j = 0; j < nCurrentVerticeCount; j++)
						{
							if (vertices[j].positionIndex == vd.positionIndex&&
								vertices[j].texcoordIndex == vd.texcoordIndex&&
								vertices[j].normalIndex == vd.normalIndex)
							{
								nCurrentIndex = j;
								break;
							}
						}
						if (nCurrentIndex==-1)
						{
							//create new vertice
							nCurrentIndex = vertices.size();
							vertices.push_back(vd);//vertexes define
						}
						objIndexes.push_back(nCurrentIndex);
					}
				}
			}
		}
		//printf("face count %u\n",objIndexes.size()/3);
		//objIndexes->indexes buffer -> ibo
		indexCount = (int)objIndexes.size();
		*indexes = new unsigned int[indexCount];
		for (int i=0;i<indexCount;i++)
		{
			(*indexes)[i] = objIndexes[i];
		}
		//vertices -> vertexes -> vbo
		vertexCount = (int)vertices.size();
		VertexData*vertexes = new VertexData[vertexCount];
		for (int i=0;i<vertexCount;++i)
		{
			memcpy(vertexes[i].position, positions[vertices[i].positionIndex].v, sizeof(float) * 3);
			memcpy(vertexes[i].texcoord, texcoords[vertices[i].texcoordIndex].v, sizeof(float) * 2);
			memcpy(vertexes[i].normal, normals[vertices[i].normalIndex].v, sizeof(float) * 3);
		}
		return vertexes;
	}
	return nullptr;
}