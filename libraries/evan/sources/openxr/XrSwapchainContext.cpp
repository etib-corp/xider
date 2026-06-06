/*
** ETIB PROJECT, 2026
** evan
** File description:
** XrSwapchainContext
*/

#include "evan/openxr/XrSwapchainContext.hpp"

#include <algorithm>

evan::XrSwapchainContext::XrSwapchainContext(const DeviceContext &deviceContext)
{
	createRenderPass(deviceContext.getDeviceBackend(),
					 deviceContext.getMsaaSamples());

	uint32_t swapchainFormatCount =
		deviceContext.getDeviceBackend()->countSwapchainFormats();
	auto swapchainFormats =
		deviceContext.getDeviceBackend()->enumerateSwapchainFormats(
			swapchainFormatCount);

	_viewsConfigurations =
		dynamic_cast<XrDeviceBackend *>(deviceContext.getDeviceBackend().get())
			->enumerateViewConfigurations();
	_views.resize(_viewsConfigurations.size(), { XR_TYPE_VIEW });

	auto swapchainFormat = selectSwapchainFormat(swapchainFormats);

	for (const auto &viewConfig: _viewsConfigurations) {
		XrSwapchainCreateInfo swapchainCreateInfo {};
		swapchainCreateInfo.type	  = XR_TYPE_SWAPCHAIN_CREATE_INFO;
		swapchainCreateInfo.arraySize = 1;
		swapchainCreateInfo.format	  = swapchainFormat;
		swapchainCreateInfo.width	  = viewConfig.recommendedImageRectWidth;
		swapchainCreateInfo.height	  = viewConfig.recommendedImageRectHeight;
		swapchainCreateInfo.mipCount  = 1;
		swapchainCreateInfo.faceCount = 1;
		swapchainCreateInfo.sampleCount =
			viewConfig.recommendedSwapchainSampleCount;
		swapchainCreateInfo.usageFlags = XR_SWAPCHAIN_USAGE_SAMPLED_BIT
			| XR_SWAPCHAIN_USAGE_COLOR_ATTACHMENT_BIT;

		XrSwapchain swapchain;
		auto session = dynamic_cast<evan::XrDeviceBackend *>(
						   deviceContext.getDeviceBackend().get())
						   ->_session;
		XrResult result =
			xrCreateSwapchain(session, &swapchainCreateInfo, &swapchain);
		if (result != XR_SUCCESS) {
			std::cerr << "Failed to create swapchain for view configuration "
						 "with error code: "
					  << result << std::endl;
			continue;
		}
		evan::XrSwapchainImage::CreateXrSwapchainImageProperties properties {
			.swapchain	   = swapchain,
			.createInfo	   = swapchainCreateInfo,
			.renderPass	   = _renderPass,
			.deviceContext = deviceContext
		};
		_swapchainImages.push_back(
			std::make_shared<XrSwapchainImage>(properties));
	}
}

////////////////////
// Public Methods //
////////////////////

void evan::XrSwapchainContext::destroy(VkDevice device)
{
	for (const auto &swapchainImage: _swapchainImages) {
		swapchainImage->destroy(device);
	}
	_swapchainImages.clear();
}

VkResult evan::XrSwapchainContext::aquireImage(
	uint32_t index, VkDevice device, VkSemaphore imageAvailableSemaphore,
	VkFence inFlightFence, uint32_t &imageIndex)
{
	XrSwapchain swapchain =
		dynamic_cast<XrSwapchainImage *>(_swapchainImages[index].get())
			->_swapchain;
	XrSwapchainImageAcquireInfo acquire_info {};
	acquire_info.type = XR_TYPE_SWAPCHAIN_IMAGE_ACQUIRE_INFO;
	XrResult result =
		xrAcquireSwapchainImage(swapchain, &acquire_info, &imageIndex);
	if (result != XR_SUCCESS) {
		std::cerr << "Failed to acquire swapchain image with error code: "
				  << result << std::endl;
		return VK_ERROR_OUT_OF_DATE_KHR;
	}
	return VK_SUCCESS;
}

void evan::XrSwapchainContext::waitForImage(uint32_t index)
{
	XrSwapchain swapchain =
		dynamic_cast<XrSwapchainImage *>(_swapchainImages[index].get())
			->_swapchain;
	XrSwapchainImageWaitInfo wait_info {};
	wait_info.type	  = XR_TYPE_SWAPCHAIN_IMAGE_WAIT_INFO;
	wait_info.timeout = XR_INFINITE_DURATION;
	XrResult result	  = xrWaitSwapchainImage(swapchain, &wait_info);
	if (result != XR_SUCCESS) {
		std::cerr << "Failed to wait for swapchain image with error code: "
				  << result << std::endl;
	}
}

