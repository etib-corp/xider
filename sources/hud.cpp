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

#include "xider/hud.hpp"

#include <algorithm>
#include <cmath>
#include <cstddef>
#include <memory>
#include <utility>

#include <guillaume/components/bound.hpp>
#include <guillaume/components/transform.hpp>

#include <utility/math/vector.hpp>

namespace xider
{
	Hud::Hud(void)
	{
	}

	Hud::~Hud(void)
	{
	}

	void Hud::setAnchorEntity(
		const std::shared_ptr<guillaume::ecs::Entity> &entity)
	{
		_anchorEntity = entity;
		_anchorPanel =
			dynamic_cast<guillaume::entities::Panel *>(entity.get());
		_placed = false;
	}

	void Hud::addElement(const std::shared_ptr<guillaume::ecs::Entity> &entity,
						 HudAnchor anchor, float centerX, float centerY)
	{
		Element element;

		element.entity = entity;
		element.panel = dynamic_cast<guillaume::entities::Panel *>(entity.get());
		element.button =
			dynamic_cast<guillaume::entities::ButtonBase *>(entity.get());
		element.anchor	= anchor;
		element.centerX = centerX;
		element.centerY = centerY;

		_elements.push_back(std::move(element));

		_placed = false;
	}

	void Hud::addPanel(const std::shared_ptr<guillaume::ecs::Entity> &entity,
					   float centerX, float centerY, float widthFraction,
					   float heightFraction)
	{
		Element element;

		element.entity = entity;
		element.panel = dynamic_cast<guillaume::entities::Panel *>(entity.get());
		element.anchor		   = HudAnchor::Rectangle;
		element.centerX		   = centerX;
		element.centerY		   = centerY;
		element.widthFraction  = widthFraction;
		element.heightFraction = heightFraction;

		_elements.push_back(std::move(element));

		_placed = false;
	}

	void Hud::addRow(
		const std::vector<std::shared_ptr<guillaume::ecs::Entity>> &entities,
		float centerX, float centerY, float gap)
	{
		Row row;

		row.centerX = centerX;
		row.centerY = centerY;
		row.gap		= gap;

		for (const auto &entity: entities) {
			row.elements.push_back(_elements.size());
			addElement(entity, HudAnchor::Rectangle, centerX, centerY);
		}

		if (row.elements.empty()) {
			return;
		}

		_rows.push_back(std::move(row));

		_placed = false;
	}

	void Hud::addModel(const std::shared_ptr<guillaume::ecs::Entity> &entity,
					   const utility::graphic::SizeF &size, float sizeFraction,
					   float offsetX, float offsetY)
	{
		Model model;

		model.entity	   = entity;
		model.size		   = size;
		model.sizeFraction = sizeFraction;
		model.offsetX	   = offsetX;
		model.offsetY	   = offsetY;

		_models.push_back(std::move(model));
	}

	void Hud::setModelAsset(
		const std::shared_ptr<guillaume::ecs::Entity> &entity,
		const utility::graphic::SizeF &size)
	{
		for (auto &model: _models) {
			if (model.entity != entity) {
				continue;
			}

			model.size = size;
			_modelsPlaced = false;
			return;
		}
	}

