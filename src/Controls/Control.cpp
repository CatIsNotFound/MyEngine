
#include "Control.h"

namespace S3GF {
    ClickArea::ClickArea(uint64_t window_id, GT graphic)
        : _base(std::move(graphic)), _winID(window_id) {
        EventSystem::global()->appendEvent(_next_id++, [this](SDL_Event ev) {
            if (ev.window.windowID != _winID) {
                _is_pressed = false;
                _is_hovered = false;
                return;
            }
            if (!_enabled) return;
            Vector2 cur_pos = Cursor::global()->position();
            int8_t tri = -1;
            switch (_base.index()) {
                case 1:
                    tri = Algorithm::comparePosInPoint(cur_pos, std::get<Graphics::Point>(_base));
                    break;
                case 2:
                    tri = Algorithm::comparePosInRect(cur_pos, std::get<Graphics::Rectangle>(_base));
                    break;
                default:
                    break;
            }
            if (tri > 0) {
                if (!_is_hovered) {
                    _is_hovered = true;
                }
                if (ev.button.type == SDL_EVENT_MOUSE_BUTTON_DOWN || ev.key.type == SDL_EVENT_KEY_DOWN) {
                    _is_down = true;
                } else if (ev.button.type == SDL_EVENT_MOUSE_BUTTON_UP || ev.key.type == SDL_EVENT_KEY_UP) {
                    _is_down = false;
                    _is_pressed = true;
                }
            } else {
                _is_hovered = false;
                _is_pressed = false;
                _is_entered = false;
            }
            if (_is_hovered && !_is_entered) _is_entered = true;
            if (_is_pressed && _func) {
                _func();
                _is_pressed = false;
            }
        });
    }

    void ClickArea::setEnabled(bool enabled) {
        _enabled = enabled;
    }

    bool ClickArea::enabled() {
        return _enabled;
    }

    void ClickArea::setPressedEvent(std::function<void()> function) {
        _func = std::move(function);
    }

    void ClickArea::resetPress() {
        _is_pressed = false;
    }

    void ClickArea::setHover(bool enabled) {
        _is_hovered = enabled;
    }

    void ClickArea::setGraphic(GT graphic) {
        _base = std::move(graphic);
    }

    bool ClickArea::setPoint(const Vector2& position, uint16_t size) {
        if (_base.index() != 1) return false;
        auto& pt = std::get<Graphics::Point>(_base);
        pt.reset(position, size, pt.color());
        return true;
    }

    bool ClickArea::setRect(const GeometryF& geometry) {
        if (_base.index() != 2) return false;
        auto& rect = std::get<Graphics::Rectangle>(_base);
        rect.setGeometry(geometry);
        return true;
    }

    Graphics::Point& ClickArea::point() {
        if (_base.index() != 1) {
            throw std::runtime_error("ClickArea: The graphic is not point!");
        }
        return std::get<Graphics::Point>(_base);
    }

    Graphics::Rectangle& ClickArea::rectangle() {
        if (_base.index() != 2) {
            throw std::runtime_error("ClickArea: The graphic is not rectangle!");
        }
        return std::get<Graphics::Rectangle>(_base);
    }

    bool ClickArea::getPoint(Graphics::Point& point) {
        if (_base.index() != 1) return false;
        point = std::get<Graphics::Point>(_base);
        return true;
    }

    bool ClickArea::getRect(Graphics::Rectangle& rect) {
        if (_base.index() != 2) return false;
        auto o_rect = std::get<Graphics::Rectangle>(_base);
        rect.reset(o_rect.geometry(), o_rect.borderSize(), o_rect.borderColor(), o_rect.backgroundColor());
        return true;
    }

    bool ClickArea::isPressed() const { return _is_pressed; }
    bool ClickArea::isHovered() const { return _is_hovered; }
    bool ClickArea::isEntered() const { return _is_entered; }
    bool ClickArea::isDown() const { return _is_down; }
    size_t ClickArea::index() const { return _base.index(); }

