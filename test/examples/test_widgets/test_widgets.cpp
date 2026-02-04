#include "MyEngine"

using namespace MyEngine;

SDL_Surface* drawColors() {
    Matrix2D<SDL_Color> _color_map(100, 100);
    auto start_color = StdColor::White;
    auto new_color = start_color;
    const uint8_t F = 2;
    for (uint32_t i = 0; i < 100; ++i) {
        _color_map.fillN({i, 0}, {i, 99}, new_color);
        new_color.r -= F; new_color.g -= F; new_color.b -= F;
    }
    return Algorithm::drawSurfaceByColorMap(_color_map);
}

int main() {
    Engine engine;
    engine.setFPS(60);
    auto window = new Window(&engine, "Hello world!");
    window->setResizable(true);
    window->show();

    auto def_fonts = FontDatabase::getSystemDefaultFont();
    auto main_font = def_fonts[0];
    auto sub_font = def_fonts[1];
    Widget::Label test_label_1("test_label_1", window);
    test_label_1.setGeometry(20, 20, 180, 60);
    test_label_1.setFont(main_font.font_name, main_font.font_path, 24.f);
    test_label_1.setText("Label 1");
    test_label_1.setBackgroundColor(RGBAColor::BlueLight);
    test_label_1.setTextColor(StdColor::Black);
    test_label_1.setTextAlignment(Widget::CenterMiddle);
    Widget::Label test_label_2("test_label_2", window);
    test_label_2.setGeometry(240, 20, 180, 60);
    test_label_2.setFont(main_font.font_name);
    test_label_2.setText("Label 2");
    test_label_2.setTextColor(StdColor::LightGray);
    test_label_2.setBackgroundColor(SDL_Color(243, 243, 243, 255));
    test_label_2.setTextAlignment(Widget::CenterMiddle);
    Widget::Label test_label_3("test_label_3", window);
    test_label_3.setGeometry(20, 100, 180, 60);
    test_label_3.setBackgroundImage(drawColors(), true);
    test_label_3.setBackgroundImageFillMode(Widget::Fill);
    test_label_3.setFont(sub_font.font_name, sub_font.font_path, 24.f);
    test_label_3.setTextColor(StdColor::Lavender);
    test_label_3.setText("Label 3");
    test_label_3.setTextAlignment(Widget::CenterMiddle);
    return engine.exec();
}
