/*
** ETIB PROJECT, 2026
** evan
** File description:
** SwapchainContext
*/

#include "evan/ASwapchainContext.hpp"

void evan::ASwapchainContext::createRenderPass(
	const std::shared_ptr<ADeviceBackend> &deviceBackend,
	VkSampleCountFlagBits msaaSamples)
{
	this->getLogger().info("Creating render pass for swapchain context...");

	auto swapchainFormatCount = deviceBackend->countSwapchainFormats();
	auto swapchainFormats =
		deviceBackend->enumerateSwapchainFormats(swapchainFormatCount);
	auto swapchainFormat = selectSwapchainFormat(swapchainFormats);

	VkAttachmentDescription colorAttachment {};
	colorAttachment.format		   = swapchainFormat;
	colorAttachment.loadOp		   = VK_ATTACHMENT_LOAD_OP_CLEAR;
	colorAttachment.storeOp		   = VK_ATTACHMENT_STORE_OP_STORE;
	colorAttachment.stencilLoadOp  = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
	colorAttachment.stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
	colorAttachment.initialLayout  = VK_IMAGE_LAYOUT_UNDEFINED;
	colorAttachment.samples		   = msaaSamples;
	colorAttachment.finalLayout	   = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;

	this->getLogger().info("Selected swapchain format: " + std::to_string(swapchainFormat));

	VkAttachmentDescription depthAttachment {};
	depthAttachment.format =
		ASwapchainImage::findDepthFormat(deviceBackend->_physicalDevice);
	depthAttachment.loadOp		   = VK_ATTACHMENT_LOAD_OP_CLEAR;
	depthAttachment.storeOp		   = VK_ATTACHMENT_STORE_OP_DONT_CARE;
	depthAttachment.stencilLoadOp  = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
	depthAttachment.stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
	depthAttachment.initialLayout  = VK_IMAGE_LAYOUT_UNDEFINED;
	depthAttachment.finalLayout =
		VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL;
	depthAttachment.samples = msaaSamples;

	this->getLogger().info("Selected depth format: " + std::to_string(depthAttachment.format));

	VkAttachmentReference colorAttachmentRef {};
	colorAttachmentRef.attachment = 0;
	colorAttachmentRef.layout	  = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;

	this->getLogger().info("Configured color attachment reference with attachment index: " + std::to_string(colorAttachmentRef.attachment)
						 + " and layout: " + std::to_string(colorAttachmentRef.layout));

	VkAttachmentReference depthAttachmentRef {};
	depthAttachmentRef.attachment = 1;
	depthAttachmentRef.layout =
		VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL;

	this->getLogger().info("Configured depth attachment reference with attachment index: " + std::to_string(depthAttachmentRef.attachment)
						 + " and layout: " + std::to_string(depthAttachmentRef.layout));

	VkAttachmentDescription colorAttachmentResolve {};
	colorAttachmentResolve.format		  = swapchainFormat;
	colorAttachmentResolve.samples		  = VK_SAMPLE_COUNT_1_BIT;
	colorAttachmentResolve.loadOp		  = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
	colorAttachmentResolve.storeOp		  = VK_ATTACHMENT_STORE_OP_STORE;
	colorAttachmentResolve.stencilLoadOp  = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
	colorAttachmentResolve.stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
	colorAttachmentResolve.initialLayout  = VK_IMAGE_LAYOUT_UNDEFINED;
	colorAttachmentResolve.finalLayout	  = VK_IMAGE_LAYOUT_PRESENT_SRC_KHR;

	this->getLogger().info("Configured color attachment resolve with format: " + std::to_string(colorAttachmentResolve.format)
						 + ", samples: " + std::to_string(colorAttachmentResolve.samples)
						 + ", load operation: " + std::to_string(colorAttachmentResolve.loadOp)
						 + ", store operation: " + std::to_string(colorAttachmentResolve.storeOp)
						 + ", stencil load operation: " + std::to_string(colorAttachmentResolve.stencilLoadOp)
						 + ", stencil store operation: " + std::to_string(colorAttachmentResolve.stencilStoreOp)
						 + ", initial layout: " + std::to_string(colorAttachmentResolve.initialLayout)
						 + ", final layout: " + std::to_string(colorAttachmentResolve.finalLayout));

	VkAttachmentReference colorAttachmentResolveRef {};
	colorAttachmentResolveRef.attachment = 2;
	colorAttachmentResolveRef.layout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;

	this->getLogger().info("Configured color attachment resolve reference with attachment index: " + std::to_string(colorAttachmentResolveRef.attachment)
						 + " and layout: " + std::to_string(colorAttachmentResolveRef.layout));

	VkSubpassDescription subpass {};
	subpass.pipelineBindPoint		= VK_PIPELINE_BIND_POINT_GRAPHICS;
	subpass.colorAttachmentCount	= 1;
	subpass.pColorAttachments		= &colorAttachmentRef;
	subpass.pDepthStencilAttachment = &depthAttachmentRef;
	subpass.pResolveAttachments		= &colorAttachmentResolveRef;

	this->getLogger().info("Configured subpass with pipeline bind point: " + std::to_string(subpass.pipelineBindPoint)
						 + ", color attachment count: " + std::to_string(subpass.colorAttachmentCount)
						 + ", depth-stencil attachment reference: " + std::to_string(depthAttachmentRef.attachment)
						 + ", resolve attachment reference: " + std::to_string(colorAttachmentResolveRef.attachment)
						 + ", and color attachment reference: " + std::to_string(colorAttachmentRef.attachment)
						 );

	VkSubpassDependency dependency {};
	dependency.srcSubpass	= VK_SUBPASS_EXTERNAL;
	dependency.dstSubpass	= 0;
	dependency.srcStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT
		| VK_PIPELINE_STAGE_EARLY_FRAGMENT_TESTS_BIT;
	dependency.srcAccessMask = 0;
	dependency.dstStageMask	 = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT
		| VK_PIPELINE_STAGE_EARLY_FRAGMENT_TESTS_BIT;
	dependency.dstAccessMask = VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT
		| VK_ACCESS_DEPTH_STENCIL_ATTACHMENT_WRITE_BIT;

	this->getLogger().info("Configured subpass dependency with source subpass: " + std::to_string(dependency.srcSubpass)
						 + ", destination subpass: " + std::to_string(dependency.dstSubpass)
						 + ", source stage mask: " + std::to_string(dependency.srcStageMask)
						 + ", source access mask: " + std::to_string(dependency.srcAccessMask)
						 + ", destination stage mask: " + std::to_string(dependency.dstStageMask)
						 + ", and destination access mask: " + std::to_string(dependency.dstAccessMask));

	this->getLogger().info("Selected swapchain format: " + std::to_string(swapchainFormat));

	std::array<VkAttachmentDescription, 3> attachments = {
		colorAttachment, depthAttachment, colorAttachmentResolve
	};
	VkRenderPassCreateInfo renderPassInfo {};
	renderPassInfo.sType		   = VK_STRUCTURE_TYPE_RENDER_PASS_CREATE_INFO;
	renderPassInfo.attachmentCount = static_cast<uint32_t>(attachments.size());
	renderPassInfo.pAttachments	   = attachments.data();
	renderPassInfo.subpassCount	   = 1;
	renderPassInfo.pSubpasses	   = &subpass;
	renderPassInfo.dependencyCount = 1;
	renderPassInfo.pDependencies   = &dependency;

	this->getLogger().info("Creating render pass with " + std::to_string(renderPassInfo.attachmentCount) + " attachments, "
						 + std::to_string(renderPassInfo.subpassCount) + " subpass, and "
						 + std::to_string(renderPassInfo.dependencyCount) + " dependency.");

	if (vkCreateRenderPass(deviceBackend->_device, &renderPassInfo, nullptr,
						   &_renderPass)
		!= VK_SUCCESS) {
		this->getLogger().error("Failed to create render pass for swapchain context.");
		return;
	}

	this->getLogger().info("Successfully created render pass for swapchain context.");
}

