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

    Graphics::Rectangle rect1(0, 0, 100, 100, 2, StdColor::RedDark, StdColor::RedLightTrans);
    Graphics::Rectangle rect2(400, 300, 200, 150, 4, StdColor::BlueDark, StdColor::BlueSeaTrans);

    Geometry clip_area(0, 0, 300, 300);
    engine.window()->installPaintEvent([&rect1, &rect2](Renderer* r) {
        // r->setClipView(clip_area);
        r->setBlendMode(SDL_BLENDMODE_BLEND);
        r->fillBackground({142, 198, 128, 128});
        r->drawRectangle(rect1);
        r->drawRectangle(rect2);
    });
    EventSystem::global()->appendEvent(10, [&rect1, &rect2](SDL_Event ev) {
        auto cur_pos = Cursor::global()->position();
        rect1.setGeometry(cur_pos, rect1.geometry().size);
        if (Algorithm::compareRects(rect1, rect2) >= 0) {
            rect1.setBackgroundColor(StdColor::MixYellowTrans);
            rect2.setBackgroundColor(StdColor::MixPurpleTrans);
        } else {
            rect1.setBackgroundColor(StdColor::RedLightTrans);
            rect2.setBackgroundColor(StdColor::BlueSeaTrans);
        }
        if (Algorithm::comparePosInRect(cur_pos, rect2) >= 0) {
            Cursor::global()->setCursor(Cursor::Hand);
        } else {
            Cursor::global()->setCursor(Cursor::Default);
        }
    });

    timer.start(0);
    return engine.exec();
}
