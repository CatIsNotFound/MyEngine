#define CATCH_CONFIG_MAIN
#include <catch2/catch_test_macros.hpp>

#include "MyEngine"
using namespace MyEngine;

extern inline constexpr bool isColorsEqual(SDL_Color color1, SDL_Color color2) noexcept {
    return (color1.r == color2.r) && (color1.g == color2.g) &&
           (color1.b == color2.b) && (color1.a == color2.a);
}

TEST_CASE("Renderer Test", "[Core][Window][Renderer]") {
    Engine engine;
    auto window = new Window(&engine, "Renderer Test");
    window->show();
    auto renderer = window->renderer();
    std::atomic<SSurface*> captured_view{};
    const SColor PT_COLOR = StdColor::Blue;
    const SColor LINE_COLOR = StdColor::Red;
    const SColor TRI_FG_COLOR = StdColor::DarkGreen;
    const SColor TRI_BG_COLOR = StdColor::LightGreen;
    const SColor RECT_FG_COLOR = StdColor::Blue;
    const SColor RECT_BG_COLOR = StdColor::Red;
    const SColor ELLI_BG_COLOR = StdColor::Pink;

    // Render Graphic
    Graphics::Point pt(100, 100, 20, PT_COLOR);
    Graphics::Line line(20.f, 20.f, 80.f, 20.f, 3, LINE_COLOR);
    Graphics::Triangle tri(400, 10, 400, 110, 600, 60, 2,
                           TRI_FG_COLOR, TRI_BG_COLOR);
    Graphics::Rectangle rect(100, 400, 100, 100, 4, RECT_FG_COLOR,
                             RECT_BG_COLOR, 90.f);
    Graphics::Ellipse elli(300, 300, 200, 100, 2, StdColor::Red, ELLI_BG_COLOR, 45.f, 64);
    window->installPaintEvent([&](Renderer* r) {
        r->drawPoint(&pt);
        r->drawLine(&line);
        r->drawTriangle(&tri);
        r->drawRectangle(&rect);
        r->drawEllipse(&elli);
    });

    SECTION("Initialized renderer") {
        REQUIRE(renderer);
        CHECK(renderer->window() == window);
    }

    SECTION("Check basic graphics") {
        // Timer for control
        Timer timer(1000, [&]() {
            static int sec = 0;
            sec++;
            // Exit engine
            if (sec == 2) Engine::exit();
            if (sec == 1) {
                captured_view = renderer->capture();

                if (!captured_view) {
                    Logger::log("Failed to get render capture!", MyEngine::Logger::Warn);
                }
            }
        });
        timer.start(0);

        bool ok;
        engine.exec();

        REQUIRE(captured_view);
        auto pt_color = Algorithm::readPixelFromSurface(captured_view,
                        static_cast<int>(pt.position().x), static_cast<int>(pt.position().y), &ok);
        REQUIRE(ok);
        CHECK(isColorsEqual(pt_color, PT_COLOR));

        auto st_line_color = Algorithm::readPixelFromSurface(captured_view,
                             static_cast<int>(line.startPosition().x),
                             static_cast<int>(line.startPosition().y), &ok);
        REQUIRE(ok);
        auto ed_line_color = Algorithm::readPixelFromSurface(captured_view,
                                static_cast<int>(line.endPosition().x),
                                static_cast<int>(line.endPosition().y), &ok);
        REQUIRE(ok);
        CHECK(isColorsEqual(st_line_color, LINE_COLOR));
        CHECK(isColorsEqual(ed_line_color, LINE_COLOR));

        // Test Triangle
        auto triangle_fg_color = Algorithm::readPixelFromSurface(captured_view,
                                    static_cast<int>(tri.position(1).x),
                                    static_cast<int>(tri.position(1).y), &ok);
        REQUIRE(ok);
        auto triangle_bg_color = Algorithm::readPixelFromSurface(captured_view,
                                       static_cast<int>((tri.position(0).x + 50)),
                                       static_cast<int>((tri.position(0).y + 30)), &ok);
        CHECK(isColorsEqual(triangle_fg_color, TRI_FG_COLOR));
        CHECK(isColorsEqual(triangle_bg_color, TRI_BG_COLOR));

        // Test Rect
        auto rect_fg_color = Algorithm::readPixelFromSurface(captured_view,
                                 static_cast<int>(rect.geometry().pos.x),
                                 static_cast<int>(rect.geometry().pos.y), &ok);
        REQUIRE(ok);
        auto rect_bg_color = Algorithm::readPixelFromSurface(captured_view,
                                 static_cast<int>(rect.geometry().pos.x + rect.geometry().size.width / 2),
                                 static_cast<int>(rect.geometry().pos.y + rect.geometry().size.height / 2), &ok);
        REQUIRE(ok);
        CHECK(isColorsEqual(rect_fg_color, RECT_FG_COLOR));
        CHECK(isColorsEqual(rect_bg_color, RECT_BG_COLOR));

        // Test ellipse
        auto ellipse_bg_color = Algorithm::readPixelFromSurface(captured_view,
                                    static_cast<int>(elli.centerPosition().x),
                                    static_cast<int>(elli.centerPosition().y), &ok);
        REQUIRE(ok);
        CHECK(isColorsEqual(ellipse_bg_color, ELLI_BG_COLOR));

        // Destroy
        SDL_DestroySurface(captured_view);
        captured_view = nullptr;
        CHECK_FALSE(captured_view);
        CHECK_FALSE(renderer->self());
        CHECK_FALSE(window->self());
    }
}

