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

#include <Wire.h>
#include "rgb_lcd.h"
#include <math.h>
#include <string.h>

long rotaryCount = 5;
long lastRotaryCount = 5;
long vorigehoek=5;
long hoek=0;

//RGB waarder plaatsen
  rgb_lcd lcd;
  const int colorR = 255;
  const int colorG = 0;
  const int colorB = 100;
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
  digitalWrite (ROTARYPA, HIGH);   // enable pull-up
  digitalWrite (ROTARYPB, HIGH);   // enable pull-up
  attachInterrupt (0, rotaryTurn, RISING);    // interrupt 0 is pin 2, For rotary encoder
  // knop
  pinMode(Knop,INPUT);
  //steppermotor outputs
  pinMode(stepPinR, OUTPUT);
  pinMode(dirPinR, OUTPUT);
  pinMode(stepPinX, OUTPUT);
  pinMode(dirPinX, OUTPUT);
  // microswitches defineren
  pinMode(microX,INPUT);
  pinMode(microR,INPUT);
  // print manieren
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
  /*while (microRlees=true){
    microRlees=digitalRead(microR);
    Serial.print("lul");
    delayMicroseconds(100);
  }*/
  Serial.print("klootzak");    
  //x-as
  lcd.print("kalibreren rotatie");
  while (microRlees==true){
    microRlees=digitalRead(microR);
    digitalWrite(stepPinR, HIGH);
    delayMicroseconds(100);
    digitalWrite(stepPinR, LOW);
    delayMicroseconds(100);
  }

  lcd.print("kalibreren translatie");
  while (microXlees==true){
    microXlees=digitalRead(microX);
    digitalWrite(stepPinX, HIGH);
    delayMicroseconds(100);
    digitalWrite(stepPinX, LOW);
    delayMicroseconds(100);
  }
  //rotatie



  lcd.clear();
  lcd.print("systeem opgestart");
  delay(1000);
  lcd.clear();
  }



void loop() {
  button= digitalRead(Knop);
  if(button==LOW){
    if(rotaryCount<5){
      rotaryCount=45;
    }
    if(rotaryCount>45){
      rotaryCount=5;
    }
    if (lastRotaryCount != rotaryCount) {
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
  if(button==HIGH){
    if (rotaryCount<5||rotaryCount>45){
      lcd.setCursor(0, 1);
      lcd.print("hoek max 5-45\337");
    }
    else{
      printen="draai hoek: "+String(rotaryCount)+"\337";
      lcd.setCursor(0, 1);
      lcd.print(printen);
      //rotatie();
      if(rotaryCount<vorigehoek){//minder
        digitalWrite(dirPinR, HIGH);
        digitalWrite(dirPinX, LOW);
        hoek = vorigehoek-rotaryCount;
        rotatie(hoek);
      }
      if(rotaryCount>vorigehoek){
        digitalWrite(dirPinR, LOW);
        digitalWrite(dirPinX, HIGH);
        hoek = rotaryCount-vorigehoek;
        rotatie(hoek);
      }
      vorigehoek=rotaryCount;

    }
  }
}
void rotaryTurn()
{
  // Interrupt Service Routine for a change to Rotary Encoder pin A

    if (digitalRead (ROTARYPB)){
      
  	  rotaryCount--;   // Turn it Down!
    }
	  else{ 
  	    rotaryCount++;   // Turn it Down!

      }
} 
//steppermotor activatie

void rotatie(int dehoek){
  long transhoek;
  long rothoek;
  //transhoek = dehoek * (stepsPerRevolutionX);
  transhoek = ((tan(dehoek*(M_PI/180))*50)/(1.7*M_PI))*stepsPerRevolutionX;
  Serial.print((transhoek));
  rothoek= dehoek * (stepsPerRevolutionR/360);
  translatie(transhoek);
  roteren(rothoek);
      
}
void translatie(int trans){
  for (int u =0; u <= trans;u++){
    digitalWrite(stepPinX, HIGH);
    delayMicroseconds(50);
    digitalWrite(stepPinX, LOW);
    delayMicroseconds(50);
  }
}
void roteren(long rot){
 for (int x=0;x<=rot*10;x++){
   digitalWrite(stepPinR, HIGH);
   delayMicroseconds(50);
   digitalWrite(stepPinR, LOW);
   delayMicroseconds(50);
 }
}