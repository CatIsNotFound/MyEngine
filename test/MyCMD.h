
#ifndef MYENGINE_TEST_MYCMD_H
#define MYENGINE_TEST_MYCMD_H
#include "MyEngine"

namespace MyEngine {
    class MyCMD : public RenderCommand::BaseCommand {
    public:
        explicit MyCMD(SDL_Renderer *renderer, const SDL_FRect rect, const SDL_Color &color)
                : RenderCommand::BaseCommand(renderer, "MyCMD"), _rect(rect), _color(color) {}

        void reset(SDL_Renderer *renderer, SDL_FRect rect, SDL_Color color) {
            _renderer = renderer;
            _rect = rect;
            _color = color;
        }

        void exec() override {
            SDL_SetRenderDrawColor(_renderer, _color.r, _color.g, _color.b, _color.a);
            auto _ret = SDL_RenderFillRect(_renderer, &_rect);
            if (!_ret) {
                Logger::log(std::format("Failed to render rectangle! Exception: {}", SDL_GetError()), Logger::Error);
                Engine::throwFatalError();
            }
        }

    private:
        SDL_FRect _rect;
        SDL_Color _color;
    };
}

#endif //MYENGINE_TEST_MYCMD_H
