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


std::vector<std::unique_ptr<utility::event::Event>>
	evan::IDesktopPlatform::pollEvents(ADeviceBackend &deviceBackend)
{
	glfwPollEvents();

	std::vector<std::unique_ptr<utility::event::Event>> events;

	events.insert(events.end(),
			   std::make_move_iterator(_keyboardEvents.begin()),
			   std::make_move_iterator(_keyboardEvents.end()));

	// Clear keyboard events after processing them
	_keyboardEvents.clear();

	events.insert(events.end(),
			   std::make_move_iterator(_mouseButtonEvents.begin()),
			   std::make_move_iterator(_mouseButtonEvents.end()));

	// Clear mouse button events after processing them
	_mouseButtonEvents.clear();

	events.insert(events.end(),
			   std::make_move_iterator(_mouseMotionEvents.begin()),
			   std::make_move_iterator(_mouseMotionEvents.end()));

	// Clear mouse motion events after processing them
	_mouseMotionEvents.clear();

	return events;
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

utility::event::KeyboardEvent::KeyCode
	evan::IDesktopPlatform::convertGlfwKeyToKeyCode(int glfwKey) const
{
	return static_cast<utility::event::KeyboardEvent::KeyCode>(glfwKey);
}

utility::event::MouseButtonEvent::Button evan::IDesktopPlatform::convertGlfwMouseButtonToButton(int glfwButton) const
{
	switch (glfwButton) {
	case GLFW_MOUSE_BUTTON_LEFT:
		return utility::event::MouseButtonEvent::Button::Left;
	case GLFW_MOUSE_BUTTON_RIGHT:
		return utility::event::MouseButtonEvent::Button::Right;
	case GLFW_MOUSE_BUTTON_MIDDLE:
		return utility::event::MouseButtonEvent::Button::Middle;
	case GLFW_MOUSE_BUTTON_4:
		return utility::event::MouseButtonEvent::Button::X1;
	case GLFW_MOUSE_BUTTON_5:
		return utility::event::MouseButtonEvent::Button::X2;
	default:
		return utility::event::MouseButtonEvent::Button::Unknown;
	}
}

utility::event::KeyboardEvent::KeyModifiers
	evan::IDesktopPlatform::convertGlfwModsToKeyModifiers(int glfwMods) const
{
	std::uint16_t result = 0;

	if (glfwMods & GLFW_MOD_SHIFT) {
		result |= static_cast<std::uint16_t>(utility::event::KeyboardEvent::KeyModifiers::Shift);
	}
	if (glfwMods & GLFW_MOD_CONTROL) {
		result |= static_cast<std::uint16_t>(utility::event::KeyboardEvent::KeyModifiers::Ctrl);
	}
	if (glfwMods & GLFW_MOD_ALT) {
		result |= static_cast<std::uint16_t>(utility::event::KeyboardEvent::KeyModifiers::Alt);
	}
	if (glfwMods & GLFW_MOD_SUPER) {
		result |= static_cast<std::uint16_t>(utility::event::KeyboardEvent::KeyModifiers::Gui);
	}

	return static_cast<utility::event::KeyboardEvent::KeyModifiers>(result);
}

utility::event::MouseMotionEvent::MousePosition
	evan::IDesktopPlatform::getMousePosition() const
{
	double xPos = 0.0;
	double yPos = 0.0;

	glfwGetCursorPos(_window, &xPos, &yPos);
	return {static_cast<float>(xPos), static_cast<float>(yPos)};
}
