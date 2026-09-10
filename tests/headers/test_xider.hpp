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

#include <filesystem>
#include <memory>
#include <vector>

#include <gtest/gtest.h>

#include <guillaume/local_storage.hpp>
#include <guillaume/scene.hpp>
#include <guillaume/session_storage.hpp>

#include <utility/graphic/pose.hpp>
#include <utility/graphic/view.hpp>
#include <utility/ressource_provider.hpp>
#include <utility/system_io/default_system_io.hpp>

namespace xider::tests
{
	/**
	 * @brief Test fixture giving each case a storage and a resource provider.
	 *
	 * The storages live in a directory of their own, removed once the case is
	 * over, so that the preferences one case writes are never read back by the
	 * next one. The provider is rooted in that same directory, which holds no
	 * asset: a scene has to survive the assets of the application being out of
	 * reach, as they are while the tests run.
	 */
	class TestXIDER: public ::testing::Test
	{
		protected:
		TestXIDER(void)			  = default;
		~TestXIDER(void) override = default;

		/**
		 * @brief Create the scratch directory and the storages of the case.
		 */
		void SetUp(void) override;

		/**
		 * @brief Release the storages and remove the scratch directory.
		 */
		void TearDown(void) override;

		/**
		 * @brief Build the view the application renders the demo with.
		 * @return A view with a field of view, looking down the Z axis.
		 */
		static utility::graphic::ViewF makeView(void);

		/**
		 * @brief Place the interface of a scene as a frame would.
		 * @param scene Scene to place.
		 * @param view View of the frame.
		 */
		static void settle(guillaume::Scene &scene,
						   const utility::graphic::ViewF &view);

		/**
		 * @brief Read the pose of every entity of a scene holding one.
		 * @param scene Scene to read.
		 * @return Poses of the scene, in breadth-first order.
		 */
		static std::vector<utility::graphic::PoseF>
			collectPoses(guillaume::Scene &scene);

		/**
		 * @brief Check that a settled scene holds a usable interface.
		 *
		 * Every pose has to be a number, to face the viewer the way the
		 * renderers of the framework can draw it, and to stand between the
		 * camera and its far plane. The console hangs on one side of the
		 * anchor, and the models of a scene on the other.
		 * @param scene Scene to check.
		 * @param hasModels Whether the scene displays models beside its
		 * console.
		 */
		static void expectUsableInterface(guillaume::Scene &scene,
										  bool hasModels);

		protected:
		std::filesystem::path
			_directory;	  ///< Directory holding the storages of the case.
		utility::DefaultSystemIO
			_systemIo;	  ///< File system the provider loads assets from.
		std::unique_ptr<guillaume::LocalStorage>
			_localStorage;	  ///< Storage the preferences are written to.
		std::unique_ptr<guillaume::SessionStorage>
			_sessionStorage;	///< Storage kept for the session.
		std::shared_ptr<utility::RessourceProvider>
			_ressourceProvider;	   ///< Provider the scenes load assets from.
	};

}	 // namespace xider::tests
