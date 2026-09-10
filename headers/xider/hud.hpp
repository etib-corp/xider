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

#include <guillaume/ecs/component_registry.hpp>
#include <guillaume/ecs/entity.hpp>
#include <guillaume/entities/button_base.hpp>
#include <guillaume/entities/panel.hpp>

#include <utility/graphic/position.hpp>
#include <utility/graphic/size.hpp>
#include <utility/graphic/view.hpp>

namespace xider
{
	/**
	 * @brief Corner a renderer grows an entity's box from.
	 *
	 * Guillaume draws every entity from its transform position, but its
	 * renderers disagree on which way the box grows: rectangles grow towards
	 * positive Y, while text and glyphs grow towards negative Y. The layout
	 * stores the wanted center of a box and converts it back to the corner
	 * the renderer expects.
	 */
	enum class HudAnchor {
		Rectangle,	  ///< Box grows towards positive X and Y: panels, buttons
					  ///< and images.
		Text,		  ///< Box grows towards positive X and negative Y: text and
					  ///< glyphs.
	};

	/**
	 * @class Hud
	 * @brief Keeps a scene's interface assembled around a placed anchor.
	 *
	 * Guillaume places the direct entities of a scene in front of the camera
	 * when the scene is entered, and again whenever the camera turns around,
	 * but it never moves their children. A scene therefore registers a single
	 * direct entity as its anchor and attaches the rest of its interface to
	 * it: the anchor marks where the framework put the interface, and every
	 * other entity is laid out relative to it. The interface is left in the
	 * world between those placements, and follows the anchor again when the
	 * framework moves it. A button is the one element carrying content of its
	 * own, so a button that is moved is asked to lay that content out again.
	 *
	 * Entities are placed along the world axes, with an identity orientation.
	 * That is the only orientation the framework renders consistently: text
	 * and glyphs always lay their quads out along the world axes and only
	 * borrow the orientation for their normals, so a turned interface would
	 * mix rotated rectangles with unrotated text. The anchor keeps its
	 * position but loses the camera orientation it was placed with.
	 *
	 * Offsets are given as a fraction of the visible extent at the anchor
	 * plane, measured when the anchor was placed, so that the interface keeps
	 * its proportions when the field of view changes. Layout work happens only
	 * when something moves: on the first frame, when the framework places the
	 * anchor again, when the visible extent changes, and while the measured
	 * bounds of an element still settle. Every pose is left untouched between
	 * those events, which keeps the render caches of the framework warm: the
	 * text renderer rebuilds a text and the model renderer reloads a model
	 * from its file whenever a pose changes.
	 */
	class Hud
	{
		public:
		/**
		 * @brief An interface entity placed relative to the anchor.
		 */
		struct Element {
			std::shared_ptr<guillaume::ecs::Entity> entity {};	///< Entity.
			guillaume::entities::Panel
				*panel { nullptr };	  ///< Entity as a panel, when it is one.
			guillaume::entities::ButtonBase
				*button { nullptr };	///< Entity as a button, when it is one.
			HudAnchor anchor { HudAnchor::Rectangle };	///< Box growth.
			float centerX { 0.0f };	  ///< Center across the view, as a
									  ///< fraction of the visible width.
			float centerY { 0.0f };	  ///< Center up the view, as a fraction
									  ///< of the visible height.
			float widthFraction { -1.0f };	 ///< Width as a fraction of the
											 ///< visible width, or a negative
											 ///< value to use the measured
											 ///< bound.
			float heightFraction { -1.0f };	 ///< Height as a fraction of the
											 ///< visible height, or a negative
											 ///< value to use the measured
											 ///< bound.
			float width { 0.0f };	///< Width used by the last layout.
			float height { 0.0f };	///< Height used by the last layout.
		};

		/**
		 * @brief Entities laid out side by side on a single line.
		 *
		 * The entities of a row are sized by their own content, which is only
		 * known once the measure systems have run: the row therefore holds
		 * them apart by a distance in world units, and is re-centered every
		 * time one of its entities is measured again.
		 */
		struct Row {
			std::vector<std::size_t> elements {};	///< Entries of the element
													///< list, from left to
													///< right.
			float centerX { 0.0f };	  ///< Center across the view, as a
									  ///< fraction of the visible width.
			float centerY { 0.0f };	  ///< Center up the view, as a fraction
									  ///< of the visible height.
			float gap { 0.0f };		  ///< Space between two neighbours, in
									  ///< world units.
		};

