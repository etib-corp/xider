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

#include "guillaume/entities/button.hpp"

#include "guillaume/theme.hpp"

#include <utility>

namespace guillaume::entities
{

	Button::Button::Builder::Builder(ecs::ComponentRegistry &componentRegistry,
									 ecs::EntityRegistry &entityRegistry)
		: ecs::EntityBuilder(componentRegistry, entityRegistry)
	{
		reset();
	}

	Button::Builder::~Builder(void)
	{
	}

	ecs::Entity::Identifier Button::Builder::registerEntity(void)
	{
		_button = std::make_unique<Button>(
			this->getComponentRegistry(), _iconGlyphName, _iconStyle,
			_labelContent, _isToggle, _colorStyle, _shape, _size, _isMorph,
			_onClick);

		ecs::Entity::Identifier identifier = _button->getIdentifier();
		this->getEntityRegistry().addEntity(std::move(_button));
		return identifier;
	}

	void Button::Builder::reset(void)
	{
		_button.reset();
		_iconGlyphName.clear();
		_iconStyle = components::Glyph::Style::Outlined;
		_labelContent.clear();
		_isToggle	= false;
		_colorStyle = Color::Filled;
		_shape		= Shape::Round;
		_size		= Size::Small;
		_isMorph	= false;
		_onClick	= {};
	}

	Button::Builder &Button::Builder::withIcon(const std::string &iconGlyphName)
	{
		_iconGlyphName = iconGlyphName;
		return *this;
	}

	Button::Builder &Button::Builder::withIconStyle(
		const components::Glyph::Style &iconStyle)
	{
		_iconStyle = iconStyle;
		return *this;
	}

	Button::Builder &Button::Builder::withLabel(const std::string &labelContent)
	{
		_labelContent = labelContent;
		return *this;
	}

	Button::Builder &
		Button::Builder::withOnClick(const std::function<void(void)> &onClick)
	{
		_onClick = std::move(onClick);
		return *this;
	}

	Button::Builder &Button::Builder::withToggle(const bool &isToggle)
	{
		_isToggle = isToggle;
		return *this;
	}

	Button::Builder &Button::Builder::withColorStyle(const Color &colorStyle)
	{
		_colorStyle = colorStyle;
		return *this;
	}

	Button::Builder &Button::Builder::withShape(const Shape &shape)
	{
		_shape = shape;
		return *this;
	}

	Button::Builder &Button::Builder::withSize(const Size &size)
	{
		_size = size;
		return *this;
	}

	Button::Builder &Button::Builder::withMorph(const bool &isMorph)
	{
		_isMorph = isMorph;
		return *this;
	}

	Button::Director::Director(void)
		: ecs::EntityDirector()
	{
	}

	Button::Director::~Director(void)
	{
	}

	ecs::Entity::Identifier Button::Director::makeButton(
		Builder &builder, const std::string &labelContent,
		std::function<void(void)> onClick, Color colorStyle, Shape shape,
		Size size, bool isMorph)
	{
		return builder.withLabel(labelContent)
			.withOnClick(onClick)
			.withColorStyle(colorStyle)
			.withShape(shape)
			.withSize(size)
			.withMorph(isMorph)
			.registerEntity();
	}

	ecs::Entity::Identifier Button::Director::makeIconButton(
		Builder &builder, const std::string &labelContent,
		const std::string &iconGlyphName,
		const components::Glyph::Style &iconStyle,
		std::function<void(void)> onClick, Color colorStyle, Shape shape,
		Size size, bool isMorph)
	{
		return builder.withIcon(iconGlyphName)
			.withIconStyle(iconStyle)
			.withLabel(labelContent)
			.withOnClick(onClick)
			.withColorStyle(colorStyle)
			.withShape(shape)
			.withSize(size)
			.withMorph(isMorph)
			.registerEntity();
	}

	static float getHeightPadding(Button::Size size)
	{
		switch (size) {
			case Button::Size::ExtraSmall:
				return 6.0f;
			case Button::Size::Small:
				return 8.0f;
			case Button::Size::Medium:
				return 12.0f;
			case Button::Size::Large:
				return 24.0f;
			case Button::Size::ExtraLarge:
				return 32.0f;
			default:
				return 16.0f;
		}
	}

