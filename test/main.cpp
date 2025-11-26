#include "Core.h"
#include "Utils/Logger.h"
#include "MultiThread/Components.h"
#include "Utils/RGBAColor.h"
#include "Controls/Button.h"
using namespace S3GF;

int main() {
    Logger::setBaseLogLevel(S3GF::Logger::DEBUG);
    Engine engine;
    engine.setFPS(60);
    auto bg_color = StdColor::GreenLight;
    auto main_win = new Window(&engine, "我的第一个窗口");
    main_win->setResizable(true);
    auto font_path = FontDatabase::findFontFromSystem("arial");
    TextSystem::global()->addFont("arial1", font_path, main_win->renderer());
    TextSystem::global()->addFont("arial2", font_path, main_win->renderer());
    Button button("btn", main_win->renderer());
    button.setFont("arial1");
    TextSystem::global()->font("arial1")->setFontSize(24.f);
    button.setText("Button 1");
    button.setKey(SDLK_SPACE);
    button.setActive();
    button.move(60, 60);
    button.resize(120, 70);
    button.setCheckable(true);
    button.setEnabled(false);
    button.setEvent([&bg_color] {
        bg_color = StdColor::RedLightPink;
    });
    Texture normal("./back_button_1.png", main_win->renderer());
    Texture hovered("./back_button_2.png", main_win->renderer());
    Texture pressed("./back_button_3.png", main_win->renderer());
    Texture invalid("./back_button_4.png", main_win->renderer());
    normal.property()->color_alpha = StdColor::BlueIce;
    hovered.property()->color_alpha = StdColor::BlueSea;
    pressed.property()->color_alpha = StdColor::BlueDark;
    TextureButton textureButton("test", main_win->renderer());
    textureButton.setTextures(&normal, &hovered, &pressed, &invalid);
    textureButton.move(400.f - textureButton.size().width / 2 + 40, 300.f - textureButton.size().height);
    textureButton.setKey(SDLK_RETURN);
    textureButton.setActive();
    textureButton.setFont("arial2");
    TextSystem::global()->font("arial2")->setFontSize(40.f);
    textureButton.setText("Hello!");
    auto& text_color_status = textureButton.textColor();
    text_color_status.active = StdColor::BlueDark;
    text_color_status.pressed = StdColor::BlueIce;
    auto size = textureButton.textSize();
    auto s_size = textureButton.geometry().size;
    textureButton.setTextPosition({s_size.width / 2 - size.width / 2, s_size.height / 2 - size.height / 2});
    textureButton.setTextVisible(true);
    textureButton.setEvent([&bg_color]{ bg_color = StdColor::BlueLight; });
    Timer timer(3000, [&button, &textureButton] {
        button.setEnabled(!button.enabled());
        textureButton.setEnabled(!textureButton.enabled());
        auto size = button.textSize();
        Logger::log(std::format("Size: {}x{}", size.width, size.height));
    });
    timer.start(0);
    main_win->installPaintEvent([&engine, &bg_color](Renderer* r) {
        r->fillBackground(bg_color);
        r->drawPixelText(std::format("FPS: {}, Button: {}",
                                     engine.fps(), EventSystem::global()->isMouseButtonDown() ? "true" : "false"), {20, 20});
    });
    Logger::log(std::format("Event count: {}", EventSystem::global()->eventCount()), S3GF::Logger::INFO);
    return engine.exec();
}
