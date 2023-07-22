#pragma once
#include "glew.h"
#include <functional>

GLuint CreateBufferObject(GLenum bufferType, GLsizeiptr size, GLenum usage, void*data = nullptr);
GLuint CreateVAOWithVBOSettings(std::function<void()>settings);
GLuint CreateFramebufferObject(GLuint&colorBuffer,GLuint&depthBuffer, int width, int height,GLuint *colorBuffer2=nullptr);

char *LoadFileContent(const char*path);
GLuint CompileShader(GLenum shaderType,const char*shaderPath);
GLuint CreateGPUProgram(const char*vsShaderPath, const char*fsShaderPath,const char*gsPath=nullptr);
GLuint CreateComputeProgram(const char*computeShaderPath);
unsigned char*DecodeBMPData(unsigned char*imgData, int&width, int&height);
GLuint CreateTextureFromFile(const char*imagePath);
GLuint CreateTexture3D(int w, int h, int d);
void SaveImage(const char*imagePath,unsigned char*imgData,int width,int height);

void CheckGLError(const char*file,int line);
#define GL_CALL(x) do{ x;CheckGLError(__FILE__,__LINE__);}while (0)