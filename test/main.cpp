#include "FilledWin.h"
#include "MyEngine"
using namespace MyEngine;

int main() {
//    Logger::setBaseLogLevel(Logger::Debug);
//    Engine engine;
////    engine.setLimitMaxMemorySize(200'000);
//    engine.setFPS(60);
//    Matrix2D<int>::Position pos;
//    pos.row = 10;
//    pos.col = 20;
//    auto win2 = new Window(&engine, engine.applicationName());
//    UI::Button button("btn1", win2->renderer());
//    auto def = FontDatabase::getSystemDefaultFont();
//    auto font = def.front();
//    button.setText("Hello");
//    button.setFont(font.font_name, font.font_path);
//    button.setGeometry(200, 160, 200, 60);
//    TextSystem::global()->font(button.fontName())->setFontSize(24.f);
//    Logger::log(std::format("Loaded font: {} -> {}", font.font_name, font.font_path));
//    button.buttonSkin().background.normal = {};
//    button.setEvent([]{ Engine::exit(); });
//    button.setVisible(true);
//    button.resize(300, 60);
//    Graphics::Rectangle rect(300, 200, 80, 40, 1, StdColor::Black, RGBAColor::GreenMossTrans);
//    Graphics::Ellipse elli(300, 400, 100, 240, 1, StdColor::Red, {255, 255, 0, 128});
//    win2->installPaintEvent([&rect, &elli](Renderer* r) {
//        r->drawDebugFPS({20, 20}, StdColor::Black);
//        r->drawDebugText(std::format("Current Used: {}kB", SysMemory::getCurProcUsedMemSize()), {20, 30}, StdColor::Black);
//        static bool trigger = false;
//        if (!trigger) {
//            r->setBlendMode(SDL_BLENDMODE_BLEND);
//            trigger = true;
//        }
//        GeometryF geo = rect.geometry();
//        rect.move(EventSystem::global()->captureMousePosition() - Vector2{geo.size.width / 2, geo.size.height / 2});
//        static int b = 1.f;
//        auto border = rect.borderSize();
//        if (border >= 32 | border < 1) b = -b;
//        rect.setBorder(border + b, rect.borderColor());
//        r->drawEllipse(&elli);
//        r->drawRectangle(&rect);
//    });
//
//    Timer timer(1000, [&button] {
//        button.move(RandomGenerator::randFloat(0, 500), RandomGenerator::randFloat(0, 400));
//    });
//
//    timer.start(0);
//    return engine.exec();


    Matrix2D<int*> test(4, 3, nullptr, [](const int* data){ delete data; });
    Matrix2D<FontDatabase::FontInfo*> font2(3, 3, nullptr, [](FontDatabase::FontInfo* ptr){ delete ptr; });
    for (int i = 0; i < 10; ++i) {
        int* n = new int(RandomGenerator::randInt(0, 100));
        test[i] = n;
    }

    for (auto & i : test) {
        std::cout << (i ? *i : 0 ) << ", ";
    }
    test(13, 23);
    std::cout << std::endl;
    return 0;
}