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

#include <string>

#include "guillaume/systems/cache_system.hpp"
#include "guillaume/ecs/system_filler.hpp"

#include "guillaume/components/bound.hpp"
#include "guillaume/components/glyph.hpp"
#include "guillaume/components/transform.hpp"
#include "guillaume/components/color.hpp"

#include "guillaume/engine.hpp"

namespace guillaume::systems
{

	/**
	 * @brief System measuring glyphs and synchronizing them to bound sizes.
	 * @see components::Glyph
	 * @see components::Bound
	 * @see components::Transform
	 */
	class MeasureGlyph:
		public CacheSystem<ecs::Entity::Identifier, utility::math::Vector2F>,
		public ecs::SystemFiller<components::Glyph, components::Bound,
								 components::Transform, components::Color>
	{
		private:
		std::shared_ptr<utility::RessourceProvider>
			_ressourceProvider;	   ///< Shared resource provider for loading
								   ///< fonts and glyphs
		std::unique_ptr<Engine>
			&_engine;	 ///< Engine instance for text measurement
		std::string
			_defaultFontPath;	 ///< Default font used for text measurement

		public:
		/**
		 * @brief Construct a glyph measuring system.
		 * @param ressourceProvider Shared resource provider for loading fonts
		 * and glyphs.
		 * @param engine The engine used to measure text.
		 */
		MeasureGlyph(
			std::shared_ptr<utility::RessourceProvider> ressourceProvider,
			std::unique_ptr<Engine> &engine);

		/**
		 * @brief Default destructor.
		 */
		~MeasureGlyph(void);

		/**
		 * @brief Update the MeasureGlyph system for one entity.
		 * @param entityIdentifier The target entity identifier.
		 */
		virtual void
			update(const ecs::Entity::Identifier &entityIdentifier) override;
	};

}	 // namespace guillaume::systems