	void Hud::settle(guillaume::ecs::ComponentRegistry &componentRegistry,
					 const utility::graphic::ViewF &view)
	{
		if (!_anchorEntity) {
			return;
		}

		const auto identifier = _anchorEntity->getIdentifier();

		if (!componentRegistry
				 .hasComponent<guillaume::components::Transform>(identifier)) {
			return;
		}

		utility::graphic::PoseF pose =
			componentRegistry
				.getComponent<guillaume::components::Transform>(identifier)
				.getPose();
		const utility::graphic::PositionF anchor = pose.getPosition();

		// The framework places the anchor facing the camera. The interface
		// itself is built along the world axes, the only orientation every
		// renderer of the framework agrees on.
		if (!(pose.getOrientation() == utility::graphic::OrientationF())) {
			pose.setOrientation(utility::graphic::OrientationF());
			setPose(componentRegistry, identifier, _anchorPanel, pose);
		}

		const bool moved = _placed && !(anchor == _anchorPosition);
		const utility::graphic::SizeF visible = visibleSizeAt(anchor, view);

		// A view that carries no field of view cannot hold an interface: every
		// entity would collapse on the anchor and the models would be pushed
		// away to infinity. The framework hands out such a view until the
		// platform has described the one it renders with.
		if (visible.getWidth() <= 0.0f || visible.getHeight() <= 0.0f) {
			return;
		}

		const bool resized = _placed && !(visible == _visibleSize);

		if (moved || resized) {
			// Models sit in front of the camera, so they follow the interface
			// when the framework moves it to the other side of the viewer.
			_modelsPlaced = false;
		}

		const bool relayout = !_placed || moved || resized
			|| hasElementBoundsChanged(componentRegistry);

		if (relayout) {
			_anchorPosition = anchor;
			_visibleSize	= visible;

			// Laying an element out can change what it measures: a button sizes
			// itself from the icon and the label it has just placed, and the row
			// holding it is centered on the sizes of its members. The layout is
			// therefore run again while the measures still move, and stops as
			// soon as they hold, which is what keeps the poses of a still camera
			// from being written again on the following frames.
			for (std::size_t pass = 0; pass < _maximumLayoutPasses; ++pass) {
				layoutElements(componentRegistry);

				if (!hasElementBoundsChanged(componentRegistry)) {
					break;
				}
			}
		}

		if (!_modelsPlaced) {
			layoutModels(componentRegistry, view);
			_modelsPlaced = true;
		}

		_placed = true;
	}

	bool Hud::isSettled(void) const noexcept
	{
		return _placed && _modelsPlaced;
	}

	void
		Hud::layoutElements(guillaume::ecs::ComponentRegistry &componentRegistry)
	{
		const std::size_t count = _elements.size();

		std::vector<float> centerX(count, 0.0f);
		std::vector<float> centerY(count, 0.0f);

		for (std::size_t index = 0; index < count; ++index) {
			Element &element = _elements[index];

			resolveSize(componentRegistry, element);

			centerX[index] = _anchorPosition.getX()
				+ element.centerX * _visibleSize.getWidth();
			centerY[index] = _anchorPosition.getY()
				+ element.centerY * _visibleSize.getHeight();
		}

		// A row is centered on its own point as a block: the width it covers
		// is only known once every one of its entities has been measured.
		for (const Row &row: _rows) {
			float total =
				row.gap * static_cast<float>(row.elements.size() - 1);
			float cursor = 0.0f;

			for (const std::size_t index: row.elements) {
				total += _elements[index].width;
			}

			cursor = -total / 2.0f;

			for (const std::size_t index: row.elements) {
				const float width = _elements[index].width;

				centerX[index] = _anchorPosition.getX()
					+ row.centerX * _visibleSize.getWidth() + cursor
					+ width / 2.0f;
				centerY[index] = _anchorPosition.getY()
					+ row.centerY * _visibleSize.getHeight();

				cursor += width + row.gap;
			}
		}

		for (std::size_t index = 0; index < count; ++index) {
			place(componentRegistry, _elements[index], centerX[index],
				  centerY[index]);
		}
	}

	void Hud::resolveSize(guillaume::ecs::ComponentRegistry &componentRegistry,
						  Element &element)
	{
		element.width  = 0.0f;
		element.height = 0.0f;

		if (!element.entity) {
			return;
		}

		const auto identifier = element.entity->getIdentifier();

		if (!componentRegistry
				 .hasComponent<guillaume::components::Bound>(identifier)) {
			return;
		}

		if (element.widthFraction > 0.0f
			&& element.heightFraction > 0.0f) {
			element.width  = element.widthFraction * _visibleSize.getWidth();
			element.height = element.heightFraction * _visibleSize.getHeight();

			componentRegistry
				.getComponent<guillaume::components::Bound>(identifier)
				.setWidth(element.width)
				.setHeight(element.height);

			return;
		}

		const auto &bound = componentRegistry
								.getComponent<guillaume::components::Bound>(
									identifier);

		element.width  = bound.getWidth();
		element.height = bound.getHeight();
	}

