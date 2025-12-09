#include "Core.h"
#include "Utils/Logger.h"
#include "MultiThread/Components.h"
#include "Utils/RGBAColor.h"
#include "Utils/Random.h"
#include "UI/Button.h"
#include "Sprite/SpriteSheet.h"

using namespace MyEngine;

int main() {
    Logger::setBaseLogLevel(MyEngine::Logger::Debug);
    Engine engine;
    engine.setFPS(60);
    auto win = new Window(&engine, engine.applicationName());
    SFX sfx(AudioSystem::global()->mixer(), "bgm2.mp3");
    win->installPaintEvent([&sfx](Renderer* r) {
        r->fillBackground(RGBAColor::MixBrownDark);
        r->drawPixelText(std::format("SFX: {} / {} speed: {:.2f}", sfx.position(), sfx.duration(), sfx.speedAndPitch()), { 20, 20 });
    });
    EventSystem::global()->appendEvent(IDGenerator::getNewEventID(), [&sfx](SDL_Event ev) {
        if (ev.key.down && ev.key.key == SDLK_SPACE) {
            sfx.setSpeedAndPitch(RandomGenerator::randFloat(0.5f, 2.f));
            sfx.play();
        }
    });
    return engine.exec();
}
