#pragma once


#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"
#include "glad/glad.h"
#include "GLFW/glfw3.h"
#include <ctype.h>          // toupper
#include <limits.h>         // INT_MIN, INT_MAX
#include <math.h>           // sqrtf, powf, cosf, sinf, floorf, ceilf
#include <stdio.h>          // vsnprintf, sscanf, printf
#include <stdlib.h>
//#include "imgui_impl_opengl3_loader.h"


#include "showimgui.h"

class UseImGui
{
public: 
	GLFWwindow* window;
	const char* glsl_version;
	UseImGui(GLFWwindow* window, const char* glsl_version) : window(window), glsl_version(glsl_version)
	{

	}
	void Init();
	void Newframe();
	virtual void Update();
	void Render();
	void Shutdown();
};

