// Outputs en inputs defineeren
  #define knop      5
  #define ROTARYPA  2
  #define ROTARYPB  3
  #define led       13
  #define Xdirpin   6
  #define Xsteppin  7 
  #define Rdirpin   8
  #define Rsteppin  9
  #define Xstaprev  200
  #define Rstaprev  200

// libraries enzo defineren
  #include <Wire.h>
  #include "rgb_lcd.h"
  #include <math.h>
  #include <string.h>
  
//RGB waarder plaatsen
  rgb_lcd lcd;
  const int colorR = 255;
  const int colorG = 0;
  const int colorB = 0;

// knop variablen defineren
  int knopstatus =0;
  long rotaryCount = 0;
  long lastRotaryCount = 0;
  
  //debounce
    /*
    long lastDebounceTime = 0;
    long debounce//delay = 5000;
    */
//Variablen van steppermotors
String printen;
String print="kut";


void setup() {

  //Definering knop
  pinMode(led, OUTPUT);
  pinMode(knop, INPUT);
  // definering draaiswitch
  pinMode(ROTARYPA,INPUT);
  pinMode(ROTARYPB,INPUT);
  digitalWrite(ROTARYPA,HIGH);
  digitalWrite(ROTARYPB, HIGH);
  //defineren stappenmotors
  pinMode(Xsteppin, OUTPUT);
  pinMode(Xdirpin, OUTPUT);
  pinMode(Rsteppin, OUTPUT);
  pinMode(Rdirpin, OUTPUT);
  //serialport
  //Serial.begin(9600);
  // interrupt
  attachInterrupt (0, rotatie, RISING);
  lcd.begin(16,2);
  lcd.setRGB(colorR, colorG, colorB);
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("opstarten");
  lcd.setCursor(0, 1);
  lcd.print("even geduld");
  //delay(5000);
}

void loop() {
  int rotaties;
  knopstatus=digitalRead(knop);

  if (lastRotaryCount != rotaryCount) {
    //Serial.println(rotaryCount);
    lastRotaryCount = rotaryCount;
  }
  if(knopstatus==HIGH){
    print="de hoek:"+String(rotaryCount);
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print(printen);
    //Serial.print(printen);
    lcd.setCursor(0, 1);
    lcd.print(print);
    //Serial.print(print);
    if(rotaryCount<rotaties){
      rotaties=rotaryCount;
      digitalWrite(Xdirpin, LOW);
      digitalWrite(Rdirpin, LOW);
      begin(rotaties);
      
    }

    if(rotaryCount>rotaties){
      rotaties=rotaryCount;
      digitalWrite(Xdirpin, HIGH);
      digitalWrite(Rdirpin, HIGH);
      begin(rotaties);
     
    }
    //delay(1000);
  } 
  else{
    lcd.clear();
    lcd.print("kut feyenoord");
    ////delay(1000);
  }
}
//hoek knop
void rotatie()
{
  // Interrupt Service Routine for a change to Rotary Encoder pin A
  //if (digitalRead (ROTARYPB)==45||digitalRead (ROTARYPB)==0){}
  //else{
    if (digitalRead (ROTARYPB)) {
      rotaryCount--;   // Turn it Down!
      lcd.clear();
      lcd.print(rotaryCount);
      //Serial.print(rotaryCount,"\n");
      }
    else{ 
      lcd.clear();
      lcd.print(rotaryCount);
      rotaryCount++;   // Turn it Up!
      //Serial.print(rotaryCount,"\n");
    }
  lcd.setCursor(0,0);
  lcd.clear();
  printen="hoek:" + (rotaryCount);
  lcd.print(printen);
  lcd.setCursor(0, 1);
  lcd.print(print);
  //Serial.print(print);
 // }
} 
// hoek en translatie waardes bepalen en uitvoeren
void begin(int rotat){
  int hoek=rotat;
  int afstand=rotat*1.5;
  draaien(hoek);
  transleren(afstand);
}

void draaien(int hoek){
  for (int i = 0; i < hoek; i++){
    digitalWrite(Rsteppin, HIGH);
    //delayMicroseconds(2000);
    digitalWrite(Rsteppin, LOW);
    //delayMicroseconds(2000);
  }
}
void transleren(int afstand){
  for (int i = 0; i < afstand; i++){
    digitalWrite(Xsteppin, HIGH);
    //delayMicroseconds(2000);
    digitalWrite(Xsteppin, LOW);
    //delayMicroseconds(2000);
  }
}