/* PMOR Weldgap measurment movement code
Written by Jasper Dekker and fellow projectstudents
Wanneer een variable eindigt op R is deze voor de rotatie-as
wanneer een variable eindigt op X is deze voor de x-as
*/
// Debug modus
  // mocht deze actief zijn kan de serialmonitor uitgelezen worden.
  bool Debug=false;
//inputs defineren
  #define ROTARYPA 2
  #define ROTARYPB 3
  #define Knop 4
  #define microX 6
  #define microR 7 
//steppermotors defineren
  //pins
    #define dirPinR 10
    #define stepPinR 8 
    #define dirPinX 11
    #define stepPinX 9
  //vaste variablen
    //stappen per revolutie
    #define stepsPerRevolutionX 3200 //1600 alternatieve steps
    #define stepsPerRevolutionR 25600
    //snelheid
    #define speed 50 //tijd tussen stappen in microseconden
    #define speedkal 150 //tijd tussen stappen tijdens kalibratie in microseconden
// libraries oproepen
  #include <Wire.h>
  #include "rgb_lcd.h"
  #include <math.h>
  #include <string.h>
//variablen van de hoek defineren
  int rotaryCount = 5;
  unsigned int lastRotaryCount = 5;
  unsigned int vorigehoek=5;
  unsigned int hoek=0;
// string speciale karakters
  String gradentek="\337";
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
  //definities en pinouts vaststellen
    //serial monitor defineren
    if (Debug==true){
      Serial.begin(9600);
    }
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
      delayMicroseconds(speedkal);
      digitalWrite(stepPinX, LOW);
      delayMicroseconds(speedkal);
    }
    lcd.clear();
  //rotatie
    lcd.print("kalibreren rotatie");
    while (microRlees==true){
      microRlees=digitalRead(microR);
      digitalWrite(stepPinR, HIGH);
      delayMicroseconds(speedkal);
      digitalWrite(stepPinR, LOW);
      delayMicroseconds(speedkal);
    }
  //scherm opschoonen
    lcd.clear();
    lcd.print("systeem opgestart");
    delay(1000);
    lcd.clear();
  //scherm initalisatie
    print = "de hoek: "+String(5)+gradentek;
    lcd.print(print);
    if (Debug==true){
      Serial.println(print);
    }
  }
//hoofd programma
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
      
        print = "de hoek: "+String(rotaryCount)+gradentek;
        lcd.print(print);
        if (Debug==true){
          Serial.println(print);
        }
        lastRotaryCount = rotaryCount;
      }
    }
    if(button==HIGH){//knop wordt ingedrukt
      if (rotaryCount<5||rotaryCount>45){//bescherming dat er alleen tussen 5 en 45 graden mag zitten mocht het voorkomen. 
        lcd.setCursor(0, 1);
        lcd.print("hoek max 5-45"+gradentek);
        if (Debug==true){
          Serial.print("hoek max 5-45"+gradentek+"\n");
        }

      }
      else{
        // tekst neerzetten voor rotatie en tanslatie
        printen="draai hoek: "+String(rotaryCount)+gradentek;
        lcd.setCursor(0, 1);
        lcd.print(printen);
        if (Debug==true){
          Serial.print(printen);
        }

        //rotatie();
        if(rotaryCount<vorigehoek){//hoek is kleiner dan vorige hoek
          //richting veranderen
          digitalWrite(dirPinR, HIGH);
          digitalWrite(dirPinX, LOW);
          hoek = vorigehoek-rotaryCount;
          if (Debug==true){
            Serial.print((hoek)); 
          }
          //rotatatie starten
          rotatie(hoek);
          //samen(hoek);
        }
        if(rotaryCount>vorigehoek){//hoek is groter dan vorige hoek
          digitalWrite(dirPinR, LOW);
          digitalWrite(dirPinX, HIGH);
          hoek = rotaryCount-vorigehoek;
          rotatie(hoek);
          //samen(hoek);
        }
        vorigehoek=rotaryCount;

      }
    }
  }
//functies
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
    if (Debug==true){
      Serial.print((transhoek)); 
    }
    rothoek= dehoek * (stepsPerRevolutionR/360);
    if (Debug==true){
      Serial.print((rothoek)); 
    }
    //aansturen van steppermotoren
    translatie(transhoek);
    roteren(rothoek);
        
  }
  //steppermotor activatie
  void translatie(int trans){//x-richting
    for (int u =0; u <= trans;u++){
      digitalWrite(stepPinX, HIGH);
      delayMicroseconds(speed);
      digitalWrite(stepPinX, LOW);
      delayMicroseconds(speed);
    }
  }
  void roteren(long rot){//rotatatie richting
  for (long x=0;x<=rot;x++){
    digitalWrite(stepPinR, HIGH);
    delayMicroseconds(speed);
    digitalWrite(stepPinR, LOW);
    delayMicroseconds(speed);
  }
  }
//poging tot samenlaten lopen van translatie en rotatie.
  /*
  void samen(long dehoek){
    long transhoek;
    long rothoek;
    //benodigde hoeveelheid steps berekenen.
    transhoek = ((tan(dehoek*(M_PI/180))*50)/(1.7*M_PI))*stepsPerRevolutionX;
    Serial.print((transhoek));
    rothoek= dehoek * (stepsPerRevolutionR/360);
    //aansturen van steppermotoren
    int t;
    int r;
    while(t<transhoek || r<rothoek){
      if(t<transhoek){
        digitalWrite(stepPinX, HIGH);
        t++;
      } 
      if(r<rothoek){
        digitalWrite(stepPinR, HIGH);
        r++;
      }
      delayMicroseconds(50);
      digitalWrite(stepPinX, LOW);
      digitalWrite(stepPinR, LOW);
      delayMicroseconds(50);
    }
  }*/
 