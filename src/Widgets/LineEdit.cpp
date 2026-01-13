
#include "LineEdit.h"

namespace MyEngine::Widget {
    LineEdit::LineEdit(Window *window) : AbstractWidget(window) {
        init();
    }

    LineEdit::LineEdit(std::string object_name, Window *window)
            : AbstractWidget(std::move(object_name), window) {
        init();
    }

    LineEdit::~LineEdit() {

    }

    void LineEdit::setFont(const std::string &font_name, const std::string &font_path, float font_size) {
        if (!TextSystem::global()->isFontContain(font_name)) {
            TextSystem::global()->addFont(font_name, font_path,
                                          AbstractWidget::render(), font_size);
            TextSystem::global()->font(font_name)->setFontPath(font_path);
            TextSystem::global()->font(font_name)->setFontSize(font_size);
        }
        _font = TextSystem::global()->font(font_name);
        if (_text_id > 0) {
            auto font = property(ENGINE_PROP_FONT_NAME)->toString();
            auto f_size = property(ENGINE_PROP_FONT_SIZE)->toFloat();
            if (font != font_name) {
                setProperty(ENGINE_PROP_FONT_NAME, font_name);
                _changer_signal |= ENGINE_SIGNAL_LINE_EDIT_FONT_CHANGED;
            }
            if (f_size != font_size) {
                setProperty(ENGINE_PROP_FONT_SIZE, font_size);
                _changer_signal |= ENGINE_SIGNAL_LINE_EDIT_FONT_SIZE_CHANGED;
            }
        } else {
            _text_id = IDGenerator::getNewTextID();
            _place_text_id = IDGenerator::getNewTextID();
            TextSystem::global()->addText(_text_id, font_name, _none_str);
            TextSystem::global()->addText(_place_text_id, font_name, _none_str);
            bool is_contain = TextSystem::global()->isTextContain(_text_id);
            if (is_contain) {
                _text = TextSystem::global()->indexOfText(_text_id);
            }
        }
        _changer_signal |= ENGINE_SIGNAL_LINE_EDIT_TEXT_COLOR_CHANGED;
    }

    void LineEdit::setFont(const std::string &font_name) {
        if (!TextSystem::global()->isFontContain(font_name)) {
            Logger::log(std::format("LineEdit ({}): The font name '{}' is not contained! "
                                "You need to specified font path.", _object_name, font_name),Logger::Error);
            return;
        }
        _font = TextSystem::global()->font(font_name);
        if (_text_id > 0) {
            _changer_signal |= ENGINE_SIGNAL_LINE_EDIT_FONT_CHANGED;
            setProperty(ENGINE_PROP_FONT_NAME, font_name);
        } else {
            _text_id = IDGenerator::getNewTextID();
            if (TextSystem::global()->isTextContain(_text_id)) {
                TextSystem::global()->setTextFont(_text_id, font_name);
            } else {
                TextSystem::global()->addText(_text_id, font_name, text().data());
            }
        }
        _text = TextSystem::global()->indexOfText(_text_id);
        _status ^= ENGINE_BOOL_LINE_EDIT_PLACEHOLDER_TEXT_VISIBLE;
        _changer_signal |= ENGINE_SIGNAL_LINE_EDIT_TEXT_COLOR_CHANGED;
    }

    const std::string &LineEdit::fontName() const {
        return _text ? _text->font_name : _none_str;
    }

    const std::string &LineEdit::fontPath() const {
        auto font = fontName();
        return _text ? _text->font_name : _none_str;
    }

    void LineEdit::setText(const std::string &text) {
        _strings = Algorithm::splitUTF_8(text);
        _changer_signal |= ENGINE_SIGNAL_LINE_EDIT_TEXT_CHANGED;
    }

    std::string_view LineEdit::text() const {
        return {property(ENGINE_PROP_LINE_EDIT_TEXT)->toString()};
    }

    const StringList &LineEdit::textList() const {
        return _strings;
    }

    void LineEdit::setPlaceHolderText(const std::string &text) {
        setProperty(ENGINE_PROP_LINE_EDIT_PLACEHOLDER_TEXT, text);
        _changer_signal |= ENGINE_SIGNAL_LINE_EDIT_PLACEHOLDER_TEXT_CHANGED;
    }

    std::string_view LineEdit::placeHolderText() const {
        return {property(ENGINE_PROP_LINE_EDIT_PLACEHOLDER_TEXT)->toString()};
    }

