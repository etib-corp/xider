/*
** ETIB PROJECT, 2026
** evan
** File description:
** DesktopSwapchainContext
*/

#include "evan/glfw/DesktopSwapchainContext.hpp"

#include "evan/DeviceContext.hpp"

evan::DesktopSwapchainContext::DesktopSwapchainContext(
	const DeviceContext &deviceContext, GLFWwindow *window)
{
	this->getLogger().info() << "Initializing DesktopSwapchainContext...";

	this->createRenderPass(deviceContext.getDeviceBackend(),
						   deviceContext.getMsaaSamples());

	this->getLogger().info()
		<< "Creating swapchain images for DesktopSwapchainContext...";
	_swapchainImages.push_back(std::make_shared<DesktopSwapchainImage>(
		deviceContext, window, _renderPass));
}

evan::DesktopSwapchainContext::~DesktopSwapchainContext()
{
	this->getLogger().info() << "Destroying DesktopSwapchainContext...";
}

////////////////////
// Public Methods //
////////////////////

void evan::DesktopSwapchainContext::destroy(VkDevice device)
{
	this->getLogger().info() << "Destroying swapchain and associated resources "
								"for DesktopSwapchainContext...";

	this->getLogger().info()
		<< "Destroying render pass for DesktopSwapchainContext...";
	vkDestroyRenderPass(device, _renderPass, nullptr);

	this->getLogger().info()
		<< "Destroying swapchain images for DesktopSwapchainContext...";
	for (const auto &swapchainImage: _swapchainImages) {
		this->getLogger().info() << "Destroying swapchain image and releasing "
									"associated resources...";
		swapchainImage->destroy(device);
	}
}

VkResult evan::DesktopSwapchainContext::aquireImage(
	uint32_t index, VkDevice device, VkSemaphore imageAvailableSemaphore,
	VkFence inFlightFence, uint32_t &imageIndex)
{
	this->getLogger().info() << "Acquiring next available image from swapchain "
								"for DesktopSwapchainContext...";
	VkSwapchainKHR swapchain =
		dynamic_cast<DesktopSwapchainImage *>(_swapchainImages[index].get())
			->_swapchain;

	this->getLogger().info()
		<< "Calling vkAcquireNextImageKHR to acquire image from swapchain...";
	return vkAcquireNextImageKHR(device, swapchain, UINT64_MAX,
								 imageAvailableSemaphore, inFlightFence,
								 &imageIndex);
}

glm::mat4 evan::DesktopSwapchainContext::getProjection(int index) const
{
	return glm::perspective(glm::radians(45.0f), 16.0f / 9.0f, 0.1f, 500.0f);
}

glm::mat4 evan::DesktopSwapchainContext::getView(int index) const
{
	return _view;
}

void evan::DesktopSwapchainContext::setView(int index, const glm::mat4 &view)
{
	_view = view;
}