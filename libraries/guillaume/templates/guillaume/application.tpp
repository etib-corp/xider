/*
 Copyright (c) 2026 ETIB Corporation

 Permission is hereby granted, free of charge, to any person obtaining a copy of
 this software and associated documentation files (the "Software"), to deal in
 the Software without restriction, including without limitation the rights to
 use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies
 of the Software, and to permit persons to whom the Software is furnished to do
 so, subject to the following conditions:

 The above copyright notice and this permission notice shall be included in all
 copies or substantial portions of the Software.

 THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 SOFTWARE.
 */

#pragma once

#include "guillaume/application.hpp"

namespace guillaume
{

	template<InheritFromScene... SceneTypes> template<typename PhaseType>
	void Application<SceneTypes...>::runPhase(PhaseType &phaseDefinition)
	{
		const ecs::Phase phase					= phaseDefinition.getPhase();
		const ecs::EntityTreeTraveler &traveler = phaseDefinition.getTraveler();

		this->getLogger().debug() << "Running systems for phase: " << static_cast<int>(phase);
		for (const auto &system: _systemRegistry.getSystemsByPhase(phase)) {
			system->routine(_sceneManager->getActiveComponentRegistry(),
							_sceneManager->getActiveEntityRegistry(), traveler);
		}
		this->getLogger().debug() << "Finished systems for phase: " << static_cast<int>(phase);
	}

	template<InheritFromScene... SceneTypes>
	void Application<SceneTypes...>::registerCoreSystems()
	{
		_systemRegistry.registerNewSystem(
			std::make_unique<systems::MeasureText>(_ressourceProvider,
												   _systemIO, _engine));
		_systemRegistry.registerNewSystem(
			std::make_unique<systems::MouseMotion>(_eventBus, _engine));
		_systemRegistry.registerNewSystem(
			std::make_unique<systems::MouseButton>(_eventBus, _engine));
		_systemRegistry.registerNewSystem(
			std::make_unique<systems::HandMotion>(_eventBus));
		_systemRegistry.registerNewSystem(
			std::make_unique<systems::HandButton>(_eventBus));
		_systemRegistry.registerNewSystem(
			std::make_unique<systems::HandPinch>(_eventBus));
		_systemRegistry.registerNewSystem(
			std::make_unique<systems::HandPoke>(_eventBus));
		_systemRegistry.registerNewSystem(
			std::make_unique<systems::HandSqueeze>(_eventBus));
		_systemRegistry.registerNewSystem(
			std::make_unique<systems::HandThumbRest>(_eventBus));
		_systemRegistry.registerNewSystem(
			std::make_unique<systems::HandThumbStick>(_eventBus));
		_systemRegistry.registerNewSystem(
			std::make_unique<systems::HandTrigger>(_eventBus));
		_systemRegistry.registerNewSystem(std::make_unique<systems::TextRender>(
			_ressourceProvider, _systemIO, _engine));
		_systemRegistry.registerNewSystem(
			std::make_unique<systems::GlyphRender>(_ressourceProvider,
												   _systemIO, _engine));
		_systemRegistry.registerNewSystem(
			std::make_unique<systems::KeyboardControl>(_eventBus));
		_systemRegistry.registerNewSystem(
			std::make_unique<systems::TextInput>(_eventBus));
		_systemRegistry.registerNewSystem(
			std::make_unique<systems::RectangleRender>(_engine));
	}

	template<InheritFromScene... SceneTypes>
	Application<SceneTypes...>::Application(void)
		: _ressourceProvider(std::make_shared<utility::RessourceProvider>())
		, _systemIO(std::make_shared<utility::DefaultSystemIO>())
		, _engine(nullptr)
		, _sceneManager(nullptr)
		, _eventBus()
		, _quitEventSubscriber(_eventBus)
		, _systemRegistry()
		, _systemPhases()
	{
		registerCoreSystems();
		_sceneManager = std::make_unique<SceneManagerFiller<SceneTypes...>>();
	}

	template<InheritFromScene... SceneTypes>
	Application<SceneTypes...>::~Application(void)
	{
	}

	template<InheritFromScene... SceneTypes>
	void Application<SceneTypes...>::setEngine(std::unique_ptr<Engine> engine)
	{
		_engine = std::move(engine);
		_engine->setEventCallback(
			[this](std::unique_ptr<utility::event::Event> &event) {
				this->_eventBus.publish(std::move(event));
			});
		std::vector<std::type_index> sceneTypes =
			_sceneManager->getRegisteredSceneTypes();
		for (const auto &sceneType: sceneTypes) {
			_engine->addScene(sceneType.hash_code());
		}
	}

	template<InheritFromScene... SceneTypes>
	void Application<SceneTypes...>::routine(void)
	{
		std::apply(
			[this](auto &...phaseDefinition) {
				(this->runPhase(phaseDefinition), ...);
			},
			_systemPhases);
	}

	template<InheritFromScene... SceneTypes>
	void Application<SceneTypes...>::pollEvents(void)
	{
		_engine->pollEvents();
	}

	template<InheritFromScene... SceneTypes>
	void Application<SceneTypes...>::clear(void)
	{
		_engine->clear();
	}

	template<InheritFromScene... SceneTypes>
	void Application<SceneTypes...>::present(void)
	{
		_engine->present();
	}

	template<InheritFromScene... SceneTypes>
	bool Application<SceneTypes...>::gotNewEvents(void) const
	{
		return _engine->gotNewEvents();
	}

	template<InheritFromScene... SceneTypes>
	bool Application<SceneTypes...>::shouldQuit(void)
	{
		if (_quitEventSubscriber.hasPendingEvents()) {
			_quitEventSubscriber.getNextEvent();
			return true;
		}
		return false;
	}

	template<InheritFromScene... SceneTypes>
	int Application<SceneTypes...>::run(void)
	{
		this->getLogger().info() << "Entering main loop";
		while (!shouldQuit()) {
			try {
				_engine->pollEvents();
				if (!_engine->gotNewEvents()) {
					continue;
				}
				_engine->clear();
				routine();
				_engine->present();
			} catch (const std::exception &exception) {
				this->getLogger().error() << std::string("Application error: ") << exception.what();
				return EXIT_FAILURE;
			}
		}
		this->getLogger().info() << "Exiting main loop";
		return EXIT_SUCCESS;
	}
}	 // namespace guillaume