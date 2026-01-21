
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
    auto win = new MyWindow(&engine, engine.applicationName().data());
    win->show();
    win->setResizable(true);
    win->installPaintEvent([&](Renderer* r){
        r->fillBackground(RGBAColor::RedLightPink);
        static bool _t = false;
        if (!_t) {
            _t = true;
            r->setBlendMode(SDL_BLENDMODE_BLEND);
        }
        r->drawDebugFPS({60, 20});
    });
    
    Widget::HorizontalLayout vlayout("vlayout", win);
    vlayout.setPadding(6);
    vlayout.setMargin(16);
    vlayout.setGeometry(60, 60, 800, 200);
    Widget::Button btn1("btn1", win), btn2("btn2", win);
    Widget::LineEdit edit1("edit1", win), edit2("edit2", win);
    vlayout.addWidgets({ &edit1, &btn1, &edit2, &btn2 });
    btn1.setTriggerEvent([&vlayout] {
        vlayout.resize(RandomGenerator::randFloat(200.f, 800.f),
                       RandomGenerator::randFloat(200.f, 800.f));
    });
    return engine.exec();
}