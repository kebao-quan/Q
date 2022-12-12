#pragma once



#include <ctype.h>          // toupper
#include <limits.h>         // INT_MIN, INT_MAX
#include <math.h>           // sqrtf, powf, cosf, sinf, floorf, ceilf
#include <stdio.h>          // vsnprintf, sscanf, printf
#include <stdlib.h>
#include "UDPServer.h"
#include "UDPClient.h"
#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"
#include "glad/glad.h"
#include "GLFW/glfw3.h"


#include "console2.h"





static AppConsole console;
static void ShowAppConsole(bool* p_open)
{
    console.Draw("Example: Console", p_open);
}


