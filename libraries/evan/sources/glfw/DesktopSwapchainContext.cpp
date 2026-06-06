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
	this->createRenderPass(deviceContext.getDeviceBackend(),
						   deviceContext.getMsaaSamples());
	_swapchainImages.push_back(std::make_shared<DesktopSwapchainImage>(
		deviceContext, window, _renderPass));
}

evan::DesktopSwapchainContext::~DesktopSwapchainContext()
{
}

////////////////////
// Public Methods //
////////////////////

void evan::DesktopSwapchainContext::destroy(VkDevice device)
{
	vkDestroyRenderPass(device, _renderPass, nullptr);
	for (const auto &swapchainImage: _swapchainImages) {
		swapchainImage->destroy(device);
	}
}

VkResult evan::DesktopSwapchainContext::aquireImage(
	uint32_t index, VkDevice device, VkSemaphore imageAvailableSemaphore,
	VkFence inFlightFence, uint32_t &imageIndex)
{
	VkSwapchainKHR swapchain =
		dynamic_cast<DesktopSwapchainImage *>(_swapchainImages[index].get())
			->_swapchain;

	return vkAcquireNextImageKHR(device, swapchain, UINT64_MAX,
								 imageAvailableSemaphore, inFlightFence,
								 &imageIndex);
}

glm::mat4 evan::DesktopSwapchainContext::getProjection(int index) const
{
	return glm::perspective(glm::radians(45.0f), _view.getAspectRatio(), 0.1f,
							500.0f);
}

glm::mat4 evan::DesktopSwapchainContext::getView(int index) const
{
	glm::vec3 eyePosition(_view.getPose().getPosition().getX(),
						  _view.getPose().getPosition().getY(),
						  _view.getPose().getPosition().getZ());
	glm::vec3 forward(_view.getPose().getOrientation().getForward());
	glm::vec3 up(_view.getPose().getOrientation().getUp());
	return glm::lookAt(eyePosition, eyePosition + forward, up);
}