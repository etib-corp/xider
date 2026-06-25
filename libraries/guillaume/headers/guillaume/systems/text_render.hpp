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

#include <map>
#include <optional>

#include "guillaume/systems/cache_system.hpp"
#include "guillaume/ecs/system_filler.hpp"

#include "guillaume/components/text.hpp"
#include "guillaume/components/transform.hpp"
#include "guillaume/components/color.hpp"

#include "guillaume/engine.hpp"

namespace guillaume::systems
{

	/**
	 * @brief System handling text rendering from ECS components.
	 * @see components::Text
	 * @see components::Transform
	 */
	class TextRender:
		public CacheSystem<ecs::Entity::Identifier, utility::graphic::Text>,
		public ecs::SystemFiller<components::Transform, components::Text,
								 components::Color>
	{
		private:
		struct ExtraCacheData {
			utility::graphic::PoseF pose;
			std::string cachedContent;
			uint32_t cachedFontSize { 0 };
			utility::graphic::Color32Bit cachedColor;
		};

		private:
		std::shared_ptr<utility::RessourceProvider>
			_ressourceProvider;	   ///< Shared resource provider for loading
								   ///< fonts and glyphs
		std::unique_ptr<Engine> &_engine;	 ///< Engine instance
		std::string _defaultFontPath;	 ///< Default font for text rendering
		std::map<ecs::Entity::Identifier, ExtraCacheData> _extraCache;

		public:
		/**
		 * @brief Construct a text rendering system.
		 * @param ressourceProvider Shared resource provider for loading fonts
		 * and glyphs.
		 * @param engine The engine used to draw text.
		 */
		TextRender(
			std::shared_ptr<utility::RessourceProvider> ressourceProvider,
			std::unique_ptr<Engine> &engine);

		/**
		 * @brief Default destructor.
		 */
		~TextRender(void);

		/**
		 * @brief Update the TextRender system for one entity.
		 * @param entityIdentifier The target entity identifier.
		 */
		virtual void
			update(const ecs::Entity::Identifier &entityIdentifier) override;
		void prepare(void) override;
		void cleanup(void) override;
	};

}	 // namespace guillaume::systems