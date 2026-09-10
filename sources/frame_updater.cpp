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

#include <utility>

#include "xider/frame_updater.hpp"

namespace xider
{
	FrameUpdate &FrameUpdater::callback(void)
	{
		static FrameUpdate instance;

		return instance;
	}

	void FrameUpdater::set(FrameUpdate callback)
	{
		FrameUpdater::callback() = std::move(callback);
	}

	void FrameUpdater::clear(void)
	{
		FrameUpdater::callback() = nullptr;
	}

	void FrameUpdater::run(const utility::graphic::ViewF &view)
	{
		const FrameUpdate &callback = FrameUpdater::callback();

		if (callback) {
			callback(view);
		}
	}

	bool FrameUpdater::isSet(void)
	{
		return static_cast<bool>(FrameUpdater::callback());
	}

}	 // namespace xider
