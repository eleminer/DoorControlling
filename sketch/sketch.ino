#include <Adafruit_NeoPixel.h>

const int endswitch1=3; //Tür geschlossen, Endschalter
const int endswitch2=4; // Tür offen, Endschalter
const int motiondetector1=5; //Bewegungsmelder 1
const int motiondetector2=6; //Bewegungsmelder 2
const int motorforward=7; // Mosfet Motor "Vor"
const int motorbackward=8; // Mosfet Motor "Zurück"
const int currentsensor=A3; // Stromsensor Motor
const int LED_PIN=9; //Neopixel Ring Datenleitung 12Pixel
const int currentlimit=1000; //Wert zwischen 0 und 1023 Stromsensor
const int closetimeMAX=4*1000; //4Sekunden in Millis
const long openingIntervalTime=10*1000; //10Sekunden in millis
const long speedLEDRing=400; //Zeit wie lange ein Pixel vom Ring leuchten soll in ms
unsigned long currentMillis=0;
unsigned long previousMillis=0;
unsigned long previousMillisLED=0;
unsigned long previousMillisLEDf=0;
int internalArrayPosition=0;
Adafruit_NeoPixel pixels(12, LED_PIN, NEO_GRB + NEO_KHZ800);

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

bool currentWatch()
{
  int value=0;
  value=analogRead(currentsensor); //
  if (value>=currentlimit)
  {
    return 1;
  }
  else
  {
    return 0;
  }
}

void neopixelblueclockwise(int *array)
{
currentMillis = millis();
if (currentMillis-previousMillisLED>speedLEDRing)
  {
    for(int i=0; i<12;i++)
      {
        array[i]=0;
      }
    if (internalArrayPosition>=12)
    {
      internalArrayPosition=0;
    }
    array[internalArrayPosition]=1;
    internalArrayPosition=internalArrayPosition+1;
    NeopixelRing(array);
    previousMillisLED=millis();
  }
}

void neopixelbluecounterclockwise(int *array)
{
currentMillis = millis();
if (currentMillis-previousMillisLEDf>speedLEDRing)
  {
    for(int i=0; i<12;i++)
      {
        array[i]=0;
      }
    if (internalArrayPosition<0)
    {
      internalArrayPosition=11;
    }
    array[internalArrayPosition]=1;
    internalArrayPosition=internalArrayPosition-1;
    NeopixelRing(array);
    previousMillisLEDf=millis();
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
          {
          neopixelbluecounterclockwise(colorarray);
          
          motor("backward");
          ReadDataEndSwitches();
          }
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
                previousMillis=millis();
                currentMillis=millis();
                int timeError=0;
                while(sensorDoorClosed()==0 && sensorMotionHandling()==0 && currentWatch()==0 && timeError==0)
                {
                  currentMillis=millis();
                  ReadDataMotionDetector(); ReadDataEndSwitches();
                  neopixelblueclockwise(colorarray);
                  
                  motor("forward");
                  if(currentMillis - previousMillis>=closetimeMAX)
                  {
                    motor("stop");
                    timeError=1;
                  }
                }
                  motor("stop");
            }
          }
        }

        else //Position nicht bekannt! Fahre auf!
        {
          while(sensorDoorOpen()==0)
          {
          neopixelbluecounterclockwise(colorarray);
          
          motor("backward");
          ReadDataEndSwitches();
          }
          motor("stop");
        }
  } 
}
