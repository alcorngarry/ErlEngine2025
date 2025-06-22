#include"Engine.h"

GLFWwindow* window;
GLFWmonitor* monitor;
const GLFWvidmode* mode;
bool fullScreen = false;
int windowedX, windowedY, windowedWidth, windowedHeight;

void framebuffer_size_callback(GLFWwindow* window, int width, int height);

GLFWwindow* Engine::start(float windowWidth, float windowHeight)
{
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	monitor = glfwGetPrimaryMonitor();
	mode = glfwGetVideoMode(monitor);
	
	if (fullScreen)
	{
		window = glfwCreateWindow(mode->width, mode->height, "Sonar", glfwGetPrimaryMonitor(), NULL);
	} else {
		window = glfwCreateWindow(windowWidth, windowHeight, "Sonar", NULL, NULL);
	}
	
	if (window == NULL)
	{
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
	}

	glfwGetWindowPos(window, &windowedX, &windowedY);
	glfwGetWindowSize(window, &windowedWidth, &windowedHeight);
	glfwMakeContextCurrent(window);

	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Failed to initialize GLAD" << std::endl;
	}

	glEnable(GL_DEPTH_TEST);
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
	glfwWindowHint(GLFW_SAMPLES, 128);
	glEnable(GL_MULTISAMPLE);

	Renderer::init_render(window);
	InputManager::init(window);
	DebugMenu::init(window);
	UIManager::init(windowWidth, windowHeight);

	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
	return window;
}

void Engine::toggle_full_screen()
{
	fullScreen = !fullScreen;

	if (fullScreen) {
		glfwSetWindowMonitor(window, glfwGetPrimaryMonitor(), 0, 0, mode->width, mode->height, mode->refreshRate);
	}
	else {
		glfwSetWindowMonitor(window, NULL, windowedX, windowedY, windowedWidth, windowedHeight, 0);
	}
}

void Engine::shut_down(GLFWwindow* window)
{
	glfwDestroyWindow(window);
	glfwTerminate();
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	glViewport(0, 0, width / 2, height);
}