VkRenderPass evan::ASwapchainContext::getRenderPass() const
{
	return _renderPass;
}

VkFormat evan::ASwapchainContext::selectSwapchainFormat(
	const std::vector<int64_t> &swapchainFormats)
{
	this->getLogger().info("Selecting swapchain format from " + std::to_string(swapchainFormats.size()) + " available formats...");

	constexpr VkFormat kPreferredSwapchainFormats[] = {
		VK_FORMAT_R8G8B8A8_SRGB, VK_FORMAT_R8G8B8A8_UNORM,
		VK_FORMAT_B8G8R8A8_SRGB, VK_FORMAT_B8G8R8A8_UNORM
	};

	for (size_t i = 0; i < swapchainFormats.size(); ++i) {
		this->getLogger().info("Available swapchain format " + std::to_string(i) + ": " + std::to_string(swapchainFormats[i]));
	}

	auto swapchainFormatIt =
		std::find_first_of(swapchainFormats.begin(), swapchainFormats.end(),
						   std::begin(kPreferredSwapchainFormats),
						   std::end(kPreferredSwapchainFormats));

	if (swapchainFormatIt == swapchainFormats.end()) {
		this->getLogger().info("No preferred swapchain format found, using first available format.");
		return static_cast<VkFormat>(swapchainFormats[0]);
	}

	this->getLogger().info("Selected preferred swapchain format: " + std::to_string(*swapchainFormatIt));

	return static_cast<VkFormat>(*swapchainFormatIt);
}

VkFormat evan::ASwapchainContext::findSupportedFormat(
	VkPhysicalDevice physicalDevice, const std::vector<VkFormat> &candidates,
	VkImageTiling tiling, VkFormatFeatureFlags features)
{
	this->getLogger().info("Finding supported format from " + std::to_string(candidates.size()) + " candidates with tiling: " + std::to_string(tiling) + " and features: " + std::to_string(features) + "...");

	for (VkFormat format: candidates) {
		this->getLogger().info("Checking candidate format: " + std::to_string(format));

		VkFormatProperties props;
		vkGetPhysicalDeviceFormatProperties(physicalDevice, format, &props);

		if (tiling == VK_IMAGE_TILING_LINEAR
			&& (props.linearTilingFeatures & features) == features) {
			this->getLogger().info("Selected supported format: " + std::to_string(format) + " with linear tiling.");
			return format;
		} else if (tiling == VK_IMAGE_TILING_OPTIMAL
				   && (props.optimalTilingFeatures & features) == features) {
			this->getLogger().info("Selected supported format: " + std::to_string(format) + " with optimal tiling.");
			return format;
		}
	}

	this->getLogger().error("Failed to find supported format from candidates with tiling: " + std::to_string(tiling) + " and features: " + std::to_string(features) + ".");
	this->getLogger().warning("Returning VK_FORMAT_UNDEFINED to indicate failure to find a supported format. Ensure that the candidates provided are compatible with the physical device and meet the required tiling and feature criteria.");
	return VK_FORMAT_UNDEFINED;
}
