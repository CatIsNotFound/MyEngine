#include "Core.h"
#include "Utils/Logger.h"
#include "MultiThread/Components.h"
#include "Utils/RGBAColor.h"
#include "Utils/Random.h"
#include "Utils/SysMemory.h"
#include "UI/Button.h"
#include "Sprite/SpriteSheet.h"

using namespace MyEngine;

int main() {
    Engine engine;
    engine.setFPS(60);

    std::vector<Graphics::Ellipse> ellipse_list(1000);
    for (auto& e : ellipse_list) {
        e.reset(RandomGenerator::randFloat(0, 800.f),
                RandomGenerator::randFloat(0, 600.f),
                RandomGenerator::randFloat(1.f, 400.f),
                RandomGenerator::randFloat(1.f, 300.f), 1, StdColor::Black,
                SDL_Color{static_cast<Uint8>(RandomGenerator::randUInt(0, 255)),
                 static_cast<Uint8>(RandomGenerator::randUInt(0, 255)),
                 static_cast<Uint8>(RandomGenerator::randUInt(0, 255)), 192}, 0.f);
    }

    auto win = new Window(&engine, engine.applicationName());
//    win->setResizable(true);
//    win->setFullScreen(true);
    win->installPaintEvent([&ellipse_list, &engine](Renderer* r) {
        static float rotate = 0;
        if (rotate >= 360.f) rotate = 0;
        r->setBlendMode(SDL_BLENDMODE_BLEND);
        r->fillBackground(StdColor::DarkGray);
        rotate++;
        for (auto& e : ellipse_list) {
            e.setRotate(rotate);
            r->drawEllipse(e);
        }
        bool ok;
        auto st = SysMemory::getSystemMemoryStatus(&ok);
        size_t used = SysMemory::getCurProcUsedMemSize();
        r->drawPixelText(std::format("FPS: {}", engine.fps()), { 20, 20 });
        if (ok) {
            r->drawPixelText(
                    std::format("Mem info: Used {} kB, Available {} kB", used, st.available_mem,
                                st.available_mem), {20, 30});
        } else {
            r->drawPixelText("Can't get system memory status!", { 20, 30 });
        }
    });

    return engine.exec();
}
