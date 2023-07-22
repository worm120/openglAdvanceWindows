#include <windows.h>
#include <stdio.h>
#include "glew.h"
#include "wglew.h"
#include "misc.h"
#include "model.h"
#include "Glm/glm.hpp"
#include "Glm/ext.hpp"
#include "timer.h"
#include "frustum.h"
#pragma comment(lib,"opengl32.lib")
#pragma comment(lib,"glew32.lib")
#pragma comment(lib,"glu32.lib")

struct FloatBundle
{
	float v[4];
};

float frandom()//0~1
{
	return rand() / (float)RAND_MAX;
}

float sfrandom()//-1~1
{
	return frandom()*2.0f - 1.0f;
}

HGLRC CreateNBRC(HDC dc)
{
	HGLRC rc;
	GLint attribs[]{
		WGL_DRAW_TO_WINDOW_ARB,GL_TRUE,
		WGL_ACCELERATION_ARB,WGL_FULL_ACCELERATION_ARB,
		WGL_DOUBLE_BUFFER_ARB,GL_TRUE,
		WGL_RED_BITS_ARB,8,
		WGL_GREEN_BITS_ARB,8,
		WGL_BLUE_BITS_ARB,8,
		WGL_ALPHA_BITS_ARB,8,
		WGL_DEPTH_BITS_ARB,24,
		WGL_STENCIL_BITS_ARB,8,
		WGL_SAMPLE_BUFFERS_ARB,GL_TRUE,
		WGL_SAMPLES_ARB,16,
		NULL,NULL
	};
	int pixelFormat[256] = {0};
	UINT formatNum = 0;
	wglChoosePixelFormatARB(dc, attribs, NULL, 256, pixelFormat, &formatNum);
	printf("support format number is %u\n",formatNum);
	if (formatNum>0)
	{
		PIXELFORMATDESCRIPTOR pfd;
		DescribePixelFormat(dc, pixelFormat[0], sizeof(pfd),&pfd);
		SetPixelFormat(dc, pixelFormat[0], &pfd);

		int contexAttributes[] = {
			WGL_CONTEXT_MAJOR_VERSION_ARB,4,
			WGL_CONTEXT_MINOR_VERSION_ARB,3,
			WGL_CONTEXT_PROFILE_MASK_ARB,WGL_CONTEXT_COMPATIBILITY_PROFILE_BIT_ARB,
			0
		};
		rc = wglCreateContextAttribsARB(dc, nullptr, contexAttributes);
	}
	return rc;
}

LRESULT CALLBACK GLWindowProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	switch (msg)
	{
	case WM_CLOSE:
		PostQuitMessage(0);
		break;
	}
	return DefWindowProc(hwnd,msg,wParam,lParam);
}

