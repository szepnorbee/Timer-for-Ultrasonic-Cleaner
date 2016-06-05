#include <LEDBlinker.h>

unsigned long keyPrevMillis = 0;
const unsigned long keySampleIntervalMs = 25;
byte longKeyPressCountMax = 80;    // 80 * 25 = 2000 ms
byte mediumKeyPressCountMin = 20;    // 20 * 25 = 500 ms
byte KeyPressCount = 0;

byte prevKeyState = HIGH;         // button is active low

// Pin nummering for attiny45

const byte button = 0;  // Input switch
LEDBlinker led(4);      // Blue led (display time)
int powerLed = 3;       // Red led (standby)
int out = 1;            // Output (low or high)

int timer = 0;

unsigned long previousMillis = 0;
const long interval = 1000; 

// called when button is kept pressed for less than .5 seconds
void shortKeyPress() {
    timer=timer+300;
    if (timer>1200) timer = 1200;
}


// called when button is kept pressed for more than 2 seconds
void mediumKeyPress() {
    timer = 0;
}


// called when button is kept pressed for 2 seconds or more
void longKeyPress() {
    timer = 0;
}

// called when key goes from not pressed to pressed
void keyPress() {
    KeyPressCount = 0;
}


// called when key goes from pressed to not pressed
void keyRelease() {
    
    if (KeyPressCount < longKeyPressCountMax && KeyPressCount >= mediumKeyPressCountMin) {
        mediumKeyPress();
    }
    else {
      if (KeyPressCount < mediumKeyPressCountMin) {
        shortKeyPress();
      }
    }
}

void setup() {
  
  pinMode(button,INPUT_PULLUP);
  pinMode(powerLed,OUTPUT);
  pinMode(out,OUTPUT);
  
  led.off();                   // Timer led off
  digitalWrite(powerLed,LOW);  // Power led on

}

void loop() {
  led.update();
  unsigned long currentMillis = millis();

  if (currentMillis - previousMillis >= interval) {

    previousMillis = currentMillis;
    
    if (timer!=0) timer--;
    
    if (timer == 0) {
      digitalWrite(out,LOW);        // Standby
      digitalWrite(powerLed,HIGH);
    } else {
      digitalWrite(out,HIGH);       // Working
      digitalWrite(powerLed,LOW);
    }
      blink();
  }

//////////////// KEY management ////////////////

    if (millis() - keyPrevMillis >= keySampleIntervalMs) {
        keyPrevMillis = millis();
        
        byte currKeyState = digitalRead(button);
        
        if ((prevKeyState == HIGH) && (currKeyState == LOW)) {
            keyPress();
        }
        else if ((prevKeyState == LOW) && (currKeyState == HIGH)) {
            keyRelease();
        }
        else if (currKeyState == LOW) {
            KeyPressCount++;
    if (KeyPressCount >= longKeyPressCountMax) {
        longKeyPress();
         }
        }        
        prevKeyState = currKeyState;
    }

///////////////////////////////
}

void blink() {

     if (timer<1) {
         led.off();
     } else if (timer>1 && timer<300) {
         led.blink(50, 150, 1, 900, 0, NULL);
     } else if (timer>=300 && timer<600) {
         led.blink(50, 150, 2, 900, 0, NULL);
     } else if (timer>=600 && timer<900) {
         led.blink(50, 150, 3, 900, 0, NULL);
     } else if (timer>900) {
         led.on();
     }   
}



