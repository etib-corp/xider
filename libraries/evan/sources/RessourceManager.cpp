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
    std::map<uint32_t, std::shared_ptr<utility::graphic::Shader>> shaders = _ressourceProvider->getShaders();
    std::map<uint32_t, std::shared_ptr<utility::graphic::Material>> materials = _ressourceProvider->getMaterials();
    std::map<uint32_t, std::shared_ptr<utility::graphic::Texture>> textures = _ressourceProvider->getTextures();

    for (const auto &[id, shader]: shaders) {
        if (refresh || _shaders.find(id) == _shaders.end()) {
            _shaders[id] = std::make_shared<GPUShader>(_deviceContext->getDeviceBackend()->_device, *shader);
        }
    }

    for (const auto &[id, material]: materials) {
        if (refresh || _materials.find(id) == _materials.end()) {
            auto shaderID = _ressourceProvider->getShaderID(material->getShaderName());

            if (shaderID == 0) {
                std::cerr << "Warning: Shader '" << material->getShaderName() << "' not found for material ID " << id << std::endl;
                continue;  // Skip this material if its shader is not found
            }
            _materials[id] = std::make_shared<GPUMaterial>(_deviceContext, *_renderer, *material, shaderID);
        }
    }

    for (const auto &[id, texture]: textures) {
        if (refresh || _textures.find(id) == _textures.end()) {
            _textures[id] = std::make_shared<GPUTexture>(_deviceContext, *_renderer, *texture);
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

std::unordered_map<uint32_t, std::shared_ptr<evan::GPUShader>> evan::RessourceManager::getShaders() const
{
    return _shaders;
}
