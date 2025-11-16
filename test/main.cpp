#include "../src/Core.h"

using namespace S3GF;

int main(int argc, char* argv[]) {
    S3GF::Engine engine;
    engine.newWindow(new Window("Hello world!"));
    engine.setFPS(30);
    engine.window()->installPaintEvent([](Renderer* r) {
        r->fillBackground(StdColor::DarkGray);
    });
    return engine.exec();
}
