
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
    Texture ori_text(FileSystem::getAbsolutePath("./cut1.jpg"), win->renderer());
    Texture white_text(Algorithm::drawGrayScale2Surface(ori_text.surface(), &ok), win->renderer());
    white_text.property()->move(0, 450);

    if (!ok) Logger::log("Failed to generate to white shape surface!", MyEngine::Logger::Error);
    else {
        auto path = FileSystem::getAbsolutePath("./output.png").c_str();
        IMG_SavePNG(white_text.surface(), path);
        Logger::log(Logger::Debug, "Output: {}", path);
    }
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
        ori_text.draw();
        white_text.draw();
    });
    win->close();
    return engine.exec();
}