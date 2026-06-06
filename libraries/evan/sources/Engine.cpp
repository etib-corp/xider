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
	: _platform(platform)
{
	if (!g_systemIO) {
#ifdef __ANDROID__
		this->getLogger().error("Android system I/O not provided! Cannot initialize system I/O.");
		return;
#else
		this->getLogger().warning("No platform-specific system I/O provided. Initializing default system I/O.");
		initializeAssetManager(nullptr);
#endif
	}

	this->getLogger().info("Loading default shader...");

	ressourceProvider->loadShader("shaders/default.vert.spv", "shaders/default.frag.spv", *g_systemIO);
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
	this->getLogger().info("Destroying engine and cleaning up resources...");

	auto deviceBackend = _deviceContext->getDeviceBackend();
	auto device		   = deviceBackend->_device;

	this->getLogger().info("Waiting for device to be idle before cleanup...");
	vkDeviceWaitIdle(device);

	_renderer->destroy(device);
	_swapchainContext->destroy(device);
	for (auto &[_, scene]: _scenes) {
		scene->destroy(device);
	}
	_deviceContext.reset();
	this->getLogger().info("Engine destroyed and resources cleaned up successfully.");
}

////////////////////
// Public Methods //
////////////////////

void evan::Engine::drawText(std::shared_ptr<utility::graphic::Text> text)
{
	this->getLogger().info("Drawing text: " + text->getContent());

	std::map<uint32_t, utility::graphic::Mesh> rawObjects;
	uint32_t material_id = 0;

	this->getLogger().info("Converting text meshes to raw objects for rendering...");
	for (const auto &mesh: text->getMeshes()) {
		this->getLogger().debug("Processing mesh with " + std::to_string(mesh->getVertices().size()) + " vertices and " +
								 std::to_string(mesh->getIndices().size()) + " indices.");
		rawObjects.emplace(material_id, *mesh);
	}

	this->getLogger().info("Creating RenderObject for text...");
	std::shared_ptr<RenderObject> textObject = std::make_shared<RenderObject>(_deviceContext, rawObjects, "text");

	this->getLogger().info("Adding text RenderObject to current scene: " + std::to_string(_currentScene));
	_scenes[_currentScene]->addObject(_nextObjectID++, textObject);
}

void evan::Engine::drawPrimitive(
	std::shared_ptr<utility::graphic::Primitive> primitive)
{
	this->getLogger().info("Drawing primitive with " + std::to_string(primitive->getMeshes().size()) + " meshes.");
	this->getLogger().warning("drawPrimitive is not fully implemented yet. This is a placeholder implementation.");
}

void evan::Engine::drawModel(std::shared_ptr<utility::graphic::Model> model)
{
	auto modelTypeStr = std::string(model->type() == utility::graphic::Model::ModelType::OBJ ? "OBJ" : "Unknown");

	this->getLogger().info("Drawing model of type: " + modelTypeStr);
	this->getLogger().warning("drawModel is not fully implemented yet. This is a placeholder implementation.");
}

void evan::Engine::addScene(size_t sceneIndex)
{
	this->getLogger().info("Adding new scene with index: " + std::to_string(sceneIndex));

	_scenes[sceneIndex] = std::make_shared<Scene>();
	if (_scenes.size() == 1) {
		_currentScene = sceneIndex;
	}
}

void evan::Engine::update()
{
	this->getLogger().info("Updating engine state...");
	// Logic updates, input handling, etc.
	// Will be implemented in the future when the input system and scene
	// management will be implemented.
}

void evan::Engine::render()
{
	this->getLogger().info("Starting render process...");

	if (_scenes.empty()) {
		this->getLogger().warning("No scenes available to render. Skipping render process.");
		return;
	}

	this->getLogger().info("Rendering current scene with index: " + std::to_string(_currentScene));
	auto currentSceneIt = _scenes.find(_currentScene);
	if (currentSceneIt == _scenes.end()) {
		this->getLogger().warning("Current scene not found. Skipping render process.");
		return;
	}

	_renderer->drawFrame(*_deviceContext, *_swapchainContext,
						 *currentSceneIt->second);
}

std::vector<std::unique_ptr<utility::event::Event>> evan::Engine::pollEvents()
{
	this->getLogger().info("Polling events from platform...");

	utility::event::QuitEvent::Factory quitEventFactory;
	auto events = _platform->pollEvents(*_deviceContext->getDeviceBackend());

	if (_platform->shouldClose())
		events.emplace_back(quitEventFactory.create());
	return events;
}

void evan::Engine::switchScene(size_t sceneIndex)
{
	this->getLogger().info("Switching to scene with index: " + std::to_string(sceneIndex));
	if (_scenes.find(sceneIndex) != _scenes.end()) {
		this->getLogger().info("Scene found. Switching current scene to index: " + std::to_string(sceneIndex));
		_currentScene = sceneIndex;
	} else {
		this->getLogger().warning("Scene index " + std::to_string(sceneIndex) + " does not exist.");
	}
}
