#include "S3GF"
using namespace S3GF;

class TestWindow : public Window {
public:
    TestWindow(Engine* engine, const std::string& title = "Test Window", int w = 800, int h = 600)
        : Window(engine, title, w, h) {}

protected:
    void paintEvent() override {
        Window::paintEvent();
        auto r = renderer();
        r->fillBackground(StdColor::RedDark);
        r->drawPoint(Graphics::Point(300, 150, 20, StdColor::GreenWillow));
    }
    void unloadEvent() override {
        Logger::log("Trying to unload...");
        Window::unloadEvent();
    }
};

int main() {
    Logger::setBaseLogLevel(S3GF::Logger::DEBUG);
    Engine engine;
    engine.newWindow(new TestWindow(&engine, "再见世界！"));

    return engine.exec();
}
