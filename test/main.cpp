#include "Core.h"
#include "Utils/Logger.h"
#include "MultiThread/Components.h"
#include "Utils/RGBAColor.h"
#include "Utils/Random.h"
#include "Utils/DateTime.h"
#include "UI/Button.h"
#include "Game/SpriteSheet.h"
#include "Game/Collider.h"
#include "Game/GObject.h"

using namespace MyEngine;

int main() {
//    Logger::setBaseLogLevel(MyEngine::Logger::Debug);
    Engine engine("MyEngine Test", "v1.0.0", "org.Myengine.test");
    engine.setFPS(120);
    auto win = new Window(&engine, engine.applicationName());
    win->setResizable(true);
    BGM bgm(AudioSystem::global()->mixer(), "bgm2.mp3");
    Sprite sprite("./back_button_1.png", win->renderer());
    sprite.setAnchors({sprite.size().width / 2, sprite.size().height / 2});
    sprite.move(100, 200);
    GObject obj("obj", &sprite);
    Graphics::Rectangle rect(100, 200, sprite.size().width, sprite.size().height, 2, RGBAColor::BlueDark, {});
    obj.setCollider(&rect);
    Graphics::Rectangle pgbar(0, 0, 0, 32, 0, {}, RGBAColor::RedFuchsia);
    Graphics::Point base(100, 200, 128, StdColor::Orange);
    Graphics::Point pt(100, 100, 20, RGBAColor::MixYellowTrans, 24);
    Graphics::Rectangle rt(100, 100, 40, 40, 10, RGBAColor::GreenMossTrans);
    Collider cl(&base), cl2(&rt);
    cl.setEnabled(true);
    cl2.setEnabled(true);
    obj.addTriggerToCollider(&cl2);
    obj.setColliderEnabled(true);
    cl2.appendCollider(&cl);

    TextureAnimation t_ani("./ani.gif", win->renderer());
    t_ani.property()->move(240, 200);
    t_ani.setDurationPerFrame(30);
    for (size_t i = 0; i < t_ani.framesCount(); ++i) {
        Logger::log(std::format("Index: {}, times: {}", i, t_ani.durationInFrame(i)), MyEngine::Logger::Info);
    }
    t_ani.play();

    win->installPaintEvent([&win, &cl, &cl2, &t_ani, &bgm, &obj, &pgbar](Renderer* r) {
        r->fillBackground(RGBAColor::MixBrownDark);
        auto per = (float)bgm.position() / (float)bgm.duration();
        pgbar.resize(static_cast<float>(win->geometry().width) * per, 20);
        r->drawRectangle(pgbar);
        obj.draw();
        obj.collider()->draw(r);
        cl.draw(r);
        cl2.draw(r);
        static float d = 0.01f;
        auto scale = t_ani.property()->scale();
        if (scale >= 4.f || scale <= 0.25f) d = -d;
        t_ani.property()->setScale(scale + d);
        t_ani.draw();
        r->drawPixelText(std::format("BGM: {} / {} speed: {:.2f}", bgm.position(), bgm.duration(), bgm.speedAndPitch()), { 20, 20 });
        r->drawPixelText(std::format("Animation: {} / {}", t_ani.currentFrame(), t_ani.framesCount()), {20, 30});
    });
    EventSystem::global()->appendEvent(IDGenerator::getNewEventID(), [&bgm, &rt, &obj](SDL_Event ev) {
        if (ev.key.down && ev.key.key == SDLK_SPACE) {
            auto p_status = bgm.playStatus();
            if (p_status == MyEngine::BGM::Loaded) bgm.play(0, true);
            else if (p_status == BGM::Playing) {
                bgm.pause();
            } else {
                bgm.resume();
            }
        }
        if (ev.key.down && ev.key.key == SDLK_UP) {
            bgm.setSpeedAndPitch(bgm.speedAndPitch() + 0.01f);
        }
        if (ev.key.down && ev.key.key == SDLK_DOWN) {
            bgm.setSpeedAndPitch(bgm.speedAndPitch() - 0.01f);
        }
        if (ev.key.down && ev.key.key == SDLK_ESCAPE) {
            bgm.stop(1000);
        }
        if (ev.key.down && ev.key.key == SDLK_Z) {
            obj.setScale(obj.scale() - 0.01f);
        }
        if (ev.key.down && ev.key.key == SDLK_X) {
            obj.setScale(obj.scale() + 0.01f);
        }
        obj.move(EventSystem::global()->captureMousePosition());
        if (obj.colliderTriggered(0)) {
            rt.setBackgroundColor(RGBAColor::RedCherryBlossom);
        } else {
            rt.setBackgroundColor(RGBAColor::RedLightPink);
        }
    });

    return engine.exec();
}
