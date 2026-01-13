
#include "FilledWin.h"
#include "MyEngine"
#include "Widgets/Button.h"
#include "Widgets/LineEdit.h"

using namespace MyEngine;

int main(int argc, const char* argv[]) {
    Logger::setBaseLogLevel(MyEngine::Logger::Warn);
    FileSystem::setCurrentPath(FileSystem::getDirectoryFromFile(argv[0]));
    Engine engine;
    engine.setFPS(60);
    engine.setLimitMaxMemorySize(FileSystem::translateSize(10.f, MyEngine::FileSystem::MB));
    auto win = new Window(&engine, "Test");
    Graphics::Rectangle progress(0, 0, 0, 0, 4, StdColor::Orange, StdColor::Yellow);
    BGM bgm(AudioSystem::global()->mixer(), FileSystem::getAbsolutePath("./main.mp3"));
    bgm.setVolume(0.10f);
    bgm.play(0, true);
    Widget::LineEdit line_edit("line_edit", win);
    win->setResizable(true);
    win->installPaintEvent([&](Renderer* r) {
        r->fillBackground(RGBAColor::BlueLake);
        r->drawRectangle(&progress);
        r->drawDebugFPS();
        r->drawDebugText(std::format("BGM: {} / {}", bgm.position(), bgm.duration()), {20, 30});
    });
    auto font_map = FontDatabase::getFontDatabaseFromSystem();
    FontDatabase::FontInfo def_font = {"simsun", font_map.at("simsun")};
    line_edit.setGeometry(20, 80, 1024, 48);
    line_edit.setFont(def_font.font_name, def_font.font_path, 24.f);
//    line_edit.setFocusEnabled(true);
//    line_edit.setFocusEnabled(true);
    line_edit.setText("123abc");
    line_edit.setPadding(90);
    EventSystem::global()->appendGlobalEvent(IDGenerator::getNewGlobalEventID(), [&] {
        float p = (float) (bgm.position()) / (float) (bgm.duration());
        progress.resize(win->geometry().width * p, 20);
    });
    engine.exec();
    return 0;
}