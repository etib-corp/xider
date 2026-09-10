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

#include <functional>

#include <utility/graphic/view.hpp>

namespace xider
{
	/**
	 * @brief Callback run once per frame by the XIDER engine.
	 *
	 * The callback receives the view of the frame it runs in, so that scenes
	 * can react to the camera without reaching for the engine.
	 */
	using FrameUpdate = std::function<void(const utility::graphic::ViewF &)>;

	/**
	 * @class FrameUpdater
	 * @brief Process-wide registry holding the active scene's frame callback.
	 *
	 * The Guillaume application shell owns the frame loop and gives scenes no
	 * hook that runs between the placement of their entities and the ECS
	 * phases, while a scene has no way to reach the engine to register one.
	 * XIDER therefore keeps the callback in a single process-wide slot, next to
	 * the process-wide session storage the framework already relies on: the
	 * active scene installs its callback in Scene::onEnter and removes it in
	 * Scene::onExit, and Engine::update runs it once per frame.
	 *
	 * Only one scene is active at a time, so only one callback is stored.
	 */
	class FrameUpdater
	{
		public:
		/**
		 * @brief Install the callback run once per frame.
		 * @param callback Callback to run. An empty callback disables it.
		 */
		static void set(FrameUpdate callback);

		/**
		 * @brief Remove the installed callback.
		 */
		static void clear(void);

		/**
		 * @brief Run the installed callback.
		 *
		 * Does nothing when no callback is installed.
		 * @param view View of the current frame.
		 */
		static void run(const utility::graphic::ViewF &view);

		/**
		 * @brief Check whether a callback is installed.
		 * @return True when a callback is installed.
		 */
		static bool isSet(void);

		private:
		/**
		 * @brief Access the single process-wide callback.
		 * @return Reference to the stored callback.
		 */
		static FrameUpdate &callback(void);
	};

}	 // namespace xider
