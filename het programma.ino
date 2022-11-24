//inputs defineren
#define ROTARYPA 2
#define ROTARYPB 3
#define Knop 5
#define microX 10
#define microR 11
//steppermotors defineren
#define dirPinR 8
#define stepPinR 9 
#define dirPinX 6
#define stepPinX 7
#define stepsPerRevolutionX 3200//1600
#define stepsPerRevolutionR 2560
// libraries oproepen
#include <Wire.h>
#include "rgb_lcd.h"
#include <math.h>
#include <string.h>
//variablen van de hoek defineren
long rotaryCount = 5;
long lastRotaryCount = 5;
long vorigehoek=5;
long hoek=0;

//RGB waarder plaatsen
  rgb_lcd lcd;
  const int colorR = 255;
  const int colorG = 0;
  const int colorB = 100;
//set van variablen
  int button=0;
  String printen;
  String print; 
  bool microXlees=true;
  bool microRlees=true; 


void setup() {
  Serial.begin(9600);
  // Rotatieknop
  pinMode(ROTARYPA, INPUT);
  pinMode(ROTARYPB, INPUT);   
  digitalWrite (ROTARYPA, HIGH);   
  digitalWrite (ROTARYPB, HIGH);   
  attachInterrupt (0, rotaryTurn, RISING);    // interrupt 0 is pin 2, For rotary encoder
  // knop encoder
  pinMode(Knop,INPUT);
  //steppermotor outputs
  pinMode(stepPinR, OUTPUT);
  pinMode(dirPinR, OUTPUT);
  pinMode(stepPinX, OUTPUT);
  pinMode(dirPinX, OUTPUT);
  // microswitches calibratie defineren
  pinMode(microX,INPUT);
  pinMode(microR,INPUT);
  // print manieren en kleur scherm
  Serial.begin(9600);
  lcd.begin(16,2);
  lcd.setRGB(colorR, colorG, colorB);

  //systeem opstarten
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("opstarten");
  lcd.setCursor(0, 1);
  lcd.print("even geduld");
  delay(1000);
  lcd.clear();
  //Kalibratie 
  digitalWrite(dirPinR, LOW);
  digitalWrite(dirPinX, LOW);
  lcd.setCursor(0, 0);
  //x-as
  lcd.print("kalibreren translatie");
  while (microXlees==true){
    microXlees=digitalRead(microX);
    digitalWrite(stepPinX, HIGH);
    delayMicroseconds(100);
    digitalWrite(stepPinX, LOW);
    delayMicroseconds(100);
  }
  //rotatie
  lcd.print("kalibreren rotatie");
  while (microRlees==true){
    microRlees=digitalRead(microR);
    digitalWrite(stepPinR, HIGH);
    delayMicroseconds(100);
    digitalWrite(stepPinR, LOW);
    delayMicroseconds(100);
  }

  lcd.clear();
  lcd.print("systeem opgestart");
  delay(1000);
  lcd.clear();
  //scherm initalisatie
  print = "de hoek: "+String(5)+"\337";
  lcd.print(print);
  }



void loop() {
  button= digitalRead(Knop);
  if(button==LOW){ //geen verdraaing tijdens rotatie
    if(rotaryCount<5){// zorgen dat er geen andere waarden weergegeven kunnen worden dan tussen 5 en 45 graden
      rotaryCount=45;
    }
    if(rotaryCount>45){
      rotaryCount=5;
    }
    if (lastRotaryCount != rotaryCount) {
      //printen van de hoekwaarde
      lcd.clear();
      lcd.setCursor(0, 1);
      lcd.print(printen);
      lcd.setCursor(0, 0);
     
      print = "de hoek: "+String(rotaryCount)+"\337";
      lcd.print(print);

      Serial.println(print);
      lastRotaryCount = rotaryCount;
    }
  }
  if(button==HIGH){//knop wordt ingedrukt
    if (rotaryCount<5||rotaryCount>45){//bescherming dat er alleen tussen 5 en 45 graden mag zitten mocht het voorkomen. 
      lcd.setCursor(0, 1);
      lcd.print("hoek max 5-45\337");
    }
    else{
      // tekst neerzetten voor rotatie en tanslatie
      printen="draai hoek: "+String(rotaryCount)+"\337";
      lcd.setCursor(0, 1);
      lcd.print(printen);
      //rotatie();
      if(rotaryCount<vorigehoek){//hoek is kleiner dan vorige hoek
        //richting veranderen
        digitalWrite(dirPinR, HIGH);
        digitalWrite(dirPinX, LOW);
        hoek = vorigehoek-rotaryCount;
        //rotatatie starten
        rotatie(hoek);
      }
      if(rotaryCount>vorigehoek){//hoek is groter dan vorige hoek
        digitalWrite(dirPinR, LOW);
        digitalWrite(dirPinX, HIGH);
        hoek = rotaryCount-vorigehoek;
        rotatie(hoek);
      }
      vorigehoek=rotaryCount;

    }
  }
}
// code voor de interrupt van encoder
void rotaryTurn()
{
     if (digitalRead (ROTARYPB)){
      
  	  rotaryCount--;   // graden omlaag
    }
	  else{ 
  	    rotaryCount++;   // graden omhoog

      }
} 

void rotatie(int dehoek){
  long transhoek;
  long rothoek;
  //benodigde hoeveelheid steps berekenen.
  transhoek = ((tan(dehoek*(M_PI/180))*50)/(1.7*M_PI))*stepsPerRevolutionX;
  Serial.print((transhoek));
  rothoek= dehoek * (stepsPerRevolutionR/360);
  //aansturen van steppermotoren
  translatie(transhoek);
  roteren(rothoek);
      
}
//steppermotor activatie
void translatie(int trans){//x-richting
  for (int u =0; u <= trans;u++){
    digitalWrite(stepPinX, HIGH);
    delayMicroseconds(50);
    digitalWrite(stepPinX, LOW);
    delayMicroseconds(50);
  }
}
void roteren(long rot){//rotatatie richting
 for (int x=0;x<=rot*10;x++){
   digitalWrite(stepPinR, HIGH);
   delayMicroseconds(50);
   digitalWrite(stepPinR, LOW);
   delayMicroseconds(50);
 }
}