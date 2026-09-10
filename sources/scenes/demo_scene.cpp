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

#include <memory>
#include <utility>
#include <vector>

#include <guillaume/components/glyph.hpp>
#include <guillaume/ecs/entity.hpp>

#include <guillaume/entities/button.hpp>
#include <guillaume/entities/icon_button.hpp>
#include <guillaume/entities/model.hpp>
#include <guillaume/entities/panel.hpp>
#include <guillaume/entities/text.hpp>

#include <utility/graphic/color.hpp>
#include <utility/graphic/pose.hpp>

#include "xider/frame_updater.hpp"
#include "xider/scenes/demo_scene.hpp"

namespace xider::scenes
{
	namespace
	{
		/// Name the framework knows the anchor of a demo scene by.
		constexpr const char *anchorName = "anchor";
	}	 // namespace

	const std::vector<ModelAsset> &getModelCatalog(void)
	{
		static const std::vector<ModelAsset> catalog {
			{ .name		= "Teapot",
			  .path		= "models/teapot.obj",
			  .texture	= "",
			  .size		= utility::graphic::SizeF(6.43f, 3.15f) },
			{ .name		= "Teddy",
			  .path		= "models/teddy.obj",
			  .texture	= "",
			  .size		= utility::graphic::SizeF(34.15f, 41.79f) },
			{ .name		= "Cow",
			  .path		= "models/cow-nonormals.obj",
			  .texture	= "",
			  .size		= utility::graphic::SizeF(10.44f, 6.40f) },
			{ .name		= "Pumpkin",
			  .path		= "models/pumpkin_tall_10k.obj",
			  .texture	= "",
			  .size		= utility::graphic::SizeF(79.14f, 76.94f) },
			{ .name		= "Viking room",
			  .path		= "models/viking_room.obj",
			  .texture	= "textures/viking_room.png",
			  .size		= utility::graphic::SizeF(1.33f, 1.46f) },
		};

		return catalog;
	}

	DemoScene::DemoScene(
		std::shared_ptr<utility::RessourceProvider> ressourceProvider,
		guillaume::LocalStorage &localStorage,
		guillaume::SessionStorage &sessionStorage, const std::string &title,
		const std::string &subtitle)
		: guillaume::Scene(ressourceProvider, localStorage, sessionStorage)
	{
		using utility::graphic::Color32Bit;
		using utility::graphic::PoseF;

		auto &panelBuilder =
			getBuilderManager().getBuilder<guillaume::entities::Panel::Builder>();
		auto &panelDirector =
			getDirectorManager()
				.getDirector<guillaume::entities::Panel::Director>();

		// The framework places the direct entities of a scene in front of the
		// camera, and moves their children never. A single invisible panel is
		// therefore used as the anchor of the interface, which the layout
		// keeps assembled around it.
		_anchor = panelDirector.makeColorPanel(
			panelBuilder, nullptr, PoseF(), Color32Bit(0, 0, 0, 0), {});

		_anchor->setBorderRadius(0.0f);
		addRootEntity(anchorName, _anchor);
		_hud.setAnchorEntity(_anchor);

		guillaume::entities::Panel::Builder childPanelBuilder(
			getComponentRegistry(), *_anchor);
		guillaume::entities::Panel::Director childPanelDirector;

		auto card = childPanelDirector.makeColorPanel(childPanelBuilder, _anchor,
													  PoseF(), color::card(),
													  {});

		card->setBorderRadius(_cardRadius);
		_hud.addPanel(card, _columnX, 0.0f, _cardWidth, _cardHeight);

		auto titleText =
			makeText(title, _titleFontSize, color::title());

		_hud.addElement(titleText, HudAnchor::Text, _columnX, _titleY);

		auto subtitleText =
			makeText(subtitle, _subtitleFontSize, color::muted());

		_hud.addElement(subtitleText, HudAnchor::Text, _columnX, _subtitleY);
	}

	DemoScene::~DemoScene(void)
	{
	}

	std::shared_ptr<guillaume::ecs::Entity> DemoScene::getAnchor(void) const
		noexcept
	{
		return _anchor;
	}

	Hud &DemoScene::getHud(void) noexcept
	{
		return _hud;
	}

