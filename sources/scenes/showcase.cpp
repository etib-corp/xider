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

#include <string>
#include <utility>
#include <vector>

#include <guillaume/entities/app_bar.hpp>
#include <guillaume/entities/badge.hpp>
#include <guillaume/entities/button.hpp>
#include <guillaume/entities/card.hpp>
#include <guillaume/entities/checkbox.hpp>
#include <guillaume/entities/chip.hpp>
#include <guillaume/entities/dialog.hpp>
#include <guillaume/entities/divider.hpp>
#include <guillaume/entities/layout.hpp>
#include <guillaume/entities/list.hpp>
#include <guillaume/entities/navigation_bar.hpp>
#include <guillaume/entities/overlay_helpers.hpp>
#include <guillaume/entities/progress_indicator.hpp>
#include <guillaume/entities/radio_button.hpp>
#include <guillaume/entities/slider.hpp>
#include <guillaume/entities/snackbar.hpp>
#include <guillaume/entities/switch.hpp>
#include <guillaume/entities/tabs.hpp>
#include <guillaume/entities/text.hpp>
#include <guillaume/entities/text_field.hpp>
#include <guillaume/systems/layout.hpp>

#include "xider/scenes/showcase.hpp"

namespace xider::scenes
{
	namespace
	{
		using guillaume::ecs::Entity;
		using guillaume::entities::Layout;
		using guillaume::entities::Text;

		using ChildList = std::vector<std::shared_ptr<Entity>>;

		/**
		 * @brief Build a plain heading text entity.
		 * @param textBuilder The shared text builder.
		 * @param textDirector The shared text director.
		 * @param parent The parent entity.
		 * @param content The heading content.
		 * @param size The font size.
		 * @return The newly created text entity.
		 */
		std::shared_ptr<Text> heading(Text::Builder &textBuilder,
									  Text::Director &textDirector,
									  std::shared_ptr<Entity> parent,
									  const std::string &content, float size)
		{
			return textDirector.makeText(
				textBuilder, std::move(parent), content, size,
				utility::graphic::Color32Bit(255, 255, 255, 255));
		}

		/**
		 * @brief Build a layout entity arranging children on one axis.
		 * @param layoutBuilder The shared layout builder.
		 * @param layoutDirector The shared layout director.
		 * @param parent The parent entity.
		 * @param children The children to arrange.
		 * @param axis The arrangement axis.
		 * @param spacing The spacing between children.
		 * @return The newly created layout entity.
		 */
		std::shared_ptr<Layout> stack(Layout::Builder &layoutBuilder,
									  Layout::Director &layoutDirector,
									  std::shared_ptr<Entity> parent,
									  const ChildList &children,
									  guillaume::components::Layout::Axis axis,
									  float spacing)
		{
			auto layout = layoutDirector.makeColorLayout(
				layoutBuilder, std::move(parent), utility::graphic::PoseF(),
				utility::graphic::Color32Bit(0, 0, 0, 0), children);

			layout->setAxis(axis).setSpacing(spacing).setPadding(0.0f);

			return layout;
		}

		/**
		 * @brief Build a horizontal row of children.
		 * @param layoutBuilder The shared layout builder.
		 * @param layoutDirector The shared layout director.
		 * @param parent The parent entity.
		 * @param children The children to arrange.
		 * @param spacing The spacing between children.
		 * @return The newly created layout entity.
		 */
		std::shared_ptr<Layout> row(Layout::Builder &layoutBuilder,
									Layout::Director &layoutDirector,
									std::shared_ptr<Entity> parent,
									const ChildList &children, float spacing)
		{
			return stack(
				layoutBuilder, layoutDirector, std::move(parent), children,
				guillaume::components::Layout::Axis::Horizontal, spacing);
		}

