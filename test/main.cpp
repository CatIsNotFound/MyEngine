#include "../src/Core.h"
#include "../src/MultiThread/Components.h"
#include "../src/Utils/Cursor.h"
#include "../src/Algorithm/Collider.h"

using namespace S3GF;
int main(int argc, char* argv[]) {
    Engine engine;
    engine.newWindow(new Window("Hello world!"));
    auto win = engine.window();
    win->setResizable(true);
    engine.setFPS(120);

    Timer timer(1000, [&engine, &win] {
        win->setWindowTitle(std::format("FPS: {}", engine.fps()));
    });

    Graphics::Triangle tri(100, 100, 200, 400, 300, 100, 8, StdColor::MixYellowLemon, StdColor::MixOrangeYellow);
    Graphics::Point pt(400, 200, 64, StdColor::GreenBean);
    Graphics::Point pt2(200, 200, 128, StdColor::RedLightPink, 320);
    Graphics::Line line3;
    Graphics::Rectangle rect(0, 0, 600, 400, 0, StdColor::GreenApple, StdColor::MixOrangeYellow);
    Graphics::Ellipse eli(300, 240, 300, 200, 6, StdColor::BlueDark, StdColor::BlueIndigoTrans, -135.f, 64);

    line3.setColor(StdColor::MixYellow);
    line3.setSize(20);
    line3.setStartPosition(500, 400);
    Geometry clip_area(0, 0, 300, 300);
    engine.window()->installPaintEvent([&pt, &pt2, &rect, &line3, &clip_area, &eli](Renderer* r) {
        // r->setClipView(clip_area);
        r->setBlendMode(SDL_BLENDMODE_BLEND);
        r->fillBackground({142, 198, 128, 128});
        r->drawRectangle(rect);
        r->drawPoint(pt);
        r->drawPoint(pt2);
    });
    EventSystem::global()->appendEvent(10, [&rect, &pt, &pt2](SDL_Event ev) {
        auto cur_pos = Cursor::global()->position();
        static bool is_first = true;
        if (is_first) {
            pt.move(cur_pos);
        } else {
            pt2.move(cur_pos);
        }
        if (ev.button.down && ev.button.button == SDL_BUTTON_LEFT) { is_first = !is_first; }
        if (Algorithm::comparePosInRect(cur_pos, rect) > 0) {
            Cursor::global()->setCursor(Cursor::Hand);
        } else {
            Cursor::global()->setCursor(Cursor::Default);
        }

        if (Algorithm::comparePoints(pt, pt2) >= 0) {
            pt.setColor(StdColor::RedDarkTrans);
            pt2.setColor(StdColor::MixYellowTrans);
        } else {
            pt.setColor(StdColor::BlueIndigoTrans);
            pt2.setColor(StdColor::MixPurpleTrans);
        }

    });

    timer.start(0);
    return engine.exec();
}