	static float getBorderRadius(Button::Size size, Button::Shape shape,
								 bool isPressed)
	{
		if (isPressed) {
			switch (size) {
				case Button::Size::ExtraSmall:
				case Button::Size::Small:
					return 8.0f;
				case Button::Size::Medium:
					return 12.0f;
				case Button::Size::Large:
				case Button::Size::ExtraLarge:
					return 16.0f;
				default:
					return 8.0f;
			}
		}

		if (shape == Button::Shape::Round) {
			return 100.0f;
		}

		switch (size) {
			case Button::Size::ExtraSmall:
			case Button::Size::Small:
				return 12.0f;
			case Button::Size::Medium:
				return 16.0f;
			case Button::Size::Large:
			case Button::Size::ExtraLarge:
				return 28.0f;
			default:
				return 12.0f;
		}
	}

	static Button::Shape getRestingShape(Button::Shape baseShape, bool isToggle,
										 bool isSelected)
	{
		if (!isToggle || !isSelected) {
			return baseShape;
		}

		return (baseShape == Button::Shape::Round) ? Button::Shape::Square
												   : Button::Shape::Round;
	}

	static float getFontSize(Button::Size size)
	{
		switch (size) {
			case Button::Size::ExtraSmall:
				return 15.0f;	 // 20 pixels
			case Button::Size::Small:
				return 15.0f;	 // 20 pixels
			case Button::Size::Medium:
				return 18.0f;	 // 24 pixels
			case Button::Size::Large:
				return 24.0f;	 // 32 pixels
			case Button::Size::ExtraLarge:
				return 30.0f;	 // 40 pixels
			default:
				return 15.0f;
		}
	}

	static float getWidthPadding(Button::Size size)
	{
		switch (size) {
			case Button::Size::ExtraSmall:
				return 12.0f;
			case Button::Size::Small:
				return 16.0f;
			case Button::Size::Medium:
				return 24.0f;
			case Button::Size::Large:
				return 48.0f;
			case Button::Size::ExtraLarge:
				return 64.0f;
			default:
				return 16.0f;
		}
	}

	static float getSpaceBetweenIconAndLabel(Button::Size size)
	{
		switch (size) {
			case Button::Size::ExtraSmall:
				return 4.0f;
			case Button::Size::Small:
				return 8.0f;
			case Button::Size::Medium:
				return 8.0f;
			case Button::Size::Large:
				return 12.0f;
			case Button::Size::ExtraLarge:
				return 16.0f;
			default:
				throw std::runtime_error("Invalid button size");
		}
	}

	static utility::graphic::Color32Bit
		getContainerColor(Button::Color style, bool isHovered, bool isPressed)
	{
		const auto &scheme = guillaume::defaultTheme.getScheme(
			guillaume::ThemeSchemeRole::Light);

		auto applyStateAlpha = [](const utility::graphic::Color32Bit &color,
								  std::uint8_t alpha) {
			return utility::graphic::Color32Bit(
				color.getRed(), color.getGreen(), color.getBlue(), alpha);
		};

		switch (style) {
			case Button::Color::Elevated:
				if (isPressed) {
					return applyStateAlpha(
						scheme.getColor(SchemeColorRole::SurfaceContainerHigh)
							.getColor(),
						220U);
				}
				if (isHovered) {
					return applyStateAlpha(
						scheme.getColor(SchemeColorRole::SurfaceContainer)
							.getColor(),
						235U);
				}
				return scheme.getColor(SchemeColorRole::SurfaceContainerLow)
					.getColor();
			case Button::Color::Filled:
				if (isPressed) {
					return applyStateAlpha(
						scheme.getColor(SchemeColorRole::Primary).getColor(),
						220U);
				}
				if (isHovered) {
					return applyStateAlpha(
						scheme.getColor(SchemeColorRole::Primary).getColor(),
						235U);
				}
				return scheme.getColor(SchemeColorRole::Primary).getColor();
			case Button::Color::Tonal:
				if (isPressed) {
					return applyStateAlpha(
						scheme.getColor(SchemeColorRole::SecondaryContainer)
							.getColor(),
						220U);
				}
				if (isHovered) {
					return applyStateAlpha(
						scheme.getColor(SchemeColorRole::SecondaryContainer)
							.getColor(),
						235U);
				}
				return scheme.getColor(SchemeColorRole::SecondaryContainer)
					.getColor();
			case Button::Color::Outlined:
				if (isPressed) {
					return applyStateAlpha(
						scheme.getColor(SchemeColorRole::OnSurfaceVariant)
							.getColor(),
						64U);
				}
				if (isHovered) {
					return applyStateAlpha(
						scheme.getColor(SchemeColorRole::OnSurfaceVariant)
							.getColor(),
						32U);
				}
				return utility::graphic::Color32Bit(0, 0, 0, 0);
			case Button::Color::Text:
				if (isPressed) {
					return applyStateAlpha(
						scheme.getColor(SchemeColorRole::Primary).getColor(),
						48U);
				}
				if (isHovered) {
					return applyStateAlpha(
						scheme.getColor(SchemeColorRole::Primary).getColor(),
						24U);
				}
				return utility::graphic::Color32Bit(0, 0, 0, 0);
		}
		return utility::graphic::Color32Bit(255, 241, 237, 255);
	}

