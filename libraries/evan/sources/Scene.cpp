/*
** ETIB PROJECT, 2026
** evan
** File description:
** Scene
*/

#include "evan/Scene.hpp"

#include "evan/Renderer.hpp"

evan::Scene::~Scene()
{
}

////////////////////
// Public Methods //
////////////////////

void evan::Scene::destroy(VkDevice device)
{
	for (auto &[_, object]: _objects) {
		object.destroy(device);
	}

	for (auto &[_, material]: _materials) {
		material.destroy(device);
	}
}

void evan::Scene::addObject(uint32_t objectID, RenderObject object)
{
	_objects[objectID] = std::move(object);
}

bool evan::Scene::removeObject(uint32_t objectID)
{
	return _objects.erase(objectID) > 0;
}

/////////////
// Getters //
/////////////

const std::vector<evan::GPUMesh> &evan::Scene::getMeshes() const
{
	static std::vector<GPUMesh> meshes;
	meshes.clear();

	for (const auto &[_, object]: _objects) {
		const std::vector<GPUMesh> &objectMeshes = object.getMeshes();
		meshes.insert(meshes.end(), objectMeshes.begin(), objectMeshes.end());
	}

	return meshes;
}

const std::map<uint32_t, evan::GPUMaterial> &evan::Scene::getMaterials() const
{
	return _materials;
}