    void LineEdit::setPasswordEnabled(bool enabled, const std::string &secret) {
        if (enabled) {
            _status |= ENGINE_BOOL_LINE_EDIT_PASSWORD_MODE;
            strncpy(_secret_char, secret.data(), sizeof(char) * 8);
        } else {
            _changer_signal ^= ENGINE_BOOL_LINE_EDIT_PASSWORD_MODE;
        }
    }

    bool LineEdit::passwordEnabled() const {
        return _status & ENGINE_BOOL_LINE_EDIT_PASSWORD_MODE;
    }

    void LineEdit::setTextColor(const SDL_Color &color) {
        auto _color = _GET_PROPERTY_PTR(this, ENGINE_PROP_LINE_EDIT_TEXT_COLOR, SDL_Color);
        _color->r = color.r; _color->g = color.g; _color->b = color.b; _color->a = color.a;
        _changer_signal |= ENGINE_SIGNAL_LINE_EDIT_TEXT_COLOR_CHANGED;
    }

    void LineEdit::setPlaceHolderTextColor(const SDL_Color &color) {
        auto _color = _GET_PROPERTY_PTR(this, ENGINE_PROP_LINE_EDIT_PLACE_HOLDER_TEXT_COLOR, SDL_Color);
        _color->r = color.r; _color->g = color.g; _color->b = color.b; _color->a = color.a;
        _changer_signal |= ENGINE_SIGNAL_LINE_EDIT_PLACEHOLDER_TEXT_COLOR_CHANGED;
    }

    const SDL_Color &LineEdit::textColor() const {
        return *_GET_PROPERTY_PTR(this, ENGINE_PROP_LINE_EDIT_TEXT_COLOR, SDL_Color);
    }

    const SDL_Color &LineEdit::placeHolderTextColor() const {
        return *_GET_PROPERTY_PTR(this, ENGINE_PROP_LINE_EDIT_PLACE_HOLDER_TEXT_COLOR, SDL_Color);
    }

    void LineEdit::setBackgroundColor(const SDL_Color &color, WidgetStatus status) {
        auto key = getBackgroundColorPropertyKey(status);
        auto _color = _GET_PROPERTY_PTR(this, key, SDL_Color);
        _color->r = color.r; _color->g = color.g; _color->b = color.b; _color->a = color.a;
    }

    void LineEdit::setBorderColor(const SDL_Color &color, WidgetStatus status) {
        auto key = getBorderColorPropertyKey(status);
        auto _color = _GET_PROPERTY_PTR(this, key, SDL_Color);
        _color->r = color.r; _color->g = color.g; _color->b = color.b; _color->a = color.a;
    }

    const SDL_Color &LineEdit::backgroundColor(WidgetStatus status) const {
        auto key = getBackgroundColorPropertyKey(status);
        return *_GET_PROPERTY_PTR(this, key, SDL_Color);
    }

    const SDL_Color &LineEdit::borderColor(WidgetStatus status) const {
        auto key = getBorderColorPropertyKey(status);
        return *_GET_PROPERTY_PTR(this, key, SDL_Color);
    }

    void LineEdit::setHorizontalPadding(int value) {
        setProperty(ENGINE_PROP_LINE_EDIT_PADDING_HORIZONTAL, value);
        auto geo = toGeometryInt(_trigger_area.geometry());
        _real_area.x = geo.x + value;
        _real_area.width = geo.width - value;
    }

    void LineEdit::setVerticalPadding(int value) {
        setProperty(ENGINE_PROP_LINE_EDIT_PADDING_VERTICAL, value);
        auto geo = toGeometryInt(_trigger_area.geometry());
        _real_area.y = geo.y + value;
        _real_area.height = geo.height - value;
    }

    void LineEdit::setPadding(int h, int v) {
        setProperty(ENGINE_PROP_LINE_EDIT_PADDING_HORIZONTAL, h);
        setProperty(ENGINE_PROP_LINE_EDIT_PADDING_VERTICAL, v);
        auto geo = toGeometryInt(_trigger_area.geometry());
        _real_area.setGeometry(geo.x + h, geo.y + v,
                               geo.width - h, geo.height - v);
    }

    void LineEdit::setPadding(int value) {
        setProperty(ENGINE_PROP_LINE_EDIT_PADDING_HORIZONTAL, value);
        setProperty(ENGINE_PROP_LINE_EDIT_PADDING_VERTICAL, value);
        auto geo = toGeometryInt(_trigger_area.geometry());
        int nx = geo.x + value, ny = geo.y + value,
            nw = geo.width - value, nh = geo.height - value;
        _real_area.setGeometry(nx, ny, nw, nh);
    }

