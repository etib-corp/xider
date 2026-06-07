/*
** ETIB PROJECT, 2026
** evan
** File description:
** Engine
*/

#include <utility/event/quit_event.hpp>

#include "evan/Engine.hpp"


evan::Engine::Engine(
	std::shared_ptr<utility::RessourceProvider> ressourceProvider,
	std::shared_ptr<IPlatform> platform)
	: _platform(platform)
	, _ressourceProvider(ressourceProvider)
{

	this->getLogger().info("Loading text shader...");

	ressourceProvider->loadShader("assets/shaders/text.vert.spv",
								  "assets/shaders/text.frag.spv");

	/**
	 * We load shaders from both "assets/shaders/" and "shaders/" directories to
	 * accommodate different platforms and build configurations. On Android, assets
	 * are typically stored in the "assets/" directory, while on other platforms,
	 * shaders may be stored in a "shaders/" directory within the project.
	 */
	ressourceProvider->loadShader("shaders/text.vert.spv",
								"shaders/text.frag.spv");


	this->getLogger().info("Loading default shader...");

	ressourceProvider->loadShader("assets/shaders/default.vert.spv",
								  "assets/shaders/default.frag.spv");

	/**
	 * We load shaders from both "assets/shaders/" and "shaders/" directories to
	 * accommodate different platforms and build configurations. On Android, assets
	 * are typically stored in the "assets/" directory, while on other platforms,
	 * shaders may be stored in a "shaders/" directory within the project.
	 */
	ressourceProvider->loadShader("shaders/default.vert.spv",
								  "shaders/default.frag.spv");

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
	this->getLogger().info(
		"Engine destroyed and resources cleaned up successfully.");
}

////////////////////
// Public Methods //
////////////////////

size_t evan::Engine::addText(std::shared_ptr<utility::graphic::Text> text)
{
	this->getLogger().info("Drawing text: " + text->getContent());

	std::map<uint32_t, utility::graphic::Mesh> rawObjects;
	auto material_id =
		_ressourceProvider->getMaterialID(text->getFontFamily() + "_material");

	if (material_id == 0) {
		std::cerr << "Warning: Material '"
				  << text->getFontFamily() + "_material"
				  << "' not found for text object. Text will not be rendered."
				  << std::endl;
		return 0;	 // Skip rendering this text if its material is not found
	}

	this->getLogger().info(
		"Converting text meshes to raw objects for rendering...");
	for (const auto &mesh: text->getMeshes()) {
		this->getLogger().debug(
			"Processing mesh with " + std::to_string(mesh->getVertices().size())
			+ " vertices and " + std::to_string(mesh->getIndices().size())
			+ " indices.");
		rawObjects.emplace(material_id, *mesh);
	}

	this->getLogger().info("Creating RenderObject for text...");
	std::shared_ptr<RenderObject> textObject =
		std::make_shared<RenderObject>(_deviceContext, rawObjects, "text");

	this->getLogger().info("Adding text RenderObject to current scene: "
						   + std::to_string(_currentScene));
	auto objectID =
		_scenes[_currentScene]->addObject(_nextObjectID++, textObject);
	_ressourceManager->sync();
	return objectID;
}

size_t evan::Engine::addPrimitive(
	std::shared_ptr<utility::graphic::Primitive> primitive)
{
	this->getLogger().info("Drawing primitive with "
						   + std::to_string(primitive->getMeshes().size())
						   + " meshes.");
	this->getLogger().warning("drawPrimitive is not fully implemented yet. "
							  "This is a placeholder implementation.");
	return 0;	 // Placeholder implementation - replace with actual primitive
				 // addition logic
}

size_t evan::Engine::addModel(std::shared_ptr<utility::graphic::Model> model)
{
	auto modelTypeStr = std::string(
		model->type() == utility::graphic::Model::ModelType::OBJ ? "OBJ"
																 : "Unknown");

	this->getLogger().info("Drawing model of type: " + modelTypeStr);
	this->getLogger().warning("drawModel is not fully implemented yet. This is "
							  "a placeholder implementation.");

	return 0;	 // Placeholder implementation - replace with actual model
				 // addition logic
}

size_t evan::Engine::addObject(
	std::shared_ptr<utility::graphic::Renderable> object,
	const std::string &renderMethod)
{
	return 0;	 // Placeholder implementation - replace with actual renderable
				 // object addition logic
}

size_t evan::Engine::addMesh(const utility::graphic::Mesh &mesh)
{
	std::map<uint32_t, utility::graphic::Mesh> rawObjects;
	auto material_id = _ressourceProvider->getMaterialID("default_material");
	rawObjects.emplace(material_id, mesh);
	std::shared_ptr<RenderObject> meshObject =
		std::make_shared<RenderObject>(_deviceContext, rawObjects, "mesh");
	auto objectID =
		_scenes[_currentScene]->addObject(_nextObjectID++, meshObject);
	_ressourceManager->sync();
	return objectID;
}

bool evan::Engine::removeObject(size_t objectID)
{
	auto currentSceneIt = _scenes.find(_currentScene);
	if (currentSceneIt == _scenes.end()) {
		return false;
	}
	return currentSceneIt->second->removeObject(static_cast<uint32_t>(objectID));
}

void evan::Engine::setView(const utility::graphic::ViewF &view)
{
	_swapchainContext->setView(view);
}

utility::graphic::ViewF evan::Engine::getView(void) const
{
	return _swapchainContext->getView();
}

void evan::Engine::addScene(size_t sceneIndex)
{
	this->getLogger().info("Adding new scene with index: "
						   + std::to_string(sceneIndex));

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
		this->getLogger().warning(
			"No scenes available to render. Skipping render process.");
		return;
	}

	this->getLogger().info("Rendering current scene with index: "
						   + std::to_string(_currentScene));
	auto currentSceneIt = _scenes.find(_currentScene);
	if (currentSceneIt == _scenes.end()) {
		this->getLogger().warning(
			"Current scene not found. Skipping render process.");
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
	this->getLogger().info("Switching to scene with index: "
						   + std::to_string(sceneIndex));
	if (_scenes.find(sceneIndex) != _scenes.end()) {
		this->getLogger().info("Scene found. Switching current scene to index: "
							   + std::to_string(sceneIndex));
		_currentScene = sceneIndex;
	} else {
		this->getLogger().warning("Scene index " + std::to_string(sceneIndex)
								  + " does not exist.");
	}
}
