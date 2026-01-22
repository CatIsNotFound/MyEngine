
#ifndef MYENGINE_ALGORITHM_RGBAPIXELS_H
#define MYENGINE_ALGORITHM_RGBAPIXELS_H
#include "../Components.h"
#include "../Core.h"
#include "../Utils/RGBAColor.h"

namespace MyEngine {
    namespace Algorithm {
        inline Matrix2D<SDL_Color> getPixelsFromSurface(SDL_Surface* surface, bool* ok = nullptr) {
            Matrix2D<SDL_Color> _map;
            if (!SDL_LockSurface(surface)) {
                if (ok) *ok = false;
                Logger::log(std::format("getPixelsFromSurface: "
                                        "Failed to get pixels from the specified surface! "
                                        "Exception: {}", SDL_GetError()), Logger::Error);
                return _map;
            }
            _map.reshape(surface->h, surface->w);
            SDL_PixelFormat fmt = surface->format;
            auto fmt_details = SDL_GetPixelFormatDetails(fmt);
            auto palette = SDL_GetSurfacePalette(surface);
            int pitch = surface->pitch;
            int pixelBytes = SDL_BYTESPERPIXEL(fmt);
            auto pixelBuffer = static_cast<uint8_t*>(surface->pixels);
            for (int x = 0; x < surface->w; ++x) {
                for (int y = 0; y < surface->h; ++y) {
                    int pixelOffset = y * pitch + x * pixelBytes;
                    auto pixelPtr = reinterpret_cast<uint32_t*>(pixelBuffer + pixelOffset);
                    uint32_t pixelValue = *pixelPtr;
                    SDL_Color& k = _map.at(y, x);
                    SDL_GetRGBA(pixelValue, fmt_details, palette, &k.r, &k.g, &k.b, &k.a);
                }
            }

            SDL_UnlockSurface(surface);
            if (ok) *ok = true;
            return _map;
        }

        inline Matrix2D<SDL_Color> getAreaOfThePixelsFromSurface(SDL_Surface* surface,
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
                Logger::log(std::format("getPixelsFromSurface: "
                                        "Failed to get pixels from the specified surface! "
                                        "Exception: {}", SDL_GetError()), Logger::Error);
                return _map;
            }
            _map.reshape(new_height, new_width);
            SDL_PixelFormat fmt = surface->format;
            auto fmt_details = SDL_GetPixelFormatDetails(fmt);
            auto palette = SDL_GetSurfacePalette(surface);
            int pitch = surface->pitch;
            int pixelBytes = SDL_BYTESPERPIXEL(fmt);
            auto pixelBuffer = static_cast<uint8_t*>(surface->pixels);
            for (int y = (int)start_pos.row; y < (int)start_pos.row + new_height; ++y) {
                for (int x = (int)start_pos.col; x < (int)start_pos.col + new_width; ++x) {
                    if ((x == start_pos.row && y < start_pos.col)) continue;
                    if (x == end_pos.row && y > end_pos.col) break;
                    int pixelOffset = y * pitch + x * pixelBytes;
                    auto pixelPtr = reinterpret_cast<uint32_t*>(pixelBuffer + pixelOffset);
                    uint32_t pixelValue = *pixelPtr;
                    SDL_Color& k = _map.at(y - start_pos.row, x - start_pos.col);
                    SDL_GetRGBA(pixelValue, fmt_details, palette, &k.r, &k.g, &k.b, &k.a);
                }
            }
            SDL_UnlockSurface(surface);
            if (ok) *ok = true;
            return _map;
        }

        inline SDL_Surface* generateSurfaceByColorMap(const Matrix2D<SDL_Color>& color_map,
                                                      SDL_PixelFormat pixel_format = SDL_PIXELFORMAT_RGBA8888,
                                                      bool* ok = nullptr) {
            if (color_map.cols() == 0 || color_map.rows() == 0) {
                Logger::log("generateSurfaceByColorMap: The color map is not valid!", Logger::Error);
                if (ok) *ok = false;
                return nullptr;
            }
            if (color_map.cols() > INT_MAX || color_map.rows() > INT_MAX) {
                Logger::log("generateSurfaceByColorMap: Surface size is too large!", Logger::Error);
                if (ok) *ok = false;
                return nullptr;
            }
            SDL_Surface* surface = SDL_CreateSurface((int)color_map.cols(),
                                                     (int)color_map.rows(), pixel_format);
            if (!surface) {
                Logger::log(std::format("generateSurfaceByColorMap: Failed to create surface! "
                                        "Exception: {}", SDL_GetError()), Logger::Error);
                if (ok) *ok = false;
                return nullptr;
            }
            auto palette = SDL_GetSurfacePalette(surface);
            int pitch = surface->pitch;
            int pixel_bytes = SDL_BYTESPERPIXEL(surface->format);
            auto pixel_buffer = static_cast<uint8_t*>(surface->pixels);
            if (!SDL_LockSurface(surface)) {
                if (ok) *ok = false;
                SDL_DestroySurface(surface);
                Logger::log(std::format("getPixelsFromSurface: "
                                        "Failed to get pixels from the specified surface! "
                                        "Exception: {}", SDL_GetError()), Logger::Error);
                return nullptr;
            }
            for (int y = 0; y < (int)color_map.rows(); ++y) {
                for (int x = 0; x < (int)color_map.cols(); ++x) {
                    int pixel_offset = y * pitch + x * pixel_bytes;
                    if (pixel_offset + pixel_bytes > pitch * surface->h) continue;
                    auto pixel_ptr = reinterpret_cast<uint32_t*>(pixel_buffer + pixel_offset);
                    SDL_Color k = color_map.get(y, x);
                    auto pixel_value = SDL_MapRGBA(SDL_GetPixelFormatDetails(pixel_format),
                                                   palette, k.r, k.g, k.b, k.a);
                    memcpy(pixel_ptr, &pixel_value, pixel_bytes);
                }
            }
            SDL_UnlockSurface(surface);
            if (ok) *ok = true;

            return surface;
        }


    }
}

#endif //MYENGINE_ALGORITHM_RGBAPIXELS_H
