#define CATCH_CONFIG_MAIN
#include <catch2/catch_test_macros.hpp>

#include "MyEngine"
using namespace MyEngine;

bool is2ColorsEqual(SDL_Color color1, SDL_Color color2) {
    if (color1.r != color2.r) return false;
    if (color1.g != color2.g) return false;
    if (color1.b != color2.b) return false;
    if (color1.a != color2.a) return false;
    return true;
}

TEST_CASE("Renderer Test", "[Core][Window][Renderer]") {
    Engine engine;
    auto window = new Window(&engine, "Renderer Test");
    window->show();
    auto renderer = window->renderer();
    SSurface* captured_view = nullptr;
    const SColor PT_COLOR = StdColor::Blue;
    const SColor LINE_COLOR = StdColor::Red;
    const SColor TRI_FG_COLOR = StdColor::DarkGreen;
    const SColor RECT_FG_COLOR = StdColor::Blue;
    const SColor ELLI_BG_COLOR = StdColor::Pink;

    // Render Graphic
    Graphics::Point pt(100, 100, 20, PT_COLOR);
    Graphics::Line line(20.f, 20.f, 80.f, 20.f, 3, LINE_COLOR);
    Graphics::Triangle tri(400, 10, 400, 110, 600, 60, 2,
                           TRI_FG_COLOR, StdColor::LightGreen);
    Graphics::Rectangle rect(100, 400, 100, 100, 2, RECT_FG_COLOR, StdColor::Red, 90.f);
    Graphics::Ellipse elli(300, 300, 200, 100, 2, StdColor::Red, ELLI_BG_COLOR, 45.f, 64);
    window->installPaintEvent([&](Renderer* r) {
        r->drawPoint(&pt);
        r->drawLine(&line);
        r->drawTriangle(&tri);
        r->drawRectangle(&rect);
        r->drawEllipse(&elli);
    });

    Timer timer(1000, [&]() {
        static int sec = 0;
        sec++;
        // Exit engine
        if (sec == 2) engine.exit();
        if (sec == 1) {
            captured_view = renderer->capture();
            if (!captured_view) Logger::log("Failed to get render capture!", MyEngine::Logger::Warn);
        }
    });
    timer.start(0);

    SECTION("Initialized renderer") {
        REQUIRE(renderer);
        CHECK(renderer->window() == window);
    }

    SECTION("Clean up renderer") {
        if (engine.exec() == 0) {
            CHECK_FALSE(renderer->self());
            CHECK_FALSE(window->self());
        }
    }

    SECTION("Check basic graphics") {
        bool ok;
        SECTION("Check point") {
            auto pt_color = Algorithm::readPixelFromSurface(captured_view,
                            static_cast<int>(pt.position().x), static_cast<int>(pt.position().y), &ok);
            REQUIRE(ok);
            CHECK(is2ColorsEqual(pt_color, PT_COLOR));
        }
        SECTION("Check line") {
            auto st_line_color = Algorithm::readPixelFromSurface(captured_view,
                                 static_cast<int>(line.startPosition().x),
                                 static_cast<int>(line.startPosition().y), &ok);
            REQUIRE(ok);
            auto ed_line_color = Algorithm::readPixelFromSurface(captured_view,
                                    static_cast<int>(line.endPosition().x),
                                    static_cast<int>(line.endPosition().y), &ok);
            REQUIRE(ok);
            CHECK(is2ColorsEqual(st_line_color, LINE_COLOR));
            CHECK(is2ColorsEqual(ed_line_color, LINE_COLOR));
        }
//        SECTION("Check triangle") {
//            auto pt
//        }
    }

    SECTION("Destroy capture view") {
        SDL_DestroySurface(captured_view);
        captured_view = nullptr;
        CHECK_FALSE(captured_view);
    }
}
