#include "Components.h"
#include "SDL3/SDL_render.h"
#include "src/Core.h"
#include <chrono>
#include <cstdint>
#include <format>
#include <memory>
#include <mutex>
#include <random>
#include <thread>

namespace S3GF {
    time_t RandomGenerator::time_seed{time(0)};
    int RandomGenerator::randInt(int start, int end) {
        std::default_random_engine rand_eng;
        rand_eng.seed(time_seed);
        std::uniform_int_distribution<int> r(start, end);
        return r(rand_eng);
    }

    int64_t RandomGenerator::randBigInt(int64_t start, int64_t end) {
        std::default_random_engine rand_eng;
        rand_eng.seed(time_seed);
        std::uniform_int_distribution<int64_t> r(start, end);
        return r(rand_eng);
    }

    uint32_t RandomGenerator::randUInt(uint32_t start, uint32_t end) {
        std::default_random_engine rand_eng;
        rand_eng.seed(time_seed);
        std::uniform_int_distribution<uint32_t> r(start, end);
        return r(rand_eng);
    }

    uint64_t RandomGenerator::randBigUInt(uint64_t start, uint64_t end) {
        std::default_random_engine rand_eng;
        rand_eng.seed(time_seed);
        std::uniform_int_distribution<uint64_t> r(start, end);
        return r(rand_eng);
    }

    float RandomGenerator::randFloat(float start, float end) {
        std::default_random_engine rand_eng;
        rand_eng.seed(time_seed);
        std::uniform_real_distribution<float> r(start, end);
        return r(rand_eng);
    }

    double RandomGenerator::randDouble(double start, double end) {
        std::default_random_engine rand_eng;
        rand_eng.seed(time_seed);
        std::uniform_real_distribution<double> r(start, end);
        return r(rand_eng);
    }


    Timer::~Timer() {
        stop();
        if (_thread.joinable()) {
            _thread.join();
        }
    }

    Timer::Timer(uint64_t delay, const std::function<void()>& event) 
        : _delay(delay), _function(event), _enabled(false) {
        _timer_id = RandomGenerator::randBigUInt(10000, 99999999);
        Logger::log(std::format("Timer ID {} is created", _timer_id));
    }

    void Timer::setDelay(uint64_t delay) {
        _delay = delay;
    }

    void Timer::start(uint32_t count) {
        if (_enabled) {
            Logger::log(std::format("The timer ID {} is already started! Please stop the current timer at first!", _timer_id), 
                Logger::ERROR);
            return;
        }
        _enabled = true;
        _run_count = count;
        _finish_count = 0;
        _current_time = SDL_GetTicks();
        _thread = std::thread(&Timer::running, this);
        Logger::log(std::format("The timer ID {} is started!", _timer_id));
    }

    void Timer::stop() {
        if (!_enabled) return;
        _enabled = false;
        Logger::log(std::format("Preparing to stop the timer ID {}!", _timer_id));
    }

    bool Timer::enabled() const {
        return _enabled;
    }

    uint64_t Timer::delay() const {
        return _delay;
    }

    void Timer::setEvent(const std::function<void()>& event) {
        _function = event;
    }

    bool Timer::isFinished() const {
        return _run_count == 0;
    }

    uint32_t Timer::triggeredCount() const {
        return _finish_count;
    }

    void Timer::running() {
        while (_enabled) {
            _lock.lock();
            SDL_Delay(1);
            _current_time = SDL_GetTicks();
            auto current_delay = _current_time - _start_time;
            if (current_delay >= _delay) {
                if (_function) {
                    _function();
                    _run_count -= 1;
                    _finish_count += 1;
                    Logger::log(std::format("Triggered the timer event by ID {}! Elapsed triggered count: {}", 
                            _timer_id, _run_count));
                    if (!_run_count) {
                        _enabled = false;
                    }
                    _start_time = SDL_GetTicks();
                }
            }
            _lock.unlock();
        }
        Logger::log(std::format("The timer ID {} is stopped running!", _timer_id));
    }

    Trigger::Trigger(const std::function<bool()>& condition, const std::function<void()>& event) 
        : _condition_function(condition), _function(event) {
        _trigger_id = RandomGenerator::randBigUInt(100000, 99999999);
        Logger::log(std::format("Trigger ID {} is loaded!", _trigger_id));
    }

    Trigger::~Trigger() {
        stop();
        if (_thread.joinable()) {
            _thread.join();
        }
    }

    void Trigger::setCondition(const std::function<bool()>& condition) {
        _condition_function = condition;
    }

    void Trigger::setEvent(const std::function<void()>& event) {
        _function = event;
    }

    void Trigger::start(uint32_t count) {
        if (_enabled) {
            Logger::log(std::format("Trigger ID {} is already started! "
                 "Please stop the current trigger at first", _trigger_id), Logger::ERROR);
            return;
        }
        _enabled = true;
        _run_count = count;
        _finish_count = 0;
        _thread = std::thread(&Trigger::running, this);
        Logger::log(std::format("Trigger ID {} is started!", _trigger_id));
    }

