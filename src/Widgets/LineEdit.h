#ifndef MYENGINE_LINEEDIT_H
#define MYENGINE_LINEEDIT_H
#include "AbstractWidget.h"

#define ENGINE_PROP_LINE_EDIT_BORDER_COLOR_INPUT       "lineEdit.input.borderColor"
#define ENGINE_PROP_LINE_EDIT_BACKGROUND_COLOR_INPUT   "lineEdit.input.backgroundColor"
#define ENGINE_PROP_LINE_EDIT_BORDER_COLOR_NORMAL      "lineEdit.normal.borderColor"
#define ENGINE_PROP_LINE_EDIT_BACKGROUND_COLOR_NORMAL  "lineEdit.normal.backgroundColor"
#define ENGINE_SIGNAL_LINE_EDIT_PASSWORD_MODE          0b1
#define ENGINE_SIGNAL_LINE_EDIT_HAS_TEXT               0b10
#define ENGINE_SIGNAL_LINE_EDIT_HAS_TEXT               0b10

namespace MyEngine {
    namespace Widget {
        class LineEdit : public AbstractWidget {
        public:
            explicit LineEdit(Window* window);
            explicit LineEdit(std::string object_name, Window* window);
            ~LineEdit();

            void setText(const std::string& text);
            [[nodiscard]] std::string_view text() const;
            [[nodiscard]] const StringList& textList() const;
            void setPlaceHolderText(const std::string& text);
            [[nodiscard]] const std::string& placeHolderText() const;
            void setPasswordEnabled(bool enabled, const std::string& secret = "●");
            [[nodiscard]] bool passwordEnabled() const;


        protected:
            void loadEvent() override;
            void unloadEvent() override;
            void paintEvent(MyEngine::Renderer *renderer) override;
            void enableChangedEvent(bool enabled) override;
            void resizeEvent(const MyEngine::Size &size) override;
            void keyPressedEvent(SDL_Scancode scancode) override;
            void mouseEnteredEvent() override;
            void mouseLeftEvent() override;
            void mouseClickedEvent() override;
            void fingerTappedEvent() override;
            void startedInputEvent() override;
            void endedInputEvent() override;
            void inputEvent(const char *string) override;
            virtual void textChangedEvent();
            virtual void placeHolderTextVisibleChangedEvent(bool visible);
        private:
            uint32_t _cur_pos{};
            std::string _hold_placer_text, _secret_char;
            StringList _strings;
            uint16_t _changer_signal{};
        };
    }
}

#endif //MYENGINE_LINEEDIT_H
