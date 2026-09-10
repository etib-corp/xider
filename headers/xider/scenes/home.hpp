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

#include <guillaume/entities/model.hpp>
#include <guillaume/entities/text.hpp>

#include "xider/preferences.hpp"
#include "xider/scenes/demo_scene.hpp"

namespace xider::scenes
{
	/**
	 * @brief Home application scene.
	 *
	 * Welcomes the user with a showcase of the models the demo ships, and
	 * leads to the other scenes of the application.
	 */
	class Home: public DemoScene
	{
		public:
		/**
		 * @brief Construct a new Home scene.
		 * @param ressourceProvider Shared pointer to the resource provider.
		 * @param localStorage Reference to persistent local storage.
		 * @param sessionStorage Reference to per-session storage.
		 */
		Home(std::shared_ptr<utility::RessourceProvider> ressourceProvider,
			 guillaume::LocalStorage &localStorage,
			 guillaume::SessionStorage &sessionStorage);

		/**
		 * @brief Destroy the Home scene.
		 */
		~Home(void) override;

		public:
		/**
		 * @brief Show the hint line the preferences ask for.
		 */
		void onEnter(void) override;

		private:
		static constexpr float _modelFraction =
			0.35f;	  ///< Share of the visible height a showcase model
				  ///< covers.
		static constexpr float _vikingRoomX =
			0.16f;	  ///< Horizontal center of the textured model.
		static constexpr float _teddyX =
			0.38f;	  ///< Horizontal center of the teddy bear.

		private:
		Preferences _preferences;	///< Settings of the demo.
		std::shared_ptr<guillaume::entities::Text>
			_hint {};	///< Line explaining how to fly the camera.
		std::shared_ptr<guillaume::entities::Model>
			_vikingRoom {};	   ///< Model showing off a texture.
		std::shared_ptr<guillaume::entities::Model>
			_teddy {};	///< Largest model of the catalogue.
	};

}	 // namespace xider::scenes
