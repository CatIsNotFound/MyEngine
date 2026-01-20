
#ifndef MYENGINE_MYWINDOW_H
#define MYENGINE_MYWINDOW_H
#include "MyEngine"

class MyWindow : public MyEngine::Window {
public:
    explicit MyWindow(MyEngine::Engine* engine, std::string title, int w = 800, int h = 600)
            : MyEngine::Window(engine, std::move(title), w, h) {}

protected:
    void windowMinimizedEvent() override {
        MyEngine::Logger::log("Minizied window!");
    }

    void windowMaximizedEvent() override {
        MyEngine::Logger::log("Maximized window!");
    }

    void windowRestoredEvent() override {
        MyEngine::Logger::log("Restored window!");
    }

    void enteredFullscreenEvent() override {
        MyEngine::Logger::log("Entered fullscreen");
    }

    void leaveFullscreenEvent() override {
        MyEngine::Logger::log("Exited fullscreen");
    }
private:
};

#endif //MYENGINE_MYWINDOW_H
