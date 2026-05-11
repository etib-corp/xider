/*
** ETIB PROJECT, 2026
** xider
** File description:
** RenderObject
*/

#include "RenderObject.hpp"

evan::RenderObject::RenderObject(std::shared_ptr<DeviceContext> deviceContext, const std::map<uint32_t, utility::graphic::Mesh> &rawObjects, const std::string &pipelineLayer)
    : _pipelineLayer(pipelineLayer)
{
    for (const auto &[id, mesh] : rawObjects) {
        std::vector<GPUVertex> gpuVertices;

        for (const auto &vertex : mesh.getVertices()) {
            GPUVertex gpuVertex = GPUVertex::createFromVertexD(vertex);
            gpuVertices.push_back(gpuVertex);
        }

        GPUMesh gpuMesh(deviceContext, gpuVertices, mesh.getIndices(), id);

        _meshes.emplace_back(gpuMesh);
    }
}

evan::RenderObject::~RenderObject()
{
}

////////////////////
// Public methods //
////////////////////

void evan::RenderObject::destroy(VkDevice device)
{
    for (GPUMesh &mesh : _meshes) {
        mesh.destroy(device);
    }
}

/////////////
// Getters //
/////////////

const std::vector<evan::GPUMesh> &evan::RenderObject::getMeshes() const
{
    return _meshes;
}

std::string evan::RenderObject::getPipelineLayer() const
{
    return _pipelineLayer;
}
