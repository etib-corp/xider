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

#include "xider/engine.hpp"

namespace xider
{
	XIDER::XIDER(std::shared_ptr<evan::IPlatform> platform,
				 std::shared_ptr<utility::RessourceProvider> ressourceProvider)
		: guillaume::Application<scenes::Main, scenes::Main, scenes::Settings>(
			  ressourceProvider)
	{
		getLogger().info() << "XIDER application initialized with Evan engine";

		std::unique_ptr<evan::Engine> evanEngine =
			std::make_unique<evan::Engine>(getRessourceProvider(), platform);

		std::unique_ptr<Engine> xiderEngine =
			std::make_unique<Engine>(std::move(evanEngine));

		setEngine(std::move(xiderEngine));
	}

	XIDER::~XIDER(void)
	{
		getLogger().info() << "XIDER application destroyed";
	}
}	 // namespace xider