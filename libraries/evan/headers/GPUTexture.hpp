/*
** ETIB PROJECT, 2026
** xider
** File description:
** GPUTexture
*/

#pragma once

#include <utility/graphic/texture.hpp>

#include "EvanPlatform.hpp"

#include "ADeviceBackend.hpp"

namespace evan
{
    class GPUTexture {
        public:
            /**
             * @brief Enum class representing different types of textures that can be
             * associated with a material, such as albedo, normal, and roughness
             * textures.
             *
             * This enum class is used to categorize textures based on their intended use
             * in the rendering pipeline. Each texture type corresponds to a specific
             * role in defining the visual properties of a material, allowing for
             * organized management of texture resources and their bindings in shader
             * programs.
             */
            enum class TextureType {
                Albedo,     ///< Represents the base color texture of a material, defining its diffuse color.
                Normal,     ///< Represents the normal map texture, which encodes surface normals for detailed lighting effects.
                Roughness   ///< Represents the roughness texture, which defines the surface roughness for specular reflections.
            };

            /**
             * @struct GenerateMipmapsProperties
             * @brief Holds properties required for generating mipmaps for a Vulkan
             * image.
             *
             * This structure encapsulates all necessary Vulkan objects and image
             * properties needed to perform mipmap generation operations.
             *
             */
            struct GenerateMipmapsProperties {
                /*
                * @brief The Vulkan command pool used to allocate command buffers
                * for mipmap generation.
                */
                VkCommandPool _commandPool;
                /*
                * @brief The Vulkan graphics queue used to submit the command
                * buffer for execution.
                */
                VkQueue _graphicsQueue;
                /*
                * @brief The Vulkan image for which mipmaps are to be generated.
                */
                VkImage _image;
                /*
                * @brief The format of the image for which mipmaps are to be
                * generated.
                */
                VkFormat _imageFormat;
                /*
                * @brief The width of the image in pixels.
                */
                uint32_t _texWidth;
                /*
                * @brief The height of the image in pixels.
                */
                uint32_t _texHeight;
                /*
                * @brief The number of mipmap levels to generate.
                */
                uint32_t _mipLevels;
            };

            GPUTexture(const ADeviceBackend &deviceBackend,
                const utility::graphic::Texture &texture,
                VkCommandPool commandPool,
                VkQueue graphicsQueue,
                TextureType type = TextureType::Albedo
            );

            ~GPUTexture();

            void destroy(VkDevice device);

            VkImageView view;
            VkSampler sampler;
            TextureType type;

        protected:
            void createImage(const ADeviceBackend &deviceBackend,
								 const utility::graphic::Texture &texture,
								 VkCommandPool commandPool,
								 VkQueue graphicsQueue);

            void createImageView(const ADeviceBackend &deviceBackend);

            void createSampler(const ADeviceBackend &deviceBackend,
                               VkSamplerCreateInfo samplerInfo);

            VkImage _image;
            VkDeviceMemory _memory;
            uint32_t _mipLevel;

        private:
            void generateMipmaps(const GenerateMipmapsProperties &properties,
                            const ADeviceBackend &deviceBackend);

            VkSamplerCreateInfo getDefaultSamplerInfo(const VkPhysicalDeviceProperties &properties);

    };
}
