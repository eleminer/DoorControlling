#include <Adafruit_NeoPixel.h>

const int endswitch1=3; //Tür geschlossen, Endschalter
const int endswitch2=4; // Tür offen, Endschalter
const int motiondetector1=5; //Bewegungsmelder 1
const int motiondetector2=6; //Bewegungsmelder 2
const int motorforward=7; // Mosfet Motor "Vor"
const int motorbackward=8; // Mosfet Motor "Zurück"
const int currentsensor=A3; // Stromsensor Motor
const int LED_PIN=9; //Neopixel Ring Datenleitung 12Pixel
Adafruit_NeoPixel pixels(12, LED_PIN, NEO_GRB + NEO_KHZ800);


const long openingIntervalTime = 10*1000; //10Sekunden in millis
unsigned long currentMillis= 0;
unsigned long previousMillis = 0;


typedef struct sensordataSwitches{
  bool sensorSwitch1;
  bool sensorSwitch2;
  bool sensorMotiondetector1;
  bool sensorMotiondetector2;
};
sensordataSwitches door = {false,false,false,false};


void setup() 
{
  pinMode(endswitch1, INPUT_PULLUP); pinMode(endswitch2, INPUT_PULLUP); pinMode(motiondetector1, INPUT_PULLUP); pinMode(motiondetector2, INPUT_PULLUP);
  digitalWrite(motorforward, LOW); digitalWrite(motorbackward, LOW);
  pinMode(motorforward, OUTPUT); pinMode(motorbackward, OUTPUT);
  pixels.begin(); pixels.clear(); pixels.show();
}

void ReadDataEndSwitches()
{
if (digitalRead(endswitch1)==LOW){door.sensorSwitch1=true;}
if (digitalRead(endswitch2)==LOW){door.sensorSwitch2=true;}
}

void ReadDataMotionDetector()
{
if (digitalRead(motiondetector1)==LOW){door.sensorMotiondetector1=true;}
if (digitalRead(motiondetector2)==LOW){door.sensorMotiondetector2=true;}
}

void NeopixelRing(int *array)
{
  for(int i=0; i<12;i++)
  {
    if (array[i]==0)
    {
      pixels.setPixelColor(i,pixels.Color(0,0,0));
    }
    else if (array[i]==1) //blau
    {
      pixels.setPixelColor(i,pixels.Color(0,0,255));
    }
    else if (array[i]==2) //grün
    {
      pixels.setPixelColor(i,pixels.Color(0,255,0));
    }
    else if (array[i]==2) //rot
    {
      pixels.setPixelColor(i,pixels.Color(255,0,0));
    }
  }
  pixels.show();
}

bool sensorDoorClosed()
{
  if (door.sensorSwitch1==true)
  {
    door.sensorSwitch1=false;
    return 1;
  }
  else
  {
    return 0;
  }
}

bool sensorDoorOpen()
{
  if (door.sensorSwitch2==true)
  {
    door.sensorSwitch2=false;
    return 1;
  }
  else
  {
    return 0;
  }
}

bool sensorMotionHandling()
{
if (door.sensorMotiondetector1==true || door.sensorMotiondetector2==true)
  {
    door.sensorMotiondetector1=false; door.sensorMotiondetector2=false;
    return 1;
  }
else
  {
    return 0;
  }
}

void allNeopixelBlue(int *array)
{
  for(int i=0; i<12;i++)
  {
    array[i]=1;
  }
}

void allNeopixelRed(int *array)
{
  for(int i=0; i<12;i++)
  {
    array[i]=2;
  }
}

void motor(String rotation)
{
  if (rotation=="backward")
  {
    digitalWrite(motorforward, LOW); digitalWrite(motorbackward, HIGH);
  }
  if (rotation=="forward")
  {
    digitalWrite(motorforward, HIGH); digitalWrite(motorbackward, LOW);
  }
  if (rotation=="stop")
  {
    digitalWrite(motorforward, LOW); digitalWrite(motorbackward, LOW);
  }
}

void loop() 
{
  int colorarray[12];
  while (1)
  {
    currentMillis = millis();
    ReadDataMotionDetector(); ReadDataEndSwitches();
        if(sensorDoorClosed()==1) //wenn Tür zu
        {
          allNeopixelBlue(colorarray);
          NeopixelRing(colorarray);
          while(sensorMotionHandling()==0)
          {
            ReadDataMotionDetector();
          }
          while(sensorDoorOpen()==0)
          motor("backward");
          ReadDataEndSwitches();
        }


        else if (sensorDoorOpen()==1) //wenn Tür offen
        {
          allNeopixelRed(colorarray);
          NeopixelRing(colorarray);
          currentMillis=millis();
          previousMillis=millis();
          ReadDataEndSwitches();
          while(sensorDoorOpen()==1)
          {
            currentMillis=millis();
            ReadDataMotionDetector();
              if(sensorMotionHandling()==1)
              {
                previousMillis=millis();
              }
            if(currentMillis - previousMillis>=openingIntervalTime)
            {
                while(sensorDoorClosed()==0 && sensorMotionHandling()==0)
                {
                  ReadDataMotionDetector(); ReadDataEndSwitches();
                  motor("forward");
                }
                  motor("stop");
            }
          }
        }

        else //Position nicht bekannt! Fahre auf!
        {
          while(sensorDoorOpen()==0)
          {
          motor("backward");
          ReadDataEndSwitches();
          }
          motor("stop");
        }
  } 
}
