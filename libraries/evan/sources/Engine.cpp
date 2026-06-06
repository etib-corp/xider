/*
** ETIB PROJECT, 2026
** evan
** File description:
** Engine
*/

#include <utility/event/quit_event.hpp>

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

evan::Engine::Engine(
	std::shared_ptr<utility::RessourceProvider> ressourceProvider,
	std::shared_ptr<IPlatform> platform)
	: _platform(platform), _ressourceProvider(ressourceProvider)
{
	if (!g_systemIO) {
#ifdef __ANDROID__
		throw std::runtime_error("Asset manager must be initialized before "
								 "creating Engine on Android");
#else
		initializeAssetManager(nullptr);
#endif
	}

	ressourceProvider->loadShader("assets/shaders/text.vert.spv",
								  "assets/shaders/text.frag.spv",
								  *g_systemIO);
	ressourceProvider->loadShader("assets/shaders/default.vert.spv",
								  "assets/shaders/default.frag.spv",
								  *g_systemIO);

	_deviceContext	  = std::make_shared<DeviceContext>(*platform);
	_swapchainContext = platform->createSwapchainContext(*_deviceContext);

	auto deviceBackend = _deviceContext->getDeviceBackend();

	_ressourceManager =
		std::make_shared<RessourceManager>(ressourceProvider, _deviceContext);
	_renderer = std::make_shared<Renderer>(
		*_deviceContext, _swapchainContext->getRenderPass(),
		_deviceContext->getMsaaSamples(), _ressourceManager);
	_ressourceManager->init(_renderer);
	_currentScene = 0;

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

size_t evan::Engine::addText(std::shared_ptr<utility::graphic::Text> text)
{
	std::map<uint32_t, utility::graphic::Mesh> rawObjects;
	auto material_id = _ressourceProvider->getMaterialID(
		text->getFontFamily() + "_material");

	if (material_id == 0) {
		std::cerr << "Warning: Material '" << text->getFontFamily() + "_material"
				  << "' not found for text object. Text will not be rendered."
				  << std::endl;
		return 0;  // Skip rendering this text if its material is not found
	}

	for (const auto &mesh: text->getMeshes()) {
		rawObjects.emplace(material_id, *mesh);
	}

	std::shared_ptr<RenderObject> textObject =
		std::make_shared<RenderObject>(_deviceContext, rawObjects, "text");

	auto objectID = _scenes[_currentScene].addObject(_nextObjectID++, textObject);
	_ressourceManager->sync();
	return objectID;
}

size_t evan::Engine::addPrimitive(
	std::shared_ptr<utility::graphic::Primitive> primitive)
{
	return 0;  // Placeholder implementation - replace with actual primitive addition logic
}

size_t evan::Engine::addModel(std::shared_ptr<utility::graphic::Model> model)
{
	return 0;  // Placeholder implementation - replace with actual model addition logic
}

size_t evan::Engine::addObject(std::shared_ptr<utility::graphic::Renderable> object,
								const std::string &renderMethod)
{
	return 0;  // Placeholder implementation - replace with actual renderable object addition logic
}

size_t evan::Engine::addMesh(const utility::graphic::Mesh &mesh)
{
	std::map<uint32_t, utility::graphic::Mesh> rawObjects;
	auto material_id = _ressourceProvider->getMaterialID("default_material");
	rawObjects.emplace(material_id, mesh);
	std::shared_ptr<RenderObject> meshObject =
		std::make_shared<RenderObject>(_deviceContext, rawObjects, "mesh");
	auto objectID = _scenes[_currentScene].addObject(_nextObjectID++, meshObject);
	_ressourceManager->sync();
	return objectID;
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
	utility::event::QuitEvent::Factory quitEventFactory;
	auto events = _platform->pollEvents(*_deviceContext->getDeviceBackend());

	if (_platform->shouldClose())
		events.emplace_back(quitEventFactory.create());
	return events;
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
