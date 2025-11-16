#pragma once
#ifndef COMPONENTS_H
#define COMPONENTS_H
#include "Basic.h"

namespace S3GF {
    class RandomGenerator {
    public:
        RandomGenerator() = delete;
        RandomGenerator(RandomGenerator &&) = delete;
        RandomGenerator(const RandomGenerator &) = delete;
        RandomGenerator &operator=(RandomGenerator &&) = delete;
        RandomGenerator &operator=(const RandomGenerator &) = delete;
        ~RandomGenerator() = delete;
        
        static int randInt(int start, int end);
        static int64_t randBigInt(int64_t start, int64_t end);
        static uint32_t randUInt(uint32_t start, uint32_t end);
        static uint64_t randBigUInt(uint64_t start, uint64_t end);
        static float randFloat(float start, float end);
        static double randDouble(double start, double end);
        static time_t time_seed;
    };


    class Timer {
    public:
        Timer() = delete;
        Timer(Timer &&) = delete;
        Timer(const Timer &) = delete;
        Timer &operator=(Timer &&) = delete;
        Timer &operator=(const Timer &) = delete;
        ~Timer();
        explicit Timer(uint64_t delay, const std::function<void()>& event);
        void setDelay(uint64_t delay);
        void start(uint32_t count = 1);
        void stop();
        bool enabled() const;
        uint64_t delay() const;
        void setEvent(const std::function<void()>& event);
        bool isFinished() const;
        uint32_t triggeredCount() const;
    private:
        void running();
        uint32_t _delay;
        std::atomic<bool> _enabled;
        std::function<void()> _function;
        std::thread _thread;
        std::mutex _lock;
        uint32_t _run_count{0};
        uint32_t _finish_count{0};
        uint64_t _start_time{0};
        uint64_t _current_time{0};
        uint64_t _timer_id{0};
    };

    class Trigger {
    public:
        Trigger() = delete;
        Trigger(Trigger &&) = delete;
        Trigger(const Trigger &) = delete;
        Trigger &operator=(Trigger &&) = delete;
        Trigger &operator=(const Trigger &) = delete;
        
        explicit Trigger(const std::function<bool()>& condition, const std::function<void()>& event);
        ~Trigger();

        void setCondition(const std::function<bool()>& condition);
        void setEvent(const std::function<void()>& event);

        void start(uint32_t count = 1);
        void stop();
        bool enabled() const;
        bool isTriggered() const;
        uint64_t triggeredCount() const;
    private:
        void running();
        std::atomic<bool> _enabled;
        std::function<bool()> _condition_function;
        std::function<void()> _function;
        std::thread _thread;
        std::mutex _mutex;
        uint32_t _run_count{0};
        uint32_t _finish_count{0};
        uint64_t _trigger_id{0};
    };

    class Renderer;
    struct Property {
        bool clip_mode;
        SDL_FRect clip_area;
        SDL_Color color_alpha;
        double rotate_angle;
        void move(const Vector2& pos) {
            _position.reset(pos);
            setScale(_scale);
        }
        void move(float x, float y) {
            _position.reset(x, y);
            setScale(_scale);
        }
        const Vector2 position() const {
            return _position;
        }
        void resize(const Size& size) {
            _size.reset(size);
            setScale(_scale);
        }
        void resize(float width, float height) {
            _size.reset(width, height);
            setScale(_scale);
        }
        const Size size() const {
            return _size;
        }
        void setGeomentry(const Vector2& pos, const Size& size) {
            _position.reset(pos);
            _size.reset(size);
            setScale(_scale);
        }
        void setGeomentry(float x, float y, float width, float height) {
            _position.reset(x, y);
            _size.reset(width, height);
            setScale(_scale);
        }
        const GeometryF geomentry() const {
            return {_position, _size};
        }
        void setScale(float scale = 1.0f) {
            _scale = scale;
            Vector2 global_center_pos = {
                _position.x + _size.width / 2, _position.y + _size.height / 2
            };
            _scaled_position.reset((_position.x - global_center_pos.x) * _scale + global_center_pos.x, 
                                    (_position.y - global_center_pos.y) * _scale + global_center_pos.y);
            _scaled_size.reset(_size.width * _scale, _size.height * _scale);
        }
        float scale() const {
            return _scale;
        }
        GeometryF scaledGeometry() const {
            return {_scaled_position, _scaled_size};
        }
    private:
        Vector2 _position;
        Size _size;
        float _scale;
        Vector2 _scaled_position;
        Size _scaled_size;
    };
    class Texture {
    public:
        Texture(const Texture &) = delete;
        Texture(Texture &&) = delete;
        Texture &operator=(const Texture &) = delete;
        Texture &operator=(Texture &&) = delete;
        explicit Texture(const std::string &path, Renderer *renderer);
        explicit Texture(SDL_Surface* surface, Renderer *renderer, bool deep_copy = false);
        explicit Texture(Renderer* renderer, SDL_PixelFormat format, int width, int height, SDL_TextureAccess access);
        ~Texture();

        bool setImagePath(const std::string& path);

        SDL_Texture* self() const;
        Property* property();

        void draw() const;
    private:
        SDL_Surface* _surface;
        SDL_Texture* _texture;
        std::unique_ptr<Property> _property;
        Renderer* _renderer;
    };
}
#include "Core.h"
#endif // !COMPONENTS_H