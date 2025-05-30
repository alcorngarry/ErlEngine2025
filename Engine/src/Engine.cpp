#include"Engine.h"

void framebuffer_size_callback(GLFWwindow* window, int width, int height);

bool fullScreen = false;

GLFWwindow* Engine::start(float windowWidth, float windowHeight)
{
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	GLFWwindow* window;
	
	if (fullScreen)
	{
		const GLFWvidmode* mode = glfwGetVideoMode(glfwGetPrimaryMonitor());
		float WIN_WIDTH = mode->width;
		float WIN_HEIGHT = mode->height;

		window = glfwCreateWindow(WIN_WIDTH, WIN_HEIGHT, "Sonar", glfwGetPrimaryMonitor(), NULL);
	}
	else {
		window = glfwCreateWindow(windowWidth, windowHeight, "Sonar", NULL, NULL);
	}
	
	if (window == NULL)
	{
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
	}

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

void Engine::set_full_screen(bool isFullScreen)
{
	fullScreen = isFullScreen;
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