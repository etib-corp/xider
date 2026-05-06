/*
** ETIB PROJECT, 2026
** evan
** File description:
** Engine
*/

#include "evan/Engine.hpp"

#ifdef __ANDROID__
std::unique_ptr<utility::AndroidSystemIO> g_systemIO;
#else
std::unique_ptr<utility::SystemIO> g_systemIO;
#endif


void evan::Engine::initializeAssetManager(void *platformAssetManager)
{
#ifdef __ANDROID__
	g_systemIO = std::make_unique<utility::AndroidSystemIO>(
		static_cast<AAssetManager *>(platformAssetManager));
#else
	g_systemIO = std::make_unique<utility::DefaultSystemIO>();
#endif
}

evan::Engine::Engine(const std::shared_ptr<IPlatform> &platform)
	: _platform(platform)
{
	if (!g_systemIO) {
#ifdef __ANDROID__
		throw std::runtime_error("Asset manager must be initialized before "
								 "creating Engine on Android");
#else
		initializeAssetManager(nullptr);
#endif
	}

	/**
	 * We load shaders from both "assets/shaders/" and "shaders/" directories to
	 * accommodate different platforms and build configurations. On Android, assets
	 * are typically stored in the "assets/" directory, while on other platforms,
	 * shaders may be stored in a "shaders/" directory within the project.
	 */
	g_assetManager->loadDirectory(std::string("assets/shaders/"));
	g_assetManager->loadDirectory(std::string("shaders/"));

	_deviceContext	  = std::make_shared<DeviceContext>(*platform);
	_swapchainContext = platform->createSwapchainContext(*_deviceContext);

	auto deviceBackend = _deviceContext->getDeviceBackend();
	_renderer		   = std::make_shared<Renderer>(*_deviceContext,
													_swapchainContext->getRenderPass(),
													_deviceContext->getMsaaSamples());
	_currentScene	   = 0;

	for (int frameIndex = 0; frameIndex < MAX_FRAMES_IN_FLIGHT; frameIndex++) {
		_renderer->createFrame(_deviceContext->getCommandPool(),
							   *deviceBackend);
	}
}

evan::Engine::~Engine()
{
	auto deviceBackend = _deviceContext->getDeviceBackend();
	auto device		   = deviceBackend->_device;

	vkDeviceWaitIdle(device);

	_renderer->destroy(device);
	_swapchainContext->destroy(device);
	for (auto &[_, scene]: _scenes) {
		scene.destroy(device);
	}
	_deviceContext.reset();
}

////////////////////
// Public Methods //
////////////////////

void evan::Engine::drawText(std::shared_ptr<utility::graphic::Text> text)
{
	std::map<uint32_t, utility::graphic::Mesh> rawObjects;
	uint32_t material_id = 0;

	for (const auto &mesh: text->getMeshes()) {
		rawObjects.emplace(material_id, *mesh);
	}

	RenderObject textObject = RenderObject(_deviceContext, rawObjects, "text");

	_scenes[_currentScene].addObject(_nextObjectID++, textObject);
}

void evan::Engine::drawPrimitive(std::shared_ptr<utility::graphic::Primitive> primitive)
{

}

void evan::Engine::drawModel(std::shared_ptr<utility::graphic::Model> model)
{

}

void evan::Engine::addScene(size_t sceneIndex)
{
	Scene newScene = Scene();

	_scenes.emplace(sceneIndex, std::move(newScene));
	if (_scenes.size() == 1) {
		_currentScene = sceneIndex;
	}
}

void evan::Engine::update()
{
	// Logic updates, input handling, etc.
	// Will be implemented in the future when the input system and scene
	// management will be implemented.
}

void evan::Engine::render()
{
	if (_scenes.empty()) {
		return;
	}

	auto currentSceneIt = _scenes.find(_currentScene);
	if (currentSceneIt == _scenes.end()) {
		return;
	}

	_renderer->drawFrame(*_deviceContext, *_swapchainContext,
						 currentSceneIt->second);
}

std::vector<std::unique_ptr<utility::event::Event>> evan::Engine::pollEvents()
{
	return _platform->pollEvents(*_deviceContext->getDeviceBackend());
}

void evan::Engine::switchScene(size_t sceneIndex)
{
	if (_scenes.find(sceneIndex) != _scenes.end()) {
		_currentScene = sceneIndex;
	} else {
		std::cerr << "Scene index " << sceneIndex << " does not exist."
				  << std::endl;
	}
}
