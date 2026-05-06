/*
** ETIB PROJECT, 2026
** xider
** File description:
** GPUTexture
*/

#include "GPUTexture.hpp"

evan::GPUTexture::GPUTexture(const ADeviceBackend &deviceBackend,
                const utility::graphic::Texture &texture,
                VkCommandPool commandPool,
                VkQueue graphicsQueue,
                TextureType type
            ) : type(type)
{
    this->createImage(deviceBackend, texture, commandPool, graphicsQueue);
    this->createImageView(deviceBackend);
    this->createSampler(deviceBackend, VkSamplerCreateInfo{});
}

evan::GPUTexture::~GPUTexture()
{
}

////////////////////
// Public methods //
////////////////////

void evan::GPUTexture::destroy(VkDevice device)
{
    vkDestroyImage(device, _image, nullptr);
    vkFreeMemory(device, _memory, nullptr);
    vkDestroySampler(device, sampler, nullptr);
}

///////////////////////
// Protected methods //
///////////////////////

void evan::GPUTexture::createImage(const ADeviceBackend &deviceBackend,
                                 const utility::graphic::Texture &texture,
                                 VkCommandPool commandPool,
                                 VkQueue graphicsQueue)
{
	uint32_t texWidth  = texture.width();
	uint32_t texHeight = texture.height();
	const uint8_t *pixels = texture._pixels.data();

	VkDeviceSize imageSize = texWidth * texHeight * 4;
	VkBuffer stagingBuffer;
	VkDeviceMemory stagingBufferMemory;

	if (texture._pixels.size() == 0) {
		throw std::runtime_error("Failed to load texture image !");
	}

	_mipLevel = static_cast<uint32_t>(
					std::floor(std::log2(std::max(texWidth, texHeight))))
		+ 1;

	ADeviceBackend::CreateBufferProperties stagingBufferProperties = {
		._size		 = imageSize,
		._usage		 = VK_BUFFER_USAGE_TRANSFER_SRC_BIT,
		._properties = VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT
			| VK_MEMORY_PROPERTY_HOST_COHERENT_BIT,
		._buffer	   = stagingBuffer,
		._bufferMemory = stagingBufferMemory
	};

	deviceBackend.createBuffer(stagingBufferProperties);

	void *data;
	vkMapMemory(deviceBackend._device, stagingBufferMemory, 0, imageSize, 0,
				&data);
	memcpy(data, pixels, static_cast<size_t>(imageSize));
	vkUnmapMemory(deviceBackend._device, stagingBufferMemory);

	ADeviceBackend::CreateImageProperties imageProperties = {
		._width		 = (uint32_t)texWidth,
		._height	 = (uint32_t)texHeight,
		._mipLevels	 = _mipLevel,
		._numSamples = VK_SAMPLE_COUNT_1_BIT,
		._format	 = VK_FORMAT_R8G8B8A8_SRGB,
		._tiling	 = VK_IMAGE_TILING_OPTIMAL,
		._usage		 = VK_IMAGE_USAGE_TRANSFER_SRC_BIT
			| VK_IMAGE_USAGE_TRANSFER_DST_BIT | VK_IMAGE_USAGE_SAMPLED_BIT,
		._properties  = VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT,
		._image		  = _image,
		._imageMemory = _memory
	};
	deviceBackend.createImage(imageProperties);

	ADeviceBackend::TransitionImageLayoutProperties transitionProperties = {
		._commandPool	= commandPool,
		._graphicsQueue = graphicsQueue,
		._image			= _image,
		._format		= VK_FORMAT_R8G8B8A8_SRGB,
		._oldLayout		= VK_IMAGE_LAYOUT_UNDEFINED,
		._newLayout		= VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL,
		._mipLevels		= _mipLevel
	};
	deviceBackend.transitionImageLayout(transitionProperties);

	ADeviceBackend::CopyBufferToImageProperties copyProperties = {
		._commandPool	= commandPool,
		._graphicsQueue = graphicsQueue,
		._buffer		= stagingBuffer,
		._image			= _image,
		._width			= (uint32_t)texWidth,
		._height		= (uint32_t)texHeight
	};
	deviceBackend.copyBufferToImage(copyProperties);

	GenerateMipmapsProperties propertiesMipmap = {
		._commandPool	= commandPool,
		._graphicsQueue = graphicsQueue,
		._image			= _image,
		._imageFormat	= VK_FORMAT_R8G8B8A8_SRGB,
		._texWidth		= (uint32_t)texWidth,
		._texHeight		= (uint32_t)texHeight,
		._mipLevels		= _mipLevel
	};
	this->generateMipmaps(propertiesMipmap, deviceBackend);

	vkDestroyBuffer(deviceBackend._device, stagingBuffer, nullptr);
	vkFreeMemory(deviceBackend._device, stagingBufferMemory, nullptr);
}

