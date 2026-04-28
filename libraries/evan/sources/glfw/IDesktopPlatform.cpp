/*
** ETIB PROJECT, 2026
** evan
** File description:
** IDesktopPlatform
*/

#include "glfw/IDesktopPlatform.hpp"

evan::IDesktopPlatform::~IDesktopPlatform()
{
	glfwDestroyWindow(_window);
	glfwTerminate();
}

////////////////////
// Public Methods //
////////////////////

std::shared_ptr<evan::ADeviceBackend>
	evan::IDesktopPlatform::createDeviceBackend() const
{
	return std::make_shared<DesktopBackend>(*this);
}

std::shared_ptr<evan::ASwapchainContext>
	evan::IDesktopPlatform::createSwapchainContext(
		const DeviceContext &deviceContext) const
{
	return std::make_shared<DesktopSwapchainContext>(deviceContext, _window);
}


void evan::IDesktopPlatform::pollEvents(ADeviceBackend &deviceBackend)
{
	glfwPollEvents();
	auto keyboardEvents = getEventKeyboardEvents();
}

std::vector<std::string>
	evan::IDesktopPlatform::getRequiredInstanceExtensions() const
{
	uint32_t glfwExtensionCount = 0;
	const char **glfwExtensions =
		glfwGetRequiredInstanceExtensions(&glfwExtensionCount);
	std::vector<const char *> extensions(glfwExtensions,
										 glfwExtensions + glfwExtensionCount);

	extensions.push_back(VK_KHR_PORTABILITY_ENUMERATION_EXTENSION_NAME);
	if (enableValidationLayers == true) {
		extensions.push_back(VK_EXT_DEBUG_UTILS_EXTENSION_NAME);
	}
	return std::vector<std::string>(extensions.begin(), extensions.end());
}

bool evan::IDesktopPlatform::shouldClose() const
{
	return glfwWindowShouldClose(_window);
}

/////////////////////
// Private Methods //
/////////////////////

std::vector<std::unique_ptr<utility::event::KeyboardEvent>>
	evan::IDesktopPlatform::getEventKeyboardEvents(void) const
{
	std::vector<std::unique_ptr<utility::event::KeyboardEvent>> events;

	for (int key = GLFW_KEY_SPACE; key <= GLFW_KEY_LAST; ++key) {
		if (glfwGetKey(_window, key) == GLFW_PRESS) {
			auto event = std::make_unique<utility::event::KeyboardEvent>();
			event->setKeycode(static_cast<utility::event::KeyboardEvent::KeyCode>(key));
			events.push_back(std::move(event));
			std::cout << "Key pressed: " << key << std::endl;
		}
	}
	return events;
}

utility::event::KeyboardEvent::KeyCode
	evan::IDesktopPlatform::convertGlfwKeyToKeyCode(int glfwKey) const
{
	return static_cast<utility::event::KeyboardEvent::KeyCode>(glfwKey);
}