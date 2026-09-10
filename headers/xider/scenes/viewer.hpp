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

#include <cstddef>
#include <memory>
#include <vector>

#include <guillaume/entities/icon_button.hpp>
#include <guillaume/entities/model.hpp>
#include <guillaume/entities/text.hpp>

#include "xider/preferences.hpp"
#include "xider/scenes/demo_scene.hpp"

namespace xider::scenes
{
	/**
	 * @brief Model viewer scene.
	 *
	 * Shows one model of the catalogue at a time and lets the user walk
	 * through the catalogue, apply or drop its texture, and fly around it with
	 * the camera of the engine.
	 */
	class Viewer: public DemoScene
	{
		public:
		/**
		 * @brief Construct a new Viewer scene.
		 * @param ressourceProvider Shared pointer to the resource provider.
		 * @param localStorage Reference to persistent local storage.
		 * @param sessionStorage Reference to per-session storage.
		 */
		Viewer(std::shared_ptr<utility::RessourceProvider> ressourceProvider,
			   guillaume::LocalStorage &localStorage,
			   guillaume::SessionStorage &sessionStorage);

		/**
		 * @brief Destroy the Viewer scene.
		 */
		~Viewer(void) override;

		public:
		/**
		 * @brief Show the first model, as the preferences ask for.
		 */
		void onEnter(void) override;

		private:
		/**
		 * @brief Show the model the cursor points at.
		 */
		void showCurrentModel(void);

		/**
		 * @brief Move the cursor to the next model of the catalogue.
		 * @param step Offset in the catalogue to move the cursor by.
		 */
		void cycle(std::ptrdiff_t step);

		/**
		 * @brief Flip the texture of the model on screen, and remember it.
		 */
		void toggleTexture(void);

		/**
		 * @brief Check whether the model on screen has a texture to show.
		 * @return True when the asset carries a texture.
		 */
		bool isTextureAvailable(void) const noexcept;

		/**
		 * @brief Apply the texture choice to the model on screen.
		 */
		void applyTexture(void);

		/**
		 * @brief Show or hide the hint line, as the preferences ask for.
		 */
		void applyHints(void);

		private:
		static constexpr float _modelFraction =
			0.40f;	  ///< Share of the visible height the model covers.
		static constexpr float _modelColumnX =
			0.26f;	  ///< Horizontal center of the model, as a fraction of
					  ///< the visible width.

		private:
		Preferences _preferences;	///< Settings of the demo.
		std::size_t _cursor { 0 };	///< Index of the model on screen.
		bool _textured { true };	///< True when the texture is applied.
		std::shared_ptr<guillaume::entities::Model>
			_model {};	///< Model on screen.
		std::shared_ptr<guillaume::entities::Text>
			_modelName {};	  ///< Name of the model on screen.
		std::shared_ptr<guillaume::entities::Text>
			_hint {};	///< Line explaining how to fly the camera.
		std::shared_ptr<guillaume::entities::IconButton>
			_textureButton {};	  ///< Toggle applying the texture.
	};

}	 // namespace xider::scenes
