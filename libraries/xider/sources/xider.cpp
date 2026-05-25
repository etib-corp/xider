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

#include "xider/xider.hpp"

namespace xider
{
	XIDER::XIDER(std::shared_ptr<evan::IPlatform> platform)
		: guillaume::Application<Renderer, EventHandler, scenes::Main,
								 scenes::Settings>()
	{
		getLogger().info("XIDER application initialized with Evan engine");
		// Configure the renderer with the Evan engine
		// The renderer is created by the Application template and is accessible
		// through the getRenderer() method provided by Guillaume's Application
		// class. We need to cast and set the engine on it.
		Renderer *renderer_ptr = dynamic_cast<Renderer *>(&getRenderer());
		renderer_ptr->setPlatform(platform);
		if (renderer_ptr && _engine) {
			renderer_ptr->setEngine(_engine);

			getLogger().info("Renderer configured with Evan engine");
		}
		EventHandler *eventHandler_ptr = dynamic_cast<EventHandler *>(&getEventHandler());
		if (eventHandler_ptr && _engine) {
			eventHandler_ptr->setEngine(_engine);
		}
	}

	XIDER::~XIDER(void)
	{
		getLogger().info("XIDER application destroyed");
	}

	std::shared_ptr<evan::Engine> XIDER::getEngine(void) const
	{
		return _engine;
	}

}	 // namespace xider