	void Hud::place(guillaume::ecs::ComponentRegistry &componentRegistry,
					const Element &element, float centerX, float centerY)
	{
		if (!element.entity) {
			return;
		}

		const auto identifier = element.entity->getIdentifier();

		if (!componentRegistry
				 .hasComponent<guillaume::components::Transform>(identifier)) {
			return;
		}

		const float cornerX = centerX - element.width / 2.0f;
		const float cornerY = element.anchor == HudAnchor::Rectangle
			? centerY - element.height / 2.0f
			: centerY + element.height / 2.0f;

		utility::graphic::PoseF pose =
			componentRegistry
				.getComponent<guillaume::components::Transform>(identifier)
				.getPose();

		pose.setPosition(utility::graphic::PositionF(cornerX, cornerY,
													 _anchorPosition.getZ()));
		pose.setOrientation(utility::graphic::OrientationF());
		setPose(componentRegistry, identifier, element.panel, pose);

		// A button is the only element holding children of its own: it lays its
		// icon and label out from the pose it has just been given.
		if (element.button != nullptr) {
			element.button->update();
		}
	}

	void Hud::layoutModels(
		guillaume::ecs::ComponentRegistry &componentRegistry,
		const utility::graphic::ViewF &view)
	{
		const utility::graphic::PositionF origin = view.getPose().getPosition();
		const auto right						 = view.getRight();
		const auto up							 = view.getUp();
		const auto &fieldOfView					 = view.getFieldOfView();
		const float verticalPerUnit =
			std::tan(fieldOfView.getUp()) - std::tan(fieldOfView.getDown());

		for (const auto &model: _models) {
			if (!model.entity) {
				continue;
			}

			const auto identifier = model.entity->getIdentifier();

			if (!componentRegistry
					 .hasComponent<guillaume::components::Transform>(
						 identifier)) {
				continue;
			}

			// The share of the view a model has to cover decides how far it
			// stands from the camera, since models carry no usable scale. The
			// small assets of a catalogue would otherwise be nailed to the
			// near plane, so the distance is floored.
			const float fraction =
				std::max(model.sizeFraction, _minimumSizeFraction);
			const float distance =
				std::max(model.size.getHeight() / (fraction * verticalPerUnit),
						 _minimumModelDistance);

			// The offsets are measured on the anchor plane, in the same frame
			// as the interface, so that a model lines up with the console
			// whatever the distance it ends up standing at.
			utility::graphic::PositionF target = _anchorPosition;

			target += right * (model.offsetX * _visibleSize.getWidth());
			target += up * (model.offsetY * _visibleSize.getHeight());

			const utility::math::Vector3F direction =
				utility::math::normalize(target - origin);

			utility::graphic::PositionF position = origin;

			position += direction * distance;

			utility::graphic::PoseF pose =
				componentRegistry
					.getComponent<guillaume::components::Transform>(identifier)
					.getPose();

			pose.setOrientation(utility::graphic::OrientationF());
			pose.setPosition(position);
			setPose(componentRegistry, identifier, nullptr, pose);
		}
	}

	bool Hud::hasElementBoundsChanged(
		guillaume::ecs::ComponentRegistry &componentRegistry) const
	{
		for (const auto &element: _elements) {
			if (!element.entity) {
				continue;
			}

			const auto identifier = element.entity->getIdentifier();

			if (!componentRegistry
					 .hasComponent<guillaume::components::Bound>(identifier)) {
				continue;
			}

			const auto &bound =
				componentRegistry.getComponent<guillaume::components::Bound>(
					identifier);

			if (bound.getWidth() != element.width
				|| bound.getHeight() != element.height) {
				return true;
			}
		}

		return false;
	}

	utility::graphic::SizeF
		Hud::visibleSizeAt(const utility::graphic::PositionF &anchor,
						   const utility::graphic::ViewF &view)
	{
		const auto &fieldOfView = view.getFieldOfView();
		const auto toAnchor		= anchor - view.getPose().getPosition();
		const float distance	= std::max(
			   utility::math::dot(toAnchor, view.getForward()),
			   _minimumAnchorDistance);
		const float width =
			distance
			* (std::tan(fieldOfView.getRight())
			   - std::tan(fieldOfView.getLeft()));
		const float height =
			distance
			* (std::tan(fieldOfView.getUp()) - std::tan(fieldOfView.getDown()));

		return utility::graphic::SizeF(width, height);
	}

	void Hud::setPose(
		guillaume::ecs::ComponentRegistry &componentRegistry,
		const guillaume::ecs::Entity::Identifier &identifier,
		guillaume::entities::Panel *panel, const utility::graphic::PoseF &pose)
	{
		if (panel != nullptr) {
			panel->setPose(pose);
			return;
		}

		componentRegistry.getComponent<guillaume::components::Transform>(
			identifier)
			.setPose(pose);
	}

}	 // namespace xider
