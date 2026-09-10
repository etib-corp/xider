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
#include <memory>
#include <string>
#include <utility>
#include <vector>

#include <guillaume/scene.hpp>

#include <guillaume/entities/button.hpp>
#include <guillaume/entities/icon_button.hpp>
#include <guillaume/entities/model.hpp>
#include <guillaume/entities/panel.hpp>
#include <guillaume/entities/text.hpp>

#include <utility/graphic/color.hpp>
#include <utility/graphic/size.hpp>

#include "xider/hud.hpp"

namespace xider::scenes
{
	/**
	 * @brief A model shipped with the application.
	 *
	 * The size of the mesh is carried along with the asset: the model renderer
	 * reloads a model from its file whenever its pose changes, so a model has
	 * to be placed correctly on the very first frame it is shown, before the
	 * framework had a chance to measure it.
	 */
	struct ModelAsset {
		std::string name;			   ///< Name shown to the user.
		std::string path;			   ///< Path of the model asset.
		std::string texture;		   ///< Path of its texture, empty when the
									   ///< model has none.
		utility::graphic::SizeF size;	///< Size of the mesh, in world units.
	};

	/**
	 * @brief Get the models the demo displays.
	 * @return Models of the demo, in presentation order.
	 */
	const std::vector<ModelAsset> &getModelCatalog(void);

	/**
	 * @brief Colors used by the demo interface.
	 */
	namespace color
	{
		/**
		 * @brief Color of the backdrop card.
		 * @return Card color.
		 */
		inline utility::graphic::Color32Bit card(void)
		{
			return utility::graphic::Color32Bit(18, 20, 28, 220);
		}

		/**
		 * @brief Color of a title.
		 * @return Title color.
		 */
		inline utility::graphic::Color32Bit title(void)
		{
			return utility::graphic::Color32Bit(255, 255, 255, 255);
		}

		/**
		 * @brief Color of a subtitle and of any secondary text.
		 * @return Secondary text color.
		 */
		inline utility::graphic::Color32Bit muted(void)
		{
			return utility::graphic::Color32Bit(150, 158, 176, 255);
		}

		/**
		 * @brief Color of a highlighted value.
		 * @return Highlight color.
		 */
		inline utility::graphic::Color32Bit accent(void)
		{
			return utility::graphic::Color32Bit(140, 200, 255, 255);
		}

		/**
		 * @brief Color of a line the preferences ask to keep out of the way.
		 * @return Fully transparent color.
		 */
		inline utility::graphic::Color32Bit hidden(void)
		{
			return utility::graphic::Color32Bit(0, 0, 0, 0);
		}
	}	 // namespace color

	/**
	 * @class DemoScene
	 * @brief Base class of the demo scenes.
	 *
	 * Builds the interface every scene shares: a console card holding a title,
	 * a subtitle and a column of rows, and the anchor the framework places in
	 * front of the viewer. A scene adds its own rows from its constructor and
	 * registers models with the layout, which keeps everything in place once
	 * per frame through the frame updater.
	 *
	 * The framework places the direct entities of a scene in front of the
	 * camera and never moves their children, so the scene keeps a single
	 * direct entity, an invisible panel used as the anchor, and hangs the
	 * whole interface from it. Every entity of the interface is therefore
	 * built with a builder bound to the anchor and not with the builder
	 * manager of the scene, which would register them as direct entities too
	 * and have the framework spread them across the view.
	 */
	class DemoScene: public guillaume::Scene
	{
		protected:
		/**
		 * @brief An icon button of a console row.
		 */
		struct IconAction {
			std::string name;	///< Name of the button entity.
			std::string icon;	///< Name of the icon glyph to display.
			std::function<void(void)> onClick;	  ///< Handler run on click.
		};

		public:
		/**
		 * @brief Destroy the scene.
		 */
		~DemoScene(void) override;

		protected:
		/**
		 * @brief Construct a scene with its console.
		 * @param ressourceProvider Shared pointer to the resource provider.
		 * @param localStorage Reference to persistent local storage.
		 * @param sessionStorage Reference to per-session storage.
		 * @param title Title shown at the top of the console.
		 * @param subtitle Secondary line shown under the title.
		 */
		DemoScene(
			std::shared_ptr<utility::RessourceProvider> ressourceProvider,
			guillaume::LocalStorage &localStorage,
			guillaume::SessionStorage &sessionStorage, const std::string &title,
			const std::string &subtitle);

		/**
		 * @brief Get the entity the whole interface hangs from.
		 * @return Anchor of the scene.
		 */
		std::shared_ptr<guillaume::ecs::Entity> getAnchor(void) const noexcept;

