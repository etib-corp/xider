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

#include <xider/xider.hpp>

#include <evan/Engine.hpp>

#include <evan/glfw/platform/LinuxDesktopPlatform.hpp>

int main(void)
{
	// Create Linux Desktop platform for graphics
	auto linuxDesktopPlatform =
		std::make_shared<evan::LinuxDesktopPlatform>("XIDER", 1280, 720);

	// Create Evan graphics engine with Linux Desktop platform
	auto evanEngine = std::make_shared<evan::Engine>(linuxDesktopPlatform);

	g_assetManager->add(std::string("assets/texture1.png"));


	// Initialize XIDER application with Evan engine
	xider::XIDER app(evanEngine);

	std::vector<std::string> texturePaths = {
		"assets/texture1.png",
	};

	std::map<std::string, std::vector<evan::Mesh>> meshData = {
		{ "assets/texture1.png",
		  { evan::Mesh { std::vector<evan::Vertex> {
							 evan::Vertex { { -0.5f, -0.5f, -2.0f },
											{ 0.0f, 0.0f, 0.0f },
											{ 0.0f, 0.0f } },
							 evan::Vertex { { 0.5f, -0.5f, -2.0f },
											{ 1.0f, 1.0f, 0.0f },
											{ 1.0f, 0.0f } },
							 evan::Vertex { { 0.5f, 0.5f, -2.0f },
											{ 1.0f, 1.0f, 0.0f },
											{ 1.0f, 1.0f } },
							 evan::Vertex { { -0.5f, 0.5f, -2.0f },
											{ 0.0f, 1.0f, 0.0f },
											{ 0.0f, 1.0f } },
						 },
						 std::vector<unsigned int> { 0, 1, 2, 2, 3, 0 } } } },
	};

	evanEngine->addScene(0, texturePaths, meshData);

	return app.run();
}