		/**
		 * @brief A model placed in front of the camera when the interface is
		 * placed.
		 */
		struct Model {
			std::shared_ptr<guillaume::ecs::Entity> entity {};	///< Entity.
			utility::graphic::SizeF size {};   ///< Size of the model asset, in
											   ///< its own units.
			float sizeFraction { 0.5f };   ///< Share of the visible height the
										   ///< model should cover.
			float offsetX { 0.0f };		   ///< Center across the view, as a
										   ///< fraction of the visible width
										   ///< at the anchor plane.
			float offsetY { 0.0f };		   ///< Center up the view, as a
										   ///< fraction of the visible height
										   ///< at the anchor plane.
		};

		public:
		/**
		 * @brief Construct an empty layout.
		 */
		Hud(void);

		/**
		 * @brief Destroy the layout.
		 */
		~Hud(void);

		public:
		/**
		 * @brief Set the direct entity the layout is anchored to.
		 *
		 * The anchor is the entity the framework places in front of the
		 * camera. Its position is never changed by the layout, only read back
		 * to place the other entities; the camera orientation it is placed
		 * with is dropped, since the interface is assembled along the world
		 * axes.
		 * @param entity Direct entity used as the anchor.
		 */
		void
			setAnchorEntity(const std::shared_ptr<guillaume::ecs::Entity> &entity);

		/**
		 * @brief Register an entity sized by its own content.
		 *
		 * Text, glyphs and buttons measure themselves, so the layout reads
		 * their bound back once the measure systems have filled it in.
		 * @param entity Entity to keep in place.
		 * @param anchor Direction in which the entity's box grows.
		 * @param centerX Center across the view, as a fraction of the visible
		 * width.
		 * @param centerY Center up the view, as a fraction of the visible
		 * height.
		 */
		void addElement(const std::shared_ptr<guillaume::ecs::Entity> &entity,
						HudAnchor anchor, float centerX, float centerY);

		/**
		 * @brief Register an entity sized by the layout.
		 *
		 * Panels carry a bound but no way to set it, so the layout writes the
		 * size of a panel directly into its bound, as a fraction of the
		 * visible extent, which keeps the interface proportional to the view.
		 * @param entity Panel to keep in place and size.
		 * @param centerX Center across the view, as a fraction of the visible
		 * width.
		 * @param centerY Center up the view, as a fraction of the visible
		 * height.
		 * @param widthFraction Width as a fraction of the visible width.
		 * @param heightFraction Height as a fraction of the visible height.
		 */
		void addPanel(const std::shared_ptr<guillaume::ecs::Entity> &entity,
					  float centerX, float centerY, float widthFraction,
					  float heightFraction);

		/**
		 * @brief Register entities laid out side by side on a single line.
		 *
		 * The entities are centered on the given point as one block, so the
		 * block stays centered whatever the size each of them measures.
		 * @param entities Entities to lay out, from left to right.
		 * @param centerX Center of the line across the view, as a fraction of
		 * the visible width.
		 * @param centerY Center of the line up the view, as a fraction of the
		 * visible height.
		 * @param gap Space kept between two neighbours, in world units.
		 */
		void addRow(
			const std::vector<std::shared_ptr<guillaume::ecs::Entity>> &entities,
			float centerX, float centerY, float gap = _defaultRowGap);

		/**
		 * @brief Register a model displayed in front of the camera.
		 *
		 * Models are placed when the interface is placed, and are left alone
		 * afterwards: the model renderer reloads a model from its file
		 * whenever its pose changes.
		 * @param entity Model entity to place.
		 * @param size Size of the model asset, in its own units.
		 * @param sizeFraction Share of the visible height to cover.
		 * @param offsetX Center across the view, as a fraction of the visible
		 * width at the anchor plane.
		 * @param offsetY Center up the view, as a fraction of the visible
		 * height at the anchor plane.
		 */
		void addModel(const std::shared_ptr<guillaume::ecs::Entity> &entity,
					  const utility::graphic::SizeF &size, float sizeFraction,
					  float offsetX = 0.0f, float offsetY = 0.0f);

		/**
		 * @brief Change the asset a registered model displays.
		 *
		 * The size of the asset decides how far the model has to stand from
		 * the camera to cover the share of the view it was registered with,
		 * so the model is placed again on the next frame.
		 * @param entity Model entity to update.
		 * @param size Size of the new model asset, in its own units.
		 */
		void setModelAsset(const std::shared_ptr<guillaume::ecs::Entity> &entity,
						   const utility::graphic::SizeF &size);

		/**
		 * @brief Keep every registered entity where it belongs.
		 *
		 * Does nothing while the layout is already up to date, so it is safe
		 * to call once per frame.
		 * @param componentRegistry Registry owning the components to update.
		 * @param view View of the current frame.
		 */
		void settle(guillaume::ecs::ComponentRegistry &componentRegistry,
					const utility::graphic::ViewF &view);

