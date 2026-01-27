
#ifndef MYENGINE_ALGORITHM_RGBAPIXELS_H
#define MYENGINE_ALGORITHM_RGBAPIXELS_H
#include "../Components.h"
#include "../Core.h"
#include "../Utils/RGBAColor.h"

namespace MyEngine {
    namespace Algorithm {
        inline SDL_Color readPixelFromSurface(int x, int y, SDL_Surface* surface, bool* ok = nullptr,
                                              const SDL_PixelFormatDetails* format = nullptr,
                                              const SDL_Palette* palette = nullptr) {
            if (!surface) {
                Logger::log("readPixelFromSurface: The specified surface is not valid!", Logger::Error);
                if (ok) *ok = false;
                return {};
            }
            if (surface->w <= x || surface->h <= y) {
                Logger::log(Logger::Error, "readPixelFromSurface: Out of range: Failed to read pixel "
                       "from the specified surface at position ({}, {}), the original surface size is {}x{}!",
                        x, y, surface->w, surface->h);
                if (ok) *ok = false;
                return {};
            }
            auto pixel_buffer = static_cast<uint8_t*>(surface->pixels);
            const SDL_PixelFormatDetails* n_format = format ? format : SDL_GetPixelFormatDetails(surface->format);
            const SDL_Palette* n_palette = palette ? palette : SDL_GetSurfacePalette(surface);
            int pitch = surface->pitch;
            int pixel_bytes = SDL_BYTESPERPIXEL(surface->format);
            int pixel_offset = y * pitch + x * pixel_bytes;
            auto pixel_ptr = reinterpret_cast<uint32_t*>(pixel_buffer + pixel_offset);
            SDL_Color _ret = {};
            SDL_GetRGBA(*pixel_ptr, n_format, n_palette, &_ret.r, &_ret.g, &_ret.b, &_ret.a);
            if (ok) *ok = true;
            return _ret;
        }

        inline bool writePixelToSurface(int x, int y, SDL_Color new_color, SDL_Surface* surface) {
            if (!surface) {
                Logger::log("writePixelFromSurface: The specified surface is not valid!", Logger::Error);
                return false;
            }
            if (surface->w <= x || surface->h <= y) {
                Logger::log(Logger::Error, "writePixelFromSurface: Out of range: Failed to read pixel "
                                           "from the specified surface at position ({}, {}), "
                                           "the original surface size is {}x{}!",
                            x, y, surface->w, surface->h);
                return false;
            }
            if (!SDL_LockSurface(surface)) {
                Logger::log(std::format("writePixelToSurface: "
                                        "Failed to write pixel from the specified surface! "
                                        "Exception: {}", SDL_GetError()), Logger::Error);
                return false;
            }
            auto pixel_buffer = static_cast<uint8_t*>(surface->pixels);
            const SDL_Palette* palette = SDL_GetSurfacePalette(surface);
            int pitch = surface->pitch;
            int pixel_bytes = SDL_BYTESPERPIXEL(surface->format);
            int pixel_offset = y * pitch + x * pixel_bytes;
            auto pixel_ptr = reinterpret_cast<uint32_t*>(pixel_buffer + pixel_offset);
            auto pixel_value = SDL_MapRGBA(SDL_GetPixelFormatDetails(surface->format),
                                           palette, new_color.r, new_color.g, new_color.b, new_color.a);
            memcpy(pixel_ptr, &pixel_value, pixel_bytes);
            SDL_UnlockSurface(surface);
            return true;
        }

        inline Matrix2D<SDL_Color> readPixelsFromSurface(SDL_Surface* surface, bool* ok = nullptr) {
            Matrix2D<SDL_Color> _map;
            if (!SDL_LockSurface(surface)) {
                if (ok) *ok = false;
                Logger::log(std::format("readPixelsFromSurface: "
                                        "Failed to get pixels from the specified surface! "
                                        "Exception: {}", SDL_GetError()), Logger::Error);
                return _map;
            }
            _map.reshape(surface->h, surface->w);
            SDL_PixelFormat fmt = surface->format;
            auto fmt_details = SDL_GetPixelFormatDetails(fmt);
            auto palette = SDL_GetSurfacePalette(surface);
            int pitch = surface->pitch;
            int pixel_bytes = SDL_BYTESPERPIXEL(fmt);
            auto pixel_buffer = static_cast<uint8_t*>(surface->pixels);
            for (int x = 0; x < surface->w; ++x) {
                for (int y = 0; y < surface->h; ++y) {
                    int pixel_offset = y * pitch + x * pixel_bytes;
                    auto pixel_ptr = reinterpret_cast<uint32_t*>(pixel_buffer + pixel_offset);
                    uint32_t pixelValue = *pixel_ptr;
                    SDL_Color& k = _map.at(y, x);
                    SDL_GetRGBA(pixelValue, fmt_details, palette, &k.r, &k.g, &k.b, &k.a);
                }
            }

            SDL_UnlockSurface(surface);
            if (ok) *ok = true;
            return _map;
        }

