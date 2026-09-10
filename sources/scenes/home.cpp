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

#include "xider/scenes/home.hpp"

#include "xider/scenes/settings.hpp"
#include "xider/scenes/sound.hpp"
#include "xider/scenes/viewer.hpp"

namespace xider::scenes
{
	Home::Home(std::shared_ptr<utility::RessourceProvider> ressourceProvider,
			   guillaume::LocalStorage &localStorage,
			   guillaume::SessionStorage &sessionStorage)
		: DemoScene(ressourceProvider, localStorage, sessionStorage,
					"XIDER Demo Lab",
					"A tour of the engine, the interface toolkit and the "
					"assets they share")
		, _preferences(localStorage)
	{
		getLogger().info() << "Home scene created";

		addButton("go_to_viewer", "Viewer", "view_in_ar",
				  [this]() {
					  this->goToScene<Viewer>();
				  });

		addButton("go_to_sound", "Sound", "computer_sound",
				  [this]() {
					  this->goToScene<Sound>();
				  });

		addButton("go_to_settings", "Settings", "settings",
				  [this]() {
					  this->goToScene<Settings>();
				  });

		skipRow(0.04f);

		_hint = addText("camera_hint",
						"Right-drag to look around, WASD to fly",
						14.0f, color::muted());

		const auto &catalog = getModelCatalog();

		// Two models of the catalogue stand next to the console: the textured
		// one, which shows what the asset pipeline carries, and the largest
		// one, which shows a mesh at a scale of its own.
		_vikingRoom = addModel("viking_room", catalog[4], _modelFraction,
							   _vikingRoomX, 0.0f);
		_teddy		= addModel("teddy", catalog[1], _modelFraction, _teddyX,
							   0.0f);
	}

	Home::~Home(void)
	{
	}

	void Home::onEnter(void)
	{
		DemoScene::onEnter();

		_hint->setColor(_preferences.areHintsShown() ? color::muted()
													 : color::hidden());
	}

}	 // namespace xider::scenes
