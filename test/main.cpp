#include "../src/Core.h"
#include "../src/Algorithm/Draw.h"
#include "../src/MultiThread/Components.h"
#include "../src/Utils/DateTime.h"

int main(int argc, char* argv[]) {
    S3GF::Engine engine;
    S3GF::DateTime::setDefaultTimezone("Asia/Shanghai");
    engine.newWindow(new S3GF::Window("Hello world!"));
    engine.window()->setResizable(true);
    engine.setFPS(30);
    S3GF::DateTime::DT dt = S3GF::DateTime::currentDateTime();
    auto timestamp = S3GF::DateTime::generateTimestamp(dt);
    S3GF::Timer timer(3000, [&timestamp] {
        auto parsed = S3GF::DateTime::parseFromTimestamp(timestamp);
        S3GF::Logger::log(S3GF::DateTime::formatDateTime(parsed, "%E"), S3GF::Logger::INFO);
        S3GF::Logger::log(S3GF::DateTime::formatDateTime(parsed, "%C"), S3GF::Logger::INFO);
    });

    S3GF::Graphics::Line line1(300, 100, 200, 600, 10, S3GF::StdColor::LightGreen);
    S3GF::Graphics::Line line2(100, 200, 400, 180, 32, S3GF::StdColor::DarkOrange);
    S3GF::Graphics::Triangle tri({100, 100}, {200, 200}, {0, 0}, {100, 100, 100, 255});
    S3GF::Graphics::Point pt(400, 200, 128, S3GF::StdColor::LightGreen);
    S3GF::Graphics::Point pt2(200, 200, 128, S3GF::StdColor::LightPink, 32);
    S3GF::Graphics::Line line3(line1);
    line3.setStartPosition(500, 400);

    engine.window()->installPaintEvent([&pt, &pt2](S3GF::Renderer* r) {
        r->fillBackground(S3GF::StdColor::DarkGray);
        r->drawPoint(pt);
        r->drawPoint(pt2);
    });

    timer.start(0);
    return engine.exec();
}