		/**
		 * @brief Get the layout keeping the interface in front of the viewer.
		 *
		 * A scene takes it to change what it shows while it runs, for instance
		 * the size of a model it swaps for another.
		 * @return Layout of the scene.
		 */
		Hud &getHud(void) noexcept;

		/**
		 * @brief Add a line of text to the console, at the next row.
		 * @param name Name of the text entity.
		 * @param content Text to display.
		 * @param fontSize Height of the font, in world units.
		 * @param textColor Color of the text.
		 * @return The created text.
		 */
		std::shared_ptr<guillaume::entities::Text>
			addText(const std::string &name, const std::string &content,
					float fontSize,
					const utility::graphic::Color32Bit &textColor);

		/**
		 * @brief Add a button to the console, at the next row.
		 * @param name Name of the button entity.
		 * @param label Label of the button.
		 * @param icon Name of the icon glyph shown before the label.
		 * @param onClick Handler run when the button is clicked.
		 * @param size Size of the button.
		 * @param isToggle Whether the button stays pressed once clicked.
		 * @return The created button.
		 */
		std::shared_ptr<guillaume::entities::Button> addButton(
			const std::string &name, const std::string &label,
			const std::string &icon, std::function<void(void)> onClick,
			guillaume::entities::Button::Size size =
				guillaume::entities::Button::Size::Medium,
			bool isToggle = false);

		/**
		 * @brief Add a row of icon buttons to the console.
		 *
		 * The buttons are laid out side by side, centered on the column of the
		 * console at the next row. A button carrying a label builds a text child
		 * for it, empty or not, so a row of glyphs alone is made of the icon
		 * buttons of the toolkit.
		 * @param actions Buttons to create, from left to right.
		 * @return The created buttons, in the same order.
		 */
		std::vector<std::shared_ptr<guillaume::entities::IconButton>>
			addIconRow(const std::vector<IconAction> &actions);

		/**
		 * @brief Add a model shown next to the console.
		 * @param name Name of the model entity.
		 * @param asset Model to display.
		 * @param sizeFraction Share of the visible height to cover.
		 * @param offsetX Center across the view, as a fraction of the visible
		 * width.
		 * @param offsetY Center up the view, as a fraction of the visible
		 * height.
		 * @return The created model.
		 */
		std::shared_ptr<guillaume::entities::Model>
			addModel(const std::string &name, const ModelAsset &asset,
					 float sizeFraction, float offsetX, float offsetY);

		/**
		 * @brief Give the row cursor a gap.
		 * @param fraction Height of the gap, as a fraction of the visible
		 * height.
		 */
		void skipRow(float fraction);

		/**
		 * @brief Take the vertical position of the next row of the console.
		 * @return Center of the row, as a fraction of the visible height.
		 */
		float takeRow(void);

		/**
		 * @brief Get the horizontal center of the console column.
		 * @return Center of the column, as a fraction of the visible width.
		 */
		float getColumn(void) const noexcept;

		public:
		/**
		 * @brief Install the layout of the scene for the frames it is active.
		 */
		void onEnter(void) override;

		/**
		 * @brief Release the layout of the scene.
		 */
		void onExit(void) override;

		private:
		/**
		 * @brief Create a text belonging to the interface.
		 * @param content Text to display.
		 * @param fontSize Height of the font, in world units.
		 * @param textColor Color of the text.
		 * @return The created text, kept in place by nothing yet.
		 */
		std::shared_ptr<guillaume::entities::Text>
			makeText(const std::string &content, float fontSize,
					 const utility::graphic::Color32Bit &textColor);

		private:
		static constexpr float _columnX =
			-0.19f;	  ///< Horizontal center of the console.
		static constexpr float _cardWidth =
			0.34f;	  ///< Width of the console card.
		static constexpr float _cardHeight =
			0.78f;	  ///< Height of the console card.
		static constexpr float _cardRadius =
			18.0f;	  ///< Corner radius of the console card, in world units.
		static constexpr float _titleY =
			0.30f;	  ///< Vertical center of the title.
		static constexpr float _titleFontSize =
			30.0f;	  ///< Font size of the title, in world units.
		static constexpr float _subtitleY =
			0.245f;	  ///< Vertical center of the subtitle.
		static constexpr float _subtitleFontSize =
			15.0f;	  ///< Font size of the subtitle, in world units.
		static constexpr float _firstRowY =
			0.17f;	  ///< Vertical center of the first row.
		static constexpr float _rowStep =
			0.105f;	  ///< Vertical distance between two rows.

		private:
		std::shared_ptr<guillaume::entities::Panel>
			_anchor {};	  ///< Entity the whole interface hangs from.
		Hud _hud;	///< Layout keeping the interface in front of the viewer.
		float _rowY { _firstRowY };	   ///< Vertical position of the next row.
	};

}	 // namespace xider::scenes
