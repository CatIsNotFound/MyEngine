#include "Core.h"
#include "Utils/Logger.h"
#include "MultiThread/Components.h"
#include "Utils/RGBAColor.h"
#include "Utils/Random.h"
#include "Controls/Button.h"

using namespace S3GF;

int main() {
    Logger::setBaseLogLevel(S3GF::Logger::DEBUG);
    Engine::disabledShowAppInfo();
    Engine engine;
    engine.setFPS(60);
    AudioSystem::global()->appendNewMixer(2);
    Logger::log(std::format("Mixer count: {}", AudioSystem::global()->mixerCount()));
    std::vector<std::shared_ptr<BGM>> bgm_list;
    bgm_list.push_back(std::make_shared<BGM>(AudioSystem::global()->mixer(), "./bgm.mp3"));
    bgm_list.push_back(std::make_shared<BGM>(AudioSystem::global()->mixer(1), "./bgm2.mp3"));
    SFX sfx(AudioSystem::global()->mixer(2), "./sound.oga");
    size_t index = 0;
    auto win = new Window(&engine, "Test window");
    win->installPaintEvent([&bgm_list, &index, &sfx](Renderer* r) {
        r->fillBackground(StdColor::DarkGray);
        r->drawPixelText(std::format("Playing BGM {}: {} / {}", index,
                                     bgm_list[index]->position(), bgm_list[index]->duration()), {20, 20});
        r->drawPixelText(std::format("SFX: {} / {}", sfx.position(), sfx.duration()), {20, 30});
    });
    EventSystem::global()->appendEvent(100, [&bgm_list, &sfx, &index](SDL_Event ev) {
        if (ev.key.type == SDL_EVENT_KEY_DOWN) {
            if (ev.key.key == SDLK_2) {
                index = 1;
                bgm_list[0]->stop(3000);
                bgm_list[1]->play(0, true, 5000);
            } else if (ev.key.key == SDLK_1) {
                index = 0;
                bgm_list[1]->stop(3000);
                bgm_list[0]->play(0, true, 5000);
            } else if (ev.key.key == SDLK_SPACE) {
                if (!bgm_list[index]->isPaused())
                    bgm_list[index]->pause();
                else
                    bgm_list[index]->play();
            } else if (ev.key.key == SDLK_LEFT) {
                bgm_list[index]->backward();
            } else if (ev.key.key == SDLK_RIGHT) {
                bgm_list[index]->forward();
            } else if (ev.key.key == SDLK_ESCAPE) {
                Engine::exit();
            } else {
                auto x = RandomGenerator::randFloat(-1.f, 1.f);
                auto y = RandomGenerator::randFloat(-1.f, 1.f);
                auto z = RandomGenerator::randFloat(-1.f, 1.f);
                auto r = RandomGenerator::randFloat(0.5f, 3.0f);
                sfx.play(false, r,{x, y, z});
                Logger::log(std::format("Play SFX: ({:.02f}, {:.02f}, {:.02f}), {:.02f}", x, y, z, r));
            }
        }
    });
    return engine.exec();
}
