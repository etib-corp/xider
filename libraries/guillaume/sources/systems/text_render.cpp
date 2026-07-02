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

#include <utility/graphic/text/text.hpp>

#include "guillaume/systems/text_render.hpp"

namespace guillaume::systems
{
	TextRender::TextRender(
		std::shared_ptr<utility::RessourceProvider> ressourceProvider,
		std::unique_ptr<Engine> &engine)
		: ecs::SystemFiller<components::Transform, components::Text,
							components::Color>(ecs::Phase::Render)
		, _ressourceProvider(ressourceProvider)
		, _engine(engine)
		, _defaultFontPath("fonts/Roboto-Regular.ttf")
	{
	}

	TextRender::~TextRender(void)
	{
		clear();
	}

	void TextRender::prepare(void)
	{
		resetUsage();
	}

	void TextRender::cleanup(void)
	{
		auto unusedKeys = getUnusedKeys();
		for (const auto &key: unusedKeys) {
			if (auto *entry = findEntry(key)) {
				if (entry->value.has_value()) {
					_engine->removeObject(entry->value.value());
				}
			}
			erase(key);
		}
	}

	void TextRender::update(const ecs::Entity::Identifier &entityIdentifier)
	{
		getLogger().debug()
			<< "Updating TextRender system for entity " << entityIdentifier;
		if (!requireComponent<components::Transform>(entityIdentifier)
			|| !requireComponent<components::Text>(entityIdentifier)
			|| !requireComponent<components::Color>(entityIdentifier)) {
			return;
		}

		const auto &transformComponent =
			getComponent<components::Transform>(entityIdentifier);
		const auto &textComponent =
			getComponent<components::Text>(entityIdentifier);
		const auto &colorComponent =
			getComponent<components::Color>(entityIdentifier);

		TextRenderCacheKey cacheKey { transformComponent.getPose(),
									  textComponent.getContent(),
									  textComponent.getFontSize(),
									  colorComponent.getColor() };

		if (cacheKey.content.empty()) {
			return;
		}

		getLogger().debug() << "Rendering text for entity " << entityIdentifier
							<< " (content: '" << cacheKey.content
							<< "', fontSize: " << cacheKey.fontSize
							<< ", color: " << cacheKey.color << ")";

		if (contains(cacheKey)) {
			markAsUsed(cacheKey);
			getLogger().debug() << "Cache hit for entity " << entityIdentifier;
			return;
		}

		utility::graphic::Text text(_ressourceProvider, cacheKey.pose,
									cacheKey.color, cacheKey.content,
									cacheKey.fontSize, _defaultFontPath);

		auto identifier	 = _engine->addText(std::move(text));
		auto &cacheValue = getOrCreateEntry(cacheKey);
		cacheValue.value = identifier;
		cacheValue.used	 = true;
	}

}	 // namespace guillaume::systems