void evan::GPUTexture::createImageView(const ADeviceBackend &deviceBackend)
{
    view = deviceBackend.createImageView(_image, VK_FORMAT_R8G8B8A8_SRGB,
                                         VK_IMAGE_ASPECT_COLOR_BIT, _mipLevel);
}

void evan::GPUTexture::createSampler(const ADeviceBackend &deviceBackend,
                                   VkSamplerCreateInfo samplerInfo)
{
    VkPhysicalDeviceProperties properties {};
    vkGetPhysicalDeviceProperties(deviceBackend._physicalDevice, &properties);

    samplerInfo.maxAnisotropy = properties.limits.maxSamplerAnisotropy;

    // Check if samplerInfo is "empty" by testing its sType field.
    // If sType is not set, it's likely uninitialized.
    if (samplerInfo.sType == 0) {
        samplerInfo = this->getDefaultSamplerInfo(properties);
    }

    if (vkCreateSampler(deviceBackend._device, &samplerInfo, nullptr, &sampler)
        != VK_SUCCESS) {
        throw std::runtime_error("Failed to create texture sampler!");
    }
}

/////////////////////
// Private methods //
/////////////////////

void evan::GPUTexture::generateMipmaps(const GenerateMipmapsProperties &properties,
                                 const ADeviceBackend &deviceBackend)
{
	VkFormatProperties formatProperties;
	vkGetPhysicalDeviceFormatProperties(deviceBackend._physicalDevice,
										properties._imageFormat,
										&formatProperties);

	if (!(formatProperties.optimalTilingFeatures
		  & VK_FORMAT_FEATURE_SAMPLED_IMAGE_FILTER_LINEAR_BIT)) {
		throw std::runtime_error(
			"texture image format does not support linear blitting!");
	}

	VkCommandBuffer commandBuffer =
		deviceBackend.beginSingleTimeCommands(properties._commandPool);

	VkImageMemoryBarrier barrier {};
	barrier.sType				= VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER;
	barrier.image				= properties._image;
	barrier.srcQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
	barrier.dstQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
	barrier.subresourceRange.aspectMask		= VK_IMAGE_ASPECT_COLOR_BIT;
	barrier.subresourceRange.baseArrayLayer = 0;
	barrier.subresourceRange.layerCount		= 1;
	barrier.subresourceRange.levelCount		= 1;

	int32_t mipWidth  = properties._texWidth;
	int32_t mipHeight = properties._texHeight;

	for (uint32_t i = 1; i < properties._mipLevels; i++) {
		barrier.subresourceRange.baseMipLevel = i - 1;
		barrier.oldLayout	  = VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL;
		barrier.newLayout	  = VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL;
		barrier.srcAccessMask = VK_ACCESS_TRANSFER_WRITE_BIT;
		barrier.dstAccessMask = VK_ACCESS_TRANSFER_READ_BIT;

		vkCmdPipelineBarrier(commandBuffer, VK_PIPELINE_STAGE_TRANSFER_BIT,
							 VK_PIPELINE_STAGE_TRANSFER_BIT, 0, 0, nullptr, 0,
							 nullptr, 1, &barrier);

		VkImageBlit blit {};
		blit.srcOffsets[0]				   = { 0, 0, 0 };
		blit.srcOffsets[1]				   = { mipWidth, mipHeight, 1 };
		blit.srcSubresource.aspectMask	   = VK_IMAGE_ASPECT_COLOR_BIT;
		blit.srcSubresource.mipLevel	   = i - 1;
		blit.srcSubresource.baseArrayLayer = 0;
		blit.srcSubresource.layerCount	   = 1;
		blit.dstOffsets[0]				   = { 0, 0, 0 };
		blit.dstOffsets[1]				   = { mipWidth > 1 ? mipWidth / 2 : 1,
							   mipHeight > 1 ? mipHeight / 2 : 1, 1 };
		blit.dstSubresource.aspectMask	   = VK_IMAGE_ASPECT_COLOR_BIT;
		blit.dstSubresource.mipLevel	   = i;
		blit.dstSubresource.baseArrayLayer = 0;
		blit.dstSubresource.layerCount	   = 1;

		vkCmdBlitImage(commandBuffer, properties._image,
					   VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL, properties._image,
					   VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL, 1, &blit,
					   VK_FILTER_LINEAR);

		barrier.oldLayout	  = VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL;
		barrier.newLayout	  = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
		barrier.srcAccessMask = VK_ACCESS_TRANSFER_READ_BIT;
		barrier.dstAccessMask = VK_ACCESS_SHADER_READ_BIT;

		vkCmdPipelineBarrier(commandBuffer, VK_PIPELINE_STAGE_TRANSFER_BIT,
							 VK_PIPELINE_STAGE_FRAGMENT_SHADER_BIT, 0, 0,
							 nullptr, 0, nullptr, 1, &barrier);

		if (mipWidth > 1)
			mipWidth /= 2;
		if (mipHeight > 1)
			mipHeight /= 2;
	}

	barrier.subresourceRange.baseMipLevel = properties._mipLevels - 1;
	barrier.oldLayout	  = VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL;
	barrier.newLayout	  = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
	barrier.srcAccessMask = VK_ACCESS_TRANSFER_WRITE_BIT;
	barrier.dstAccessMask = VK_ACCESS_SHADER_READ_BIT;

	vkCmdPipelineBarrier(commandBuffer, VK_PIPELINE_STAGE_TRANSFER_BIT,
						 VK_PIPELINE_STAGE_FRAGMENT_SHADER_BIT, 0, 0, nullptr,
						 0, nullptr, 1, &barrier);

	deviceBackend.endSingleTimeCommands(
		properties._commandPool, properties._graphicsQueue, commandBuffer);
}

