#include "FilledWin.h"
#include "MyEngine"
#include "Widgets/AbstractWidget.h"
using namespace MyEngine;

int main() {
    Logger::setBaseLogLevel(MyEngine::Logger::Debug);
    Engine engine;
    engine.setFPS(60);
    auto window = new Window(&engine, engine.applicationName(), 1024, 800);
    auto win2 = new Window(&engine, "2nd window");
    window->setResizable(true);
    Widget::AbstractWidget wid(window);
    wid.setFocusEnabled(true);
    wid.setGeometry(100, 200, 100, 200);
    wid.setHotKey(SDL_SCANCODE_LCTRL, SDL_SCANCODE_LALT, SDL_SCANCODE_SPACE);
    wid.printHotKeys();
    Graphics::Rectangle rect(300, 300, 100, 200, 8, RGBAColor::GreenDark, RGBAColor::MixOrangeYellow);
    Graphics::Rectangle rect2(200, 100, 200, 100, 8, RGBAColor::RedDark, RGBAColor::RedLightPink);
    Graphics::Rectangle rect3(100, 200, 100, 200);
    window->installPaintEvent([&rect, &rect2, &rect3](Renderer* r) {
        auto ro = rect.rotate();
        if (ro + 1.f >= 360.f) ro = 0; else ro += 1.f;
        rect.setRotate(ro);
        rect2.setRotate(360.f - ro);
        r->drawRectangle(&rect3);
        r->drawRectangle(&rect);
        r->drawRectangle(&rect2);
        r->drawDebugFPS();
        auto mouse_pos = EventSystem::global()->captureMousePosition();
        r->drawDebugText(std::format("Mouse pos: ({}, {})", mouse_pos.x, mouse_pos.y), {20, 30});
    });
    win2->installPaintEvent([](Renderer* r) {
        r->fillBackground(RGBAColor::BlueLake);
        r->drawDebugFPS();
    });

    EventSystem::global()->appendEvent(IDGenerator::getNewEventID(), [&window, &rect, &rect2](SDL_Event e) {
        if (Cursor::global()->focusOn() != window->windowID()) return;
        auto mouse_pos = Cursor::global()->position();
        static bool trigger = false;
        static bool drag_1 = false, drag_2 = false;
        static Vector2 old_pos_1, old_pos_2;
        if (EventSystem::global()->captureMouse(EventSystem::Left)) {
            if (!trigger) {
                trigger = true;
                if (Algorithm::comparePosInRotatedRect(mouse_pos, rect) > 0) {
                    drag_1 = true;
                    old_pos_1 = rect.geometry().pos;
                    Cursor::global()->setCursor(Cursor::Move);
                    return;
                } else if (Algorithm::comparePosInRotatedRect(mouse_pos, rect2) > 0) {
                    drag_2 = true;
                    old_pos_2 = rect2.geometry().pos;
                    Cursor::global()->setCursor(Cursor::Move);
                    return;
                }
            }
            auto moved_distance = EventSystem::global()->captureMouseAbsDistance();
            if (drag_1) {
                rect.move(old_pos_1 + moved_distance);
            } else if (drag_2) {
                rect2.move(old_pos_2 + moved_distance);
            }
        } else {
            if (trigger) {
                trigger = false;
                drag_1 = false;
                drag_2 = false;
                if (rect.geometry().pos < Vector2(0, 0)) rect.move(200, 200);
                if (rect2.geometry().pos < Vector2(0, 0)) rect2.move(400, 200);
                Cursor::global()->setCursor(Cursor::Normal);
            }
        }

    });

    return engine.exec();
}