	static utility::graphic::Color32Bit getContentColor(Button::Color style)
	{
		const auto &scheme = guillaume::defaultTheme.getScheme(
			guillaume::ThemeSchemeRole::Light);

		switch (style) {
			case Button::Color::Elevated:
				return scheme.getColor(SchemeColorRole::Primary).getColor();
			case Button::Color::Filled:
				return scheme.getColor(SchemeColorRole::OnPrimary).getColor();
			case Button::Color::Tonal:
				return scheme.getColor(SchemeColorRole::OnSecondaryContainer)
					.getColor();
			case Button::Color::Outlined:
				return scheme.getColor(SchemeColorRole::OnSurfaceVariant)
					.getColor();
			case Button::Color::Text:
				return scheme.getColor(SchemeColorRole::Primary).getColor();
		}
		return scheme.getColor(SchemeColorRole::Primary).getColor();
	}

	void Button::hoverHandler(void)
	{
	}

	void Button::unHoverHandler(void)
	{
	}

	void Button::leftClickPressHandler()
	{
		if (_onClick) {
			_onClick();
		}
	}

	void Button::leftClickReleaseHandler()
	{
	}

	Button::Button(ecs::ComponentRegistry &registry,
				   const std::string &iconGlyphName,
				   const components::Glyph::Style &iconStyle,
				   const std::string &labelContent, bool isToggle,
				   Color colorStyle, Shape shape, Size size, bool isMorph,
				   std::function<void(void)> onClick)
		: ecs::ParentEntityFiller<components::Transform, components::Bound,
								  components::Color, components::Borders,
								  components::Focus,
								  components::HandButtonInteraction,
								  components::HandHoverInteraction,
								  components::MouseHoverInteraction,
								  components::MouseButtonInteraction>(registry)
		, _iconGlyphName(iconGlyphName)
		, _iconStyle(iconStyle)
		, _labelContent(labelContent)
		, _iconIdentifier(ecs::Entity::InvalidIdentifier)
		, _labelIdentifier(ecs::Entity::InvalidIdentifier)
		, _isToggle(isToggle)
		, _colorStyle(colorStyle)
		, _shape(shape)
		, _size(size)
		, _isMorph(isMorph)
		, _onClick(std::move(onClick))
	{
		Icon::Builder iconBuilder(getComponentRegistry(), *this);
		Icon::Director iconDirector;
		Text::Builder labelBuilder(getComponentRegistry(), *this);
		Text::Director labelDirector;

		_iconIdentifier = iconDirector.makeIcon(
			iconBuilder, _iconGlyphName, getFontSize(_size),
			getContentColor(_colorStyle), _iconStyle);

		_labelIdentifier = labelDirector.makeText(labelBuilder, _labelContent,
												  getFontSize(_size),
												  getContentColor(_colorStyle));

		update();

		getComponentRegistry()
			.getComponent<components::HandButtonInteraction>(getIdentifier())
			.setOnButtonPressHandler(
				utility::event::HandButtonEvent::Button::A,
				std::bind(&Button::leftClickPressHandler, this))
			.setOnButtonReleaseHandler(
				utility::event::HandButtonEvent::Button::A,
				std::bind(&Button::leftClickReleaseHandler, this));

		getComponentRegistry()
			.getComponent<components::MouseButtonInteraction>(getIdentifier())
			.setOnButtonPressHandler(
				utility::event::MouseButtonEvent::Button::Left,
				std::bind(&Button::leftClickPressHandler, this))
			.setOnButtonReleaseHandler(
				utility::event::MouseButtonEvent::Button::Left,
				std::bind(&Button::leftClickReleaseHandler, this));

		getComponentRegistry()
			.getComponent<components::HandHoverInteraction>(getIdentifier())
			.setOnHoverHandler(std::bind(&Button::hoverHandler, this))
			.setOnUnhoverHandler(std::bind(&Button::unHoverHandler, this));

		getComponentRegistry()
			.getComponent<components::MouseHoverInteraction>(getIdentifier())
			.setOnHoverHandler(std::bind(&Button::hoverHandler, this))
			.setOnUnhoverHandler(std::bind(&Button::unHoverHandler, this));
	}

