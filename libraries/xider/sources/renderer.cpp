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

#include "xider/renderer.hpp"

namespace xider
{
	Renderer::Renderer(void)
		: guillaume::Renderer()
		, _engine(nullptr)
	{
	}

	Renderer::Renderer(std::shared_ptr<evan::Engine> engine)
		: guillaume::Renderer()
		, _engine(engine)
	{
	}

	Renderer::~Renderer(void)
	{
	}

	void Renderer::setEngine(std::shared_ptr<evan::Engine> engine)
	{
		_engine = engine;
	}

	std::shared_ptr<evan::Engine> Renderer::getEngine(void) const
	{
		return _engine;
	}

	void Renderer::clear(void)
	{
	}

	void Renderer::present(void)
	{
		if (_engine) {
			_engine->render();
		}
	}

	void Renderer::drawVertices(
		const std::vector<utility::graphic::VertexF> &vertices)
	{
		(void)vertices;
	}

	utility::math::Vector2F
		Renderer::measureText(const utility::graphic::Text &text)
	{
		return { 0.0f, 0.0f };
	}

	guillaume::Renderer::ViewportSize Renderer::getViewportSize(void) const
	{
		return { 0.0f, 0.0f };
	}

	void Renderer::drawText(const utility::graphic::Text &text,
							const utility::graphic::PoseF &pose)
	{
		(void)text;	   // Suppress unused parameter warning
		(void)pose;	   // Suppress unused parameter warning
	}

}	 // namespace xider