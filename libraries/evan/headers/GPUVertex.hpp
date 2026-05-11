/*
** ETIB PROJECT, 2025
** evan
** File description:
** GPUVertex
*/

#ifndef GPUVERTEX_HPP_
#define GPUVERTEX_HPP_

#include "EvanPlatform.hpp"

#include <utility/graphic/vertex.hpp>

namespace evan
{
	/**
	 * @brief GPUVertex structure.
	 *
	 * This structure represents a vertex in the 3D space. It contains the
	 * position, color, and texture coordinates of the vertex. It is used to
	 * define the vertex data for rendering.
	 *
	 */
	struct GPUVertex {
		glm::vec3 pos;	  // Position of the vertex
		glm::vec3
			color;	  // Color of the vertex    (TODO: change to vec4 for alpha)
		glm::vec2 texCoord;	   // Texture coordinates of the vertex

		/*
		 * @brief Get the binding description for the vertex.
		 *
		 * This function returns the binding description for the vertex,
		 * which specifies how the vertex data is laid out in memory.
		 *
		 * @return VkVertexInputBindingDescription The binding description
		 *         for the vertex.
		 *
		 */
		static VkVertexInputBindingDescription getBindingDescription()
		{
			VkVertexInputBindingDescription bindingDescription {};
			bindingDescription.binding	 = 0;
			bindingDescription.stride	 = sizeof(GPUVertex);
			bindingDescription.inputRate = VK_VERTEX_INPUT_RATE_VERTEX;

			return bindingDescription;
		}

		/*
		 * @brief Get the attribute descriptions for the vertex.
		 *
		 * This function returns an array of attribute descriptions for the
		 * vertex, which specify the format and offset of each attribute in
		 * the vertex data.
		 *
		 * @return std::array<VkVertexInputAttributeDescription, 3> The
		 *         attribute descriptions for the vertex.
		 *
		 */
		static std::array<VkVertexInputAttributeDescription, 3>
			getAttributeDescriptions()
		{
			std::array<VkVertexInputAttributeDescription, 3>
				attributeDescriptions {};

			attributeDescriptions[0].binding  = 0;
			attributeDescriptions[0].location = 0;
			attributeDescriptions[0].format	  = VK_FORMAT_R32G32B32_SFLOAT;
			attributeDescriptions[0].offset	  = offsetof(GPUVertex, pos);

			attributeDescriptions[1].binding  = 0;
			attributeDescriptions[1].location = 1;
			attributeDescriptions[1].format	  = VK_FORMAT_R32G32B32_SFLOAT;
			attributeDescriptions[1].offset	  = offsetof(GPUVertex, color);

			attributeDescriptions[2].binding  = 0;
			attributeDescriptions[2].location = 2;
			attributeDescriptions[2].format	  = VK_FORMAT_R32G32_SFLOAT;
			attributeDescriptions[2].offset	  = offsetof(GPUVertex, texCoord);

			return attributeDescriptions;
		}

		/*
		 * @brief Equality operator for the GPUVertex structure.
		 *
		 * This operator compares two GPUVertex objects for equality based on
		 * their position, color, and texture coordinates.
		 *
		 * @param other The other GPUVertex object to compare with.
		 * @return true if the two GPUVertex objects are equal, false otherwise.
		 *
		 */
		bool operator==(const GPUVertex &other) const
		{
			return pos == other.pos && color == other.color
				&& texCoord == other.texCoord;
		}

		static GPUVertex createFromVertexD(const utility::graphic::VertexD &vertex)
		{
			auto color32Bit = vertex.getColor();
			auto red 	= color32Bit.getRed() / 255.0f;
			auto green = color32Bit.getGreen() / 255.0f;
			auto blue 	= color32Bit.getBlue() / 255.0f;

			return GPUVertex {
				glm::vec3(vertex.getPosition().x, vertex.getPosition().y,
						  vertex.getPosition().z),
				glm::vec3(red, green, blue),
				glm::vec2(vertex.getTextureCoordinates().x,
						  vertex.getTextureCoordinates().y)
			};
		}
	};
}	 // namespace evan

#endif /* !GPUVERTEX_HPP_ */
