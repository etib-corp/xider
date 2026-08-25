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

#include <memory>

#include <guillaume/application.hpp>
#include <evan/Engine.hpp>

#include "xider/engine.hpp"
#include "scenes/home.hpp"
#include "scenes/settings.hpp"
#include "scenes/sound.hpp"

namespace xider
{
	/**
	 * @class XIDER
	 * @brief Main XIDER application class combining the GUI framework with Evan
	 * graphics engine.
	 *
	 * XIDER is the entry point for the application, managing the integration
	 * between Guillaume (GUI framework) and Evan (graphics engine). It provides
	 * a unified interface for application lifecycle management, rendering, and
	 * event handling across all platforms.
	 */
	class XIDER:
		public guillaume::Application<scenes::Home, scenes::Home,
									  scenes::Settings, scenes::Sound>
	{
		public:
		/**
		 * @brief Constructs a XIDER application with an Evan graphics engine.
		 * @param platform The Evan platform to use for rendering.
		 * @param ressourceProvider Shared pointer to the resource provider for
		 * loading assets. This allows the application to access resources such
		 * as textures, models, and shaders through a centralized resource
		 * management system.
		 *
		 * This constructor initializes the XIDER application and configures the
		 * renderer to use the provided Evan platform for all graphics
		 * operations.
		 */
		XIDER(std::shared_ptr<evan::IPlatform> platform,
			  std::shared_ptr<utility::RessourceProvider> ressourceProvider);

		/**
		 * @brief Default destructor.
		 */
		~XIDER(void);
	};

}	 // namespace xider
