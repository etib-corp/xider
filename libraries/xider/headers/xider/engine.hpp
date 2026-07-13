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

#include <guillaume/engine.hpp>

#include <evan/IPlatform.hpp>
#include <evan/Engine.hpp>

namespace xider
{
	/**
	 * @class Engine
	 * @brief XIDER's engine combining rendering and event handling backed by
	 * the Evan graphics engine.
	 *
	 * This class provides a unified interface that wraps the Evan graphics
	 * engine, which uses Vulkan for high-performance graphics, and integrates
	 * platform event polling. It provides a consistent API across all XIDER
	 * platforms.
	 */
	class Engine: public guillaume::Engine
	{
		private:
		std::unique_ptr<evan::Engine> _evanEngine;	  ///< Evan graphics engine
		public:
		/**
		 * @brief Construct a new Engine
		 *
		 * @param engine Shared pointer to the Evan engine instance.
		 */
		Engine(std::unique_ptr<evan::Engine> engine);

		/**
		 * @brief Destructor for Engine.
		 * Cleans up any resources associated with the Evan engine.
		 */
		~Engine(void) override;

		/**
		 * @brief Clears the current render target.
		 * Prepares the rendering surface for the next frame.
		 */
		void clear(void) override;

		/**
		 * @brief Presents the rendered frame to the display.
		 * Executes the Evan renderer's frame drawing pipeline.
		 */
		void present(void) override;

		/**
		 * @brief Adds a mesh to the renderer.
		 * @param mesh The mesh to add to the renderer.
		 * @param materialName The name of the material to use for rendering the
		 * mesh. This allows for flexibility in specifying the visual properties
		 * of the mesh during rendering, such as textures, colors, and shader
		 * parameters.
		 * @return A unique identifier for the added mesh.
		 */
		size_t addMesh(
			const utility::graphic::Mesh &mesh,
			const std::string &materialName = "default_material") override;

		/**
		 * @brief Returns the mirrored view state.
		 * @return The current view.
		 */
		utility::graphic::ViewF getView(void) const override;

		bool removeObject(size_t objectID) override;

		/**
		 * @brief Draws text at the specified position.
		 * @param text The text to render.
		 * @return A unique identifier for the added text.
		 */
		size_t addText(utility::graphic::Text text) override;

		/**
		 * @brief Adds a model to the renderer.
		 * @param model A shared pointer to the model to add.
		 * @return A unique identifier for the added model.
		 */
		size_t
			addModel(std::shared_ptr<utility::graphic::Model> model) override;

		/**
		 * @brief Measures text dimensions.
		 * @param text The text to measure.
		 * @return A SizeF containing the width and height.
		 */
		utility::graphic::SizeF
			measureText(const utility::graphic::Text &text) const override;

		/**
		 * @brief Add a scene to the renderer.
		 * @param sceneIndex The index of the scene to add.
		 */
		void addScene(size_t sceneIndex) override;

		/**
		 * @brief Poll for OS/input events and dispatch them.
		 */
		void pollEvents(void) override;

		/**
		 * @brief Update the engine state.
		 *
		 * Delegates to the Evan engine's update method to handle
		 * input processing and logic updates.
		 */
		void update(void) override;

		/**
		 * @brief Check if viewport input capture is enabled.
		 * @return True when viewport input is captured for camera movement.
		 */
		bool shouldCaptureViewportInput(void) const;

		/**
		 * @brief Set the viewport input capture state.
		 * @param capture True to enable viewport input capture, false to
		 * disable. When disabled, input is routed to UI instead of camera
		 * controls.
		 */
		void setShouldCaptureViewportInput(bool capture);
	};

}	 // namespace xider
