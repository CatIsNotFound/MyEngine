
#include "Basic.h"

namespace S3GF {
    Geometry::Geometry() : x(0), y(0), width(0), height(0) {}

    Geometry::Geometry(int x, int y, int width, int height)
            : x(x), y(y), width(width), height(height) {}

    void Geometry::setGeometry(int x, int y, int width, int height) {
        this->x = x;
        this->y = y;
        this->width = width;
        this->height = height;
    }

    void Geometry::setPosition(int x, int y) {
        this->x = x;
        this->y = y;
    }

    void Geometry::resize(int width, int height) {
        this->width = width;
        this->height = height;
    }

    Vector2::Vector2(float x, float y) : x(x), y(y) {}

    void Vector2::reset(float x, float y) {
        this->x = x;
        this->y = y;
    }

    void Vector2::reset(const Vector2 &vector2) { reset(vector2.x, vector2.y); }

    Size::Size(float width, float height) : width(width), height(height) {}

    void Size::reset(float width, float height) {
        this->width = width;
        this->height = height;
    }

    void Size::reset(const Size &size) { reset(size.width, size.height); }

    // Graphics::Point::Point(Vector2 pos, const std::string &hex_color) :
    //     pos(pos), color(Algorithm::hexToRGBA(hex_color)){}

    // Graphics::Point::Point(float x, float y, const std::string &hex_color) :
    //     pos(x, y), color(Algorithm::hexToRGBA(hex_color)) {}

    // Graphics::Line::Line(float x1, float y1, float x2, float y2, uint8_t width, const std::string &hex_color) :
    //         start(x1, y1), end(x2, y2), width(width), color(Algorithm::hexToRGBA(hex_color)) {}

    // Graphics::Line::Line(Vector2 start, Vector2 end, uint8_t width, const std::string &hex_color) :
    //         start(start), end(end), width(width), color(Algorithm::hexToRGBA(hex_color)) {}

    // Graphics::Rectangle::Rectangle(Vector2 pos, Size size, const std::string &foreground, bool bordered_mode,
    //                                bool filled_mode, const std::string &background) :
    //                                pos(pos), size(size), fore_color(Algorithm::hexToRGBA(foreground)),
    //                                bordered_mode(bordered_mode), filled_mode(filled_mode),
    //                                back_color(Algorithm::hexToRGBA(background)) {}

    // Graphics::Rectangle::Rectangle(float x, float y, float width, float height, const std::string &foreground,
    //                                bool bordered_mode, bool filled_mode, const std::string& background) :
    //                                 pos(x, y), size(width, height), fore_color(Algorithm::hexToRGBA(foreground)),
    //                                 bordered_mode(bordered_mode), filled_mode(filled_mode),
    //                                 back_color(Algorithm::hexToRGBA(background)) {}


}