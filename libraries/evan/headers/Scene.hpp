/*
** ETIB PROJECT, 2026
** evan
** File description:
** Scene
*/

#pragma once

#include <utility/graphic/material.hpp>

#include "EvanPlatform.hpp"

#include "GPUMaterial.hpp"

#include "RenderObject.hpp"

#include <map>

namespace evan
{
	class Renderer;

	/**
	 * @brief The Scene class represents a collection of meshes and materials
	 * that can be rendered.
	 *
	 * It manages the GPU resources associated with these meshes and materials,
	 * such as vertex buffers and index buffers. The Scene class provides
	 * methods to access the meshes and materials, as well as to destroy the GPU
	 * resources when they are no longer needed.
	 *
	 * @note The Scene class is designed to work with the Vulkan API, and it
	 * assumes that the necessary Vulkan resources (such as the device and
	 * renderer) are provided during construction.
	 */
	class Scene
	{
		public:
		Scene() = default;

		~Scene();

		/**
		 * @brief Destroys the Scene by cleaning up GPU resources associated
		 * with the meshes and materials.
		 *
		 * This method is responsible for releasing GPU resources allocated for
		 * the vertex and index buffers of the meshes, as well as any resources
		 * associated with the materials. It takes a VkDevice representing the
		 * Vulkan device used for resource management and ensures that all GPU
		 * resources are properly cleaned up to prevent memory leaks and ensure
		 * efficient resource usage.
		 *
		 * @param device The Vulkan device used for resource management, which
		 * is required to clean up GPU resources associated with the meshes and
		 * materials in the scene.
		 *
		 * @note The method assumes that the Vulkan device is properly
		 * initialized and can be used to manage GPU resources. It also assumes
		 * that the meshes and materials were created using the same device
		 * context and are associated with the provided device for cleanup.
		 */
		void destroy(VkDevice device);

		void addObject(uint32_t objectID, std::shared_ptr<RenderObject> renderObject);

		bool removeObject(uint32_t objectID);

		/**
		 * @brief Retrieves the meshes contained in the Scene.
		 *
		 * This method returns a constant reference to a vector of GPUMesh
		 * objects representing the meshes contained in the Scene. Each GPUMesh
		 * object contains information about the vertex and index buffers, as
		 * well as the material ID associated with that mesh. The returned
		 * vector allows access to the meshes for rendering or other operations
		 * while ensuring that the internal state of the Scene is not modified.
		 *
		 * @return A constant reference to a vector of GPUMesh objects
		 * representing the meshes contained in the Scene.
		 *
		 * @note The method assumes that the meshes have been properly
		 * initialized and are ready for use in rendering or other operations.
		 * It also assumes that the caller will not modify the returned vector,
		 * as it is a constant reference to the internal state of the Scene.
		 */
		const std::vector<GPUMesh> &getMeshes() const;

		/**
		 * @brief Retrieves the materials contained in the Scene.
		 *
		 * This method returns a constant reference to a map where the key is a
		 * uint32_t representing the material ID, and the value is a GPUMaterial
		 * object representing the material properties associated with that
		 * material ID. The returned map allows access to the materials for
		 * rendering or other operations while ensuring that the internal state
		 * of the Scene is not modified.
		 *
		 * @return A constant reference to a map of material IDs to GPUMaterial
		 * objects representing the materials contained in the Scene.
		 *
		 * @note The method assumes that the materials have been properly
		 * initialized and are ready for use in rendering or other operations.
		 * It also assumes that the caller will not modify the returned map, as
		 * it is a constant reference to the internal state of the Scene.
		 */
		const std::map<uint32_t, std::shared_ptr<evan::GPUMaterial>> &getMaterials() const;

		protected:
		/**
		 * @brief A map of object IDs to RenderObject objects representing the
		 * objects contained in the Scene.
		 *
		 * The key of the map is a uint32_t representing the object ID, and the
		 * value is a RenderObject object representing the renderable object
		 * associated with that object ID. This map is used to store and manage
		 * the renderable objects in the Scene for rendering and other operations.
		 */
		std::map<uint32_t, std::shared_ptr<RenderObject>> _objects;

		/**
		 * @brief A map of material IDs to GPUMaterial objects representing the
		 * materials contained in the Scene.
		 *
		 * The key of the map is a uint32_t representing the material ID, and
		 * the value is a GPUMaterial object representing the material properties
		 * associated with that material ID. This map is used to store and
		 * manage the materials in the Scene for rendering and other operations.
		 *
		 * @note The map is initialized during the construction of the Scene and
		 * is used to access the materials for rendering or other operations. It
		 * should be properly managed to ensure that GPU resources are cleaned
		 * up when the Scene is destroyed.
		 */
		std::map<uint32_t, std::shared_ptr<GPUMaterial>> _materials;
	};
}	 // namespace evan
