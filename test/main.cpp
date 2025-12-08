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
    Logger::setBaseLogLevel(MyEngine::Logger::Debug);
    Engine engine;
    engine.setFPS(60);
    engine.setLimitMaxMemorySize(1000000);
    std::list<Graphics::Ellipse> ellipse_list(1000);
    for (auto& e : ellipse_list) {
        e.reset(RandomGenerator::randFloat(0, 800.f),
                RandomGenerator::randFloat(0, 600.f),
                RandomGenerator::randFloat(100.f, 400.f),
                RandomGenerator::randFloat(100.f, 300.f),
                RandomGenerator::randUInt(0, 10),
                SDL_Color(static_cast<uint8_t>(RandomGenerator::randUInt(0, 255)),
                 static_cast<Uint8>(RandomGenerator::randUInt(0, 255)),
                 static_cast<Uint8>(RandomGenerator::randUInt(0, 255)),
                 128),
                SDL_Color{static_cast<Uint8>(RandomGenerator::randUInt(0, 255)),
                 static_cast<Uint8>(RandomGenerator::randUInt(0, 255)),
                 static_cast<Uint8>(RandomGenerator::randUInt(0, 255)), 64}, 0.f);
    }

    auto win = new Window(&engine, engine.applicationName());
    win->setResizable(true);
    win->renderer()->setDiscardCommandStrategy(Renderer::DiscardStrategy::Oldest);
    win->installPaintEvent([&ellipse_list, &engine, &win](Renderer* r) {
        win->setWindowTitle(std::format("{} FPS: {}", engine.applicationName(), engine.fps()));
        static float rotate = 0;
        if (rotate >= 360.f) rotate = 0;

        r->fillBackground(StdColor::DarkGray);
        rotate++;
        r->setBlendMode(SDL_BLENDMODE_BLEND);
        for (auto& e : ellipse_list) {
            e.setRotate(rotate);
            r->drawEllipse(e);
        }
        bool ok;
        auto st = SysMemory::getSystemMemoryStatus(&ok);
        size_t used = SysMemory::getCurProcUsedMemSize();
        if (ok) {
            r->drawPixelText(
                    std::format("Mem info: Used {} kB, Available {} kB", used, st.available_mem,
                                st.available_mem), {20, 20});
        } else {
            r->drawPixelText("Can't get system memory status!", { 20, 20 });
        }

    });

    return engine.exec();
}