VkSamplerCreateInfo evan::GPUTexture::getDefaultSamplerInfo(
    const VkPhysicalDeviceProperties &properties)
{
	VkSamplerCreateInfo samplerInfo {};
	samplerInfo.sType			 = VK_STRUCTURE_TYPE_SAMPLER_CREATE_INFO;
	samplerInfo.magFilter		 = VK_FILTER_LINEAR;
	samplerInfo.minFilter		 = VK_FILTER_LINEAR;
	samplerInfo.addressModeU	 = VK_SAMPLER_ADDRESS_MODE_REPEAT;
	samplerInfo.addressModeV	 = VK_SAMPLER_ADDRESS_MODE_REPEAT;
	samplerInfo.addressModeW	 = VK_SAMPLER_ADDRESS_MODE_REPEAT;
	samplerInfo.anisotropyEnable = VK_TRUE;
	samplerInfo.maxAnisotropy	 = properties.limits.maxSamplerAnisotropy;
	samplerInfo.borderColor		 = VK_BORDER_COLOR_INT_OPAQUE_BLACK;
	samplerInfo.unnormalizedCoordinates = VK_FALSE;
	samplerInfo.compareEnable			= VK_FALSE;
	samplerInfo.compareOp				= VK_COMPARE_OP_ALWAYS;
	samplerInfo.mipmapMode				= VK_SAMPLER_MIPMAP_MODE_LINEAR;
	samplerInfo.mipLodBias				= 0.0f;
	samplerInfo.minLod					= 0.0f;
	samplerInfo.maxLod					= 1;

	return samplerInfo;
}