        inline Matrix2D<SDL_Color> readPixelAreaOnlyFromSurface(SDL_Surface* surface,
                         Matrix2D<SDL_Color>::Position&& start_pos, Matrix2D<SDL_Color>::Position&& end_pos,
                         bool* ok = nullptr) {
            Matrix2D<SDL_Color> _map;
            if (start_pos.row >= surface->w || start_pos.col >= surface->h) {
                if (ok) *ok = false; return _map;
            }
            if (start_pos > end_pos) std::swap(start_pos, end_pos);
            uint32_t new_width = end_pos.col - start_pos.col + 1,
                     new_height = end_pos.row - start_pos.row + 1;
            if (end_pos.col >= surface->w) new_width -= end_pos.col - surface->w + 1;
            if (end_pos.row >= surface->h) new_height -= end_pos.row - surface->h + 1;
            if (!SDL_LockSurface(surface)) {
                if (ok) *ok = false;
                Logger::log(std::format("readPixelsFromSurface: "
                                        "Failed to get pixels from the specified surface! "
                                        "Exception: {}", SDL_GetError()), Logger::Error);
                return _map;
            }
            _map.reshape(new_height, new_width);
            SDL_PixelFormat fmt = surface->format;
            auto fmt_details = SDL_GetPixelFormatDetails(fmt);
            auto palette = SDL_GetSurfacePalette(surface);
            int pitch = surface->pitch;
            int pixel_bytes = SDL_BYTESPERPIXEL(fmt);
            auto pixel_buffer = static_cast<uint8_t*>(surface->pixels);
            for (int y = (int)start_pos.row; y < (int)start_pos.row + new_height; ++y) {
                for (int x = (int)start_pos.col; x < (int)start_pos.col + new_width; ++x) {
                    if ((x == start_pos.row && y < start_pos.col)) continue;
                    if (x == end_pos.row && y > end_pos.col) break;
                    int pixel_offset = y * pitch + x * pixel_bytes;
                    auto pixel_ptr = reinterpret_cast<uint32_t*>(pixel_buffer + pixel_offset);
                    uint32_t pixelValue = *pixel_ptr;
                    SDL_Color& k = _map.at(y - start_pos.row, x - start_pos.col);
                    SDL_GetRGBA(pixelValue, fmt_details, palette, &k.r, &k.g, &k.b, &k.a);
                }
            }
            SDL_UnlockSurface(surface);
            if (ok) *ok = true;
            return _map;
        }

        inline SDL_Surface* drawSurfaceByColorMap(const Matrix2D<SDL_Color>& color_map,
                                                  SDL_PixelFormat pixel_format = SDL_PIXELFORMAT_RGBA8888,
                                                  bool* ok = nullptr) {
            if (color_map.cols() == 0 || color_map.rows() == 0) {
                Logger::log("drawSurfaceByColorMap: The color map is not valid!", Logger::Error);
                if (ok) *ok = false;
                return nullptr;
            }
            if (color_map.cols() > INT_MAX || color_map.rows() > INT_MAX) {
                Logger::log("drawSurfaceByColorMap: Surface size is too large!", Logger::Error);
                if (ok) *ok = false;
                return nullptr;
            }
            SDL_Surface* surface = SDL_CreateSurface((int)color_map.cols(),
                                                     (int)color_map.rows(), pixel_format);
            if (!surface) {
                Logger::log(std::format("drawSurfaceByColorMap: Failed to create surface! "
                                        "Exception: {}", SDL_GetError()), Logger::Error);
                if (ok) *ok = false;
                return nullptr;
            }
            auto palette = SDL_GetSurfacePalette(surface);
            int pitch = surface->pitch;
            int pixel_bytes = SDL_BYTESPERPIXEL(surface->format);
            auto pixel_buffer = static_cast<uint8_t*>(surface->pixels);
            auto fmt_details = SDL_GetPixelFormatDetails(pixel_format);
            if (!SDL_LockSurface(surface)) {
                if (ok) *ok = false;
                SDL_DestroySurface(surface);
                Logger::log(std::format("readPixelsFromSurface: "
                                        "Failed to get pixels from the specified surface! "
                                        "Exception: {}", SDL_GetError()), Logger::Error);
                return nullptr;
            }
            for (int y = 0; y < (int)color_map.rows(); ++y) {
                for (int x = 0; x < (int)color_map.cols(); ++x) {
                    int pixel_offset = y * pitch + x * pixel_bytes;
                    auto pixel_ptr = reinterpret_cast<uint32_t*>(pixel_buffer + pixel_offset);
                    SDL_Color k = color_map.get(y, x);
                    auto pixel_value = SDL_MapRGBA(fmt_details, palette, k.r, k.g, k.b, k.a);
                    memcpy(pixel_ptr, &pixel_value, pixel_bytes);
                }
            }
            SDL_UnlockSurface(surface);
            if (ok) *ok = true;

            return surface;
        }

