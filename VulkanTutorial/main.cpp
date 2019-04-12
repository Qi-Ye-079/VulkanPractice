
#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#include <iostream>
#include <stdexcept>
#include <cstdlib>
#include <functional>
#include <vector>

const int WIDTH = 800;
const int HEIGHT = 600;

const std::vector<const char*> validationLayers =
{
	"VK_LAYER_LUNARG_standard_validation"
};

#ifdef _DEBUG
const bool enableValidationLayers = true;
#else
const bool enableValidationLayers = false;
#endif

class HelloTriangleApplication
{
public:
	void run()
	{
		initWindow();
		initVulkan();
		mainLoop();
		cleanup();
	}

private:
	GLFWwindow* window;
	VkInstance instance;

	// Initialize OpenGL and create a window
	void initWindow()
	{
		if (!glfwInit())
		{
			std::cerr << "GLFW initialization failed!!" << std::endl;
		}

		// Create window hints - no OpenGL context
		glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
		glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);

		// Initialize window
		window = glfwCreateWindow(WIDTH, HEIGHT, "Vulkan", nullptr, nullptr);
	}

	void createInstance()
	{
		if (enableValidationLayers && !checkValidationLayerSupport())
		{
			throw std::runtime_error("Validation Layer not supported!!");
		}

		// Optional: application info
		VkApplicationInfo appInfo = {};
		appInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
		appInfo.pApplicationName = "Hello Triangle";
		appInfo.applicationVersion = VK_MAKE_VERSION(1, 0, 0);
		appInfo.pEngineName = "No Engine";
		appInfo.engineVersion = VK_MAKE_VERSION(1, 0, 0);
		appInfo.apiVersion = VK_API_VERSION_1_0;

		// Mandatory: instance create info
		VkInstanceCreateInfo createInfo = {};
		createInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
		createInfo.pApplicationInfo = &appInfo;
		if (enableValidationLayers)
		{
			createInfo.enabledLayerCount = (uint32_t)validationLayers.size();
			createInfo.ppEnabledLayerNames = validationLayers.data();
		}
		else
		{
			createInfo.enabledLayerCount = 0;
		}

		// get the needed extensions by GLFW
		uint32_t glfwExtensionCount = 0;
		const char** glfwExtensions;
		glfwExtensions = glfwGetRequiredInstanceExtensions(&glfwExtensionCount);
		createInfo.enabledExtensionCount = glfwExtensionCount;
		createInfo.ppEnabledExtensionNames = glfwExtensions;
		createInfo.enabledLayerCount = 0;

		// Create Vulkan instance
		if (vkCreateInstance(&createInfo, nullptr, &instance) != VK_SUCCESS)
		{
			throw std::runtime_error("Failed to create instance!");
		}

		// Get the list of supported instance extensions
		uint32_t extensionCount = 0;
		vkEnumerateInstanceExtensionProperties(nullptr, &extensionCount, nullptr);
		std::vector<VkExtensionProperties> extProperties(extensionCount);
		vkEnumerateInstanceExtensionProperties(nullptr, &extensionCount, extProperties.data());

		std::cout << "Available extensions: " << std::endl;
		for (int i = 0; i < extProperties.size(); i++)
		{
			std::cout << extProperties[i].extensionName << "\t" << extProperties[i].specVersion << std::endl;
			std::cout << "==========================================" << std::endl;
		}

		std::cout << checkRequiredExtensionsIncluded(glfwExtensions, glfwExtensionCount, extProperties) << std::endl;
	}

	void initVulkan()
	{
		createInstance();
	}

	void mainLoop()
	{
		while (!glfwWindowShouldClose(window))
		{
			glfwPollEvents();
		}

	}

	void cleanup()
	{
		vkDestroyInstance(instance, nullptr);

		glfwDestroyWindow(window);

		glfwTerminate();
	}

	bool checkRequiredExtensionsIncluded(
		const char**                              glfwExtensions,
		uint32_t                                  glfwExtensionCount,
		const std::vector<VkExtensionProperties>& extVector) const
	{
		bool match = true;
		for (int i = 0; i < glfwExtensionCount; i++)
		{
			if (strcmp(glfwExtensions[i], extVector[i].extensionName) != 0)
			{
				match = false;
				break;
			}
		}
		return match;
	}

	/**
	 * Check for all available layers supported
	 */
	bool checkValidationLayerSupport()
	{
		// Find all available layers
		uint32_t layerCount;
		vkEnumerateInstanceLayerProperties(&layerCount, nullptr);
		std::vector<VkLayerProperties> availableLayers(layerCount);
		vkEnumerateInstanceLayerProperties(&layerCount, availableLayers.data());

		// Check if all validation layers can be found in available layers
		for (const char* layer : validationLayers)
		{
			bool layerFound = false;

			for (const VkLayerProperties& layerProperty : availableLayers)
			{
				if (strcmp(layer, layerProperty.layerName) == 0)
				{
					layerFound = true;
					break;
				}
			}

			if (!layerFound) return false;
		}
		return true;
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

