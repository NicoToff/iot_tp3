#include <Arduino.h>

// Création d'un type pour les boutons, permettant de retenir leur état
typedef struct
{
    byte pin;
    boolean *isPressed;
} button_t;

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

/**
 * @brief Vérifie si un bouton branché en INPUT_PULLUP est appuyé ou relâché.
 * @param button Le bouton à vérifier.
 * @param wiringMode INPUT ou INPUT_PULLUP (par défaut : INPUT_PULLUP).
 * @return `true` si le bouton est appuyé.
 */
boolean isMaintained(button_t button, uint8_t wiringMode = INPUT_PULLUP)
{
    if (wiringMode == INPUT_PULLUP)
    {
        return !digitalRead(button.pin);
    }
    else
    {
        return digitalRead(button.pin);
    }
}