        inline SDL_Surface* drawSurface(const SDL_Color& color,
                                        int width, int height,
                                        SDL_PixelFormat pixel_format = SDL_PIXELFORMAT_RGBA8888,
                                        bool* ok = nullptr) {
            if (width <= 0 || height <= 0) {
                Logger::log("drawSurface: "
                            "The width or height is not valid!", Logger::Error);
                if (ok) *ok = false;
                return nullptr;
            }
            SDL_Surface* surface = SDL_CreateSurface(width, height, pixel_format);
            if (!surface) {
                Logger::log(Logger::Error, "drawSurface: Failed to create surface! "
                                        "Exception: {}", SDL_GetError());
                if (ok) *ok = false;
                return nullptr;
            }
            auto palette = SDL_GetSurfacePalette(surface);
            int pitch = surface->pitch;
            int pixel_bytes = SDL_BYTESPERPIXEL(surface->format);
            auto pixel_buffer = static_cast<uint8_t*>(surface->pixels);
            auto fmt_details = SDL_GetPixelFormatDetails(pixel_format);
            if (!SDL_LockSurface(surface)) {
                if (ok) *ok = false;
                SDL_DestroySurface(surface);
                Logger::log(std::format("readPixelsFromSurface: "
                                        "Failed to get pixels from the specified surface! "
                                        "Exception: {}", SDL_GetError()), Logger::Error);
                return nullptr;
            }
            for (int y = 0; y < height; ++y) {
                for (int x = 0; x < width; ++x) {
                    int pixel_offset = y * pitch + x * pixel_bytes;
                    auto pixel_ptr = reinterpret_cast<uint32_t*>(pixel_buffer + pixel_offset);
                    auto pixel_value = SDL_MapRGBA(fmt_details, palette, color.r, color.g, color.b, color.a);
                    memcpy(pixel_ptr, &pixel_value, pixel_bytes);
                }
            }
            SDL_UnlockSurface(surface);
            if (ok) *ok = true;

            return surface;
        }

        inline SDL_Surface* drawSurface(uint64_t hex_code, int width, int height,
                                        SDL_PixelFormat pixel_format = SDL_PIXELFORMAT_RGBA8888,
                                        bool* ok = nullptr) {
            return drawSurface(RGBAColor::hexCode2RGBA(hex_code, false),
                               width, height, pixel_format, ok);
        }

        inline SDL_Surface* processSurface(SDL_Surface* surface,
                                           const std::function<void(SDL_Color&)>& process_function,
                                           bool* ok = nullptr) {
            if (!surface) {
                Logger::log(Logger::Error, "Process surface failed: "
                                           "The specified surface is not valid!");
                if (ok) *ok = false;
                return nullptr;
            }
            SDL_Surface* _ret = SDL_DuplicateSurface(surface);
            if (!_ret) {
                Logger::log(Logger::Error, "Process surface failed: "
                                           "Can't copy from the specified surface.");
                if (ok) *ok = false;
                return nullptr;
            }
            if (!SDL_LockSurface(_ret)) {
                if (ok) *ok = false;
                SDL_DestroySurface(_ret);
                Logger::log(Logger::Error, "Process surface failed: Failed to lock surface! "
                                        "Exception: {}", SDL_GetError());
                return nullptr;
            }
            auto palette = SDL_GetSurfacePalette(_ret);
            int pitch = _ret->pitch;
            int pixel_bytes = SDL_BYTESPERPIXEL(_ret->format);
            auto pixel_buffer = static_cast<uint8_t*>(_ret->pixels);
            auto fmt_details = SDL_GetPixelFormatDetails(_ret->format);
            for (int y = 0; y < _ret->h; ++y) {
                for (int x = 0; x < _ret->w; ++x) {
                    int pixel_offset = y * pitch + x * pixel_bytes;
                    auto pixel_ptr = reinterpret_cast<uint32_t*>(pixel_buffer + pixel_offset);
                    uint32_t pixel_value = *pixel_ptr;
                    SDL_Color k = StdColor::White;
                    SDL_GetRGBA(pixel_value, fmt_details, palette, &k.r, &k.g, &k.b, &k.a);
                    process_function(k);
                    pixel_value = SDL_MapRGBA(fmt_details, palette, k.r, k.g, k.b, k.a);
                    memcpy(pixel_ptr, &pixel_value, pixel_bytes);
                }
            }
            SDL_UnlockSurface(_ret);
            if (ok) *ok = true;
            return _ret;
        }

        inline SDL_Surface* drawWhiteShape2Surface(SDL_Surface* surface, bool* ok = nullptr) {
            return processSurface(surface, [](SDL_Color& color) {
                color.r = 255; color.g = 255; color.b = 255;
            }, ok);
        }

        inline SDL_Surface* drawInvertedColor2Surface(SDL_Surface* surface, bool* ok = nullptr) {
            return processSurface(surface, [](SDL_Color& color) {
                color.r = 255 - color.r; color.g = 255 - color.g; color.b = 255 - color.b;
            }, ok);
        }

        inline SDL_Surface* drawGrayScale2Surface(SDL_Surface* surface, bool* ok = nullptr) {
            return processSurface(surface, [](SDL_Color& color) {
                auto gray = static_cast<uint8_t>((77 * color.r + 150 * color.g + 29 * color.b) >> 8);
                color = { gray, gray, gray, color.a };
            }, ok);
        }

    }
}

#endif //MYENGINE_ALGORITHM_RGBAPIXELS_H
