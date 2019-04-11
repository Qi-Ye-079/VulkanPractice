#include <vulkan/vulkan.hpp>

#include <iostream>
#include <stdexcept>
#include <cstdlib>
#include <functional>

class HelloTriangleApplication
{
public:
	void run()
	{
		initVulkan();
		mainLoop();
		cleanup();
	}

private:
	void initVulkan()
	{

	}

	void mainLoop()
	{

	}

	void cleanup()
	{

	}
};

int main()
{
	HelloTriangleApplication app;

	try
	{
		app.run();
	}
	catch (const std::exception& e)
	{
		std::cerr << e.what() << std::endl;
		return EXIT_FAILURE;
	}

	return EXIT_SUCCESS;
}