/*
** ETIB PROJECT, 2026
** xider
** File description:
** RessourceManager
*/

#include "RessourceManager.hpp"

evan::RessourceManager::RessourceManager(std::unique_ptr<utility::RessourceProvider> ressourceProvider, std::shared_ptr<DeviceContext> deviceContext, std::shared_ptr<Renderer> renderer)
    : _ressourceProvider(std::move(ressourceProvider)), _deviceContext(deviceContext), _renderer(renderer)
{
    sync();
}

evan::RessourceManager::~RessourceManager()
{
}

////////////////////
// Public Methods //
////////////////////

void evan::RessourceManager::sync(bool refresh)
{
    std::map<uint32_t, std::shared_ptr<utility::graphic::Material>> materials = _ressourceProvider->getMaterials();
    std::map<uint32_t, std::shared_ptr<utility::graphic::Texture>> textures = _ressourceProvider->getTextures();
    std::map<uint32_t, std::shared_ptr<utility::graphic::Shader>> shaders = _ressourceProvider->getShaders();

    for (const auto &[id, material]: materials) {
        if (refresh || _materials.find(id) == _materials.end()) {
            _materials[id] = std::make_shared<GPUMaterial>(_deviceContext, *_renderer, *material);
        }
    }

    for (const auto &[id, texture]: textures) {
        if (refresh || _textures.find(id) == _textures.end()) {
            _textures[id] = std::make_shared<GPUTexture>(_deviceContext, *_renderer, *texture);
        }
    }

    for (const auto &[id, shader]: shaders) {
        if (refresh || _shaders.find(id) == _shaders.end()) {
            _shaders[id] = std::make_shared<GPUShader>(_deviceContext->getDeviceBackend()->_device, *shader);
        }
    }
}

/////////////
// Getters //
/////////////

std::shared_ptr<evan::GPUMaterial> evan::RessourceManager::getMaterial(uint32_t id) const
{
    auto it = _materials.find(id);
    if (it != _materials.end()) {
        return it->second;
    }
    return nullptr;
}

std::shared_ptr<evan::GPUTexture> evan::RessourceManager::getTexture(uint32_t id) const
{
    auto it = _textures.find(id);
    if (it != _textures.end()) {
        return it->second;
    }
    return nullptr;
}

std::shared_ptr<evan::GPUShader> evan::RessourceManager::getShader(uint32_t id) const
{
    auto it = _shaders.find(id);
    if (it != _shaders.end()) {
        return it->second;
    }
    return nullptr;
}
