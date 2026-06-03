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

#include <guillaume/renderer.hpp>

#include <evan/IPlatform.hpp>
#include <evan/Engine.hpp>

namespace xider
{
	/**
	 * @class Renderer
	 * @brief XIDER's rendering system backed by the Evan graphics engine.
	 *
	 * This class provides a unified rendering interface that wraps the Evan
	 * graphics engine, which uses Vulkan for high-performance graphics. It
	 * integrates with the Guillaume framework to provide a consistent API
	 * across all XIDER platforms.
	 */
	class Renderer: public guillaume::Renderer
	{
		private:
		std::shared_ptr<evan::Engine> _engine;	  ///< Evan graphics engine
		public:
		/**
		 * @brief Default constructor for template compatibility.
		 * Initialize with a null engine; must call setEngine() before
		 * rendering.
		 *
		 * @param engine Shared pointer to the Evan engine instance.
		 */
		Renderer(std::shared_ptr<evan::Engine> engine);

		/**
		 * @brief Destructor for Renderer.
		 * Cleans up any resources associated with the Evan engine.
		 */
		~Renderer(void) override;

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
		 * @brief Draws a set of vertices.
		 * @param vertices The vertices to draw.
		 */
		void drawVertices(
			const std::vector<utility::graphic::VertexF> &vertices) override;

		/**
		 * @brief Draws text at the specified position.
		 * @param text The text to render.
		 * @param pose The position and orientation for the text.
		 */
		void drawText(const utility::graphic::Text &text,
					  const utility::graphic::PoseF &pose) override;

		/**
		 * @brief Measures text dimensions.
		 * @param text The text to measure.
		 * @return A Vector2F containing the width and height.
		 */
		utility::math::Vector2F
			measureText(const utility::graphic::Text &text) override;

		/**
		 * @brief Gets the current viewport size.
		 * @return ViewportSize containing width and height.
		 */
		ViewportSize getViewportSize(void) const override;
	};

}	 // namespace xider