    int LineEdit::horizontalPadding() const {
        return property(ENGINE_PROP_LINE_EDIT_PADDING_HORIZONTAL)->toInt32();
    }

    int LineEdit::verticalPadding() const {
        return property(ENGINE_PROP_LINE_EDIT_PADDING_VERTICAL)->toInt32();
    }

    void LineEdit::loadEvent() {
        AbstractWidget::loadEvent();
        auto status = WidgetStatus::Normal;
        if (isFocusEnabled()) status = WidgetStatus::Active;
        if (!enabled()) status = WidgetStatus::Disabled;
        updateStatus(status);
        _changer_signal |= ENGINE_SIGNAL_LINE_EDIT_PLACEHOLDER_TEXT_COLOR_CHANGED;
        _changer_signal |= ENGINE_SIGNAL_LINE_EDIT_TEXT_COLOR_CHANGED;
    }

    void LineEdit::unloadEvent() {
        AbstractWidget::unloadEvent();
    }

    void LineEdit::focusInEvent() {
        if (!(_status & ENGINE_BOOL_LINE_EDIT_HAS_TEXT) && (_status & ENGINE_BOOL_LINE_EDIT_PLACEHOLDER_TEXT_VISIBLE)) {
            _status ^= ENGINE_BOOL_LINE_EDIT_PLACEHOLDER_TEXT_VISIBLE;
        }
        updateStatus(WidgetStatus::Active);
    }

    void LineEdit::focusOutEvent() {
        if (!(_status & ENGINE_BOOL_LINE_EDIT_HAS_TEXT)) {
            _status |= ENGINE_BOOL_LINE_EDIT_PLACEHOLDER_TEXT_VISIBLE;
        }
        updateStatus(WidgetStatus::Normal);
    }

    void LineEdit::paintEvent(MyEngine::Renderer *renderer) {
        AbstractWidget::paintEvent(renderer);
        renderer->drawRectangle(&_trigger_area);

        if (_text) {
            bool show_text_mode = false;
            if (_status & ENGINE_BOOL_LINE_EDIT_HAS_TEXT) {
                show_text_mode = false;     // show text
            } else if (_status & ENGINE_BOOL_LINE_EDIT_PLACEHOLDER_TEXT_VISIBLE) {
                show_text_mode = true;      // show placeholder text
            }
            if (_changer_signal & ENGINE_SIGNAL_LINE_EDIT_TEXT_CHANGED) {
                auto text = Algorithm::mergeStringList(_strings);
                setProperty(ENGINE_PROP_LINE_EDIT_TEXT, text);
                TextSystem::global()->setText(_text_id, text);
            }
            if (_changer_signal & ENGINE_SIGNAL_LINE_EDIT_TEXT_COLOR_CHANGED) {
                auto color = *_GET_PROPERTY_PTR(this, ENGINE_PROP_LINE_EDIT_TEXT_COLOR, SDL_Color);
                TextSystem::global()->setTextColor(_text_id, color);
            }
            if (_changer_signal & ENGINE_SIGNAL_LINE_EDIT_PLACEHOLDER_TEXT_CHANGED) {
                auto text = property(ENGINE_PROP_LINE_EDIT_PLACEHOLDER_TEXT)->toString();
                TextSystem::global()->setText(_place_text_id, text);
            }
            renderer->setViewport(_real_area);
            if (show_text_mode) {
                TextSystem::global()->drawText(_place_text_id, _text_pos, renderer);
            } else {
                TextSystem::global()->drawText(_text_id, _text_pos, renderer);
            }
            renderer->setViewport({});
        }
        _changer_signal = 0;
    }

    void LineEdit::enableChangedEvent(bool enabled) {
        AbstractWidget::enableChangedEvent(enabled);
        updateStatus(enabled ? (isFocusEnabled() ? WidgetStatus::Active : WidgetStatus::Normal)
                                    : WidgetStatus::Disabled);
    }

    void LineEdit::resizeEvent(const Size &size) {
        AbstractWidget::resizeEvent(size);
        setPadding(horizontalPadding(), verticalPadding());
    }

