
#include "FilledWin.h"
#include "MyEngine"
#include "Widgets/VerticalLayout.h"
#include "Widgets/HorizontalLayout.h"
#include "Widgets/Button.h"
#include "Widgets/LineEdit.h"
#include "MyWindow.h"

using namespace MyEngine;

int main(int argc, const char* argv[]) {
    Logger::setBaseLogLevel(MyEngine::Logger::Debug);
    FileSystem::setCurrentPath(FileSystem::getDirectoryFromFile(argv[0]));
    Engine engine("Test App", "v0.1.1", "test.app");
    engine.setApplicationTypeName("Unknown");
    engine.setApplicationAuthor("CatIsNotFound");
    engine.setApplicationCopyright("(C) 2026 Made by MyEngine");
    engine.setApplicationURL("https://github.com/CatIsNotFound/MyEngine");
    engine.setFPS(30);
    engine.setLimitMaxMemorySize(FileSystem::translateSize(100.f, MyEngine::FileSystem::KB));
    Logger::log(std::format("Mem: {} KB", engine.limitMaxMemorySize()));
    Graphics::Rectangle rect(20, 40, 120, 200, 0, {}, RGBAColor::BluePaleTrans);
    auto win = new MyWindow(&engine, engine.applicationName().data(), 1024, 800);
    win->show();
    win->setResizable(true);
    win->installPaintEvent([&](Renderer* r){
        r->fillBackground(RGBAColor::RedLightPink);
        static bool _t = false;
        if (!_t) {
            _t = true;
            r->setBlendMode(SDL_BLENDMODE_BLEND);
        }
        r->setViewport({200, 400, 200, 200});
        r->setClipView({0, 0, 300, 500});
        r->drawRectangle(&rect);
        r->setClipView({});
        r->setViewport({});
        r->drawDebugFPS({60, 20});
    });

    auto def_fonts_db = FontDatabase::getSystemDefaultFont();
    FontDatabase::FontInfo first_font = def_fonts_db.front();
    Widget::HorizontalLayout vlayout("vlayout", win);
    vlayout.setPadding(6);
    vlayout.setMargin(6);
    vlayout.setGeometry(60, 60, 800, 200);
    Widget::Button btn1("btn1", win), btn2("btn2", win);
    btn1.setFont(first_font.font_name, first_font.font_path, 24.f);
    btn2.setFont(first_font.font_name, first_font.font_path);
    btn1.setText("Button 1");
    btn2.setText("Button 2");
    btn2.setGeometry(20, 20, 180, 60);
    Widget::LineEdit edit1("edit1", win), edit2("edit2", win);
    edit1.setFont(first_font.font_name, first_font.font_path);
    edit2.setFont(first_font.font_name, first_font.font_path);
    edit1.setPlaceHolderText("Press me...");
    edit2.setPlaceHolderText("Press me...");
    vlayout.addWidgets({ &edit1, &btn1, &edit2 });
    btn1.setTriggerEvent([&vlayout] {
        vlayout.resize(RandomGenerator::randFloat(200.f, 800.f),
                       RandomGenerator::randFloat(200.f, 800.f));
    });
    btn2.setTriggerEvent([&vlayout] {
        vlayout.setVisible(!vlayout.visible());
    });
    Logger::log(Logger::Info, "Size: {}, {}, {}, {}",
                vlayout.position().x, vlayout.position().y,
                vlayout.size().width, vlayout.size().height);
    return engine.exec();
}