	Button::~Button()
	{
	}

	static const utility::graphic::PoseF
		applyLayerToPosition(const utility::graphic::PositionF &position,
							 const utility::graphic::OrientationF &orientation,
							 const std::uint32_t &layer)
	{
		const auto forwardVector					= orientation.getForward();
		utility::graphic::PositionF forwardPosition = position;
		forwardPosition.translate(utility::graphic::PositionF(
			-forwardVector * static_cast<float>(layer) * 1.0f));
		return utility::graphic::PoseF(forwardPosition, orientation);
	}

	static const utility::graphic::PoseF
		getIconPose(const utility::graphic::PoseF &buttonPose,
					const Button::Size &size, const std::uint32_t &buttonLayer)
	{
		const float widthPadding  = getWidthPadding(size);
		const float heightPadding = getHeightPadding(size);

		const float iconX = buttonPose.getPosition().x + widthPadding;
		const float iconY = buttonPose.getPosition().y - heightPadding;
		const float iconZ = buttonPose.getPosition().z;

		const utility::graphic::PositionF iconPosition(iconX, iconY, iconZ);

		const utility::graphic::OrientationF iconOrientation =
			buttonPose.getOrientation();

		auto iconPoseWithLatyer = applyLayerToPosition(
			buttonPose.getPosition(), buttonPose.getOrientation(), buttonLayer);

		return iconPoseWithLatyer;
	}

	static const utility::graphic::PoseF
		getIconPose(const utility::graphic::PoseF &buttonPose,
					const Button::Size &size, const std::uint32_t &buttonLayer,
					const float &iconWidth)
	{
		const float widthPadding  = getWidthPadding(size);
		const float heightPadding = getHeightPadding(size);
		const float spaceBetweenIconAndLabel =
			getSpaceBetweenIconAndLabel(size);

		const float iconX = buttonPose.getPosition().x + widthPadding
			+ iconWidth + spaceBetweenIconAndLabel;
		const float iconY = buttonPose.getPosition().y - heightPadding;
		const float iconZ = buttonPose.getPosition().z;

		const utility::graphic::PositionF iconPosition(iconX, iconY, iconZ);

		const utility::graphic::OrientationF iconOrientation =
			buttonPose.getOrientation();

		auto iconPoseWithLatyer = applyLayerToPosition(
			buttonPose.getPosition(), buttonPose.getOrientation(), buttonLayer);

		return iconPoseWithLatyer;
	}

	Button &Button::setIconGlyphName(const std::string &iconGlyphName)
	{
		_iconGlyphName = iconGlyphName;

		if (auto &icon = getEntity<Icon>(_iconIdentifier)) {
			icon->setGlyphName(_iconGlyphName);
		}

		return *this;
	}

	Button &Button::setIconStyle(const components::Glyph::Style &iconStyle)
	{
		_iconStyle = iconStyle;

		if (auto &icon = getEntity<Icon>(_iconIdentifier)) {
			icon->setStyle(_iconStyle);
		}

		return *this;
	}

	Button &Button::setLabelContent(const std::string &labelContent)
	{
		_labelContent = labelContent;

		if (auto &label = getEntity<Text>(_labelIdentifier)) {
			label->setContent(_labelContent);
		}

		return *this;
	}

	Button &Button::setIsToggle(const bool &isToggle)
	{
		_isToggle = isToggle;

		return *this;
	}

	Button &Button::setColorStyle(const Color &colorStyle)
	{
		_colorStyle = colorStyle;

		if (auto &icon = getEntity<Icon>(_iconIdentifier)) {
			icon->setColor(getContentColor(_colorStyle));
		}

		if (auto &label = getEntity<Text>(_labelIdentifier)) {
			label->setColor(getContentColor(_colorStyle));
		}

		return *this;
	}

