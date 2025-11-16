
#pragma once
#ifndef S3GF_DRAW_H
#define S3GF_DRAW_H

#include "Libs.h"

namespace S3GF {
    namespace Algorithm {
        inline SDL_FColor convert2FColor(const SDL_Color &color) {
            return {color.r / 255.f, color.g / 255.f, color.b / 255.f, color.a / 255.f};
        }

        inline SDL_Color convert2Color(const SDL_FColor &color) {
            return {static_cast<uint8_t>(255 * color.r),
                    static_cast<uint8_t>(255 * color.g),
                    static_cast<uint8_t>(255 * color.b),
                    static_cast<uint8_t>(255 * color.a)};
        }

        inline void calcPoint(const Vector2& position, float radius, SDL_Color color,
                      std::vector<SDL_Vertex>& vertices, std::vector<int>& indices,
                      const uint16_t count = 32) {
            vertices.clear();
            indices.clear();
            
            const uint16_t actual_count = std::max(count, static_cast<uint16_t>(3));
            
            SDL_FColor fcolor = convert2FColor(color);
            
            vertices.push_back({{position.x, position.y}, fcolor, {0, 0}});
            
            for (uint16_t i = 0; i < actual_count; ++i) {
                float angle = 2.0f * M_PI * static_cast<float>(i) / static_cast<float>(actual_count);
                float x = position.x + cosf(angle) * radius;
                float y = position.y + sinf(angle) * radius;
                vertices.push_back({{x, y}, fcolor, {0, 0}});
            }
            
            // 添加索引，确保最后一个三角形正确连接到起始顶点
            for (uint16_t i = 0; i < actual_count; ++i) {
                indices.push_back(0);
                indices.push_back(i + 1);
                indices.push_back((i + 1) % actual_count + 1);
            }
        }

        inline void calcLine(float x1, float y1, float x2, float y2, uint16_t thickness, const SDL_Color &color,
                             std::array<SDL_Vertex, 4> &vertex, std::array<int, 6> &indices) {
            float dx = x2 - x1;
            float dy = y2 - y1;
            float len = hypotf(dx, dy);
            if (len == 0.0f) return;

            float nx = -dy / len;
            float ny = dx / len;

            float px = nx * (float) thickness * 0.5f;
            float py = ny * (float) thickness * 0.5f;

            SDL_FColor fcolor = convert2FColor(color);
            vertex[0] = {{x1 - px, y1 - py}, fcolor, {0, 0}};
            vertex[1] = {{x1 + px, y1 + py}, fcolor, {0, 0}};
            vertex[2] = {{x2 + px, y2 + py}, fcolor, {0, 0}};
            vertex[3] = {{x2 - px, y2 - py}, fcolor, {0, 0}};
            indices[0] = 0;
            indices[1] = 1;
            indices[2] = 2;
            indices[3] = 0;
            indices[4] = 2;
            indices[5] = 3;
        }
    }
}

#endif //S3GF_DRAW_H
