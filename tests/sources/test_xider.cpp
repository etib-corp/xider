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

#include <cmath>
#include <cstddef>
#include <filesystem>
#include <functional>
#include <numbers>
#include <string>
#include <system_error>
#include <vector>

#include "test_xider.hpp"

#include <guillaume/components/transform.hpp>

#include <utility/graphic/orientation.hpp>

#include <xider/frame_updater.hpp>
#include <xider/preferences.hpp>
#include <xider/scenes/home.hpp>
#include <xider/scenes/settings.hpp>
#include <xider/scenes/sound.hpp>
#include <xider/scenes/viewer.hpp>

namespace xider::tests
{
	namespace
	{
		/// Aspect ratio the demo interface is laid out for.
		constexpr float aspectRatio = 16.0f / 9.0f;

		/**
		 * @brief Check that a pose can be rendered.
		 * @param pose Pose to check.
		 * @return True when every component of the position is a number.
		 */
		bool isFinite(const utility::graphic::PoseF &pose)
		{
			const auto position = pose.getPosition();

			return std::isfinite(position.getX())
				&& std::isfinite(position.getY())
				&& std::isfinite(position.getZ());
		}

		/**
		 * @brief Build the name of the directory a case works in.
		 * @param name Name of the case.
		 * @return A directory name no other case uses.
		 */
		std::string directoryName(const char *name)
		{
			return "xider_tests_"
				+ std::to_string(std::hash<std::string> {}(name));
		}
	}	 // namespace

	void TestXIDER::SetUp(void)
	{
		std::error_code error;

		_directory = std::filesystem::temp_directory_path(error);

		if (error) {
			_directory = std::filesystem::path(".");
		}

		_directory /= directoryName(
			::testing::UnitTest::GetInstance()->current_test_info()->name());

		std::filesystem::remove_all(_directory, error);
		std::filesystem::create_directories(_directory, error);

		_localStorage = std::make_unique<guillaume::LocalStorage>(
			_directory / "preferences.sqlite");
		_sessionStorage = std::make_unique<guillaume::SessionStorage>();

		_ressourceProvider = std::make_shared<utility::RessourceProvider>(
			_systemIo, _directory.string());
	}

	void TestXIDER::TearDown(void)
	{
		// A callback left installed would call into a scene that is gone.
		FrameUpdater::clear();

		_ressourceProvider.reset();
		_sessionStorage.reset();
		_localStorage.reset();

		std::error_code error;

		std::filesystem::remove_all(_directory, error);

		EXPECT_FALSE(error) << "Could not remove " << _directory << ": "
							<< error.message();
	}

	utility::graphic::ViewF TestXIDER::makeView(void)
	{
		utility::graphic::ViewF view;

		view.setPerspective(std::numbers::pi_v<float> / 2.0f, aspectRatio);

		return view;
	}

	void TestXIDER::settle(guillaume::Scene &scene,
						   const utility::graphic::ViewF &view)
	{
		// The framework places the direct entities of the scene, then the
		// engine runs the layout of the scene for the frame.
		scene.placeEntitiesInFrontOfView(view);

		FrameUpdater::run(view);
	}

	std::vector<utility::graphic::PoseF>
		TestXIDER::collectPoses(guillaume::Scene &scene)
	{
		std::vector<utility::graphic::PoseF> poses;

		auto &components = scene.getComponentRegistry();

		for (const auto &entity:
			 scene.getEntityRegistry().getEntitiesBreadthFirst()) {
			const auto identifier = entity->getIdentifier();

			if (!components.hasComponent<guillaume::components::Transform>(
					identifier)) {
				continue;
			}

			poses.push_back(
				components
					.getComponent<guillaume::components::Transform>(identifier)
					.getPose());
		}

		return poses;
	}

	void TestXIDER::expectUsableInterface(guillaume::Scene &scene, bool hasModels)
	{
		const std::vector<utility::graphic::PoseF> poses = collectPoses(scene);

		ASSERT_FALSE(poses.empty());

		std::size_t aside	 = 0;
		std::size_t onCamera = 0;

		for (const auto &pose: poses) {
			ASSERT_TRUE(isFinite(pose));

			// The framework only draws text and rectangles along the world
			// axes, so the interface is placed with an identity orientation.
			EXPECT_EQ(pose.getOrientation(), utility::graphic::OrientationF());

			// Everything stands between the camera and its far plane.
			EXPECT_GT(pose.getPosition().getZ(), -1000.0f);
			EXPECT_LE(pose.getPosition().getZ(), 0.0f);

			aside += pose.getPosition().getX() < 0.0f ? 1U : 0U;
			onCamera += pose.getPosition().getX() > 0.0f ? 1U : 0U;
		}

		EXPECT_GT(aside, 0U);

		if (hasModels) {
			EXPECT_GT(onCamera, 0U);
		}
	}

	TEST_F(TestXIDER, FrameUpdaterRunsTheCallbackItWasGiven)
	{
		FrameUpdater::clear();

		EXPECT_FALSE(FrameUpdater::isSet());

		const utility::graphic::ViewF view = makeView();

		// Running without a callback is what a frame between two scenes looks
		// like, and it must not do anything.
		EXPECT_NO_THROW(FrameUpdater::run(view));

		std::size_t runs						= 0;
		const utility::graphic::ViewF *received = nullptr;

		FrameUpdater::set(
			[&runs, &received](const utility::graphic::ViewF &current) {
				++runs;
				received = &current;
			});

		EXPECT_TRUE(FrameUpdater::isSet());

		FrameUpdater::run(view);

		EXPECT_EQ(runs, 1U);
		EXPECT_EQ(received, &view);

		FrameUpdater::clear();

		EXPECT_FALSE(FrameUpdater::isSet());

		FrameUpdater::run(view);

		EXPECT_EQ(runs, 1U);
	}

