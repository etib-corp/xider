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
        _meshes.emplace_back(deviceContext, mesh.getVertices(), mesh.getIndices(), id);
    }
}

evan::RenderObject::~RenderObject()
{
}