    void LineEdit::keyDownEvent(SDL_Scancode scancode) {
        AbstractWidget::keyDownEvent(scancode);
        if (!isInputModeEnabled()) return;
        switch (scancode) {
            case SDL_SCANCODE_RETURN:
            case SDL_SCANCODE_KP_ENTER:
                setInputModeEnabled(false);
                break;
            case SDL_SCANCODE_BACKSPACE:
                if (!_strings.empty()) {
                    _strings.pop_back();
                    _changer_signal |= ENGINE_SIGNAL_LINE_EDIT_TEXT_CHANGED;
                }
                break;
            default:
                break;
        }
    }

    void LineEdit::mouseEnteredEvent() {
        AbstractWidget::mouseEnteredEvent();
        if (!isInputModeEnabled())
            updateStatus(WidgetStatus::Hovered);
    }

    void LineEdit::mouseLeftEvent() {
        AbstractWidget::mouseLeftEvent();
        if (!isInputModeEnabled())
            updateStatus(isFocusEnabled() ? WidgetStatus::Active : WidgetStatus::Normal);
    }

    void LineEdit::mouseClickedEvent() {
        AbstractWidget::mouseClickedEvent();
        if (!isInputModeEnabled()) {
            updateStatus(WidgetStatus::Input);
            setInputModeEnabled(true);
        }
    }

    void LineEdit::fingerTappedEvent() {
        AbstractWidget::fingerTappedEvent();
        if (!isInputModeEnabled()) {
            updateStatus(WidgetStatus::Input);
            setInputModeEnabled(true);
        }
    }

    void LineEdit::startedInputEvent() {
        AbstractWidget::startedInputEvent();
        Logger::log("Entered input mode");
        _trigger_area.setBorder(4, _trigger_area.borderColor());
    }

    void LineEdit::endedInputEvent() {
        AbstractWidget::endedInputEvent();
        updateStatus(isHovered() ? WidgetStatus::Hovered :
                     (isFocusEnabled() ? WidgetStatus::Active : WidgetStatus::Normal));
        Logger::log("Exited input mode");
        _trigger_area.setBorder(2, _trigger_area.borderColor());
    }

    void LineEdit::inputEvent(const char *string) {
        AbstractWidget::inputEvent(string);
        _strings.emplace_back(string);
        _changer_signal |= ENGINE_SIGNAL_LINE_EDIT_TEXT_CHANGED;
    }

    void LineEdit::textChangedEvent() {

    }

    void LineEdit::placeHolderTextVisibleChangedEvent(bool visible) {

    }

    void LineEdit::init() {
        setProperty(ENGINE_PROP_FONT_NAME);
        setProperty(ENGINE_PROP_FONT_SIZE, 9.f);
        setProperty(ENGINE_PROP_BACKGROUND_IMAGE_PATH);
        setProperty(ENGINE_PROP_BACKGROUND_IMAGE_SELF);
        setProperty(ENGINE_PROP_BACKGROUND_IMAGE_TEXTURE);
        setProperty(ENGINE_PROP_LINE_EDIT_TEXT, "");
        setProperty(ENGINE_PROP_LINE_EDIT_PLACEHOLDER_TEXT, "");
        setProperty(ENGINE_PROP_LINE_EDIT_PADDING_HORIZONTAL, 0);
        setProperty(ENGINE_PROP_LINE_EDIT_PADDING_VERTICAL, 0);
        _NEW_PROPERTY_WITH_DEFAULT_VALUE_PTR(this, ENGINE_PROP_LINE_EDIT_BACKGROUND_COLOR_NORMAL,
                                             SDL_Color, RGBAColor::BlueIce);
        _NEW_PROPERTY_WITH_DEFAULT_VALUE_PTR(this, ENGINE_PROP_LINE_EDIT_BORDER_COLOR_NORMAL,
                                             SDL_Color, RGBAColor::MixGrayLight);
        _NEW_PROPERTY_WITH_DEFAULT_VALUE_PTR(this, ENGINE_PROP_LINE_EDIT_BACKGROUND_COLOR_HOVERED,
                                             SDL_Color, RGBAColor::BlueLight);
        _NEW_PROPERTY_WITH_DEFAULT_VALUE_PTR(this, ENGINE_PROP_LINE_EDIT_BORDER_COLOR_HOVERED,
                                             SDL_Color, RGBAColor::BlueDark);
        _NEW_PROPERTY_WITH_DEFAULT_VALUE_PTR(this, ENGINE_PROP_LINE_EDIT_BACKGROUND_COLOR_ACTIVE,
                                             SDL_Color, RGBAColor::BlueIce);
        _NEW_PROPERTY_WITH_DEFAULT_VALUE_PTR(this, ENGINE_PROP_LINE_EDIT_BORDER_COLOR_ACTIVE,
                                             SDL_Color, RGBAColor::BlueDeepSky);
        _NEW_PROPERTY_WITH_DEFAULT_VALUE_PTR(this, ENGINE_PROP_LINE_EDIT_BACKGROUND_COLOR_INPUT,
                                             SDL_Color, RGBAColor::White);
        _NEW_PROPERTY_WITH_DEFAULT_VALUE_PTR(this, ENGINE_PROP_LINE_EDIT_BORDER_COLOR_INPUT,
                                             SDL_Color, RGBAColor::MixGrayDark);
        _NEW_PROPERTY_WITH_DEFAULT_VALUE_PTR(this, ENGINE_PROP_LINE_EDIT_BACKGROUND_COLOR_DISABLED,
                                             SDL_Color, RGBAColor::MixGrayLight);
        _NEW_PROPERTY_WITH_DEFAULT_VALUE_PTR(this, ENGINE_PROP_LINE_EDIT_BORDER_COLOR_DISABLED,
                                             SDL_Color, RGBAColor::MixGrayLight);
        _NEW_PROPERTY_WITH_DEFAULT_VALUE_PTR(this, ENGINE_PROP_LINE_EDIT_TEXT_COLOR,
                                             SDL_Color, RGBAColor::MixGrayDark);
        _NEW_PROPERTY_WITH_DEFAULT_VALUE_PTR(this, ENGINE_PROP_LINE_EDIT_PLACE_HOLDER_TEXT_COLOR,
                                             SDL_Color, RGBAColor::MixGray);
        _NEW_PROPERTY_WITH_DEFAULT_VALUE_PTR(this, ENGINE_PROP_LINE_EDIT_TEXT_COLOR,
                                             SDL_Color, RGBAColor::MixGrayDark);
        setCursor(Cursor::Edit);
        _trigger_area.setBorder(2, RGBAColor::MixGrayDark);
    }

