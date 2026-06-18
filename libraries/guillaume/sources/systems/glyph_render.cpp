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

#include "guillaume/systems/glyph_render.hpp"

namespace guillaume::systems
{
	void GlyphRender::prepare(void)
	{
		for (auto &[_, entry]: _cache) {
			entry.used = false;
		}

		if (!_glyphCodesLoaded) {
			_codePoints = _ressourceProvider->loadCodePoints(_glyphCodePath);
			if (_codePoints) {
				getLogger().info()
					<< "Loaded code points from " + _glyphCodePath;
			} else {
				getLogger().error()
					<< "Failed to load glyph code asset: " << _glyphCodePath;
			}
			_glyphCodesLoaded = true;
		}
	}

	void GlyphRender::cleanup(void)
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

	GlyphRender::GlyphRender(
		std::shared_ptr<utility::RessourceProvider> ressourceProvider,
		std::unique_ptr<Engine> &engine)
		: ecs::SystemFiller<components::Transform, components::Bound,
							components::Glyph, components::Color>(
			  ecs::Phase::Render)
		, _ressourceProvider(ressourceProvider)
		, _engine(engine)
		, _defaultFontPath(
			  "fonts/Material_Symbols_Outlined/"
			  "MaterialSymbolsOutlined-VariableFont_FILL,GRAD,opsz,wght.ttf")
		, _glyphCodePath(
			  "fonts/Material_Symbols_Outlined/"
			  "MaterialSymbolsOutlined[FILL,GRAD,opsz,wght].codepoints")
	{
	}

	GlyphRender::~GlyphRender(void)
	{
		for (auto &[_, entry]: _cache) {
			if (entry.objectId != 0) {
				_renderer->removeObject(entry.objectId);
			}
		}
	}

	void GlyphRender::update(const ecs::Entity::Identifier &entityIdentifier)
	{
		getLogger().debug()
			<< "Updating GlyphRender system for entity " << entityIdentifier;
		if (!requireComponent<components::Transform>(entityIdentifier)
			|| !requireComponent<components::Bound>(entityIdentifier)
			|| !requireComponent<components::Glyph>(entityIdentifier)
			|| !requireComponent<components::Color>(entityIdentifier)) {
			return;
		}

		const auto &transformComponent =
			getComponent<components::Transform>(entityIdentifier);
		const auto &boundComponent =
			getComponent<components::Bound>(entityIdentifier);
		const auto &glyphComponent =
			getComponent<components::Glyph>(entityIdentifier);
		const auto &colorComponent =
			getComponent<components::Color>(entityIdentifier);

		const std::string glyphName = glyphComponent.getName();

		getLogger().debug() << "Rendering glyph '" << glyphName
							<< "' for entity " << entityIdentifier;
		getLogger().debug() << "Glyph code found for '" << glyphName
							<< "': " << glyphComponent.getCode();

		uint32_t glyphCode = 0;
		if (_codePoints) {
			glyphCode = _codePoints->getCode(glyphName);
		}
		if (glyphCode == 0) {
			glyphCode = '?';
		}

		auto &cacheEntry = _cache[entityIdentifier];
		cacheEntry.used	 = true;

		const auto &currentPose = transformComponent.getPose();
		const auto &currentHeight = boundComponent.getHeight();
		const auto &currentColor = colorComponent.getColor();

		// If properties match cached values, skip Text object creation
		if (cacheEntry.objectId != 0
			&& cacheEntry.cachedGlyphName == glyphName
			&& cacheEntry.cachedHeight == currentHeight
			&& cacheEntry.cachedColor == currentColor
			&& cacheEntry.pose == currentPose) {
			getLogger().debug() << "GlyphRender: Skipping entity " << entityIdentifier 
								<< " - properties unchanged";
			return;
		}

		getLogger().info() << "GlyphRender: Creating new Text object for entity " 
						  << entityIdentifier << " (glyph: '" << glyphName 
						  << "', height: " << currentHeight << ")";

		utility::graphic::Text glyphText(
			_ressourceProvider, utility::graphic::CodePoints::toUtf8(glyphCode),
			currentHeight, _defaultFontPath);
		glyphText.setColor(currentColor);

		if (cacheEntry.objectId != 0) {
			_engine->removeObject(cacheEntry.objectId);
		}


		cacheEntry.objectId = _renderer->addText(glyphText, currentPose);

    cacheEntry.text		= glyphText;
		cacheEntry.pose		= currentPose;
		
		// Update cached properties
		cacheEntry.cachedGlyphName	= glyphName;
		cacheEntry.cachedHeight		= currentHeight;
		cacheEntry.cachedColor		= currentColor;
		cacheEntry.pose		= currentPose;
	}

}	 // namespace guillaume::systems
