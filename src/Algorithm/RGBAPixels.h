
#ifndef MYENGINE_ALGORITHM_RGBAPIXELS_H
#define MYENGINE_ALGORITHM_RGBAPIXELS_H
#include "../Components.h"
#include "../Core.h"
#include "../Utils/RGBAColor.h"

namespace MyEngine {
    namespace Algorithm {
        inline SDL_Color readPixelFromSurface(SDL_Surface* surface, int x, int y, bool* ok = nullptr) {
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
            const SDL_PixelFormatDetails* format = SDL_GetPixelFormatDetails(surface->format);
            const SDL_Palette* palette = SDL_GetSurfacePalette(surface);
            int pitch = surface->pitch;
            int pixel_bytes = SDL_BYTESPERPIXEL(surface->format);
            int pixel_offset = y * pitch + x * pixel_bytes;
            auto pixel_ptr = reinterpret_cast<uint32_t*>(pixel_buffer + pixel_offset);
            SDL_Color _ret = {};
            SDL_GetRGBA(*pixel_ptr, format, palette, &_ret.r, &_ret.g, &_ret.b, &_ret.a);
            if (ok) *ok = true;
            return _ret;
        }

        inline bool writePixelToSurface(SDL_Surface* surface, int x, int y, SDL_Color new_color) {
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

        inline bool writePixelsToSurface(SDL_Surface* surface, int x1, int y1, int x2, int y2, SDL_Color new_color) {
            if (!surface) {
                Logger::log("writePixelsToSurface: The specified surface is not valid!", Logger::Error);
                return false;
            }
            if (y1 >= y2 && x1 > x2) {
                std::swap(x1, x2);
                std::swap(y1, y2);
            }
            if (surface->w <= x1 || surface->h <= y1) {
                Logger::log("writePixelsToSurface: The start position is out of range!", Logger::Error);
                return false;
            }
            if (surface->w <= x2 || surface->h <= y2) {
                x2 = surface->w - 1; y2 = surface->h - 1;
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
            for (int y = y1; y <= y2; ++y) {
                for (int x = x1; x <= x2; ++x) {
                    int pixel_offset = y * pitch + x * pixel_bytes;
                    auto pixel_ptr = reinterpret_cast<uint32_t*>(pixel_buffer + pixel_offset);
                    auto pixel_value = SDL_MapRGBA(SDL_GetPixelFormatDetails(surface->format),
                                                   palette, new_color.r, new_color.g, new_color.b, new_color.a);
                    memcpy(pixel_ptr, &pixel_value, pixel_bytes);
                }
            }
            SDL_UnlockSurface(surface);
            return true;
        }

        inline bool writePixelsToSurface(SDL_Surface* surface, const Matrix2D<SDL_Color>& color_map,
                                         Matrix2D<SDL_Color>::Position start_pos = {0, 0}) {
            if (!surface) {
                Logger::log("writePixelsToSurface: The specified surface is not valid!", Logger::Error);
                return false;
            }
            if (surface->w <= start_pos.col || surface->h <= start_pos.row) {
                Logger::log("writePixelsToSurface: The start position is out of range!", Logger::Error);
                return false;
            }

            if (!SDL_LockSurface(surface)) {
                Logger::log(std::format("writePixelToSurface: "
                                        "Failed to write pixel from the specified surface! "
                                        "Exception: {}", SDL_GetError()), Logger::Error);
                return false;
            }
            auto max_row = start_pos.row + color_map.rows();
            auto max_col = start_pos.col + color_map.cols();
            max_row = (surface->h <= max_row ? surface->h - 1 : max_row);
            max_col = (surface->w <= max_col ? surface->w - 1 : max_col);
            auto pixel_buffer = static_cast<uint8_t*>(surface->pixels);
            const SDL_Palette* palette = SDL_GetSurfacePalette(surface);
            int pitch = surface->pitch;
            int pixel_bytes = SDL_BYTESPERPIXEL(surface->format);
            for (int y = (int)start_pos.row; y <= (int)(max_row); ++y) {
                for (int x = (int)start_pos.col; x <= (int)(max_col); ++x) {
                    auto& new_color = color_map.get(y, x);
                    int pixel_offset = y * pitch + x * pixel_bytes;
                    auto pixel_ptr = reinterpret_cast<uint32_t*>(pixel_buffer + pixel_offset);
                    auto pixel_value = SDL_MapRGBA(SDL_GetPixelFormatDetails(surface->format),
                                                   palette, new_color.r, new_color.g, new_color.b, new_color.a);
                    memcpy(pixel_ptr, &pixel_value, pixel_bytes);
                }
            }
            SDL_UnlockSurface(surface);
            Logger::log(Logger::Debug, "Success: Rendered {}x{} pixels, statred from ({}, {})",
                        max_col, max_row, start_pos.row, start_pos.col);
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
            for (int y = 0; y < surface->h; ++y) {
                for (int x = 0; x < surface->w; ++x) {
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

        inline Matrix2D<SDL_Color> readPixelsOnlyFromSurface(SDL_Surface* surface,
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
                Logger::log(std::format("readPixelsOnlyFromSurface: "
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

        inline SDL_Surface* drawFilledWhiteSurface(SDL_Surface* surface, bool* ok = nullptr) {
            return processSurface(surface, [](SDL_Color& color) {
                color.r = 255; color.g = 255; color.b = 255;
            }, ok);
        }

        inline SDL_Surface* drawInvertedColorSurface(SDL_Surface* surface, bool* ok = nullptr) {
            return processSurface(surface, [](SDL_Color& color) {
                color.r = 255 - color.r; color.g = 255 - color.g; color.b = 255 - color.b;
            }, ok);
        }

        inline SDL_Surface* drawGraySurface(SDL_Surface* surface, bool* ok = nullptr) {
            return processSurface(surface, [](SDL_Color& color) {
                auto gray = static_cast<uint8_t>((77 * color.r + 150 * color.g + 29 * color.b) >> 8);
                color = { gray, gray, gray, color.a };
            }, ok);
        }

        inline SDL_Surface* drawDarkGraySurface(SDL_Surface* surface, bool* ok = nullptr) {
            return processSurface(surface, [](SDL_Color& color) {
                auto gray = std::min(std::min(color.r, color.g), color.b);
                color = { gray, gray, gray, color.a };
            }, ok);
        }

        inline SDL_Surface* drawLightGraySurface(SDL_Surface* surface, bool* ok = nullptr) {
            return processSurface(surface, [](SDL_Color& color) {
                auto gray = std::max(std::max(color.r, color.g), color.b);
                color = { gray, gray, gray, color.a };
            }, ok);
        }

        inline SDL_Surface* drawAvgGraySurface(SDL_Surface* surface, bool* ok = nullptr) {
            return processSurface(surface, [](SDL_Color& color) {
                auto gray = static_cast<uint8_t>((color.r + color.g + color.b) / 3);
                color = { gray, gray, gray, color.a };
            }, ok);
        }

        inline SDL_Surface* drawBrightnessSurface(SDL_Surface* surface, int16_t value, bool* ok = nullptr) {
            value = std::clamp(value, (int16_t)-255, (int16_t)255);
            return processSurface(surface, [&value](SDL_Color& color) {
                color.r = std::clamp(color.r + value, 0, 255);
                color.g = std::clamp(color.g + value, 0, 255);
                color.b = std::clamp(color.b + value, 0, 255);
            }, ok);
        }

        inline SDL_Surface* drawContrastSurface(SDL_Surface* surface, float alpha, bool* ok = nullptr) {
            alpha = std::clamp(alpha, 0.f, 10.f);
            return processSurface(surface, [&alpha](SDL_Color& color) {
                color.r = static_cast<uint8_t>(std::clamp(static_cast<uint32_t>
                                                (static_cast<float>(color.r - 128) * alpha + 128.f),
                                             (uint32_t)0, (uint32_t)255));
                color.g = static_cast<uint8_t>(std::clamp(static_cast<uint32_t>
                                                (static_cast<float>(color.g - 128) * alpha + 128.f),
                                             (uint32_t)0, (uint32_t)255));
                color.b = static_cast<uint8_t>(std::clamp(static_cast<uint32_t>
                                                (static_cast<float>(color.b - 128) * alpha + 128.f),
                                             (uint32_t)0, (uint32_t)255));
            }, ok);
        }
    }
}

#endif //MYENGINE_ALGORITHM_RGBAPIXELS_H
