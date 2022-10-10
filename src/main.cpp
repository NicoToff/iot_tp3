#include <Arduino.h>
#include <LiquidCrystal.h>
#include "buttons.h"

const byte RS = 8;
const byte ENABLE = 9;
const byte D4 = 4;
const byte D5 = 5;
const byte D6 = 6;
const byte D7 = 7;
const byte SCREEN = 10; // Backlight

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

boolean btnWake_isBeingPressed = false;
boolean btnMode_isBeingPressed = false;
boolean btnPgm_isBeingPressed = false;
button_t btnWake = {42, &btnWake_isBeingPressed};
button_t btnMode = {43, &btnMode_isBeingPressed};
button_t btnPgm = {44, &btnPgm_isBeingPressed};

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

void setup()
{
    lcd.begin(COLS, ROWS);
    pinMode(SCREEN, OUTPUT);

    pinMode(btnWake.pin, INPUT_PULLUP);
    pinMode(btnMode.pin, INPUT_PULLUP);
    pinMode(btnPgm.pin, INPUT_PULLUP);

    Serial.begin(115200);
    lcd.write("TP3 - IoT");
    digitalWrite(SCREEN, HIGH);
    delay(2000);
    displayMode(lcd, selMode);
}

void loop()
{
    // Éteins l'écran après 5 secondes d'inactivité
    if (millis() - prevTime > SLEEP_DELAY)
    {
        digitalWrite(SCREEN, LOW);
    }

    while (isMaintained(btnWake))
    {
        digitalWrite(SCREEN, HIGH);
        prevTime = millis();
        if (isPressed(btnMode))
        {
            selMode = selMode == AUTO ? MANUAL : AUTO;
            displayMode(lcd, selMode);
            displayPgm(lcd, selMode, selPgm);
        }
        if (isPressed(btnPgm) && selMode == AUTO)
        {
            selPgm = (selPgm % 12) + 1; // Min 1, max 12
            displayPgm(lcd, selMode, selPgm);
        }
        delay(10);
    }

    delay(10);
}