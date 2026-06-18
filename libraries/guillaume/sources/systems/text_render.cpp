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
	void TextRender::prepare(void)
	{
		for (auto &[_, entry]: _cache) {
			entry.used = false;
		}
	}

	void TextRender::cleanup(void)
	{
		for (auto it = _cache.begin(); it != _cache.end();) {
			if (it->second.used) {
				++it;
				continue;
			}
			if (it->second.objectId != 0) {
				_engine->removeObject(it->second.objectId);
			}
			it = _cache.erase(it);
		}
	}

	TextRender::TextRender(
		std::shared_ptr<utility::RessourceProvider> ressourceProvider,
		std::unique_ptr<Engine> &engine)
		: ecs::SystemFiller<components::Transform, components::Text,
							components::Color>(ecs::Phase::Render)
		, _ressourceProvider(ressourceProvider)
		, _engine(engine)
		, _defaultFontPath("fonts/Roboto/Roboto-VariableFont_wdth,wght.ttf")
	{
	}

	TextRender::~TextRender(void)
	{
		for (auto &[_, entry]: _cache) {
			if (entry.objectId != 0) {
				_engine->removeObject(entry.objectId);
			}
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

		auto &cacheEntry = _cache[entityIdentifier];
		cacheEntry.used	 = true;


		const auto &currentContent = textComponent.getContent();
		const auto &currentFontSize = textComponent.getFontSize();
		const auto &currentColor = colorComponent.getColor();
		const auto &currentPose = transformComponent.getPose();

		if (cacheEntry.objectId != 0
			&& cacheEntry.cachedContent == currentContent
			&& cacheEntry.cachedFontSize == currentFontSize
			&& cacheEntry.cachedColor == currentColor
			&& cacheEntry.pose == currentPose) {
			getLogger().debug() << "TextRender: Skipping entity " << entityIdentifier 
								<< " - properties unchanged";
			return;
		}

		getLogger().info() << "TextRender: Creating new Text object for entity " 
						  << entityIdentifier << " (content: '" << currentContent 
						  << "', fontSize: " << currentFontSize << ")";

		utility::graphic::Text text(
			_ressourceProvider, currentContent, currentFontSize, _defaultFontPath);
		text.setColor(currentColor);

		if (cacheEntry.objectId != 0) {
			_engine->removeObject(cacheEntry.objectId);
		}

		cacheEntry.objectId = _engine->addText(text, currentPose);
		cacheEntry.text		= text;
		cacheEntry.pose		= currentPose;
		
		// Update cached properties
		cacheEntry.cachedContent	= currentContent;
		cacheEntry.cachedFontSize	= currentFontSize;
		cacheEntry.cachedColor		= currentColor;
		cacheEntry.pose				= currentPose;
	}

}	 // namespace guillaume::systems