void evan::XrSwapchainContext::updateProjectionLayerViews()
{
	const size_t count = std::min(_views.size(), _swapchainImages.size());

	_projectionLayerViews.resize(count);
	for (size_t i = 0; i < count; ++i) {
		auto *viewSwapchain =
			dynamic_cast<XrSwapchainImage *>(_swapchainImages[i].get());

		if (viewSwapchain == nullptr) {
			continue;
		}

		_projectionLayerViews[i] = {};
		_projectionLayerViews[i].type =
			XR_TYPE_COMPOSITION_LAYER_PROJECTION_VIEW;
		_projectionLayerViews[i].pose				= _views[i].pose;
		_projectionLayerViews[i].fov				= _views[i].fov;
		_projectionLayerViews[i].subImage.swapchain = viewSwapchain->_swapchain;
		_projectionLayerViews[i].subImage.imageRect.offset = { 0, 0 };
		_projectionLayerViews[i].subImage.imageRect.extent = {
			static_cast<int32_t>(viewSwapchain->_width),
			static_cast<int32_t>(viewSwapchain->_height)
		};
	}
}

/////////////
// Getters //
/////////////

const std::vector<XrCompositionLayerProjectionView> &
	evan::XrSwapchainContext::getProjectionLayerViews() const
{
	return _projectionLayerViews;
}

glm::mat4 evan::XrSwapchainContext::getProjection(int index) const
{
	const XrFovf &fov = _views[index].fov;
	float nearZ		  = 0.1f;
	float farZ		  = 10.0f;

	// Update the view's field of view (convert from radians to degrees)
	const float radToDeg = 180.0f / 3.14159265f;
	_view.setFieldOfView(
		utility::graphic::FieldOfView<float>(
			fov.angleUp * radToDeg,	  // up
			fov.angleDown * radToDeg, // down
			fov.angleLeft * radToDeg, // left
			fov.angleRight * radToDeg // right
		));

	float tanLeft  = tanf(fov.angleLeft);
	float tanRight = tanf(fov.angleRight);
	float tanUp	   = tanf(fov.angleUp);
	float tanDown  = tanf(fov.angleDown);

	float width	 = tanRight - tanLeft;
	float height = tanUp - tanDown;

	glm::mat4 projection(0.0f);
	projection[0][0] = 2.0f / width;
	projection[1][1] = 2.0f / height;
	projection[2][0] = (tanRight + tanLeft) / width;
	projection[2][1] = (tanUp + tanDown) / height;
	projection[2][2] = -farZ / (farZ - nearZ);
	projection[2][3] = -1.0f;
	projection[3][2] = -(farZ * nearZ) / (farZ - nearZ);

	return projection;
}

glm::mat4 evan::XrSwapchainContext::getView(int index) const
{
	const auto &pose = _views[index].pose;
	const auto &fov = _views[index].fov;

	// Update the view's pose and field of view
	utility::graphic::Position<float> position(
		static_cast<float>(pose.position.x),
		static_cast<float>(pose.position.y),
		static_cast<float>(pose.position.z));

	utility::graphic::Orientation<float> orientation(
		static_cast<float>(pose.orientation.x),
		static_cast<float>(pose.orientation.y),
		static_cast<float>(pose.orientation.z),
		static_cast<float>(pose.orientation.w));

	_view.setPose(
		utility::graphic::Pose<float>(position, orientation));

	// Also update field of view (convert from radians to degrees)
	const float radToDeg = 180.0f / 3.14159265f;
	_view.setFieldOfView(
		utility::graphic::FieldOfView<float>(
			fov.angleUp * radToDeg,	  // up
			fov.angleDown * radToDeg, // down
			fov.angleLeft * radToDeg, // left
			fov.angleRight * radToDeg // right
		));

	const glm::quat orientation_glm(pose.orientation.w, pose.orientation.x,
									 pose.orientation.y, pose.orientation.z);
	const glm::vec3 position_glm(pose.position.x, pose.position.y, pose.position.z);

	const glm::mat4 rotation	= glm::mat4_cast(glm::conjugate(orientation_glm));
	const glm::mat4 translation = glm::translate(glm::mat4(1.0f), -position_glm);

	return rotation * translation;
}
