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

#include "test_xider.hpp"

#include <memory>

#include <xider/engine.hpp>

namespace xider::tests
{
	void TestXIDER::SetUp(void)
	{
	}

	void TestXIDER::TearDown(void)
	{
	}

	TEST_F(TestXIDER, EngineConstructsWithNullBackend)
	{
		// A null Evans backend is valid: the wrapper is constructed headless
		// and only dereferences the backend on non-guarded calls.
		EXPECT_NO_THROW(xider::Engine { nullptr });
	}

	TEST_F(TestXIDER, PresentIsSafeWithoutBackend)
	{
		xider::Engine engine { nullptr };

		EXPECT_NO_THROW(engine.present());
	}

	TEST_F(TestXIDER, UpdateIsSafeWithoutBackend)
	{
		xider::Engine engine { nullptr };

		EXPECT_NO_THROW(engine.update());
	}

	TEST_F(TestXIDER, ClearIsSafeWithoutBackend)
	{
		xider::Engine engine { nullptr };

		EXPECT_NO_THROW(engine.clear());
	}

	TEST_F(TestXIDER, ViewportCaptureDefaultsToOffWithoutBackend)
	{
		xider::Engine engine { nullptr };

		EXPECT_FALSE(engine.shouldCaptureViewportInput());
	}

	TEST_F(TestXIDER, ViewportCaptureSetterIsIgnoredWithoutBackend)
	{
		xider::Engine engine { nullptr };

		engine.setShouldCaptureViewportInput(true);

		EXPECT_FALSE(engine.shouldCaptureViewportInput());
	}

	TEST_F(TestXIDER, GetViewThrowsWithoutBackend)
	{
		xider::Engine engine { nullptr };

		EXPECT_THROW(engine.getView(), std::runtime_error);
	}
}	 // namespace xider::tests
