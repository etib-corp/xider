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

#include "guillaume/systems/mouse_button.hpp"

namespace guillaume::systems
{
	MouseButton::MouseButton(event::EventBus &eventBus, std::unique_ptr<Renderer> &renderer)
		: ecs::SystemFiller<components::MouseButtonInteraction,
							components::Transform, components::Bound>(
			  ecs::Phase::Event)
		, event::EventManager<utility::event::MouseButtonEvent>(eventBus)
		, _renderer(renderer)
	{
	}

	MouseButton::~MouseButton(void)
	{
	}

	void MouseButton::update(const ecs::Entity::Identifier &entityIdentifier)
	{
		auto mouseButtonEvent = getLastEvent();
		if (!mouseButtonEvent)
			return;

		const auto button = mouseButtonEvent->getButton();
		if (button == utility::event::MouseButtonEvent::Button::Unknown)
			return;

		const auto &position = mouseButtonEvent->getPosition();
		auto ray			 = _renderer->getView().viewPointToRay(position);

		auto &transform = getComponent<components::Transform>(entityIdentifier);
		auto &bound		= getComponent<components::Bound>(entityIdentifier);
		auto &mouseButtonInteraction =
			getComponent<components::MouseButtonInteraction>(entityIdentifier);

		const auto size =
			utility::math::Vector2UI({ bound.getWidth(), bound.getHeight() });
		const bool isIntersecting =
			ray.intersectRectangle(transform.getPose(), size);

		const bool isPressed = mouseButtonEvent->isPressed();

		if (isPressed && isIntersecting) {
			mouseButtonInteraction.setButtonPressed(button, true);
			const auto &handler =
				mouseButtonInteraction.getOnButtonPressHandler(button);
			if (handler) {
				handler();
			}
		} else if (!isPressed && mouseButtonInteraction.isButtonPressed(button)
				   && isIntersecting) {
			mouseButtonInteraction.setButtonPressed(button, false);
			const auto &handler =
				mouseButtonInteraction.getOnButtonReleaseHandler(button);
			if (handler) {
				handler();
			}
		} else if (!isPressed) {
			mouseButtonInteraction.setButtonPressed(button, false);
		}
	}

}	 // namespace guillaume::systems