INT WINAPI WinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ LPSTR lpCmdLine, _In_ int nShowCmd)
{
	WNDCLASSEX wndClass;
	wndClass.cbClsExtra = 0;
	wndClass.cbSize = sizeof(WNDCLASSEX);
	wndClass.cbWndExtra = 0;
	wndClass.hbrBackground = NULL;
	wndClass.hCursor = LoadCursor(NULL,IDC_ARROW);
	wndClass.hIcon = NULL;
	wndClass.hIconSm = NULL;
	wndClass.hInstance = hInstance;
	wndClass.lpfnWndProc=GLWindowProc;
	wndClass.lpszClassName = L"OpenGL";
	wndClass.lpszMenuName = NULL;
	wndClass.style = CS_VREDRAW | CS_HREDRAW;
	ATOM atom = RegisterClassEx(&wndClass);
	RECT rect;
	rect.left = 0;
	rect.right = 800;
	rect.bottom = 600;
	rect.top = 0;
	AdjustWindowRect(&rect, WS_OVERLAPPEDWINDOW, FALSE);

	HWND hwnd = CreateWindowEx(NULL, L"OpenGL", L"RenderWindow", WS_OVERLAPPEDWINDOW, 100, 100, rect.right-rect.left, rect.bottom-rect.top, NULL, NULL, hInstance, NULL);
	HDC dc = GetDC(hwnd);
	PIXELFORMATDESCRIPTOR pfd;
	memset(&pfd, 0, sizeof(PIXELFORMATDESCRIPTOR));
	pfd.nVersion = 1;
	pfd.dwFlags = PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL | PFD_TYPE_RGBA | PFD_DOUBLEBUFFER;
	pfd.iLayerType = PFD_MAIN_PLANE;
	pfd.iPixelType = PFD_TYPE_RGBA;
	pfd.cColorBits = 32;
	pfd.cDepthBits = 24;
	pfd.cStencilBits = 8;

	int pixelFormatID = ChoosePixelFormat(dc, &pfd);

	SetPixelFormat(dc,pixelFormatID,&pfd);
	
	HGLRC rc = wglCreateContext(dc);
	wglMakeCurrent(dc, rc);

	glewInit();
	if (wglChoosePixelFormatARB)
	{
		//destroy window
		wglMakeCurrent(dc, nullptr);
		wglDeleteContext(rc);
		rc = nullptr;
		ReleaseDC(hwnd, dc);
		dc = nullptr;
		DestroyWindow(hwnd);
		hwnd = CreateWindowEx(NULL, L"OpenGL", L"RenderWindow", WS_OVERLAPPEDWINDOW, 100, 100, rect.right - rect.left, rect.bottom - rect.top, NULL, NULL, hInstance, NULL);
		//create msaa rc
		dc = GetDC(hwnd);
		rc = CreateNBRC(dc);
		wglMakeCurrent(dc, rc);
	}

	int width, height;

	GetClientRect(hwnd, &rect);
	width = rect.right - rect.left;
	height = rect.bottom - rect.top;

	glm::mat4 model = glm::translate(0.0f, 0.0f, -2.0f);
	glm::mat4 projection = glm::perspective(45.0f, (float)width / (float)height, 0.1f, 1000.0f);
	glm::mat4 normalMatrix = glm::inverseTranspose(model);

	FloatBundle vertexes[3];
	vertexes[0].v[0] = 0.0f;
	vertexes[0].v[1] = 0.0f;
	vertexes[0].v[2] = 0.0f;
	vertexes[0].v[3] = 1.0f;

	vertexes[1].v[0] = 0.5f;
	vertexes[1].v[1] = 0.0f;
	vertexes[1].v[2] = 0.0f;
	vertexes[1].v[3] = 1.0f;

	vertexes[2].v[0] = 0.0f;
	vertexes[2].v[1] = 0.5f;
	vertexes[2].v[2] = 0.0f;
	vertexes[2].v[3] = 1.0f;

	GLuint vbo = CreateBufferObject(GL_ARRAY_BUFFER, sizeof(FloatBundle)*3, GL_STATIC_DRAW, &vertexes);
	GLuint tfo;
	glGenTransformFeedbacks(1, &tfo);
	glBindTransformFeedback(GL_TRANSFORM_FEEDBACK, tfo);
	GLuint tfoBuffer = CreateBufferObject(GL_ARRAY_BUFFER, sizeof(FloatBundle) * 3, GL_STATIC_DRAW, nullptr);
	glBindTransformFeedback(GL_TRANSFORM_FEEDBACK, 0);

	//local coordinate -> world coordinate : execute once
	GLuint vsShader = CompileShader(GL_VERTEX_SHADER, "res/shader/tfo_translateM.vs");
	GLuint fsShader = CompileShader(GL_FRAGMENT_SHADER, "res/shader/tfo_translateM.fs");
	GLuint translateMProgram = glCreateProgram();
	glAttachShader(translateMProgram, vsShader);
	glAttachShader(translateMProgram, fsShader);

	const char*attribs[] = {"gl_Position"};
	glTransformFeedbackVaryings(translateMProgram, 1, attribs, GL_INTERLEAVED_ATTRIBS);

	glLinkProgram(translateMProgram);
	glDetachShader(translateMProgram, vsShader);
	glDetachShader(translateMProgram, fsShader);
	glDeleteShader(vsShader);
	glDeleteShader(fsShader);
	GLint translateMPosLocation, translateMMLocation;
	translateMPosLocation = glGetAttribLocation(translateMProgram, "pos");
	translateMMLocation = glGetUniformLocation(translateMProgram, "M");

	glBindTransformFeedback(GL_TRANSFORM_FEEDBACK, tfo);
	glBindBufferBase(GL_TRANSFORM_FEEDBACK_BUFFER, 0, tfoBuffer);
	glUseProgram(translateMProgram);
	glUniformMatrix4fv(translateMMLocation, 1, GL_FALSE, glm::value_ptr(model));

	glBeginTransformFeedback(GL_TRIANGLES);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glEnableVertexAttribArray(translateMPosLocation);
	glVertexAttribPointer(translateMPosLocation, 4, GL_FLOAT, GL_FALSE, sizeof(float) * 4, 0);
	glDrawArrays(GL_TRIANGLES, 0, 3);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glEndTransformFeedback();
	glUseProgram(0);
	
	glBindBuffer(GL_ARRAY_BUFFER, tfoBuffer);
	FloatBundle*vertices = (FloatBundle*)glMapBuffer(GL_ARRAY_BUFFER,GL_READ_WRITE);
	for (int i=0;i<3;i++)
	{
		printf("%f,%f,%f,%f\n",vertices[i].v[0], vertices[i].v[1], vertices[i].v[2], vertices[i].v[3]);
	}

	glUnmapBuffer(GL_ARRAY_BUFFER);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	//above is success

	//world world -> screen coordinate : execute per frame
	GLuint program = CreateGPUProgram("res/shader/tfo_translateScreen.vs", "res/shader/tfo_translateScreen.fs");
	GLint VLocation, PLocation,posLocation;

	posLocation = glGetAttribLocation(program,"pos");
	VLocation = glGetUniformLocation(program, "V");
	PLocation = glGetUniformLocation(program, "P");

	
	GL_CALL(glClearColor(0.1f, 0.4f, 0.7f,1.0f));
	ShowWindow(hwnd, SW_SHOW);
	UpdateWindow(hwnd);
	glViewport(0, 0, width,height);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE);
	glDisable(GL_CULL_FACE);
	float identity[] = {
		1,0,0,0,
		0,1,0,0,
		0,0,1,0,
		0,0,0,1
	};
	MSG msg;
	while (true)
	{
		if (PeekMessage(&msg,NULL,NULL,NULL,PM_REMOVE))
		{
			if (msg.message==WM_QUIT)
			{
				break;
			}
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		glUseProgram(program);
		glUniformMatrix4fv(VLocation, 1, GL_FALSE, identity);
		glUniformMatrix4fv(PLocation, 1, GL_FALSE, glm::value_ptr(projection));

		glBindBuffer(GL_ARRAY_BUFFER, tfoBuffer);
		glEnableVertexAttribArray(posLocation);
		glVertexAttribPointer(posLocation, 4, GL_FLOAT, GL_FALSE, sizeof(float) * 4, 0);
		glDrawArrays(GL_TRIANGLES, 0,3);
		glBindBuffer(GL_ARRAY_BUFFER, 0);

		glUseProgram(0);
		glFlush();
		SwapBuffers(dc);
	}
	return 0;
}