		/**
		 * @brief Build a vertical column of children.
		 * @param layoutBuilder The shared layout builder.
		 * @param layoutDirector The shared layout director.
		 * @param parent The parent entity.
		 * @param children The children to arrange.
		 * @param spacing The spacing between children.
		 * @return The newly created layout entity.
		 */
		std::shared_ptr<Layout> column(Layout::Builder &layoutBuilder,
									   Layout::Director &layoutDirector,
									   std::shared_ptr<Entity> parent,
									   const ChildList &children, float spacing)
		{
			return stack(
				layoutBuilder, layoutDirector, std::move(parent), children,
				guillaume::components::Layout::Axis::Vertical, spacing);
		}
	}	 // namespace

	Showcase::Showcase(
		std::shared_ptr<utility::RessourceProvider> ressourceProvider,
		guillaume::LocalStorage &localStorage,
		guillaume::SessionStorage &sessionStorage)
		: guillaume::Scene(ressourceProvider, localStorage, sessionStorage)
	{
		using namespace guillaume::entities;

		getLogger().info() << "Showcase scene created";

		auto &textBuilder  = getBuilderManager().getBuilder<Text::Builder>();
		auto &textDirector = getDirectorManager().getDirector<Text::Director>();
		auto &layoutBuilder = getBuilderManager().getBuilder<Layout::Builder>();
		auto &layoutDirector =
			getDirectorManager().getDirector<Layout::Director>();
		auto &appBarBuilder = getBuilderManager().getBuilder<AppBar::Builder>();
		auto &buttonBuilder = getBuilderManager().getBuilder<Button::Builder>();
		auto &buttonDirector =
			getDirectorManager().getDirector<Button::Director>();
		auto &navigationBuilder =
			getBuilderManager().getBuilder<NavigationBar::Builder>();
		auto &navigationDirector =
			getDirectorManager().getDirector<NavigationBar::Director>();
		auto &tabsBuilder  = getBuilderManager().getBuilder<Tabs::Builder>();
		auto &tabsDirector = getDirectorManager().getDirector<Tabs::Director>();
		auto &listBuilder  = getBuilderManager().getBuilder<List::Builder>();
		auto &listDirector = getDirectorManager().getDirector<List::Director>();
		auto &cardBuilder  = getBuilderManager().getBuilder<Card::Builder>();
		auto &cardDirector = getDirectorManager().getDirector<Card::Director>();
		auto &chipBuilder  = getBuilderManager().getBuilder<Chip::Builder>();
		auto &chipDirector = getDirectorManager().getDirector<Chip::Director>();
		auto &badgeBuilder = getBuilderManager().getBuilder<Badge::Builder>();
		auto &badgeDirector =
			getDirectorManager().getDirector<Badge::Director>();
		auto &switchBuilder = getBuilderManager().getBuilder<Switch::Builder>();
		auto &switchDirector =
			getDirectorManager().getDirector<Switch::Director>();
		auto &checkboxBuilder =
			getBuilderManager().getBuilder<Checkbox::Builder>();
		auto &checkboxDirector =
			getDirectorManager().getDirector<Checkbox::Director>();
		auto &radioBuilder =
			getBuilderManager().getBuilder<RadioButton::Builder>();
		auto &radioDirector =
			getDirectorManager().getDirector<RadioButton::Director>();
		auto &sliderBuilder = getBuilderManager().getBuilder<Slider::Builder>();
		auto &sliderDirector =
			getDirectorManager().getDirector<Slider::Director>();
		auto &textFieldBuilder =
			getBuilderManager()
				.getBuilder<guillaume::entities::TextField::Builder>();
		auto &textFieldDirector =
			getDirectorManager()
				.getDirector<guillaume::entities::TextField::Director>();
		auto &dividerBuilder =
			getBuilderManager().getBuilder<Divider::Builder>();
		auto &dividerDirector =
			getDirectorManager().getDirector<Divider::Director>();
		auto &progressBuilder =
			getBuilderManager().getBuilder<ProgressIndicator::Builder>();
		auto &progressDirector =
			getDirectorManager().getDirector<ProgressIndicator::Director>();
		auto &snackbarBuilder =
			getBuilderManager().getBuilder<Snackbar::Builder>();
		auto &snackbarDirector =
			getDirectorManager().getDirector<Snackbar::Director>();
		auto &dialogBuilder = getBuilderManager().getBuilder<Dialog::Builder>();
		auto &dialogDirector =
			getDirectorManager().getDirector<Dialog::Director>();

		// --- App bar -----------------------------------------------------
		auto appBar = appBarBuilder.withVariant(AppBar::Variant::Small)
						  .withTitle("XIDER Showcase")
						  .withNavigationIcon("menu")
						  .addAction("search")
						  .addAction("more_vert")
						  .registerEntity(nullptr);
		addRootEntity("app_bar", appBar);

		// --- Tabs --------------------------------------------------------
		addRootEntity(
			"section_tabs",
			tabsDirector.makeTabs(tabsBuilder, nullptr, Tabs::Variant::Primary,
								  { "Overview", "Components", "About" }, 0));

		// --- Feature list ------------------------------------------------
		std::vector<List::Item> capabilities {
			{ "Scenes", "Compose entity trees per screen", "dashboard", "" },
			{ "Theming", "Material Design 3 surface colors", "palette", "" },
			{ "Events", "Mouse, keyboard and hand input", "touch_app", "" },
			{ "Rendering", "Vulkan-backed 2D and 3D", "view_in_ar", "" },
		};

		addRootEntity("capability_list",
					  listDirector.makeList(listBuilder, nullptr,
											List::Variant::TwoLine,
											capabilities));

		// --- Cards -------------------------------------------------------
		auto elevatedCard = cardDirector.makeCard(cardBuilder, nullptr,
												  Card::Variant::Elevated);
		elevatedCard->setChildren(
			{ heading(textBuilder, textDirector, elevatedCard, "Elevated",
					  18.0f),
			  chipDirector.makeChip(chipBuilder, elevatedCard,
									Chip::Variant::Assist, "Assist", "star",
									false),
			  badgeDirector.makeBadge(badgeBuilder, elevatedCard, "3",
									  Badge::Variant::Small) });

		auto filledCard =
			cardDirector.makeCard(cardBuilder, nullptr, Card::Variant::Filled);
		filledCard->setChildren(
			{ heading(textBuilder, textDirector, filledCard, "Filled", 18.0f),
			  chipDirector.makeChip(chipBuilder, filledCard,
									Chip::Variant::Filter, "Filter",
									"filter_alt", true),
			  badgeDirector.makeBadge(badgeBuilder, filledCard, "New",
									  Badge::Variant::Large) });

		auto outlinedCard = cardDirector.makeCard(cardBuilder, nullptr,
												  Card::Variant::Outlined);
		outlinedCard->setChildren(
			{ heading(textBuilder, textDirector, outlinedCard, "Outlined",
					  18.0f),
			  chipDirector.makeChip(chipBuilder, outlinedCard,
									Chip::Variant::Suggestion, "Suggestion",
									"lightbulb", false),
			  badgeDirector.makeBadge(badgeBuilder, outlinedCard, "1",
									  Badge::Variant::Dot) });

		addRootEntity("card_row",
					  row(layoutBuilder, layoutDirector, nullptr,
						  { elevatedCard, filledCard, outlinedCard }, 16.0f));

		// --- Selection controls -----------------------------------------
		auto switches =
			column(layoutBuilder, layoutDirector, nullptr,
				   { switchDirector.makeSwitch(switchBuilder, nullptr,
											   Switch::Variant::WithIcon, true,
											   "dark_mode", "Dark mode"),
					 switchDirector.makeSwitch(switchBuilder, nullptr,
											   Switch::Variant::Enabled, false,
											   "", "Auto-save") },
				   12.0f);

		auto checkboxes =
			column(layoutBuilder, layoutDirector, nullptr,
				   { checkboxDirector.makeCheckbox(
						 checkboxBuilder, nullptr, Checkbox::State::Checked,
						 Checkbox::Variant::Enabled, "Show grid"),
					 checkboxDirector.makeCheckbox(
						 checkboxBuilder, nullptr, Checkbox::State::Unchecked,
						 Checkbox::Variant::Enabled, "Snap to grid") },
				   12.0f);

		auto radios = column(
			layoutBuilder, layoutDirector, nullptr,
			{ radioDirector.makeRadioButton(radioBuilder, nullptr,
											RadioButton::Variant::Enabled,
											"Light"),
			  radioDirector.makeRadioButton(
				  radioBuilder, nullptr, RadioButton::Variant::Enabled, "Dark"),
			  radioDirector.makeRadioButton(radioBuilder, nullptr,
											RadioButton::Variant::Enabled,
											"System") },
			12.0f);

		addRootEntity("selection_row",
					  row(layoutBuilder, layoutDirector, nullptr,
						  { switches, checkboxes, radios }, 32.0f));

		// --- Slider, text field and progress ----------------------------
		auto slider	   = sliderDirector.makeSlider(sliderBuilder, nullptr,
												   Slider::Variant::Continuous,
												   0.0f, 100.0f, 35.0f);
		auto textField = textFieldDirector.makeTextField(
			textFieldBuilder, nullptr,
			guillaume::entities::TextField::Variant::Filled, "Project name",
			"folder", "clear", "Shown in the title bar", false, "xider-app");

		addRootEntity(
			"input_column",
			column(layoutBuilder, layoutDirector, nullptr,
				   { slider, textField,
					 progressDirector.makeProgressIndicator(
						 progressBuilder, nullptr,
						 ProgressIndicator::Variant::LinearDeterminate, 0.6f) },
				   16.0f));

		addRootEntity("section_divider",
					  dividerDirector.makeDivider(dividerBuilder, nullptr,
												  400.0f,
												  Divider::Variant::FullWidth));

		// --- Content list ------------------------------------------------
		std::vector<List::Item> shortcuts {
			{ "New scene", "Add a fresh entity tree", "note_add",
			  "chevron_right" },
			{ "Open asset", "Browse the resource provider", "folder_open",
			  "chevron_right" },
			{ "Preferences", "Tune the editor", "settings", "chevron_right" },
		};

		addRootEntity("shortcut_list",
					  listDirector.makeList(listBuilder, nullptr,
											List::Variant::TwoLine, shortcuts));

		// --- Overlay actions --------------------------------------------
		auto dialog = dialogDirector.makeDialog(
			dialogBuilder, nullptr, Dialog::Variant::Alert, "Delete project?",
			"This action cannot be undone.", { "Cancel", "Delete" });

		auto snackbar = snackbarDirector.makeSnackbar(
			snackbarBuilder, nullptr, Snackbar::Variant::WithAction,
			"Project saved", "Undo");

		addRootEntity("overlay_actions",
					  row(layoutBuilder, layoutDirector, nullptr,
						  { buttonDirector.makeButton(
								buttonBuilder, nullptr, "Show dialog",
								[dialog]() {
									dialog->show();
								},
								Button::Color::Filled, Button::Shape::Round,
								Button::Size::Medium, false),
							buttonDirector.makeButton(
								buttonBuilder, nullptr, "Show snackbar",
								[this, snackbar]() {
									setOverlayVisible(
										this->getComponentRegistry(),
										snackbar->getIdentifier(), true);
								},
								Button::Color::Tonal, Button::Shape::Round,
								Button::Size::Medium, false) },
						  16.0f));

		addRootEntity("alert_dialog", dialog);
		addRootEntity("save_snackbar", snackbar);

		// --- Bottom navigation -------------------------------------------
		addRootEntity("navigation_bar",
					  navigationDirector.makeNavigationBar(
						  navigationBuilder, nullptr,
						  NavigationBar::Variant::WithLabels,
						  { { "widgets", "Cards" },
							{ "tune", "Controls" },
							{ "text_fields", "Inputs" },
							{ "layers", "Overlays" } }));
	}

	Showcase::~Showcase(void)
	{
	}

}	 // namespace xider::scenes
