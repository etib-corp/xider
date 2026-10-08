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

#include <type_traits>

#include <evan/Engine.hpp>
#include <utility/engine.hpp>

#include <xider/scenes/showcase.hpp>

namespace xider::tests
{
	void TestXIDER::SetUp(void)
	{
	}

	void TestXIDER::TearDown(void)
	{
	}

	TEST_F(TestXIDER, EvanImplementsUtilityEngine)
	{
		EXPECT_TRUE((std::is_base_of_v<utility::Engine, evan::Engine>));
	}

	TEST_F(TestXIDER, ShowcaseDerivesFromGuillaumeScene)
	{
		EXPECT_TRUE(
			(std::is_base_of_v<guillaume::Scene, xider::scenes::Showcase>));
	}

}	 // namespace xider::tests
