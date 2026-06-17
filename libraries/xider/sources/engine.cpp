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

#include "xider/engine.hpp"

namespace xider
{
	Engine::Engine(std::unique_ptr<evan::Engine> engine)
		: _evanEngine(std::move(engine))
		, guillaume::Engine()
	{
	}

	Engine::~Engine(void)
	{
	}

	void Engine::clear(void)
	{
	}

	void Engine::present(void)
	{
		if (_evanEngine) {
			_evanEngine->render();
		}
	}

	size_t Engine::addMesh(const utility::graphic::Mesh &mesh,
						   const std::string &materialName)
	{
		return _evanEngine->addMesh(mesh, materialName);
	}

	bool Engine::removeObject(size_t objectID)
	{
		return _evanEngine->removeObject(objectID);
	}

	utility::graphic::ViewF Engine::getView(void) const
	{
		if (_evanEngine) {
			return _evanEngine->getView();
		}
		throw std::runtime_error("Engine not initialized, cannot get view");
	}

	utility::math::Vector2D
		Engine::measureText(const utility::graphic::Text &text) const
	{
		return text.getTextDimensions();
	}

	guillaume::Engine::ViewportSize Engine::getViewportSize(void) const
	{
		return { 0.0f, 0.0f };
	}

	size_t Engine::addText(const utility::graphic::Text &text,
						   const utility::graphic::PoseF &pose)
	{
		return _evanEngine->addText(
			std::make_shared<utility::graphic::Text>(text));
	}

	void Engine::addScene(size_t sceneIndex)
	{
		_evanEngine->addScene(sceneIndex);
	}

	void Engine::pollEvents(void)
	{
		auto events	  = _evanEngine->pollEvents();
		auto callback = this->getEventCallback();

		if (!callback) {
			getLogger().warning()
				<< "No event callback set, skipping event dispatch.";
			return;
		}
		for (auto &event: events) {
			callback(event);
		}
	}

	void Engine::update(void)
	{
		if (_evanEngine) {
			_evanEngine->update();
		}
	}
}	 // namespace xider
