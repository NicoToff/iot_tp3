#include <Arduino.h>
#include <LiquidCrystal.h>

#define BTN_WAKE 1 << PD3
#define btnWakeIsPressed() (PIND & BTN_WAKE) == 0
#define BTN_MODE 1 << PD2
#define BTN_MODE_PIN 19
#define BTN_PGM 1 << PD1
#define BTN_PGM_PIN 20

#define BACKLIGHT 1 << PB4 // Pin 10
#define backlightOn() PORTB |= BACKLIGHT
#define backlightOff() PORTB &= ~BACKLIGHT

const byte RS = 8;
const byte ENABLE = 9;
const byte D4 = 4;
const byte D5 = 5;
const byte D6 = 6;
const byte D7 = 7;

LiquidCrystal lcd(RS, ENABLE, D4, D5, D6, D7);

const byte COLS = 16;
const byte ROWS = 2;

unsigned long prevTime = 0;
const int SLEEP_DELAY = 5000;

// Variables pour les modes et les programmes
const char *MODES[] = {"Mode manuel", "Mode automatique"};

enum selMode_t
{
    MANUAL, // 0
    AUTO    // 1
};

byte selPgm = 1;

selMode_t selMode = MANUAL;

void displayMode(LiquidCrystal lcd, selMode_t selMode)
{
    lcd.clear();
    delay(100);
    lcd.setCursor(0, 0);
    lcd.print(MODES[selMode]);
}

void displayPgm(LiquidCrystal lcd, selMode_t selMode, int selPgm)
{
    if (selMode == AUTO)
    {
        lcd.setCursor(0, 1);
        lcd.write("PGM");
        selPgm < 10 && lcd.write("0");
        lcd.print(selPgm);
    }
    else
    {
        lcd.setCursor(0, 1);
        lcd.write("     ");
    }
}

void changeMode()
{
    if (btnWakeIsPressed())
    {
        selMode = selMode == AUTO ? MANUAL : AUTO;
        displayMode(lcd, selMode);
        displayPgm(lcd, selMode, selPgm);
    }
}

void changePgm()
{
    if (btnWakeIsPressed() && selMode == AUTO)
    {
        selPgm = (selPgm % 12) + 1; // Min 1, max 12
        displayPgm(lcd, selMode, selPgm);
    }
}

void setup()
{
    const byte BTN_MASK = (BTN_WAKE | BTN_MODE | BTN_PGM); // == 0b11100000

    DDRD |= BTN_MASK;  // Mis à "output"
    PORTD |= BTN_MASK; // pullup

    DDRB |= BACKLIGHT; // Backlight en output

    Serial.begin(115200);
    attachInterrupt(digitalPinToInterrupt(BTN_MODE_PIN), changeMode, FALLING);
    attachInterrupt(digitalPinToInterrupt(BTN_PGM_PIN), changePgm, FALLING);

    lcd.begin(COLS, ROWS);
    lcd.write("TP3 - IoT");
    backlightOn();
    delay(2000);
    displayMode(lcd, selMode);
}

void loop()
{
    // Éteins l'écran après 5 secondes d'inactivité
    if (!btnWakeIsPressed() && millis() - prevTime > SLEEP_DELAY)
    {
        backlightOff();
    }

    while (btnWakeIsPressed())
    {
        backlightOn();
        prevTime = millis();
        delay(2000);
    }

    delay(10);
}