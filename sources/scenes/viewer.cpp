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

#include <cstddef>
#include <string>
#include <vector>

#include "xider/scenes/home.hpp"
#include "xider/scenes/viewer.hpp"

namespace xider::scenes
{
	Viewer::Viewer(std::shared_ptr<utility::RessourceProvider> ressourceProvider,
				   guillaume::LocalStorage &localStorage,
				   guillaume::SessionStorage &sessionStorage)
		: DemoScene(ressourceProvider, localStorage, sessionStorage, "Viewer",
					"Walk the catalogue of models the application ships")
		, _preferences(localStorage)
	{
		getLogger().info() << "Viewer scene created";

		_textured = _preferences.areTexturesShown();

		_model = addModel("model", getModelCatalog()[_cursor], _modelFraction,
						  _modelColumnX, 0.0f);

		_modelName = addText("model_name", "", 22.0f, color::accent());

		auto buttons = addIconRow({
			{ .name	   = "previous_model",
			  .icon	   = "chevron_left",
			  .onClick = [this]() {
				  this->cycle(-1);
			  } },
			{ .name	   = "next_model",
			  .icon	   = "chevron_right",
			  .onClick = [this]() {
				  this->cycle(1);
			  } },
			{ .name	   = "toggle_texture",
			  .icon	   = "texture",
			  .onClick = [this]() {
				  this->toggleTexture();
			  } },
		});

		_textureButton = buttons[2];

		// The button shows whether the texture is applied, which is a state the
		// framework only draws on a toggle.
		_textureButton->setIsToggle(true);

		_hint = addText("camera_hint",
						"Right-drag to look around, WASD to fly",
						14.0f, color::muted());

		addButton("go_to_home", "Home", "home", [this]() {
			this->goToScene<Home>();
		});

		showCurrentModel();
	}

	Viewer::~Viewer(void)
	{
	}

	void Viewer::onEnter(void)
	{
		DemoScene::onEnter();

		_textured = _preferences.areTexturesShown();

		applyTexture();
		applyHints();
	}

	void Viewer::showCurrentModel(void)
	{
		const ModelAsset &asset = getModelCatalog()[_cursor];

		_modelName->setContent(asset.name);
		_model->setModelPath(asset.path);

		// The renderer keys the model it holds on the path and the texture of
		// the entity, and the layout places a model from the size of its mesh,
		// so both have to follow the asset that is now on screen.
		getHud().setModelAsset(_model, asset.size);

		_textureButton->setDisabled(!isTextureAvailable());

		applyTexture();
	}

	void Viewer::cycle(std::ptrdiff_t step)
	{
		const std::ptrdiff_t count =
			static_cast<std::ptrdiff_t>(getModelCatalog().size());

		// The catalogue is walked as a ring, in both directions.
		std::ptrdiff_t index = static_cast<std::ptrdiff_t>(_cursor) + step;

		index %= count;

		if (index < 0) {
			index += count;
		}

		_cursor = static_cast<std::size_t>(index);

		showCurrentModel();
	}

	void Viewer::toggleTexture(void)
	{
		_textured = !_textured;

		_preferences.setTexturesShown(_textured);

		applyTexture();
	}

	void Viewer::applyTexture(void)
	{
		const ModelAsset &asset = getModelCatalog()[_cursor];

		_model->setTexturePath(_textured ? asset.texture : std::string());
		_textureButton->setSelected(_textured);
	}

	void Viewer::applyHints(void)
	{
		_hint->setColor(_preferences.areHintsShown() ? color::muted()
													 : color::hidden());
	}

	bool Viewer::isTextureAvailable(void) const noexcept
	{
		return !getModelCatalog()[_cursor].texture.empty();
	}

}	 // namespace xider::scenes
