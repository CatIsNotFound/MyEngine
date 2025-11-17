
#ifndef S3GF_COLLIDER_H
#define S3GF_COLLIDER_H
#include "Basic.h"

namespace S3GF {
    namespace Algorithm {
        int8_t comparePosInRect(const Vector2& vec, const Graphics::Rectangle& rect) {

        }

        int8_t comparePosInPoint(const Vector2& pos, const Graphics::Point& point) {
            auto pt = point.position();
            if (point.size() < 2) {
                return (point.position().isEqual(pos) ? 1 : -1);
            }
            float dx = pos.x - pt.x;
            float dy = pos.y - pt.y;
            float dd = dx * dx + dy * dy;          // 距离平方
            float rr = (point.size() / 2.f) * (point.size() / 2.f);

            if (dd > rr + 1e-5f) return -1;
            if (std::fabs(dd - rr) < 1e-5f) return 0;
            return 1;
        }

        int8_t comparePoints(const Graphics::Point& point1, const Graphics::Point& point2) {
            auto p1 = point1.position(), p2 = point2.position();
            if (point1.size() < 2 && point2.size() < 2) {
                return (point1.position().isEqual(point2.position()) ? 1 : -1);
            }
            auto r1 = point1.size() / 2.f, r2 = point2.size() / 2.f;
            float dx = p1.x - p2.x;
            float dy = p1.y - p2.y;
            float d  = std::hypot(dx, dy);          // 圆心距
            float rSum = r1 + r2;
            float rDif = std::fabs(r1 - r2);

            if (d > rSum + 1e-5f) return -1;                // 完全相离
            if (std::fabs(d - rSum) < 1e-5f) return 0;   // 外切（只有一个交点）
            return 1;
        }
    }
}

#endif //S3GF_COLLIDER_H
