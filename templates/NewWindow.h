
#ifndef MYENGINE_NEWWINDOW_H
#define MYENGINE_NEWWINDOW_H
#include <MyEngine>

class NewWindow : public MyEngine::Window {
public:
    explicit NewWindow(MyEngine::Engine* engine, const std::string& title, int width = 800, int height = 600);
    ~NewWindow();

    /// TODO: Add more functions to improve your custom window.

protected:
    /// TODO: The following are all the supported events,
    ///       which you can use to implement the functionality of your window.
    void paintEvent() override;
    void resizeEvent() override;
    void moveEvent() override;
    void getFocusEvent() override;
    void lostFocusEvent() override;
    void unloadEvent() override;
    void showEvent() override;
    void hideEvent() override;
    void windowMinimizedEvent() override;
    void windowMaximizedEvent() override;
    void enteredFullscreenEvent() override;
    void leaveFullscreenEvent() override;
    void mouseEnteredEvent() override;
    void mouseLeftEvent() override;
    void mouseUpEvent() override;
    void mouseDownEvent(MyEngine::MouseStatus button) override;
    void mouseClickedEvent(MyEngine::MouseStatus button) override;
    void mouseMovedEvent(const MyEngine::Vector2& position, const MyEngine::Vector2& distance) override;
    void keyUpEvent(SDL_Scancode keycode) override;
    void keyDownEvent(SDL_Scancode keycode) override;
    void keyPressedEvent(SDL_Scancode keycode) override;
    void dragInEvent() override;
    void dragOutEvent() override;
    void dragMovedEvent(const MyEngine::Vector2 &position, const char *url) override;
    void dropEvent(const char *url) override;
    /// TIPS: you can add more functions below. :)


private:
    /// TODO: Add the private members or functions here.

};

#endif //MYENGINE_NEWWINDOW_H
