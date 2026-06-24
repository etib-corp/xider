/*
** ETIB PROJECT, 2026
** evan
** File description:
** Engine
*/

#include <utility/event/quit_event.hpp>
#include <utility/event/keyboard_event.hpp>
#include <utility/event/mouse_motion_event.hpp>
#include <utility/event/mouse_button_event.hpp>

#include "evan/Engine.hpp"

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/matrix_decompose.hpp>

void evan::Engine::updateDeltaTime(void)
{
	auto currentTime					   = std::chrono::steady_clock::now();
	std::chrono::duration<float> deltaTime = currentTime - _lastFrameTime;
	_deltaTime							   = deltaTime.count();
	_lastFrameTime						   = currentTime;
}

evan::Engine::Engine(
	std::shared_ptr<utility::RessourceProvider> ressourceProvider,
	std::shared_ptr<IPlatform> platform)
	: _platform(platform)
	, _ressourceProvider(ressourceProvider)
	, _lastFrameTime(std::chrono::steady_clock::now())
	, _deltaTime(0.0f)
{
	this->getLogger().info() << "Loading text shader...";

	std::string shaderPrefix = "shaders/";

	#ifdef DEBUG
		shaderPrefix = "shaders/debug/";
		this->getLogger().info() << "Debug mode enabled. Using debug shader prefix: "
								 << shaderPrefix;
	#endif

	ressourceProvider->loadShader(shaderPrefix + "text.vert.spv",
								  shaderPrefix + "text.frag.spv");

	this->getLogger().info() << "Loading default shader...";

	ressourceProvider->loadShader(shaderPrefix + "default.vert.spv",
								  shaderPrefix + "default.frag.spv");

	this->getLogger().info() << "Loading mesh shader...";

	ressourceProvider->loadShader(shaderPrefix + "mesh.vert.spv",
								  shaderPrefix + "mesh.frag.spv");

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

	// Debug view setup - create initial view matrix
	// _viewMatrix =
	// 	glm::lookAt(glm::vec3(0.0f, 0.0f, 100.0f), glm::vec3(0.0f, 0.0f,
	// -10.0f), 				glm::vec3(0.0f, 1.0f, 1.0f));
	// _swapchainContext->setView(0, _viewMatrix);
}

evan::Engine::~Engine()
{
	this->getLogger().info()
		<< "Destroying engine and cleaning up resources...";

	auto deviceBackend = _deviceContext->getDeviceBackend();
	auto device		   = deviceBackend->_device;

	this->getLogger().info()
		<< "Waiting for device to be idle before cleanup...";
	vkDeviceWaitIdle(device);

	_renderer->destroy(device);
	_swapchainContext->destroy(device);
	for (auto &[_, scene]: _scenes) {
		scene->destroy(device);
	}
	_deviceContext.reset();
	this->getLogger().info()
		<< "Engine destroyed and resources cleaned up successfully.";
}

////////////////////
// Public Methods //
////////////////////

size_t evan::Engine::addText(std::shared_ptr<utility::graphic::Text> text)
{
	this->getLogger().info() << "Drawing text: " << text->getContent();

	std::map<uint32_t, utility::graphic::Mesh> rawObjects;
	auto material_id =
		_ressourceProvider->getMaterialID(text->getFontFamily() + "_material");

	if (material_id == 0) {
		this->getLogger().warning()
			<< "Material '" << text->getFontFamily()
			<< "' not found for text object. Text will not be rendered.";
		return 0;	 // Skip rendering this text if its material is not found
	}

	this->getLogger().info()
		<< "Converting text meshes to raw objects for rendering...";
	for (const auto &mesh: text->getMeshes()) {
		this->getLogger().debug()
			<< "Processing mesh with " << mesh->getVertices().size()
			<< " vertices and " << mesh->getIndices().size() << " indices.";
		rawObjects.emplace(material_id, *mesh);
	}

	this->getLogger().info() << "Creating RenderObject for text...";
	std::shared_ptr<RenderObject> textObject =
		std::make_shared<RenderObject>(_deviceContext, rawObjects, "text");

	this->getLogger().info()
		<< "Adding text RenderObject to current scene: " << _currentScene;
	auto objectID =
		_scenes[_currentScene]->addObject(_nextObjectID++, textObject);
	_ressourceManager->sync();
	return objectID;
}

