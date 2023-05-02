#include "button.hpp"

void Button::draw(LGFX lcd) {
    lcd.setColor(TFT_GOLD);
    Rectangle bounds = this->bounds;
    lcd.drawRect(bounds.origin.x, bounds.origin.y, bounds.w, bounds.h);
    Point middle = bounds.getMiddle();
    lcd.setCursor(middle.x, middle.y);
    lcd.setColor(TFT_BLACK);
    lcd.print(this->text);
}
