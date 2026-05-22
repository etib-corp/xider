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

#include <map>
#include <memory>
#include <stdexcept>
#include <string>
#include <tuple>
#include <typeinfo>
#include <utility>

#include <utility/logging/loggable.hpp>
#include <utility/logging/standard_logger.hpp>

#include <utility/demangle.hpp>

#include "guillaume/ecs/system_phase.hpp"
#include "guillaume/ecs/system_registry.hpp"

#include "guillaume/metadata.hpp"
#include "guillaume/renderer.hpp"
#include "guillaume/scene.hpp"
#include "guillaume/scene_manager.hpp"
#include "guillaume/scene_manager_filler.hpp"

#include "guillaume/event/event_bus.hpp"
#include "guillaume/event/event_handler.hpp"

#include "guillaume/systems/glyph_render.hpp"
#include "guillaume/systems/hand_button.hpp"
#include "guillaume/systems/hand_motion.hpp"
#include "guillaume/systems/hand_pinch.hpp"
#include "guillaume/systems/hand_poke.hpp"
#include "guillaume/systems/hand_squeeze.hpp"
#include "guillaume/systems/hand_thumb_rest.hpp"
#include "guillaume/systems/hand_thumb_stick.hpp"
#include "guillaume/systems/hand_trigger.hpp"
#include "guillaume/systems/mouse_button.hpp"
#include "guillaume/systems/mouse_motion.hpp"
#include "guillaume/systems/keyboard_control.hpp"
#include "guillaume/systems/measure_text.hpp"
#include "guillaume/systems/rectangle_render.hpp"
#include "guillaume/systems/text_input.hpp"
#include "guillaume/systems/text_render.hpp"

namespace guillaume
{

	/**
	 * @brief Application base class.
	 *
	 * @tparam RendererType The type of the renderer used by the application.
	 * @tparam EventHandlerType The type of the event handler used by the
	 * application.
	 *
	 * @code
	 * class MyRenderer : public Renderer { ... };
	 * class MyEventHandler : public event::EventHandler { ... };
	 * Application<MyRenderer, MyEventHandler> app;
	 * return app.run();
	 * @endcode
	 *
	 * @see ecs::SystemRegistry
	 * @see event::EventBus
	 * @see event::EventHandler
	 * @see Renderer
	 */
	template<InheritFromRenderer RendererType,
			 event::InheritFromEventHandler EventHandlerType,
			 InheritFromScene... SceneTypes>
	class Application:
		protected utility::logging::Loggable<
			Application<RendererType, EventHandlerType, SceneTypes...>,
			utility::logging::StandardLogger>
	{
		private:
		RendererType _renderer;			   ///< Main application renderer
		EventHandlerType _eventHandler;	   ///< Application event handler
		std::unique_ptr<SceneManager>
			_sceneManager;			  ///< Manager for application scenes
		event::EventBus _eventBus;	  ///< Event bus dispatching to systems
		ecs::SystemRegistry _systemRegistry;	///< Shared system registry
		ecs::SystemPhaseList _systemPhases;		///< Ordered list of phases and
												///< traversal strategies

		template<typename PhaseType> void runPhase(PhaseType &phaseDefinition);

		/**
		 * @brief Register core systems used by the application.
		 */
		void registerCoreSystems(void);

		public:
		/**
		 * @brief Default constructor
		 */
		Application(void);
		/**
		 * @brief Default destructor
		 */
		virtual ~Application(void);

		/**
		 * @brief Get a reference to the renderer.
		 * @return Reference to the application renderer.
		 */
		RendererType &getRenderer(void);

		/**
		 * @brief Get a const reference to the renderer.
		 * @return Const reference to the application renderer.
		 */
		const RendererType &getRenderer(void) const;

		/**
		 * @brief Get a reference to the event handler.
		 * @return Reference to the application event handler.
		 */
		EventHandlerType &getEventHandler(void);

		/**
		 * @brief Get a const reference to the event handler.
		 * @return Const reference to the application event handler.
		 */
		const EventHandlerType &getEventHandler(void) const;

		/**
		 * @brief Run one system update pass for the active scene.
		 */
		void routine(void);

		/**
		 * @brief Poll events from the event handler.
		 */
		void pollEvents(void);

		/**
		 * @brief Clear the renderer.
		 */
		void clear(void);

		/**
		 * @brief Present the rendered frame.
		 */
		void present(void);

		/**
		 * @brief Check if the event handler has new events.
		 * @return True if new events were received, false otherwise.
		 */
		bool gotNewEvents(void) const;

		/**
		 * @brief Check if the application should quit (using event handler).
		 * @return True if the application should quit, false otherwise.
		 */
		bool shouldQuit(void) const;

		/**
		 * @brief Run the application main loop using the event handler's
		 * shouldQuit.
		 * @return Exit code.
		 */
		int run(void);
	};

}	 // namespace guillaume

// Include the implementation of the Application template class
#include "guillaume/application.tpp"