    AbstractControl::AbstractControl(const std::string& name, Renderer* renderer,
                                     GT click_area, AbstractControl* parent)
        : _name(name), _renderer(renderer), _parent(parent), _id(++_next_id),
          _click_area(renderer->window()->windowID(), std::move(click_area)) {

        renderer->window()->installPaintEvent([this] (Renderer* r) {
            if (!_visible) return;
            paintEvent(r);
        });

        EventSystem::global()->appendEvent(_next_id++, [this](SDL_Event ev) {
            if (!_visible || !_enabled) return;

            bool _entered_changed = (_entered != _click_area.isEntered());
            if (_entered_changed) {
                _entered = _click_area.isEntered();
                if (_entered) onEntered(); else onLeft();
            }
            if (_click_area.isHovered()) onHovered();
            bool _button_down_changed = (_is_down != _click_area.isDown());
            if (_button_down_changed) {
                _is_down = _click_area.isDown();
                if (_is_down) onPressed(); else onReleased();
            }
            if (_click_area.isPressed()) {
                onTriggered();
                _click_area.resetPress();
            }
        });

        if (_click_area.index() == 1) {
            Graphics::Point& pt = _click_area.point();
            _geometry.resetPos(pt.position());
            auto size = static_cast<float>(pt.size());
            _geometry.resetSize({size, size});
        } else if (_click_area.index() == 2) {
            Graphics::Rectangle& re = _click_area.rectangle();
            _geometry.reset(re.geometry());
        }
    }

    void AbstractControl::setName(const std::string& name) {
        _name = name;
    }

    const std::string& AbstractControl::name() const {
        return _name;
    }

    void AbstractControl::move(float x, float y) {
        _geometry.resetPos({x, y});
        _update_click_area();
        moveEvent();
    }

    void AbstractControl::move(const Vector2& pos) {
        _geometry.resetPos(pos);
        _update_click_area();
        moveEvent();
    }

    void AbstractControl::move(Vector2&& pos) {
        _geometry.resetPos(pos);
        _update_click_area();
        moveEvent();
    }

    void AbstractControl::resize(float w, float h) {
        _geometry.resetSize({w, h});
        _update_click_area();
        resizeEvent();
    }

    void AbstractControl::resize(const Size& size) {
        _geometry.resetSize(size);
        _update_click_area();
        resizeEvent();
    }

    void AbstractControl::resize(Size&& size) {
        _geometry.resetSize(size);
        _update_click_area();
        resizeEvent();
    }

    void AbstractControl::setGeometry(float x, float y, float w, float h) {
        _geometry.reset(x, y, w, h);
        _update_click_area();
        moveEvent();
        resizeEvent();
    }

    void AbstractControl::setGeometry(const Vector2& pos, const Size& size) {
        _geometry.reset(pos, size);
        _update_click_area();
        moveEvent();
        resizeEvent();
    }

    void AbstractControl::setGeometry(Vector2&& pos, Size&& size) {
        _geometry.reset(pos, size);
        _update_click_area();
        moveEvent();
        resizeEvent();
    }

    void AbstractControl::setGeometry(const GeometryF& geometry) {
        _geometry.reset(geometry);
        _update_click_area();
        moveEvent();
        resizeEvent();
    }

    void AbstractControl::setGeometry(GeometryF&& geometry) {
        _geometry.reset(geometry);
        _update_click_area();
        moveEvent();
        resizeEvent();
    }

    void AbstractControl::setVisible(bool visible) {
        _visible = visible;
        visibleChanged(visible);
    }

    void AbstractControl::setEnabled(bool enabled) {
        _enabled = enabled;
        enabledChanged(enabled);
    }

    void AbstractControl::_update_click_area() {
        if (_click_area.index() == 1) {
            auto min = std::min(_geometry.size.width, _geometry.size.height);
            auto real_size = std::min(static_cast<int>(min), UINT16_MAX);
            _click_area.setPoint(_geometry.pos, real_size);
        } else if (_click_area.index() == 2) {
            _click_area.setRect(_geometry);
            Logger::log(std::format("Changed: {}, {} {}x{}",
                    _geometry.pos.x, _geometry.pos.y, _geometry.size.width, _geometry.size.height));
        }
    }

}

