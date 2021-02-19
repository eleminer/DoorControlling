//Zeigt die Werte von allen Sensoren an.
//Zum Testen des Motors im Seriellen Fenster 0,1 oder 2 eingeben.
//1=Motor dreht so, dass die Tür auf geht
//2=Motor dreht so, dass die Tür zu geht
//3=Motor aus

const int endswitch1=2; //Tür geschlossen, Endschalter
const int endswitch2=4; // Tür offen, Endschalter
const int motiondetector1=5; //Bewegungsmelder 1
const int motiondetector2=6; //Bewegungsmelder 2
const int motorforward=7; // Mosfet Motor "Vor"
const int motorbackward=8; // Mosfet Motor "Zurück"
const int currentsensor=A3; // Stromsensor Motor
char motorcontrol = '0';
String motorstatus="aus";
void setup() {
  pinMode(endswitch1, INPUT_PULLUP); pinMode(endswitch2, INPUT_PULLUP); pinMode(motiondetector1, INPUT_PULLUP); pinMode(motiondetector2, INPUT_PULLUP);
  digitalWrite(motorforward, LOW); digitalWrite(motorbackward, LOW);
  pinMode(motorforward, OUTPUT); pinMode(motorbackward, OUTPUT);
  Serial.begin(9600);
  Serial.println("Testskript Pinbelegung Automatik Tuer");
  Serial.println("PULLUP!!!! 1 heißt nicht getriggert");
  Serial.println("------------------------------------");
  delay(5000);
  
}

void loop() {
  
int endswitchOne=digitalRead(endswitch1);
int endswitchTwo=digitalRead(endswitch2);
int motiondetectorOne=digitalRead(motiondetector1);
int motiondetectorTwo=digitalRead(motiondetector2);
int current=analogRead(currentsensor);
motorcontrol=Serial.read();
  if(motorcontrol=='1'){ 
  motorstatus="auf";
  digitalWrite(motorforward, LOW); digitalWrite(motorbackward, HIGH);
  }
  if(motorcontrol=='2'){
  motorstatus="zu";
  digitalWrite(motorforward, HIGH); digitalWrite(motorbackward, LOW);
  }
  if(motorcontrol=='0'){
  motorstatus="aus";
  digitalWrite(motorforward, LOW); digitalWrite(motorbackward, LOW); 
  }
Serial.println("E.closed:"+String(endswitchOne)+"|"+"E.open:"+String(endswitchTwo)+"|"+"Motion:"+String(motiondetectorOne)+"/"+String(motiondetectorTwo)+"|"+"Current:"+String(current)+"|"+"Motor:"+String(motorstatus));
delay(50);
}
