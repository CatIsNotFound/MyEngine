
#ifndef S3GF_BUTTON_H
#define S3GF_BUTTON_H
#include "../Basic.h"
#include "Control.h"
namespace S3GF {
    class Button : public AbstractControl {
    public:
        explicit Button(const std::string& name, Renderer* renderer, const Vector2& position = {0, 0},
                             AbstractControl* parent = nullptr);


    protected:
        void onEntered() override;
        void onHovered() override;
        void onLeft() override;
        void onPressed() override;
        void resizeEvent() override;
        void moveEvent() override;
        void paintEvent(Renderer* ren) override;

    private:
        Graphics::Rectangle _rect;
        std::string _font_name;
        std::string _text_name;
        std::string _btn_text;

    };

} // S3GF

#endif //S3GF_BUTTON_H