    void Trigger::stop() {
        _enabled = false;
        Logger::log(std::format("Trigger ID {} is preparing to stop the trigger!", _trigger_id));
    }

    bool Trigger::enabled() const {
        return _enabled;
    }

    bool Trigger::isTriggered() const {
        return _finish_count > 0;
    }

    uint64_t Trigger::triggeredCount() const {
        return _finish_count;
    }

    void Trigger::running() {
        while (_enabled) {
            _mutex.lock();
            SDL_Delay(1);
            bool c_switch = false;
            if (_condition_function) {
                c_switch = _condition_function();
            }
            if (c_switch && _function) {
                _function();
                _run_count -= 1;
                _finish_count += 1;
                if (!_run_count) {
                    _enabled = false;
                }
                Logger::log(std::format("Triggered the trigger event by ID {}, Elapsed count: {}", 
                    _trigger_id, _run_count));
            } 
            std::this_thread::sleep_for(std::chrono::milliseconds(50));
            _mutex.unlock();
        }
        Logger::log(std::format("Trigger ID {} is stopped!", _trigger_id));
    }


    Texture::Texture(const std::string &path, Renderer *renderer) : _renderer(renderer) {
        _surface = IMG_Load(path.c_str());
        if (!_surface) {
            Logger::log(std::format("The image path '{}' is not found!", path), Logger::ERROR);
            _property = std::make_unique<Property>();
            return;
        }
        _texture = SDL_CreateTextureFromSurface(_renderer->self(), _surface);
        _property = std::make_unique<Property>();
        _property->resize(_surface->w, _surface->h);
        _property->clip_mode = false;
        _property->color_alpha = StdColor::White;
        _property->setScale(1.0f);
        Logger::log(std::format("Texture created from image path '{}'", path), Logger::DEBUG);
        Logger::log(std::format("Texture size set to {}x{}", _surface->w, _surface->h), Logger::DEBUG);
    }

    Texture::Texture(SDL_Surface* surface, Renderer *renderer, bool deep_copy) : _renderer(renderer) {
        if (!_surface) {
            Logger::log(std::format("The surface is not valid!\nException: {}", SDL_GetError()), Logger::ERROR);
            _property = std::make_unique<Property>();
            return;
        }
        _surface = (deep_copy ? SDL_DuplicateSurface(surface) : surface);
        _texture = SDL_CreateTextureFromSurface(_renderer->self(), _surface);
        _property = std::make_unique<Property>();
        _property->resize(_surface->w, _surface->h);
        _property->clip_mode = false;
        _property->color_alpha = StdColor::White;
        _property->setScale(1.0f);
        Logger::log(std::format("Texture created from surface"), Logger::DEBUG);
        Logger::log(std::format("Texture size set to {}x{}", _surface->w, _surface->h), Logger::DEBUG);
    }

    Texture::Texture(Renderer* renderer, SDL_PixelFormat format, int width, int height, SDL_TextureAccess access)
        : _renderer(renderer), _surface(nullptr) {
        _texture = SDL_CreateTexture(renderer->self(), format, access, width, height);
        if (!_texture) {
            Logger::log(std::format("Created texture failed!\nException: {}", SDL_GetError()), Logger::ERROR);
            _property = std::make_unique<Property>();
            return;
        }
        _property = std::make_unique<Property>();
        _property->resize(width, height);
        _property->setScale(1.0f);
        _property->clip_mode = false;
        _property->color_alpha = StdColor::White;
        Logger::log(std::format("Texture created from custom"), Logger::DEBUG);
        Logger::log(std::format("Texture size set to {}x{}", width, height), Logger::DEBUG);
    }

    Texture::~Texture() {
        if (_texture) {
            SDL_DestroyTexture(_texture);
        }
        if (_surface) {
            SDL_DestroySurface(_surface);
        }
    }

    bool Texture::setImagePath(const std::string& path) {
        auto img = IMG_Load(path.c_str());
        if (!img) {
            Logger::log(std::format("The image path '{}' is not found!", path), Logger::ERROR);
            return false;
        }
        if (_texture) {
            SDL_DestroyTexture(_texture);
        }
        if (_surface) {
            SDL_DestroySurface(_surface);
        }
        _surface = img;
        _texture = SDL_CreateTextureFromSurface(_renderer->self(), _surface);
        _property->resize(_surface->w, _surface->h);
        Logger::log(std::format("Texture image changed to '{}'", path), Logger::DEBUG);
        Logger::log(std::format("Texture size updated to {}x{}", _surface->w, _surface->h), Logger::DEBUG);
        return true;
    }

    SDL_Texture* Texture::self() const {
        if (!_texture) {
            Logger::log("The texture is not created!", Logger::ERROR);
        }
        return _texture;
    }

    Property* Texture::property() {
        return _property.get();
    }

    void Texture::draw() const {
        if (!_texture) {
            Logger::log("The texture is not created!", Logger::ERROR);
            return;
        }
        _renderer->drawTexture(_texture, _property.get());
    }

}