		/**
		 * @brief Check whether the layout has been applied at least once.
		 * @return True when every entity has been placed once.
		 */
		bool isSettled(void) const noexcept;

		private:
		/**
		 * @brief Position every element relative to the anchor.
		 * @param componentRegistry Registry owning the components to update.
		 */
		void layoutElements(
			guillaume::ecs::ComponentRegistry &componentRegistry);

		/**
		 * @brief Resolve the size an element occupies in the layout.
		 *
		 * A sized element is set to its fraction of the visible extent, which
		 * also writes its bound; a content-sized element reads the bound the
		 * measure systems filled in. The result is remembered on the element,
		 * both to place it and to notice later that a measurement changed.
		 * @param componentRegistry Registry owning the components to read.
		 * @param element Element to resolve.
		 */
		void resolveSize(guillaume::ecs::ComponentRegistry &componentRegistry,
						 Element &element);

		/**
		 * @brief Place an element around a point of the anchor plane.
		 *
		 * A button lays its own icon and label out from the pose it carries, and
		 * only does so when it is asked to, so a button is updated right after
		 * being moved: its content would otherwise stay where it was built, at
		 * the origin of the world.
		 * @param componentRegistry Registry owning the components to update.
		 * @param element Element to place.
		 * @param centerX Center across the view, in world units.
		 * @param centerY Center up the view, in world units.
		 */
		void place(guillaume::ecs::ComponentRegistry &componentRegistry,
				   const Element &element, float centerX, float centerY);

		/**
		 * @brief Place the registered models in front of the camera.
		 * @param componentRegistry Registry owning the components to update.
		 * @param view View of the current frame.
		 */
		void layoutModels(guillaume::ecs::ComponentRegistry &componentRegistry,
						  const utility::graphic::ViewF &view);

		/**
		 * @brief Check whether an element is not laid out at its own size yet.
		 * @param componentRegistry Registry owning the components to read.
		 * @return True when the measured bound of an element changed.
		 */
		bool hasElementBoundsChanged(
			guillaume::ecs::ComponentRegistry &componentRegistry) const;

		/**
		 * @brief Measure the extent of the view at a point of the scene.
		 * @param anchor Point where the extent is measured.
		 * @param view View of the current frame.
		 * @return Visible width and height at the anchor.
		 */
		static utility::graphic::SizeF
			visibleSizeAt(const utility::graphic::PositionF &anchor,
						  const utility::graphic::ViewF &view);

		/**
		 * @brief Move an entity to a pose.
		 *
		 * Panels remember the pose they were given and write it back whenever
		 * they are updated, so a panel has to be moved through its own setter
		 * to keep that memory in step with the transform.
		 * @param componentRegistry Registry owning the components to update.
		 * @param identifier Entity to move.
		 * @param panel Entity as a panel, when it is one.
		 * @param pose Pose to move the entity to.
		 */
		static void setPose(
			guillaume::ecs::ComponentRegistry &componentRegistry,
			const guillaume::ecs::Entity::Identifier &identifier,
			guillaume::entities::Panel *panel,
			const utility::graphic::PoseF &pose);

		private:
		static constexpr float _defaultRowGap =
			16.0f;	///< Space kept between two entities of a row, in world
					///< units.
		static constexpr float _minimumAnchorDistance =
			1.0f;	///< Closest distance kept between the camera and the
					///< anchor plane.
		static constexpr float _minimumModelDistance =
			4.0f;	///< Closest distance a model may be placed at, so that the
					///< smallest assets stay outside of the near plane and off
					///< the face of the viewer.
		static constexpr float _minimumSizeFraction =
			0.01f;	///< Smallest share of the view a model may be asked to
					///< cover, so that the distance it implies stays finite.
		static constexpr std::size_t _maximumLayoutPasses =
			4;	///< Times the layout may be run for a single frame, so that an
				///< element whose measure never holds cannot hold the frame.

		private:
		std::shared_ptr<guillaume::ecs::Entity> _anchorEntity {};	///< Anchor.
		guillaume::entities::Panel
			*_anchorPanel { nullptr };	///< Anchor as a panel, when it is one.
		std::vector<Element> _elements {};	 ///< Entities to keep in place.
		std::vector<Row> _rows {};			 ///< Lines of entities.
		std::vector<Model> _models {};		 ///< Models to place.
		utility::graphic::PositionF _anchorPosition {};	  ///< Placed anchor.
		utility::graphic::SizeF _visibleSize {};   ///< Extent seen at the
												   ///< anchor plane.
		bool _placed { false };			 ///< True once the anchor was seen
										 ///< at a placement position.
		bool _modelsPlaced { false };	 ///< True once the models were placed.
	};

}	 // namespace xider
