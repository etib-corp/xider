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

#include "guillaume/renderer.hpp"

namespace guillaume
{
	Renderer::Renderer(void)
		: Loggable()
	{
	}

	void Renderer::setView(const utility::graphic::ViewF &view)
	{
		_view = view;
	}

	/**
	 * @brief Get the full view model.
	 * @return The view instance.
	 */
	utility::graphic::ViewF Renderer::getView(void) const
	{
		return _view;
	}

	/**
	 * @brief Get the shared utility resource manager.
	 * @return Reference to the renderer resource manager.
	 */
	utility::RessourceManager &Renderer::getRessourceManager(void)
	{
		return _ressourceManager;
	}

	/**
	 * @brief Get the shared utility asset manager.
	 * @return Reference to the renderer asset manager.
	 */
	utility::DefaultAssetManager &Renderer::getAssetManager(void)
	{
		return _assetManager;
	}
}	 // namespace guillaume