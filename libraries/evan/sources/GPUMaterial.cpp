/*
** ETIB PROJECT, 2026
** evan
** File description:
** GPUGPUMaterial
*/

#include "evan/Material.hpp"

#include <stb_image.h>

#include "evan/Renderer.hpp"

evan::GPUMaterial::GPUMaterial(std::shared_ptr<DeviceContext> deviceContext,
							   const Renderer &renderer,
							   const utility::graphic::Material &material)
{
	auto deviceBackend = deviceContext->getDeviceBackend();

	auto textures = material.getTextures();

	this->createDescriptorSets(
		deviceBackend->_device, renderer.getDescriptorSetLayout(),
		renderer.getDescriptorPool(), renderer.getUniformBuffers());
}

evan::GPUMaterial::~GPUMaterial()
{
}

////////////////////
// Public Methods //
////////////////////

void evan::GPUMaterial::destroy(VkDevice device)
{
}

/////////////
// Getters //
/////////////

std::vector<VkDescriptorSet> evan::GPUMaterial::getDescriptorSets() const
{
	return _descriptorSets;
}

///////////////////////
// Protected methods //
///////////////////////

void evan::GPUMaterial::createDescriptorSets(
	VkDevice logicalDevice, VkDescriptorSetLayout descriptorSetLayout,
	VkDescriptorPool descriptorPool,
	const std::vector<VkBuffer> &uniformBuffers)
{
	std::vector<VkDescriptorSetLayout> layouts(MAX_FRAMES_IN_FLIGHT,
											   descriptorSetLayout);
	VkDescriptorSetAllocateInfo allocInfo {};
	allocInfo.sType			 = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO;
	allocInfo.descriptorPool = descriptorPool;
	allocInfo.descriptorSetCount = static_cast<uint32_t>(MAX_FRAMES_IN_FLIGHT);
	allocInfo.pSetLayouts		 = layouts.data();

	_descriptorSets.resize(MAX_FRAMES_IN_FLIGHT);
	if (vkAllocateDescriptorSets(logicalDevice, &allocInfo,
								 _descriptorSets.data())
		!= VK_SUCCESS) {
		throw std::runtime_error("Failed to allocate descriptor sets !");
	}
	for (size_t i = 0; i < MAX_FRAMES_IN_FLIGHT; i++) {
		std::vector<VkWriteDescriptorSet> descriptorWrites;

		VkDescriptorBufferInfo bufferInfo {};
		bufferInfo.buffer = uniformBuffers[i];
		bufferInfo.offset = 0;
		bufferInfo.range  = sizeof(Frame::UniformBufferObject);

		VkWriteDescriptorSet uboWrite {};
		uboWrite.sType			 = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
		uboWrite.dstSet			 = _descriptorSets[i];
		uboWrite.dstBinding		 = 0;
		uboWrite.descriptorType	 = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
		uboWrite.descriptorCount = 1;
		uboWrite.pBufferInfo	 = &bufferInfo;

		descriptorWrites.push_back(uboWrite);

		std::vector<VkDescriptorImageInfo> imageInfos;
		imageInfos.reserve(_textures.size());

		for (const auto &texture: _textures) {
			VkDescriptorImageInfo imageInfo {};
			imageInfo.imageLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
			imageInfo.imageView	  = texture.view;
			imageInfo.sampler	  = texture.sampler;

			imageInfos.push_back(imageInfo);

			VkWriteDescriptorSet write {};
			write.sType			  = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
			write.dstSet		  = _descriptorSets[i];
			write.dstBinding	  = getBinding(texture.type);
			write.descriptorType  = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
			write.descriptorCount = 1;
			write.pImageInfo	  = &imageInfos.back();

			descriptorWrites.push_back(write);
		}

		vkUpdateDescriptorSets(logicalDevice,
							   static_cast<uint32_t>(descriptorWrites.size()),
							   descriptorWrites.data(), 0, nullptr);
	}
}

uint32_t evan::GPUMaterial::getBinding(GPUTexture::TextureType type)
{
    switch (type) {
        case GPUTexture::TextureType::Albedo: return 1;
        case GPUTexture::TextureType::Normal: return 2;
        case GPUTexture::TextureType::Roughness: return 3;
    }
    throw std::runtime_error("Unknown texture type");
}