TEST_CASE("Render performance test", "[Core][Renderer][Performance]") {
    using RG = RandomGenerator;
    constexpr uint32_t COUNT = 256;
    Engine engine;
    engine.setRenderSetup(1024, true);
    auto window = new Window(&engine, "Render Performance Test");
    window->setResizable(true);
    auto renderer = window->renderer();
    std::vector<Graphics::Point*> points_list(COUNT);
    std::vector<Graphics::Line*> lines_list(COUNT);
    std::vector<Graphics::Triangle*> triangles_list(COUNT);
    std::vector<Graphics::Rectangle*> rectangles_list(COUNT);
    std::vector<Graphics::Ellipse*> ellipses_list(COUNT);
    std::atomic<uint8_t> visible_mode{0};

    window->installPaintEvent([&](Renderer* r) {
        auto WIN_W = static_cast<float>(window->geometry().width);
        auto WIN_H = static_cast<float>(window->geometry().height);
        r->fillBackground(RGBAColor::MixViolet);
        r->setBlendMode(SDL_BLENDMODE_MOD);
        if (visible_mode >= 1) {
            for (uint32_t i = 0; i < COUNT; ++i) {
                if (points_list[i]) {
                    auto &pt = points_list[i];
                    pt->reset(RG::randFloat(0, WIN_W), RG::randFloat(0, WIN_H),
                              RG::randUInt(1, 32), RGBAColor::BlueLightTrans);
                } else {
                    auto pt = new Graphics::Point(RG::randFloat(0, WIN_W),
                                                  RG::randFloat(0, WIN_H),
                                                  RG::randUInt(1, 32),
                                                  RGBAColor::MixYellowTrans);
                    points_list[i] = pt;
                }
            }
        }

        if (visible_mode >= 2) {
            for (uint32_t i = 0; i < COUNT; ++i) {
                if (lines_list[i]) {
                    auto &line = lines_list[i];
                    line->reset(RG::randFloat(0, WIN_W), RG::randFloat(0, WIN_H),
                                RG::randFloat(0, WIN_W), RG::randFloat(0, WIN_H),
                                RG::randUInt(1, 16), RGBAColor::GreenOliveTrans);
                } else {
                    auto line = new Graphics::Line(RG::randFloat(0, WIN_W), RG::randFloat(0, WIN_H),
                                                   RG::randFloat(0, WIN_W), RG::randFloat(0, WIN_H),
                                                   RG::randUInt(1, 16), RGBAColor::GreenOliveTrans);
                    lines_list[i] = line;
                }
            }
        }

        if (visible_mode >= 3) {
            for (uint32_t i = 0; i < COUNT; ++i) {
                if (triangles_list[i]) {
                    auto &triangle = triangles_list[i];
                    triangle->reset(RG::randFloat(0, WIN_W), RG::randFloat(0, WIN_H),
                                    RG::randFloat(0, WIN_W), RG::randFloat(0, WIN_H),
                                    RG::randFloat(0, WIN_W), RG::randFloat(0, WIN_H),
                                    RG::randUInt(1, 4), RGBAColor::BlueDarkTrans,
                                    RGBAColor::BlueLightTrans);
                } else {
                    auto triangle = new Graphics::Triangle(RG::randFloat(0, WIN_W), RG::randFloat(0, WIN_H),
                                                           RG::randFloat(0, WIN_W), RG::randFloat(0, WIN_H),
                                                           RG::randFloat(0, WIN_W), RG::randFloat(0, WIN_H),
                                                           RG::randUInt(1, 4), RGBAColor::BlueDarkTrans,
                                                           RGBAColor::BlueLightTrans);
                    triangles_list[i] = triangle;
                }
            }
        }
        if (visible_mode >= 4) {
            for (uint32_t i = 0; i < COUNT; ++i) {
                if (rectangles_list[i]) {
                    auto &rect = rectangles_list[i];
                    rect->move(RG::randFloat(0, 700), RG::randFloat(0, 500));
                    rect->resize(std::clamp(RG::randFloat(10, WIN_W), 10.f, WIN_W - rect->geometry().pos.x),
                                 std::clamp(RG::randFloat(10, WIN_H), 10.f, WIN_H - rect->geometry().pos.y));
                    rect->setBorder(RG::randUInt(0, 4), RGBAColor::RedDarkTrans);
                    rect->setRotate(RG::randFloat(0, 360));
                } else {
                    auto rect = new Graphics::Rectangle();
                    rect->move(RG::randFloat(0, 700), RG::randFloat(0, 500));
                    rect->resize(std::clamp(RG::randFloat(10, WIN_W), 10.f, WIN_W - rect->geometry().pos.x),
                                 std::clamp(RG::randFloat(10, WIN_H), 10.f, WIN_H - rect->geometry().pos.y));
                    rect->setBorder(RG::randUInt(0, 4), RGBAColor::RedDarkTrans);
                    rect->setBackgroundColor(RGBAColor::RedLightTrans);
                    rect->setRotate(RG::randFloat(0, 360));
                    rectangles_list[i] = rect;
                }
            }
        }
        if (visible_mode >= 5) {
            for (uint32_t i = 0; i < COUNT; i++) {
                if (ellipses_list[i]) {
                    auto &elli = ellipses_list[i];
                    elli->setGeometry(RG::randFloat(0, WIN_W), RG::randFloat(0, WIN_H),
                                      RG::randFloat(10, 400), RG::randFloat(10, 400));
                    elli->setBorder(RG::randUInt(0, 4), RGBAColor::GreenLimeTrans);
                } else {
                    auto elli = new Graphics::Ellipse(RG::randFloat(0, WIN_W), RG::randFloat(0, WIN_H),
                                                      RG::randFloat(10, 400), RG::randFloat(10, 400),
                                                      RG::randUInt(0, 4), RGBAColor::GreenLimeTrans,
                                                      RGBAColor::GreenLightTrans);
                    ellipses_list[i] = elli;
                }
            }
        }
        if (visible_mode >= 5) r->drawEllipses(ellipses_list);
        if (visible_mode >= 4) r->drawRectangles(rectangles_list);
        if (visible_mode >= 3) r->drawTriangles(triangles_list);
        if (visible_mode >= 2) r->drawLines(lines_list);
        if (visible_mode >= 1) r->drawPoints(points_list);
        r->drawDebugFPS({20, 20}, StdColor::White);
    });

    window->show();

    std::atomic<uint32_t> render_cnt{}, frames_cnt{}, record_count{};

    Timer recorder(1000, [&engine, &renderer, &render_cnt, &frames_cnt, &record_count] {
        render_cnt += renderer->renderCountInSec();
        frames_cnt += engine.fps();
        record_count++;
    });
    recorder.start(0);

    Timer setter(8000, [&renderer, &render_cnt, &frames_cnt, &record_count, &visible_mode] {
        static int sec = 11;
        sec--;
        uint32_t avg_render_cnt = render_cnt / record_count;
        uint32_t avg_frames_cnt = frames_cnt / record_count;
        switch (sec) {
            case 10:
            case 9:
            case 8:
            case 7:
            case 6:
                Logger::log(Logger::Info, "Render Mode {}: {} Commands, {} FPS",
                            static_cast<uint32_t>(visible_mode), avg_render_cnt, avg_frames_cnt);
                visible_mode++;
                break;
            case 5:
                visible_mode = 1;
                Logger::log(Logger::Info, "No VSync: {} Commands, {} FPS", avg_render_cnt, avg_frames_cnt);
                renderer->setVSyncMode(Renderer::QuarterRate);
                break;
            case 4:
                Logger::log(Logger::Info, "QuarterRate: {} Commands, {} FPS", avg_render_cnt, avg_frames_cnt);
                renderer->setVSyncMode(Renderer::ThirdRate);
                break;
            case 3:
                Logger::log(Logger::Info, "ThirdRate: {} Commands, {} FPS", avg_render_cnt, avg_frames_cnt);
                renderer->setVSyncMode(Renderer::HalfRate);
                break;
            case 2:
                Logger::log(Logger::Info, "HalfRate: {} Commands, {} FPS", avg_render_cnt, avg_frames_cnt);
                renderer->setVSyncMode(Renderer::Enabled);
                break;
            case 1:
                Logger::log(Logger::Info, "NormalRate: {} Commands, {} FPS", avg_render_cnt, avg_frames_cnt);
                renderer->setVSyncMode(Renderer::Adaptive);
                break;
            case 0:
                Logger::log(Logger::Info, "Adaptive: {} Commands, {} FPS", avg_render_cnt, avg_frames_cnt);
                Engine::exit();
                break;
            default:
                return;
        }
        delay(2);
        render_cnt = 0;
        frames_cnt = 0;
        record_count = 0;
    });
    setter.start(12);

    engine.installCleanUpEvent([&] {
        static bool do_clean = false;
        if (do_clean) return;
        do_clean = true;
        for (auto p : points_list) {
            delete p;
            p = nullptr;
        }
        for (auto l : lines_list) {
            delete l;
            l = nullptr;
        }
        for (auto t : triangles_list) {
            delete t;
            t = nullptr;
        }
        for (auto r : rectangles_list) {
            delete r;
            r = nullptr;
        }
        for (auto e : ellipses_list) {
            delete e;
            e = nullptr;
        }
    });

    CHECK_NOFAIL(engine.exec());
}
