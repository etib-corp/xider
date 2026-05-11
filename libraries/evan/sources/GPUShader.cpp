/*
** ETIB PROJECT, 2025
** evan
** File description:
** GPUShader
*/

#include "GPUShader.hpp"

evan::GPUShader::GPUShader(VkDevice device, const utility::graphic::Shader &shader)
{
	VkShaderModuleCreateInfo createInfo {};

	auto vertexCode   = shader.getVertexCode();
	auto fragmentCode = shader.getFragmentCode();

	createInfo.sType	= VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
	createInfo.codeSize = vertexCode.size() * sizeof(uint32_t);
	createInfo.pCode	= vertexCode.data();

	_vertexCode	   = vertexCode;
	_fragmentCode  = fragmentCode;
	_logicalDevice = device;

	if (vkCreateShaderModule(device, &createInfo, nullptr, &_vertexShaderModule)
		!= VK_SUCCESS) {
		// throw an error if the shader module creation fails
		return;
	}

	createInfo.codeSize = fragmentCode.size() * sizeof(uint32_t);
	createInfo.pCode	= fragmentCode.data();

	if (vkCreateShaderModule(device, &createInfo, nullptr,
							 &_fragmentShaderModule)
		!= VK_SUCCESS) {
		// throw an error if the shader module creation fails
		return;
	}
}

evan::GPUShader::~GPUShader()
{
}

////////////////////
// Public Methods //
////////////////////

void evan::GPUShader::destroy()
{
	vkDestroyShaderModule(_logicalDevice, _vertexShaderModule, nullptr);
	vkDestroyShaderModule(_logicalDevice, _fragmentShaderModule, nullptr);
}

/////////////
// Getters //
/////////////

VkShaderModule evan::GPUShader::getVertexShaderModule() const
{
	return _vertexShaderModule;
}

VkShaderModule evan::GPUShader::getFragmentShaderModule() const
{
	return _fragmentShaderModule;
}
