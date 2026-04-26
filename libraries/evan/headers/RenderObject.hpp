/*
** ETIB PROJECT, 2026
** xider
** File description:
** RenderObject
*/

#pragma once

#include <utility/graphic/mesh.hpp>

#include <map>

#include "DeviceContext.hpp"
#include "GPUMesh.hpp"

namespace evan
{
	/**
	 * @brief A Renderable object. It is a simple interface between our
	 * core renderer and on-top built objects
	 * (such as texts, basic shapes and 3d objects)
	 */
	class RenderObject
	{
		public:
		/**
		 * @brief Constructor: creates a single renderable object using
		 * its meshes and its associated material ID.
		 */
		RenderObject(std::shared_ptr<DeviceContext> deviceContext, const std::map<uint32_t, utility::graphic::Mesh> &rawObjects, const std::string &pipelineLayer);

		~RenderObject();

		void destroy(VkDevice device);

		protected:
        std::vector<GPUMesh> _meshes;
        std::string _pipelineLayer;
	};
}	 // namespace evan
