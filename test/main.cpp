
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
    bool ok;
    auto new_pic = Algorithm::drawSurface(StdColor::White, 400, 300);
    Algorithm::writePixelsToSurface(new_pic, 0, 0, 200, 150, RGBAColor::RedBrick);
    Algorithm::writePixelsToSurface(new_pic, 200, 0, 400, 150, RGBAColor::GreenApple);
    Algorithm::writePixelsToSurface(new_pic, 0, 150, 200, 300, RGBAColor::BlueBaby);
    Algorithm::writePixelsToSurface(new_pic, 200, 150, 400, 300, RGBAColor::MixYellowLemon);
    auto color_map = Algorithm::readPixelsFromSurface(new_pic);
    color_map.reverse(true, true);
    Algorithm::writePixelsToSurface(new_pic, color_map, {0, 0});
    Texture ori_text(Algorithm::drawGraySurface(new_pic), win->renderer());
    Texture white_text(new_pic, win->renderer());
    white_text.property()->move(0, 450);
//    if (!ok) Logger::log("Failed to generate to white shape surface!", MyEngine::Logger::Error);
//    else {
        auto path = FileSystem::getAbsolutePath("./output.png").c_str();
        IMG_SavePNG(white_text.surface(), path);
        Logger::log(Logger::Debug, "Output: {}", path);
//    }
    win->show();
    win->setResizable(true);
    win->installPaintEvent([&](Renderer* r){
        r->fillBackground(RGBAColor::RedLightPink);
        static bool _t = false;
        if (!_t) {
            _t = true;
            r->setBlendMode(SDL_BLENDMODE_BLEND);
        }
        ori_text.draw();
        white_text.draw();
        r->drawDebugFPS({60, 20});
    });
    return engine.exec();
}