	std::shared_ptr<guillaume::entities::Text> DemoScene::makeText(
		const std::string &content, float fontSize,
		const utility::graphic::Color32Bit &textColor)
	{
		guillaume::entities::Text::Builder builder(getComponentRegistry(),
												   *_anchor);
		guillaume::entities::Text::Director director;

		return director.makeText(builder, _anchor, content, fontSize, textColor);
	}

	std::shared_ptr<guillaume::entities::Text> DemoScene::addText(
		const std::string &name, const std::string &content, float fontSize,
		const utility::graphic::Color32Bit &textColor)
	{
		auto text = makeText(content, fontSize, textColor);

		_hud.addElement(text, HudAnchor::Text, getColumn(), takeRow());

		getLogger().debug() << "Console of " << name << " holds the text \""
							<< content << "\"";

		return text;
	}

	std::shared_ptr<guillaume::entities::Button> DemoScene::addButton(
		const std::string &name, const std::string &label, const std::string &icon,
		std::function<void(void)> onClick, guillaume::entities::Button::Size size,
		bool isToggle)
	{
		guillaume::entities::Button::Builder builder(getComponentRegistry(),
													 *_anchor);
		guillaume::entities::Button::Director director;

		auto button = director.makeIconButton(
			builder, _anchor, label, icon,
			guillaume::components::Glyph::Style::Outlined, std::move(onClick),
			guillaume::entities::Button::Color::Filled,
			guillaume::entities::Button::Shape::Round, size, false);

		button->setIsToggle(isToggle);

		_hud.addElement(button, HudAnchor::Rectangle, getColumn(), takeRow());

		getLogger().debug() << "Console of " << name << " holds the button \""
							<< label << "\"";

		return button;
	}

	std::vector<std::shared_ptr<guillaume::entities::IconButton>>
		DemoScene::addIconRow(const std::vector<IconAction> &actions)
	{
		guillaume::entities::IconButton::Director director;

		std::vector<std::shared_ptr<guillaume::entities::IconButton>> buttons;
		std::vector<std::shared_ptr<guillaume::ecs::Entity>> row;

		buttons.reserve(actions.size());
		row.reserve(actions.size());

		for (const auto &action: actions) {
			// A builder remembers what it is building and only resets when it
			// is created, so each button of the row gets a builder of its own.
			guillaume::entities::IconButton::Builder builder(
				getComponentRegistry(), *_anchor);

			// A button showing a glyph and nothing else has no label to be
			// known by, so its name is carried as an accessibility label.
			builder.withAccessibilityLabel(action.name);

			buttons.push_back(director.makeIconButton(
				builder, _anchor, action.icon, action.onClick,
				guillaume::entities::IconButton::Variant::Tonal,
				guillaume::entities::IconButton::Size::Medium));
			row.push_back(buttons.back());

			getLogger().debug() << "Console holds the icon button \""
								<< action.name << "\"";
		}

		if (!row.empty()) {
			_hud.addRow(row, getColumn(), takeRow());
		}

		return buttons;
	}

	std::shared_ptr<guillaume::entities::Model> DemoScene::addModel(
		const std::string &name, const ModelAsset &asset, float sizeFraction,
		float offsetX, float offsetY)
	{
		guillaume::entities::Model::Builder builder(getComponentRegistry(),
													*_anchor);
		guillaume::entities::Model::Director director;

		auto model =
			director.makeModel(builder, _anchor, asset.path, asset.texture);

		_hud.addModel(model, asset.size, sizeFraction, offsetX, offsetY);

		getLogger().debug() << "Display of " << name << " holds the model \""
							<< asset.name << "\"";

		return model;
	}

	void DemoScene::skipRow(float fraction)
	{
		_rowY -= fraction;
	}

	float DemoScene::takeRow(void)
	{
		const float row = _rowY;

		_rowY -= _rowStep;

		return row;
	}

	float DemoScene::getColumn(void) const noexcept
	{
		return _columnX;
	}

	void DemoScene::onEnter(void)
	{
		guillaume::Scene::onEnter();

		FrameUpdater::set([this](const utility::graphic::ViewF &view) {
			_hud.settle(getComponentRegistry(), view);
		});
	}

	void DemoScene::onExit(void)
	{
		FrameUpdater::clear();

		guillaume::Scene::onExit();
	}

}	 // namespace xider::scenes
