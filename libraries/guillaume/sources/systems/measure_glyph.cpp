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
 FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT IN NO EVENT SHALL THE
 AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 SOFTWARE.
 */

#include <utility/graphic/text/text.hpp>

#include "guillaume/systems/measure_glyph.hpp"

namespace guillaume::systems
{

	MeasureGlyph::MeasureGlyph(
		std::shared_ptr<utility::RessourceProvider> ressourceProvider,
		std::unique_ptr<Engine> &engine)
		: ecs::SystemFiller<components::Glyph, components::Bound,
							components::Transform, components::Color>(
			  ecs::Phase::Measure)
		, _ressourceProvider(ressourceProvider)
		, _engine(engine)
		, _defaultFontPath(
			  "fonts/Material_Symbols_Outlined/"
			  "MaterialSymbolsOutlined-VariableFont_FILL,GRAD,opsz,wght.ttf")
	{
	}

	MeasureGlyph::~MeasureGlyph(void)
	{
	}

	void MeasureGlyph::update(const ecs::Entity::Identifier &entityIdentifier)
	{
		getLogger().debug()
			<< "Updating MeasureGlyph system for entity " << entityIdentifier;
		if (!requireComponent<components::Glyph>(entityIdentifier)
			|| !requireComponent<components::Bound>(entityIdentifier)
			|| !requireComponent<components::Transform>(entityIdentifier)
			|| !requireComponent<components::Color>(entityIdentifier)) {
			return;
		}

		const auto &glyphComponent =
			getComponent<components::Glyph>(entityIdentifier);
		auto &boundComponent =
			getComponent<components::Bound>(entityIdentifier);
		auto &transformComponent =
			getComponent<components::Transform>(entityIdentifier);
		auto &colorComponent =
			getComponent<components::Color>(entityIdentifier);

		const auto name		= glyphComponent.getName();
		const auto fontSize = glyphComponent.getFontSize();

		if (name.empty()) {
			return;
		}

		// We cannot use isValid() here because the criteria (name/size)
		// is not the same type as the value (width/height).
		auto entry = getEntry(entityIdentifier);
		if (entry && entry->value.has_value()) {
			boundComponent.setWidth(entry->value->x).setHeight(entry->value->y);
			return;
		}

		utility::graphic::Text text(
			_ressourceProvider, transformComponent.getPose(),
			colorComponent.getColor(), name, fontSize, _defaultFontPath);
		text.setColor(utility::graphic::Color32Bit());

		auto textSize = _engine->measureText(text);

		boundComponent.setWidth(textSize.getWidth())
			.setHeight(textSize.getHeight());

		getOrCreateEntry(entityIdentifier).value = utility::math::Vector2F(
			{ (float)textSize.getWidth(), (float)textSize.getHeight() });
	}

}	 // namespace guillaume::systems
