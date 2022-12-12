//Entry point of the application


#include "QMessage.h"



int main()
{
	if (!initSockEnv())
	{
		std::cout << "socket environment initialization failed." << std::endl;
		return 1;
	}
	if (!glfwInit())
	{
		return 1;
	}

	const char* glsl_version = "#version 130";
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);
	glfwWindowHint(GLFW_TRANSPARENT_FRAMEBUFFER, GLFW_TRUE);



	GLFWwindow* window = glfwCreateWindow(1280, 720, "QMessage", NULL, NULL);
	if (window == NULL)
		return 1;


	glfwMakeContextCurrent(window);
	glfwSwapInterval(1);

	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Failed to initialize GLAD" << std::endl;
		return -1;
	}


	int screen_width, screen_height;
	glfwGetFramebufferSize(window, &screen_width, &screen_height);
	//glViewport(0, 0, screen_width, screen_height);



	UseImGui myimgui(window, glsl_version);


	myimgui.Init();
	while (!glfwWindowShouldClose(window))
	{
		glfwPollEvents();
		myimgui.Newframe();
		myimgui.Update();
		myimgui.Render();
		glfwSwapBuffers(window);
	}
	myimgui.Shutdown();






    



    return 0;
}
