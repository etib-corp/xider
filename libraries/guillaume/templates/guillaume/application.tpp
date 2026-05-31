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

	template<InheritFromRenderer RendererType,
			 event::InheritFromEventHandler EventHandlerType,
			 InheritFromScene... SceneTypes>
	template<typename PhaseType>
	void Application<RendererType, EventHandlerType, SceneTypes...>::runPhase(
		PhaseType &phaseDefinition)
	{
		const ecs::Phase phase					= phaseDefinition.getPhase();
		const ecs::EntityTreeTraveler &traveler = phaseDefinition.getTraveler();

		this->getLogger().debug("Running systems for phase: "
								+ std::to_string(static_cast<int>(phase)));
		for (const auto &system: _systemRegistry.getSystemsByPhase(phase)) {
			system->routine(_sceneManager->getActiveComponentRegistry(),
							_sceneManager->getActiveEntityRegistry(), traveler);
		}
		this->getLogger().debug("Finished systems for phase: "
								+ std::to_string(static_cast<int>(phase)));
	}

	template<InheritFromRenderer RendererType,
			 event::InheritFromEventHandler EventHandlerType,
			 InheritFromScene... SceneTypes>
	void Application<RendererType, EventHandlerType,
					 SceneTypes...>::registerCoreSystems()
	{
		_systemRegistry.registerNewSystem(
			std::make_unique<systems::MeasureText>(_renderer));
		_systemRegistry.registerNewSystem(
			std::make_unique<systems::MouseMotion>(_eventBus, _renderer));
		_systemRegistry.registerNewSystem(
			std::make_unique<systems::MouseButton>(_eventBus, _renderer));
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
		_systemRegistry.registerNewSystem(
			std::make_unique<systems::TextRender>(_renderer));
		_systemRegistry.registerNewSystem(
			std::make_unique<systems::GlyphRender>(_renderer));
		_systemRegistry.registerNewSystem(
			std::make_unique<systems::KeyboardControl>(_eventBus));
		_systemRegistry.registerNewSystem(
			std::make_unique<systems::TextInput>(_eventBus));
		_systemRegistry.registerNewSystem(
			std::make_unique<systems::RectangleRender>(_renderer));
	}

	template<InheritFromRenderer RendererType,
			 event::InheritFromEventHandler EventHandlerType,
			 InheritFromScene... SceneTypes>
	Application<RendererType, EventHandlerType, SceneTypes...>::Application(
		void)
		: _renderer()
		, _eventHandler()
		, _sceneManager(nullptr)
		, _eventBus()
		, _systemRegistry()
		, _systemPhases()
	{
		registerCoreSystems();
		_eventHandler.setEventCallback(
			[this](std::unique_ptr<utility::event::Event> &event) {
				this->_eventBus.publish(std::move(event));
			});
		_sceneManager = std::make_unique<SceneManagerFiller<SceneTypes...>>();
		_ressourceProvider = std::make_shared<utility::RessourceProvider>();
	}

	template<InheritFromRenderer RendererType,
			 event::InheritFromEventHandler EventHandlerType,
			 InheritFromScene... SceneTypes>
	Application<RendererType, EventHandlerType, SceneTypes...>::~Application(
		void)
	{
	}

	template<InheritFromRenderer RendererType,
			 event::InheritFromEventHandler EventHandlerType,
			 InheritFromScene... SceneTypes>
	void Application<RendererType, EventHandlerType, SceneTypes...>::setRenderer(RendererType renderer)
	{
		_renderer = std::move(renderer);
	}

	template<InheritFromRenderer RendererType,
			 event::InheritFromEventHandler EventHandlerType,
			 InheritFromScene... SceneTypes>
	RendererType &
		Application<RendererType, EventHandlerType, SceneTypes...>::getRenderer(
			void)
	{
		return _renderer;
	}

	template<InheritFromRenderer RendererType,
			 event::InheritFromEventHandler EventHandlerType,
			 InheritFromScene... SceneTypes>
	const RendererType &
		Application<RendererType, EventHandlerType, SceneTypes...>::getRenderer(
			void) const
	{
		return _renderer;
	}

	template<InheritFromRenderer RendererType,
			 event::InheritFromEventHandler EventHandlerType,
			 InheritFromScene... SceneTypes>
	EventHandlerType &Application<RendererType, EventHandlerType,
								  SceneTypes...>::getEventHandler(void)
	{
		return _eventHandler;
	}

	template<InheritFromRenderer RendererType,
			 event::InheritFromEventHandler EventHandlerType,
			 InheritFromScene... SceneTypes>
	const EventHandlerType &
		Application<RendererType, EventHandlerType,
					SceneTypes...>::getEventHandler(void) const
	{
		return _eventHandler;
	}

	template<InheritFromRenderer RendererType,
			 event::InheritFromEventHandler EventHandlerType,
			 InheritFromScene... SceneTypes>
	void Application<RendererType, EventHandlerType, SceneTypes...>::routine(
		void)
	{
		std::apply(
			[this](auto &...phaseDefinition) {
				(this->runPhase(phaseDefinition), ...);
			},
			_systemPhases);
	}

	template<InheritFromRenderer RendererType,
			 event::InheritFromEventHandler EventHandlerType,
			 InheritFromScene... SceneTypes>
	void Application<RendererType, EventHandlerType, SceneTypes...>::pollEvents(
		void)
	{
		_eventHandler.pollEvents();
	}

	template<InheritFromRenderer RendererType,
			 event::InheritFromEventHandler EventHandlerType,
			 InheritFromScene... SceneTypes>
	void Application<RendererType, EventHandlerType, SceneTypes...>::clear(void)
	{
		_renderer.clear();
	}

	template<InheritFromRenderer RendererType,
			 event::InheritFromEventHandler EventHandlerType,
			 InheritFromScene... SceneTypes>
	void Application<RendererType, EventHandlerType, SceneTypes...>::present(
		void)
	{
		_renderer.present();
	}

	template<InheritFromRenderer RendererType,
			 event::InheritFromEventHandler EventHandlerType,
			 InheritFromScene... SceneTypes>
	bool Application<RendererType, EventHandlerType,
					 SceneTypes...>::gotNewEvents(void) const
	{
		return _eventHandler.gotNewEvents();
	}

	template<InheritFromRenderer RendererType,
			 event::InheritFromEventHandler EventHandlerType,
			 InheritFromScene... SceneTypes>
	bool Application<RendererType, EventHandlerType, SceneTypes...>::shouldQuit(
		void) const
	{
		return _eventHandler.shouldQuit();
	}

	template<InheritFromRenderer RendererType,
			 event::InheritFromEventHandler EventHandlerType,
			 InheritFromScene... SceneTypes>
	int Application<RendererType, EventHandlerType, SceneTypes...>::run(void)
	{
		this->getLogger().info("Entering main loop");
		while (!_eventHandler.shouldQuit()) {
			try {
				_eventHandler.pollEvents();
				if (!_eventHandler.gotNewEvents()) {
					continue;
				}
				_renderer.clear();
				routine();
				_renderer.present();
			} catch (const std::exception &exception) {
				this->getLogger().error(std::string("Application error: ")
										+ exception.what());
				return EXIT_FAILURE;
			}
		}
		this->getLogger().info("Exiting main loop");
		return EXIT_SUCCESS;
	}
}	 // namespace guillaume