
#ifndef MYENGINE_CUSTOMWIDGET_H
#define MYENGINE_CUSTOMWIDGET_H

#include <MyEngine>

class MyWidget : public MyEngine::Widget::AbstractWidget {
public:
    explicit MyWidget(std::string object_name, MyEngine::Window* window);
    explicit MyWidget(MyEngine::Window* window);
    ~MyWidget();

    // TODO: Add the functions to improve your widgets

protected:
    // TODO: The following are all the supported events,
    //       which you can use to implement the functionality of your widget.
    void loadEvent() override;
    void unloadEvent() override;
    void moveEvent(const MyEngine::Vector2& position) override;
    void resizeEvent(const MyEngine::Size& size) override;
    void enableChangedEvent(bool enabled) override;
    void visibleChangedEvent(bool visible) override;
    void objectNameChangedEvent(const std::string& object_name) override;
    void paintEvent(MyEngine::Renderer *renderer) override;
    void focusInEvent() override;
    void focusOutEvent() override;
    void mouseClickedEvent() override;
    void mouseDblClickedEvent() override;
    void mouseDownEvent() override;
    void mouseUpEvent() override;
    void mouseEnteredEvent() override;
    void mouseLeftEvent() override;
    void customContextMenuRequestEvent(const Vector2 &position) override;
    void keyDownEvent(SDL_Scancode scancode) override;
    void keyUpEvent(SDL_Scancode scancode) override;
    void keyPressedEvent(SDL_Scancode scancode) override;
    void hotKeysPressedEvent() override;
    void fingerDownEvent(const MyEngine::Vector2& position) override;
    void fingerUpEvent(const MyEngine::Vector2& position) override;
    void fingerMovedEvent(const MyEngine::Vector2& position, const MyEngine::Vector2& distance) override;
    void fingerMoveOutEvent() override;
    void fingerMoveInEvent() override;
    void fingerTappedEvent() override;
    void startedInputEvent() override;
    void endedInputEvent() override;
    void inputEvent(const char* string) override;
    void propertyChanged(const std::string& property, const Variant& variant) override;

private:
    // TODO: Add the private members what your widget need


};

#endif //MYENGINE_CUSTOMWIDGET_H
