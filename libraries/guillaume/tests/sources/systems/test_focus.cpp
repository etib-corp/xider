/*
 Copyright (c) 2026 ETIB Corporation

 Permission is hereby granted, free of charge, to any person obtaining a copy of
 this software and associated documentation files (the "Software"), to deal in
 the Software without restriction, including without limitation the rights to
 use, copy, modify, merge, distribute, sublicense, and/or sell copies
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

#include "systems/test_focus.hpp"

namespace guillaume::systems::tests
{

	TEST_F(TestFocus, Constructor)
	{
		EXPECT_NO_THROW({
			auto focusSystem = Focus(*_eventBus);
		});
	}

	TEST_F(TestFocus, InitialFocusState)
	{
		EXPECT_FALSE(_focusSystem->getFocusedEntity().has_value());
	}

	TEST_F(TestFocus, SetFocusToEntity)
	{
		auto entity1 = createFocusableEntity();
		auto ray	 = utility::graphic::RayF(
			utility::graphic::PositionF(0, 0, 0),
			utility::math::Vector3F(0, 0, -1));

		_focusSystem->setFocus(entity1, ray);

		EXPECT_TRUE(_focusSystem->getFocusedEntity().has_value());
		EXPECT_EQ(_focusSystem->getFocusedEntity().value(), entity1);
	}

	TEST_F(TestFocus, SetFocusToEntityWithHandler)
	{
		auto entity1 = createFocusableEntity();
		auto ray	 = utility::graphic::RayF(
			utility::graphic::PositionF(0, 0, 0),
			utility::math::Vector3F(0, 0, -1));

		bool focusGained = false;
		auto &focusComponent =
			_componentRegistry.getComponent<components::Focus>(entity1);
		focusComponent.setOnFocusGainedHandler([&focusGained]() {
			focusGained = true;
		});

		_focusSystem->setFocus(entity1, ray);

		EXPECT_TRUE(focusGained);
	}

	TEST_F(TestFocus, ClearFocus)
	{
		auto entity1 = createFocusableEntity();
		auto ray	 = utility::graphic::RayF(
			utility::graphic::PositionF(0, 0, 0),
			utility::math::Vector3F(0, 0, -1));

		_focusSystem->setFocus(entity1, ray);
		EXPECT_TRUE(_focusSystem->getFocusedEntity().has_value());

		_focusSystem->clearFocus();
		EXPECT_FALSE(_focusSystem->getFocusedEntity().has_value());
	}

	TEST_F(TestFocus, ClearFocusWithHandler)
	{
		auto entity1 = createFocusableEntity();
		auto ray	 = utility::graphic::RayF(
			utility::graphic::PositionF(0, 0, 0),
			utility::math::Vector3F(0, 0, -1));

		bool focusLost = false;
		auto &focusComponent =
			_componentRegistry.getComponent<components::Focus>(entity1);
		focusComponent.setOnFocusLostHandler([&focusLost]() {
			focusLost = true;
		});

		_focusSystem->setFocus(entity1, ray);
		_focusSystem->clearFocus();

		EXPECT_TRUE(focusLost);
	}

	TEST_F(TestFocus, SwitchFocusBetweenEntities)
	{
		auto entity1 = createFocusableEntity(
			utility::graphic::PoseF(utility::graphic::PositionF(-100, 0, 0)),
			100, 50);
		auto entity2 = createFocusableEntity(
			utility::graphic::PoseF(utility::graphic::PositionF(100, 0, 0)),
			100, 50);

		auto ray1 = utility::graphic::RayF(
			utility::graphic::PositionF(-100, 0, 0),
			utility::math::Vector3F(0, 0, -1));
		auto ray2 = utility::graphic::RayF(
			utility::graphic::PositionF(100, 0, 0),
			utility::math::Vector3F(0, 0, -1));

		bool entity1Lost = false;
		bool entity2Gained = false;

		auto &focus1 = _componentRegistry.getComponent<components::Focus>(entity1);
		focus1.setOnFocusLostHandler([&entity1Lost]() {
			entity1Lost = true;
		});

		auto &focus2 = _componentRegistry.getComponent<components::Focus>(entity2);
		focus2.setOnFocusGainedHandler([&entity2Gained]() {
			entity2Gained = true;
		});

		// Set focus to entity1
		_focusSystem->setFocus(entity1, ray1);
		EXPECT_EQ(_focusSystem->getFocusedEntity().value(), entity1);

		// Switch focus to entity2
		_focusSystem->setFocus(entity2, ray2);
		EXPECT_EQ(_focusSystem->getFocusedEntity().value(), entity2);
		EXPECT_TRUE(entity1Lost);
		EXPECT_TRUE(entity2Gained);
	}

	TEST_F(TestFocus, SetFocusNoIntersection)
	{
		auto entity1 = createFocusableEntity();
		// Ray pointing away from entity
		auto ray = utility::graphic::RayF(
			utility::graphic::PositionF(0, 0, 1000),
			utility::math::Vector3F(0, 0, 1));

		_focusSystem->setFocus(entity1, ray);

		// Focus should not be set since ray doesn't intersect
		EXPECT_FALSE(_focusSystem->getFocusedEntity().has_value());
	}

	TEST_F(TestFocus, UpdateDoesNotChangeFocus)
	{
		auto entity1 = createFocusableEntity();
		auto ray	 = utility::graphic::RayF(
			utility::graphic::PositionF(0, 0, 0),
			utility::math::Vector3F(0, 0, -1));

		_focusSystem->setFocus(entity1, ray);
		EXPECT_TRUE(_focusSystem->getFocusedEntity().has_value());

		_focusSystem->update(entity1);

		// Focus should remain unchanged
		EXPECT_TRUE(_focusSystem->getFocusedEntity().has_value());
		EXPECT_EQ(_focusSystem->getFocusedEntity().value(), entity1);
	}

	TEST_F(TestFocus, MultipleEntitiesSameFocus)
	{
		auto entity1 = createFocusableEntity(
			utility::graphic::PoseF(utility::graphic::PositionF(-100, 0, 0)),
			100, 50);
		auto entity2 = createFocusableEntity(
			utility::graphic::PoseF(utility::graphic::PositionF(100, 0, 0)),
			100, 50);

		auto ray = utility::graphic::RayF(
			utility::graphic::PositionF(-100, 0, 0),
			utility::math::Vector3F(0, 0, -1));

		_focusSystem->setFocus(entity1, ray);
		EXPECT_EQ(_focusSystem->getFocusedEntity().value(), entity1);

		// Setting focus to same entity again should not trigger handler twice
		int focusCount = 0;
		auto &focus1   = _componentRegistry.getComponent<components::Focus>(entity1);
		focus1.setOnFocusGainedHandler([&focusCount]() {
			focusCount++;
		});

		_focusSystem->setFocus(entity1, ray);
		EXPECT_EQ(focusCount, 1);
	}

}	 // namespace guillaume::systems::tests
