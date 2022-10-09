#include <Arduino.h>
#include <LiquidCrystal.h>

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

typedef struct
{
    byte pin;
    boolean *isPressed;
} button_t;

boolean btnWake_isBeingPressed = false;
boolean btnMode_isBeingPressed = false;
boolean btnProg_isBeingPressed = false;
button_t buttons[] = {
    {42, &btnWake_isBeingPressed},
    {43, &btnMode_isBeingPressed},
    {44, &btnProg_isBeingPressed}};

/**
 * @brief Vérifie l'appui d'un bouton. Ne renvoie `true` qu'une fois par appui.
 * @param button Un objet de type button_t
 * @return `true` quand le bouton vient d'être pressé
 */
boolean isPressed(button_t button)
{
    if (!digitalRead(button.pin) && !*(button.isPressed))
    {
        return *(button.isPressed) = true;
    }
    else if (digitalRead(button.pin) && *(button.isPressed))
    {
        return *(button.isPressed) = false;
    }
    else
    {
        return false;
    }
}

void setup()
{
    lcd.begin(COLS, ROWS);
    pinMode(SCREEN, OUTPUT);
    for (size_t i = 0; i < sizeof(buttons) / sizeof(button_t); i++)
    {
        pinMode(buttons[i].pin, INPUT_PULLUP);
    }
    Serial.begin(115200);
    lcd.write("Hello");
    digitalWrite(SCREEN, LOW);
}

void loop()
{
    for (size_t i = 0; i < sizeof(buttons) / sizeof(button_t); i++)
    {
        if (isPressed(buttons[i]))
        {
            Serial.print("Button ");
            Serial.print(i);
            Serial.println(" just pressed");
        }
    }
}