	Button &Button::setShape(const Shape &shape)
	{
		bool isPressed = false;

		_shape = shape;

		isPressed =
			getComponentRegistry()
				.getComponent<components::HandButtonInteraction>(
					getIdentifier())
				.isButtonPressed(utility::event::HandButtonEvent::Button::A)
			? true
			: isPressed;
		isPressed =
			getComponentRegistry()
				.getComponent<components::MouseButtonInteraction>(
					getIdentifier())
				.isButtonPressed(utility::event::MouseButtonEvent::Button::Left)
			? true
			: isPressed;

		getComponentRegistry()
			.getComponent<components::Borders>(getIdentifier())
			.setBorderRadius(getBorderRadius(_size, _shape, isPressed));

		getComponentRegistry()
			.getComponent<components::Color>(getIdentifier())
			.setColor(getContainerColor(_colorStyle, false, isPressed));

		return *this;
	}

	Button &Button::setSize(const Size &size)
	{
		_size = size;

		if (auto &icon = getEntity<Icon>(_iconIdentifier)) {
			icon->setFontSize(getFontSize(_size));
		}

		if (auto &label = getEntity<Text>(_labelIdentifier)) {
			label->setFontSize(getFontSize(_size));
		}

		const auto &buttonPose =
			getComponentRegistry()
				.getComponent<components::Transform>(getIdentifier())
				.getPose();

		auto iconPose = getIconPose(buttonPose, _size, getLayer());

		getComponentRegistry()
			.getComponent<components::Transform>(_iconIdentifier)
			.setPose(iconPose);

		const auto &iconBound =
			getComponentRegistry().getComponent<components::Bound>(
				_iconIdentifier);

		auto labelPose =
			getIconPose(buttonPose, _size, getLayer(), iconBound.getWidth());

		getComponentRegistry()
			.getComponent<components::Transform>(_labelIdentifier)
			.setPose(labelPose);

		const auto &labelBound =
			getComponentRegistry().getComponent<components::Bound>(
				_labelIdentifier);

		auto buttonWidth = getWidthPadding(_size) * 2.0f + iconBound.getWidth()
			+ getSpaceBetweenIconAndLabel(_size) + labelBound.getWidth();

		getLogger().error() << "Entity ID: " << getIdentifier()
							<< ", width padding: " << getWidthPadding(_size)
							<< ", icon width: " << iconBound.getWidth()
							<< ", space between icon and label: "
							<< getSpaceBetweenIconAndLabel(_size)
							<< ", label width: " << labelBound.getWidth()
							<< ", total button width: " << buttonWidth;

		auto buttonHeight = 0.0f;

		buttonHeight = iconBound.getHeight() > buttonHeight
			? iconBound.getHeight()
			: buttonHeight;
		buttonHeight = labelBound.getHeight() > buttonHeight
			? labelBound.getHeight()
			: buttonHeight;

		buttonHeight += getHeightPadding(_size) * 2.0f;

		getLogger().error() << "Entity ID: " << getIdentifier()
							<< ", height padding: " << getHeightPadding(_size)
							<< ", icon height: " << iconBound.getHeight()
							<< ", label height: " << labelBound.getHeight()
							<< ", total button height: " << buttonHeight;

		getComponentRegistry()
			.getComponent<components::Bound>(getIdentifier())
			.setWidth(buttonWidth)
			.setHeight(buttonHeight);

		getLogger().error()
			<< "Button width: " << buttonWidth << ", height: " << buttonHeight;

		return *this;
	}

	Button &Button::setMorph(const bool &isMorph)
	{
		_isMorph = isMorph;
		return *this;
	}

	Button &Button::setOnClick(const std::function<void(void)> &onClick)
	{
		_onClick = onClick;
		return *this;
	}

	void Button::update(void)
	{
		setIconGlyphName(_iconGlyphName);
		setIconStyle(_iconStyle);
		setLabelContent(_labelContent);
		setIsToggle(_isToggle);
		setColorStyle(_colorStyle);
		setShape(_shape);
		setSize(_size);
		setMorph(_isMorph);
		setOnClick(_onClick);
	}
}	 // namespace guillaume::entities