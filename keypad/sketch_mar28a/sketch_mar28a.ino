/*
  Mini système d'alarme - Matrix Keypad + LEDs
  Arduino IDE 1.0.1
*/

#include "Password.h" // http://www.arduino.cc/playground/uploads/Code/Password.zip
#include "Keypad.h"   // http://www.arduino.cc/playground/uploads/Code/keypad.zip

#define green 12                   // LED rouge attachée à pin 12 de l'Arduino
#define red 13                     // LED verte attachée à pin 13 de l'Arduino

Password pwd = Password("1234");   // définition du mot de passe

boolean alarm = false;             // variable stockant l'état de l'alarme
                                   // false = alarme OFF | true = alarme ON
boolean intruder = false;          // variable stockant l'état de l'intrusion
                                   // false = pas d'intrus détecté | true = intrus détecté
                                  
unsigned long previousMillis = 0;  // dernière fois que la LED rouge a clignoté 
int interval = 250;                // intervalle auquel faire clignoter la LED

#define ROWS 4                                                       // 
#define COLS 3                                                       // 
char keys[ROWS][COLS] =                                              //
{                                                                    //
  {'1','2','3'},                                                     //
  {'4','5','6'},                                                     // configuration
  {'7','8','9'},                                                     // du clavier
  {'*','0','#'}                                                      // Orange 8 / Jaune 7 / Vert 6 / Bleu 5 / Rouge 4 / Noir 3 / Blanc 2
};                                                                   //
byte rowPins[ROWS] = {8, 7, 6, 5};                                   //
byte colPins[COLS] = {4, 3, 2};                                      //
                                                                     //
Keypad kpd = Keypad(makeKeymap(keys), rowPins, colPins, ROWS, COLS); //

void setup()
{
  pinMode(red, OUTPUT);
  pinMode(green, OUTPUT);
  
  kpd.addEventListener(kpdEvent);
}

void loop()
{
  char Key = kpd.getKey();
  // en cas d'intrus détecté :
  if(intruder == true && alarm == true)
  {
    unsigned long currentMillis = millis();
    // ... on regarde s'il est temps de faire clignoter la LED, c'est-à-dire
    // s'il s'est passé suffisamment de temps depuis la dernière fois qu'on a vérifié
    if(currentMillis - previousMillis > interval)
    {
      previousMillis = currentMillis;
      digitalWrite(red, !digitalRead(red));
    }
  }
  else
  {
    digitalWrite(red, LOW);
  }
}

void kpdEvent (KeypadEvent Key)
{
  switch (kpd.getState())
  {
    case PRESSED :
      switch (Key)
      {
        // appui sur '*' -> vérification de la saisie en cours
        case '*' : checkPassword(); break;
        // appui sur '#' -> réinitialisation de la saisie en cours
        case '#' : pwd.reset(); break;
        // sinon on ajoute le chiffre à la combinaison
        default  : pwd.append(Key); break;
      }
    default : break;
  }
}

void checkPassword(void)
{
  // on remet à zéro l'état du mot de passe
  intruder = false;
  // si le mot de passe est juste...
  if (pwd.evaluate())
  {
    // ...on met à jour l'état de l'alarme : ON>OFF / OFF>ON
    alarm = !alarm;
    
    switch (alarm)
    {
      case true :
      // la LED rouge signale la mise en service de l'alarme
        digitalWrite(red, HIGH);
        delay(2000);
        digitalWrite(red, LOW);
        pwd.reset();  // on remet à zéro la saisie
        break;
      case false : 
      // la LED verte signale l'extinction de l'alarme
        digitalWrite(green, HIGH);
        delay(2000);
        digitalWrite(green, LOW);
        pwd.reset();  // on remet à zéro la saisie
        break;
      default : 
        break;
    }
  }
  // si le mot de passe est faux...
  else
 {
   // ...on signale l'intrusion à la loop() qui déclenche l'alarme
   // jusqu'à ce que le bon mot de passe soit rentré
   intruder = true;
 }
 // on remet à zéro systématiquement après avoir vérifié pour ne pas avoir d'erreur
 pwd.reset();
}
