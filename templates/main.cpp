#include <MyEngine>
#include "NewWindow.h"
using namespace MyEngine;

int main(int argc, char* argv[]) {
    Engine engine("your.application.name", "v1.0.0", "your.application.id");
    engine.setApplicationTypeName("Game;App");
    engine.setApplicationAuthor("Your Name");
    engine.setApplicationCopyright("(C) 2026 ...... Reserved");
    engine.setApplicationURL("https://....");
    engine.setFPS(30);
    auto my_window = new NewWindow(&engine, engine.applicationName().data(), 1024, 800);
    my_window->show();
    return engine.exec();
}