    void LineEdit::updateStatus(WidgetStatus status) {
        _wid_status = status;

        auto bd_color = getBorderColor(status);
        auto bg_color = getBackgroundColor(status);

        _trigger_area.setBorderColor(bd_color);
        _trigger_area.setBackgroundColor(bg_color);
    }

    std::string LineEdit::getBorderColorPropertyKey(WidgetStatus status) {
        switch (status) {
            case WidgetStatus::Active:
                return ENGINE_PROP_LINE_EDIT_BORDER_COLOR_ACTIVE;
            case WidgetStatus::Disabled:
                return ENGINE_PROP_LINE_EDIT_BORDER_COLOR_DISABLED;
            case WidgetStatus::Hovered:
                return ENGINE_PROP_LINE_EDIT_BORDER_COLOR_HOVERED;
            case WidgetStatus::Input:
                return ENGINE_PROP_LINE_EDIT_BORDER_COLOR_INPUT;
            default:
                return ENGINE_PROP_LINE_EDIT_BORDER_COLOR_NORMAL;
        }
    }

    std::string LineEdit::getBackgroundColorPropertyKey(WidgetStatus status) {
        switch (status) {
            case WidgetStatus::Active:
                return ENGINE_PROP_LINE_EDIT_BACKGROUND_COLOR_ACTIVE;
            case WidgetStatus::Disabled:
                return ENGINE_PROP_LINE_EDIT_BACKGROUND_COLOR_DISABLED;
            case WidgetStatus::Hovered:
                return ENGINE_PROP_LINE_EDIT_BACKGROUND_COLOR_HOVERED;
            case WidgetStatus::Input:
                return ENGINE_PROP_LINE_EDIT_BACKGROUND_COLOR_INPUT;
            default:
                return ENGINE_PROP_LINE_EDIT_BACKGROUND_COLOR_NORMAL;
        }
    }

    SDL_Color LineEdit::getBackgroundColor(WidgetStatus status) {
        auto key = getBackgroundColorPropertyKey(status);
        if (hasProperty(key)) {
            return *_GET_PROPERTY_PTR(this, key, SDL_Color);
        }
        return SDL_Color(0, 0, 0, 255);
    }

    SDL_Color LineEdit::getBorderColor(WidgetStatus status) {
        auto key = getBorderColorPropertyKey(status);
        if (hasProperty(key)) {
            return *_GET_PROPERTY_PTR(this, key, SDL_Color);
        }
        return SDL_Color(0, 0, 0, 255);
    }
}
