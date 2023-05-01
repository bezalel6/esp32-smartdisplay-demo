// 测试通过

#include <FS.h>
#include <SD.h>
#include <SPI.h>

#include <vector>

#include "ESP32_SPI_9341.h"

#define SD_SCK 18
#define SD_MISO 19
#define SD_MOSI 23
#define SD_CS 5

#define LIGHT_ADC 34

int led_pin[3] = {17, 4, 16};

SPIClass SD_SPI;

LGFX lcd;
void touch_calibration();
void touch_continue();
void led_set(int i);
void setup(void) {
    pinMode(led_pin[0], OUTPUT);
    pinMode(led_pin[1], OUTPUT);
    pinMode(led_pin[2], OUTPUT);

    Serial.begin(115200);

    // pinMode(LCD_BL, OUTPUT);
    // digitalWrite(LCD_BL, HIGH);

    lcd.init();
    // sd_init();
    // sd_test();

    // lcd.setRotation(0);
    // print_img(SD, "/logo.bmp", 240, 320);

    delay(1000);

    // touch_calibration();
    touch_continue();
}

static int colors[] = {TFT_RED, TFT_GREEN, TFT_BLUE, TFT_YELLOW, TFT_OLIVE};

int i = 0;
long runtime_0 = 0;
long runtime_1 = 0;

void loop(void) {
    if ((millis() - runtime_0) > 1000) {
        led_set(i);
        lcd.fillScreen(colors[i++]);

        if (i > 4) {
            i = 0;
        }

        runtime_0 = millis();
    }

    if ((millis() - runtime_1) > 300) {
        int adc_value = analogRead(LIGHT_ADC);
        Serial.printf("ADC:%d\n", adc_value);
        if (adc_value > 50)
            lcd.setBrightness(50);
        else
            lcd.setBrightness(255);
        runtime_1 = millis();
    }
    delay(10);
}

void touch_calibration() {
    lcd.fillScreen(TFT_YELLOW);

    lcd.setTextColor(TFT_BLACK);
    lcd.setTextSize(4);
    lcd.setCursor(70, 110);
    lcd.println("SCREEN");
    lcd.setCursor(70, 150);
    lcd.println("CALIBRATION");

    // タッチを使用する場合、キャリブレーションを行います。画面の四隅に表示される矢印の先端を順にタッチしてください。
    std::uint16_t fg = TFT_WHITE;
    std::uint16_t bg = TFT_BLACK;
    if (lcd.isEPD())
        std::swap(fg, bg);
    lcd.calibrateTouch(nullptr, fg, bg, std::max(lcd.width(), lcd.height()) >> 3);
}

void touch_continue() {
    lcd.fillScreen(TFT_YELLOW);

    lcd.fillRect(60, 100, 120, 120, TFT_BLACK);

    lcd.setTextColor(TFT_MAGENTA);
    lcd.setTextSize(2);
    lcd.setCursor(70, 110);
    lcd.println(" TOUCH");
    lcd.setCursor(70, 130);
    lcd.println("  TO");
    lcd.setCursor(70, 150);
    lcd.println("CONTINUE");

    int pos[2] = {0, 0};

    lcd.drawPixel(50, 50, TFT_BROWN);

    while (1) {
        if (lcd.getTouch(&pos[0], &pos[1])) {
            if (pos[0] > 60 && pos[0] < 180 && pos[1] > 120 && pos[1] < 240)
                break;
            delay(100);
        }
    }
}

void led_set(int i) {
    if (i == 0) {
        digitalWrite(led_pin[0], LOW);
        digitalWrite(led_pin[1], HIGH);
        digitalWrite(led_pin[2], HIGH);
    }
    if (i == 1) {
        digitalWrite(led_pin[0], HIGH);
        digitalWrite(led_pin[1], LOW);
        digitalWrite(led_pin[2], HIGH);
    }
    if (i == 2) {
        digitalWrite(led_pin[0], HIGH);
        digitalWrite(led_pin[1], HIGH);
        digitalWrite(led_pin[2], LOW);
    }

    if (i == 3) {
        digitalWrite(led_pin[0], LOW);
        digitalWrite(led_pin[1], LOW);
        digitalWrite(led_pin[2], LOW);
    }
}
