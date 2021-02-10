#include <Adafruit_NeoPixel.h>

const int endswitch1=3; //Tür geschlossen, Endschalter
const int endswitch2=4; // Tür offwn, Endschalter
const int motiondetector1=5; //Bewegungsmelder 1
const int motiondetector2=6; //Bewegungsmelder 2
const int motorforward=7; // Mosfet Motor "Vor"
const int motorbackward=8; // Mosfet Motor "Zurück"
const int currentsensor=A3; // Stromsensor Motor
const int LED_PIN=9; //Neopixel Ring Datenleitung 12Pixel
Adafruit_NeoPixel pixels(12, LED_PIN, NEO_GRB + NEO_KHZ800);

typedef struct sensordataSwitches{
  bool sensorSwitch1;
  bool sensorSwitch2;
  bool sensorMotiondetector1;
  bool sensorMotiondetector2;
}
sensordataSwitches door = {false,false,false,false};


void setup() {
  pinMode(endswitch1, INPUT_PULLUP); pinMode(endswitch2, INPUT_PULLUP); pinMode(motiondetector1, INPUT_PULLUP); pinMode(motiondetector2, INPUT_PULLUP);
  digitalWrite(motorforward, LOW); digitalWrite(motorbackward, LOW);
  pinMode(motorforward, OUTPUT); pinMode(motorbackward, OUTPUT);
  pixels.clear(); pixels.show();
}



void loop() {
  // put your main code here, to run repeatedly:

}