	TEST_F(TestXIDER, PreferencesStartAtTheirDocumentedValues)
	{
		Preferences preferences(*_localStorage);

		EXPECT_TRUE(preferences.areHintsShown());
		EXPECT_TRUE(preferences.areTexturesShown());
		EXPECT_FALSE(preferences.isPlaybackLoud());
	}

	TEST_F(TestXIDER, PreferencesAreReadBackByTheNextReader)
	{
		{
			Preferences preferences(*_localStorage);

			preferences.setHintsShown(false);
			preferences.setTexturesShown(false);
			preferences.setPlaybackLoud(true);
		}

		// A scene built after the application restarts reads what the previous
		// run wrote.
		Preferences restored(*_localStorage);

		EXPECT_FALSE(restored.areHintsShown());
		EXPECT_FALSE(restored.areTexturesShown());
		EXPECT_TRUE(restored.isPlaybackLoud());
	}

	TEST_F(TestXIDER, PreferencesResetLeavesNothingBehind)
	{
		Preferences preferences(*_localStorage);

		preferences.setHintsShown(false);
		preferences.setTexturesShown(false);
		preferences.setPlaybackLoud(true);
		preferences.reset();

		EXPECT_TRUE(preferences.areHintsShown());
		EXPECT_TRUE(preferences.areTexturesShown());
		EXPECT_FALSE(preferences.isPlaybackLoud());

		Preferences restored(*_localStorage);

		EXPECT_TRUE(restored.areHintsShown());
		EXPECT_TRUE(restored.areTexturesShown());
		EXPECT_FALSE(restored.isPlaybackLoud());
	}

	TEST_F(TestXIDER, HomeSceneSettlesItsInterfaceInFrontOfTheView)
	{
		const utility::graphic::ViewF view = makeView();

		scenes::Home scene(_ressourceProvider, *_localStorage,
						   *_sessionStorage);

		scene.onEnter();

		ASSERT_TRUE(FrameUpdater::isSet());

		settle(scene, view);
		expectUsableInterface(scene, true);

		scene.onExit();

		EXPECT_FALSE(FrameUpdater::isSet());
	}

	TEST_F(TestXIDER, ViewerSceneSettlesItsInterfaceInFrontOfTheView)
	{
		const utility::graphic::ViewF view = makeView();

		scenes::Viewer scene(_ressourceProvider, *_localStorage,
							 *_sessionStorage);

		scene.onEnter();

		ASSERT_TRUE(FrameUpdater::isSet());

		settle(scene, view);
		expectUsableInterface(scene, true);

		scene.onExit();

		EXPECT_FALSE(FrameUpdater::isSet());
	}

	TEST_F(TestXIDER, SoundSceneSettlesItsConsoleInFrontOfTheView)
	{
		const utility::graphic::ViewF view = makeView();

		scenes::Sound scene(_ressourceProvider, *_localStorage,
							*_sessionStorage);

		scene.onEnter();

		ASSERT_TRUE(FrameUpdater::isSet());

		settle(scene, view);
		expectUsableInterface(scene, false);

		scene.onExit();

		EXPECT_FALSE(FrameUpdater::isSet());
	}

	TEST_F(TestXIDER, SettingsSceneSettlesItsConsoleInFrontOfTheView)
	{
		const utility::graphic::ViewF view = makeView();

		scenes::Settings scene(_ressourceProvider, *_localStorage,
							   *_sessionStorage);

		scene.onEnter();

		ASSERT_TRUE(FrameUpdater::isSet());

		settle(scene, view);
		expectUsableInterface(scene, false);

		scene.onExit();

		EXPECT_FALSE(FrameUpdater::isSet());
	}

	TEST_F(TestXIDER, TheLayoutLeavesTheSceneAloneWhileNothingMoves)
	{
		const utility::graphic::ViewF view = makeView();

		scenes::Home scene(_ressourceProvider, *_localStorage,
						   *_sessionStorage);

		scene.onEnter();
		settle(scene, view);

		const std::vector<utility::graphic::PoseF> placed = collectPoses(scene);

		// Every following frame of a still camera has to leave the poses alone:
		// the text renderer rebuilds its text, and the model renderer reloads
		// its model from its file, whenever a pose changes.
		for (std::size_t frame = 0; frame < 3; ++frame) {
			FrameUpdater::run(view);

			EXPECT_EQ(collectPoses(scene), placed);
		}

		scene.onExit();
	}

	TEST_F(TestXIDER, TheLayoutHoldsItselfBackWithoutAFieldOfView)
	{
		// The framework hands out a view without a field of view until the
		// platform has described the one it renders with.
		const utility::graphic::ViewF view;

		scenes::Viewer scene(_ressourceProvider, *_localStorage,
							 *_sessionStorage);

		scene.onEnter();

		ASSERT_NO_THROW(settle(scene, view));

		for (const auto &pose: collectPoses(scene)) {
			EXPECT_TRUE(isFinite(pose));
		}

		scene.onExit();
	}

}	 // namespace xider::tests
