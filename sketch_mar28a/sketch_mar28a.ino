
#include <Password.h>

#include <Keypad.h>

Password password = Password( "12" );


int sel_number;
int number_secret;
int tent;

const byte ROWS = 4; // Four rows
const byte COLS = 3; // Three columns
// Define the Keymap
char keys[ROWS][COLS] = {
  {'1', '2', '3',},
  {'4', '5', '6',},
  {'7', '8', '9',},
  {'*', '0', '#',}
};
// Connect keypad ROW0, ROW1, ROW2 and ROW3 to these Arduino pins.

byte rowPins[ROWS] = {8, 7, 6, 5};                                   //
byte colPins[COLS] = {4, 3, 2};
//byte rowPins[ROWS] = { 2, 3, 4, 5, };// Connect keypad COL0, COL1 and COL2 to these Arduino pins.
//byte colPins[COLS] = { 6, 7, 8 };


// Create the Keypad
Keypad keypad = Keypad( makeKeymap(keys), rowPins, colPins, ROWS, COLS );

#define ledPin 13

void setup() {

  digitalWrite(ledPin, LOW);   // sets the LED on
  Serial.begin(9600);
  keypad.addEventListener(keypadEvent); //add an event listener for this keypad
  keypad.setDebounceTime(250);
}

void loop() {
  keypad.getKey();
}

//take care of some special events
void keypadEvent(KeypadEvent eKey) {
  switch (keypad.getState()) {
    case PRESSED:
      Serial.print("Pressed: ");
      Serial.println(eKey);
      switch (eKey) {
        case '#': guessPassword(); break;
        default:
          password.append(eKey);
      }
  }
}

void guessPassword() {
  Serial.print("Validation en cours... ");
  if (password.evaluate()) {
    digitalWrite(ledPin, HIGH); //activates garaged door relay
    delay(500);
    digitalWrite(ledPin, LOW); //turns off door relay after .5 sec
    Serial.println("Mot de passe validé "); //
    Serial.println("A vous de jouer ");
    delay(500);
    password.reset(); //Reset du password
    number_secret = random(0, 10); // Chiffre généré aléatoirement
    tent = 3; // 5 Tentatives pour le joueur
    while (tent != 0) // Tant qu'il lui reste des tentatives il peut jouer
    {
      sel_number = 1;

      while (sel_number == 1) {
        sel_number = keypad.waitForKey() - 48;// = Chiffre Choisi
      }

      if (sel_number < number_secret) {         //
        tent = tent - 1;                        //
        Serial.print("Au dessus, ");            //Vérifie si le chiffre
        Serial.print("il vous reste : ");       //à trouver est
        Serial.print(tent);                     //au dessus
        Serial.println(" tentatives");          //
      }

      else if (sel_number > number_secret) {    //
        tent = tent - 1;                        //
        Serial.println(sel_number);             //Vérifie si le chiffre
        Serial.print("C'est en dessous, ");     //à trouver est
        Serial.print("il vous reste : ");       //en dessous
        Serial.print(tent);                     //
        Serial.println(" tentatives");          //
      }
      else if (sel_number = number_secret) {    //
        Serial.println("Bien joué ! ");         //Le bon chiffre à été trouver
        break;                                  //
      }
      else if (tent = 0) {                      //
        Serial.println("Perdu !");              //Le joueur n'a plus de tentatives
        break;                                  //
      }
    
    }


  } else {
    digitalWrite(ledPin, LOW);
    Serial.println("INVALID PASSWORD ");
    password.reset(); //resets password after INCORRECT entry
  }
}
