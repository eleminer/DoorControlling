#include <Adafruit_NeoPixel.h>

const int endswitch1=2; //Tür geschlossen, Endschalter
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
const long speedLEDRing=100; //Zeit wie lange ein Pixel vom Ring leuchten soll in ms
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
int temp=0;

void setup() 
{
  pinMode(endswitch1, INPUT_PULLUP); pinMode(endswitch2, INPUT_PULLUP); pinMode(motiondetector1, INPUT_PULLUP); pinMode(motiondetector2, INPUT_PULLUP);
  digitalWrite(motorforward, LOW); digitalWrite(motorbackward, LOW);
  pinMode(motorforward, OUTPUT); pinMode(motorbackward, OUTPUT);
  pixels.begin(); pixels.clear(); pixels.show();
  Serial.begin(9600);
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
    Serial.println(array[i]);
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
    else if (array[i]==3) //rot
    {
      pixels.setPixelColor(i,pixels.Color(255,0,0));
    }
  }
  pixels.show();
  Serial.print("\n");
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
    array[i]=3;
  }
}

void motor(String rotation)
{
  if (rotation=="backward")
  {
    digitalWrite(motorforward, LOW); digitalWrite(motorbackward, HIGH);
    Serial.println("Motor: backward");
  }
  if (rotation=="forward")
  {
    digitalWrite(motorforward, HIGH); digitalWrite(motorbackward, LOW);
    Serial.println("Motor: forward");
  }
  if (rotation=="stop")
  {
    digitalWrite(motorforward, LOW); digitalWrite(motorbackward, LOW);
    Serial.println("Motor: stop");
  }
}

bool currentWatch()
{
  int value=0;
  value=analogRead(currentsensor);
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

void blueALLone(int *colorarray)
{
  allNeopixelBlue(colorarray);
  NeopixelRing(colorarray);
}

void redALLone(int *colorarray)
{
  allNeopixelRed(colorarray);
  NeopixelRing(colorarray);
}


void startpointOpen(int *array)
{
  while (1)
  {
    Serial.println("startpointOpen"); Serial.println(temp);
    temp = closing(array);
    if (temp==0)
    {
    startpointClosed(array);
    }
  }
}

void startpointClosed(int *array)
{
  while (1)
  {
    Serial.println("startpointClosed"); Serial.println(temp);
    if (temp==0)
    {
    opening(array);
    }
    temp= closing(array);
  }
}

bool closing(int *array)
{
  currentMillis=millis();
  previousMillis=millis();
  while(currentMillis-previousMillis<=openingIntervalTime)
  {
    currentMillis=millis();
    ReadDataMotionDetector();
    if (sensorMotionHandling()==1)
    {
      previousMillis=millis();
      currentMillis=millis();
    }
  }
  ReadDataEndSwitches();
  previousMillis=millis();
  while(sensorDoorClosed()==0)
  {
    ReadDataEndSwitches();ReadDataMotionDetector();
    currentMillis=millis();
    if(sensorMotionHandling()==0 && currentWatch()==0 && currentMillis-previousMillis<=closetimeMAX)
    {
      neopixelblueclockwise(array);
      motor("forward");
    }
    else
    {
      motor("stop");
      ReadDataEndSwitches();
      while(sensorDoorOpen()==0)
      {
        motor("backward");
        neopixelbluecounterclockwise(array);
        ReadDataEndSwitches();
      }
      motor("stop");
      redALLone(array);
      return 1;
    }
  }
  motor("stop");
  blueALLone(array);
  return 0;
}

void opening(int *array)
{
  ReadDataMotionDetector();
  while(sensorMotionHandling()==0)
  {
    ReadDataMotionDetector();
  }
  ReadDataEndSwitches();
  while(sensorDoorOpen()==0)
  {
    ReadDataEndSwitches();
    neopixelbluecounterclockwise(array);
    motor("backward");
  }
  motor("stop");
  redALLone(array);
}

void loop() 
{
  int colorarray[12];
  while (1)
  {
    currentMillis = millis();
    ReadDataMotionDetector(); ReadDataEndSwitches();

        if(sensorDoorClosed()==1)
        {
          blueALLone(colorarray);
          startpointClosed(colorarray);
        }
        if (sensorDoorOpen()==1)
        {
          redALLone(colorarray);
          startpointOpen(colorarray);
        }
        if (sensorDoorClosed()==0 && sensorDoorOpen()==0)
        {
          while (sensorDoorOpen()==0)
          {
          motor("backward");
          ReadDataEndSwitches();
          neopixelbluecounterclockwise(colorarray);
          }
          motor("stop");
          redALLone(colorarray);
          startpointOpen(colorarray);
        }
  } 
}