size_t evan::Engine::addPrimitive(
	std::shared_ptr<utility::graphic::Primitive> primitive)
{
	this->getLogger().info() << "Drawing primitive with "
							 << primitive->getMeshes().size() << " meshes.";
	this->getLogger().warning()
		<< "drawPrimitive is not fully implemented yet. "
		   "This is a placeholder implementation.";
	return 0;	 // Placeholder implementation - replace with actual primitive
				 // addition logic
}

size_t evan::Engine::addModel(std::shared_ptr<utility::graphic::Model> model)
{
	auto modelTypeStr = std::string(
		model->type() == utility::graphic::Model::ModelType::OBJ ? "OBJ"
																 : "Unknown");

	this->getLogger().info() << "Drawing model of type: " << modelTypeStr;
	this->getLogger().warning()
		<< "drawModel is not fully implemented yet. This is "
		   "a placeholder implementation.";

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

size_t evan::Engine::addMesh(const utility::graphic::Mesh &mesh,
							 const std::string &materialName,
							 const std::string &shader)
{
	std::map<uint32_t, utility::graphic::Mesh> rawObjects;
	auto material_id = _ressourceProvider->getMaterialID(materialName);
	rawObjects.emplace(material_id, mesh);
	std::shared_ptr<RenderObject> meshObject =
		std::make_shared<RenderObject>(_deviceContext, rawObjects, shader);
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
	return currentSceneIt->second->removeObject(
		static_cast<uint32_t>(objectID));
}

utility::graphic::ViewF evan::Engine::getView(void) const
{
	// Extract position and orientation from view matrix
	utility::graphic::PositionF position =
		extractPositionFromViewMatrix(_viewMatrix);
	utility::graphic::OrientationF orientation =
		extractOrientationFromViewMatrix(_viewMatrix);

	// Create Pose from position and orientation
	utility::graphic::PoseF pose(position, orientation);

	// Create ViewF with default FOV and viewport (these would need to be stored
	// separately if needed)
	utility::math::Vector2F viewportSize { 1920.0f, 1080.0f };
	return utility::graphic::ViewF(pose, 45.0f, 16.0f / 9.0f, viewportSize);
}

glm::mat4 evan::Engine::getViewMatrix(void) const
{
	return _viewMatrix;
}

void evan::Engine::addScene(size_t sceneIndex)
{
	this->getLogger().info() << "Adding new scene with index: " << sceneIndex;

	_scenes[sceneIndex] = std::make_shared<Scene>();
	if (_scenes.size() == 1) {
		_currentScene = sceneIndex;
	}
}

void evan::Engine::update()
{
	updateDeltaTime();
	this->getLogger().info() << "Updating engine state...";

	// Handle viewport input if capture is enabled
	if (_shouldCaptureViewportInput) {
		handleViewportInput(_capturedViewportEvents);
		_capturedViewportEvents.clear();
	}
}

void evan::Engine::render()
{
	this->getLogger().info() << "Starting render process...";

	if (_scenes.empty()) {
		this->getLogger().warning()
			<< "No scenes available to render. Skipping render process.";
		return;
	}

	this->getLogger().info()
		<< "Rendering current scene with index: " << _currentScene;
	auto currentSceneIt = _scenes.find(_currentScene);
	if (currentSceneIt == _scenes.end()) {
		this->getLogger().warning()
			<< "Current scene not found. Skipping render process.";
		return;
	}

	_renderer->drawFrame(*_deviceContext, *_swapchainContext,
						 *currentSceneIt->second);
}

std::vector<std::shared_ptr<utility::event::Event>> evan::Engine::pollEvents()
{
	this->getLogger().info() << "Polling events from platform...";

	utility::event::QuitEvent::Factory quitEventFactory;
	auto events = _platform->pollEvents(*_deviceContext->getDeviceBackend());

	if (_platform->shouldClose())
		events.emplace_back(quitEventFactory.create());

	if (_shouldCaptureViewportInput) {
		this->getLogger().info()
			<< "Captured " << events.size() << " events for viewport input.";
		_capturedViewportEvents.insert(_capturedViewportEvents.end(),
									   events.begin(), events.end());

	} else {
		this->getLogger().info()
			<< "Captured " << events.size()
			<< " events, but viewport input capture is disabled.";
	}

	return events;
}

void evan::Engine::switchScene(size_t sceneIndex)
{
	this->getLogger().info() << "Switching to scene with index: " << sceneIndex;
	if (_scenes.find(sceneIndex) != _scenes.end()) {
		this->getLogger().info()
			<< "Scene found. Switching current scene to index: " << sceneIndex;
		_currentScene = sceneIndex;
	} else {
		this->getLogger().warning()
			<< "Scene index " << sceneIndex << " does not exist.";
	}
}

void evan::Engine::handleViewportInput(
	const std::vector<std::shared_ptr<utility::event::Event>> &events)
{
	utility::graphic::PositionF position =
		extractPositionFromViewMatrix(_viewMatrix);
	utility::graphic::OrientationF orientation =
		extractOrientationFromViewMatrix(_viewMatrix);

	const float movementSpeed = 10.0f;
	const float rotationSpeed = 0.01f;

	// Use persistent state (not reset every frame)
	bool isRightMouseButtonPressed			  = _isRightMouseButtonPressed;
	utility::math::Vector2F lastMousePosition = _lastMousePosition;

	for (const auto &event: events) {
		// Handle keyboard events for movement
		if (event->getEventType() == utility::event::Event::Type::Keyboard) {
			std::shared_ptr<utility::event::KeyboardEvent> keyboardEvent =
				std::dynamic_pointer_cast<utility::event::KeyboardEvent>(event);
			if (keyboardEvent) {
				handleKeyboardMovement(keyboardEvent, _viewMatrix, position,
									   movementSpeed, _deltaTime);
			}
		}

		// Handle mouse button events for rotation
		else if (event->getEventType()
				 == utility::event::Event::Type::MouseButton) {
			std::shared_ptr<utility::event::MouseButtonEvent> mouseButtonEvent =
				std::dynamic_pointer_cast<utility::event::MouseButtonEvent>(
					event);
			if (mouseButtonEvent) {
				handleMouseButtonEvent(mouseButtonEvent,
									   isRightMouseButtonPressed,
									   lastMousePosition);
			}
		}

		// Handle mouse motion events for rotation
		else if (event->getEventType()
				 == utility::event::Event::Type::MouseMotion) {
			std::shared_ptr<utility::event::MouseMotionEvent> mouseMotionEvent =
				std::dynamic_pointer_cast<utility::event::MouseMotionEvent>(
					event);
			if (mouseMotionEvent) {
				handleMouseMotionEvent(
					mouseMotionEvent, isRightMouseButtonPressed,
					lastMousePosition, orientation, rotationSpeed, _deltaTime);
			}
		}
	}

	// Update view matrix
	_viewMatrix = buildViewMatrix(position, orientation);

	// Save mouse state for next frame
	_isRightMouseButtonPressed = isRightMouseButtonPressed;
	_lastMousePosition		   = lastMousePosition;

	_swapchainContext->setView(0, _viewMatrix);
}

void evan::Engine::handleKeyboardMovement(
	const std::shared_ptr<utility::event::KeyboardEvent> &keyboardEvent,
	const glm::mat4 &viewMatrix, utility::graphic::PositionF &position,
	float movementSpeed, float deltaTime)
{
	if (!keyboardEvent->getIsDownEvent()) {
		return;
	}

	// Use keycode instead of scancode (GLFW callbacks set keycode, not
	// scancode)
	auto keycode = keyboardEvent->getKeycode();
	utility::math::Vector3F movement { 0.0f, 0.0f, 0.0f };

	// Extract forward, right, and up vectors from view matrix
	// View matrix transforms world to view space, so we need the inverse
	glm::mat3 viewRotation = glm::mat3(viewMatrix);
	auto forward =
		-utility::math::Vector3F { viewRotation[2].x, viewRotation[2].y,
								   viewRotation[2].z };
	auto right = utility::math::Vector3F { viewRotation[0].x, viewRotation[0].y,
										   viewRotation[0].z };
	auto up	   = utility::math::Vector3F { viewRotation[1].x, viewRotation[1].y,
										   viewRotation[1].z };

	switch (keycode) {
		case utility::event::KeyboardEvent::KeyCode::W:
		case utility::event::KeyboardEvent::KeyCode::Up:
			movement = forward * movementSpeed * deltaTime;
			break;
		case utility::event::KeyboardEvent::KeyCode::S:
		case utility::event::KeyboardEvent::KeyCode::Down:
			movement = -forward * movementSpeed * deltaTime;
			break;
		case utility::event::KeyboardEvent::KeyCode::A:
		case utility::event::KeyboardEvent::KeyCode::Left:
			movement = -right * movementSpeed * deltaTime;
			break;
		case utility::event::KeyboardEvent::KeyCode::D:
		case utility::event::KeyboardEvent::KeyCode::Right:
			movement = right * movementSpeed * deltaTime;
			break;
		case utility::event::KeyboardEvent::KeyCode::Q:
			movement = -up * movementSpeed * deltaTime;
			break;
		case utility::event::KeyboardEvent::KeyCode::E:
			movement = up * movementSpeed * deltaTime;
			break;
		default:
			return;
	}

	if (movement != utility::math::Vector3F { 0.0f, 0.0f, 0.0f }) {
		position = utility::graphic::PositionF(position.getX() + movement.x,
											   position.getY() + movement.y,
											   position.getZ() + movement.z);
	}
}

void evan::Engine::handleMouseButtonEvent(
	const std::shared_ptr<utility::event::MouseButtonEvent> &mouseButtonEvent,
	bool &isRightMouseButtonPressed, utility::math::Vector2F &lastMousePosition)
{
	if (mouseButtonEvent->getButton()
		== utility::event::MouseButtonEvent::Button::Right) {
		isRightMouseButtonPressed = mouseButtonEvent->isPressed();
		if (isRightMouseButtonPressed) {
			lastMousePosition = mouseButtonEvent->getPosition();
		}
	}
}

void evan::Engine::handleMouseMotionEvent(
	const std::shared_ptr<utility::event::MouseMotionEvent> &mouseMotionEvent,
	bool isRightMouseButtonPressed, utility::math::Vector2F &lastMousePosition,
	utility::graphic::OrientationF &orientation, float rotationSpeed,
	float deltaTime)
{
	if (!isRightMouseButtonPressed) {
		return;
	}

	auto currentPosition = mouseMotionEvent->getPosition();
	auto deltaX = static_cast<float>(static_cast<int>(currentPosition.x)
									 - static_cast<int>(lastMousePosition.x));
	auto deltaY = static_cast<float>(static_cast<int>(currentPosition.y)
									 - static_cast<int>(lastMousePosition.y));

	// Create rotation from mouse delta
	glm::quat currentQuat(orientation.w, orientation.x, orientation.y,
						  orientation.z);

	// Rotate around Y axis (yaw) for horizontal movement
	glm::quat yawRotation = glm::angleAxis(deltaX * rotationSpeed * deltaTime,
										   glm::vec3(0.0f, 1.0f, 0.0f));

	// Rotate around X axis (pitch) for vertical movement
	glm::quat pitchRotation = glm::angleAxis(deltaY * rotationSpeed * deltaTime,
											 glm::vec3(1.0f, 0.0f, 0.0f));

	glm::quat newOrientation = yawRotation * pitchRotation * currentQuat;
	newOrientation			 = glm::normalize(newOrientation);

	orientation = utility::graphic::OrientationF(
		newOrientation.x, newOrientation.y, newOrientation.z, newOrientation.w);

	lastMousePosition = currentPosition;
}

utility::graphic::PositionF evan::Engine::extractPositionFromViewMatrix(
	const glm::mat4 &viewMatrix) const
{
	// Position is in the 4th column of the inverse view matrix
	// For a view matrix V = [R | -R*P], where R is rotation and P is position
	// The inverse is V^-1 = [R^T | P], so position is in the translation part
	glm::mat3 rotation	  = glm::mat3(viewMatrix);
	glm::vec3 translation = glm::vec3(viewMatrix[3]);

	// Extract position by applying inverse rotation to negative translation
	glm::vec3 position = -glm::transpose(rotation) * translation;
	return utility::graphic::PositionF(position.x, position.y, position.z);
}

utility::graphic::OrientationF evan::Engine::extractOrientationFromViewMatrix(
	const glm::mat4 &viewMatrix) const
{
	// Extract rotation matrix from view matrix
	glm::mat3 rotation = glm::mat3(viewMatrix);

	// View matrix rotation is the inverse of camera orientation
	// So we need to transpose it to get the camera orientation
	glm::mat3 cameraRotation = glm::transpose(rotation);

	// Convert to quaternion
	glm::quat quat = glm::quat(cameraRotation);
	return utility::graphic::OrientationF(quat.x, quat.y, quat.z, quat.w);
}

glm::mat4 evan::Engine::buildViewMatrix(
	const utility::graphic::PositionF &position,
	const utility::graphic::OrientationF &orientation) const
{
	// Build view matrix from position and orientation
	// View matrix = inverse of camera world matrix
	glm::quat quat(orientation.w, orientation.x, orientation.y, orientation.z);
	glm::mat4 cameraWorld = glm::mat4_cast(quat);
	cameraWorld[3] =
		glm::vec4(position.getX(), position.getY(), position.getZ(), 1.0f);

	// Return inverse (view matrix)
	return glm::inverse(cameraWorld);
}
