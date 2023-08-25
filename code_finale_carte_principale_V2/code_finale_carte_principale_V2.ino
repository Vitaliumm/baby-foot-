//ECRANS// 
#include "EasyNextionLibrary.h"
EasyNex myNex(Serial);

// CAPTEUR VITESSE//
const int sensor1Pin_a = 18; // Broche pour le capteur laser 1_a
const int sensor2Pin_a = 19; // Broche pour le capteur laser 2_a
const int sensor1Pin_b = 20; // Broche pour le capteur laser 1_b
const int sensor2Pin_b = 21; // Broche pour le capteur laser 2_b
volatile unsigned long sensor1Time = 0;  // millisecondes
volatile unsigned long sensor2Time = 0;  // millisecondes  // millisecondes
float distance = 0.08;                   // en mètre
volatile float speed_a = 0; 
volatile float speed_b = 0;              // Vitesse en km/h
//volatile unsigned long derniereActivation = 0;


// BOUTONS // le coté a = coté bleu 
const int boutonPlus_a = 4;
const int boutonPlus_b = 6;
const int boutonMoins_a = 5;
const int boutonMoins_b = 7;
const int boutonReset_a = 34;
const int boutonReset_b = 36;

//CAPTEUR INFRAROUGE// 
const int capteur_infrarouge_a = 2;
const int capteur_infrarouge_b = 3;

//AUTRE// //
int tableau_vitesse_a [] = {0,0,0,0,0,0,0,0,0,0,0,0};
int tableau_vitesse_b [] = {0,0,0,0,0,0,0,0,0,0,0,0};
bool boucle_score = true;
volatile int score_bleu = 0 ; // a = bleu
volatile int score_rouge = 0 ; // b = rouge 
volatile unsigned long derniereActivation = 0; // pour éviter les rebonds du bouton poussoir

void setup() {
  Serial.begin(9600);// ECRANS
  myNex.begin(9600);

  pinMode(boutonPlus_a, INPUT_PULLUP);//BOUTONS
  pinMode(boutonPlus_b, INPUT_PULLUP);
  pinMode(boutonMoins_a, INPUT_PULLUP);
  pinMode(boutonMoins_b, INPUT_PULLUP);
  pinMode(boutonReset_a, INPUT_PULLUP);
  pinMode(boutonReset_b, INPUT_PULLUP);
  pinMode(capteur_infrarouge_a, INPUT_PULLUP);//CAPTEURS INFRAROUGE
  pinMode(capteur_infrarouge_b, INPUT_PULLUP);
  pinMode(sensor1Pin_a, INPUT_PULLUP); // CAPTEUR VITESSE
  pinMode(sensor2Pin_a, INPUT_PULLUP);
  pinMode(sensor1Pin_b, INPUT_PULLUP);
  pinMode(sensor2Pin_b, INPUT_PULLUP);

attachInterrupt(digitalPinToInterrupt(capteur_infrarouge_a), infrarouge_a, FALLING);//CAPTEURS INFRAROUGE
attachInterrupt(digitalPinToInterrupt(capteur_infrarouge_b), infrarouge_b, FALLING);
attachInterrupt(digitalPinToInterrupt(sensor1Pin_a), sensor1ISR, FALLING); // CAPTEUR VITESSE
attachInterrupt(digitalPinToInterrupt(sensor2Pin_a), sensor2ISR_a, FALLING);
attachInterrupt(digitalPinToInterrupt(sensor1Pin_b), sensor1ISR, FALLING);
attachInterrupt(digitalPinToInterrupt(sensor2Pin_b), sensor2ISR_b, FALLING);
}

void loop() {
   int tempsActuel = millis();

  if (tempsActuel - derniereActivation > 300) { 

    if (digitalRead(boutonPlus_a) == LOW) {
    derniereActivation = tempsActuel;
    score_bleu ++ ; 
    Serial.print("bleu");
    Serial.println(score_bleu);
    }

    if (digitalRead(boutonPlus_b) == LOW) {
    derniereActivation = tempsActuel;
    score_rouge ++ ; 
    Serial.print("rouge");
    Serial.println(score_rouge);
    }

    if (digitalRead(boutonMoins_a) == LOW) {
    derniereActivation = tempsActuel;
    score_bleu -- ; 
    Serial.print("bleu");
    Serial.println(score_bleu);
    }

    if (digitalRead(boutonMoins_b) == LOW) { // attention certains fois ça ne marche pas (surrement probleme hardware)
    derniereActivation = tempsActuel;
    score_rouge -- ; 
    Serial.print("rouge");
    Serial.println(score_rouge);
    }

    if (digitalRead(boutonReset_a) == LOW) {
      Serial.println("dans la boucle reset a  ");
      derniereActivation = tempsActuel;
      bool reset = false ;
      delay(200);
      myNex.writeStr("page page3");
      myNex.writeStr("page page3");  
      while ((tempsActuel - derniereActivation) < 5000 ){
        tempsActuel = millis();
        int temps = (5-((tempsActuel - derniereActivation) / 1000));
        myNex.writeNum("n6.val", temps );
        if (digitalRead(boutonReset_a) == LOW) {
          bool reset = true ;
          myNex.writeStr("page page0");
          myNex.writeStr("page page0"); 
          delay(200);
          asm volatile("jmp 0x00");
        }
      }
      if (!reset){
        myNex.writeStr("page page1");
        myNex.writeStr("page page1");
      }
    }

      if (digitalRead(boutonReset_b) == LOW) {
      Serial.println("dans la boucle reset b  ");
      derniereActivation = tempsActuel;
      bool reset = false ;
      delay(200);
      myNex.writeStr("page page3");
      myNex.writeStr("page page3");  
      while ((tempsActuel - derniereActivation) < 5000 ){
        tempsActuel = millis();
        int temps = (5-((tempsActuel - derniereActivation) / 1000));
        myNex.writeNum("n6.val", temps );
        if (digitalRead(boutonReset_b) == LOW) {
          bool reset = true ;
          myNex.writeStr("page page0");
          myNex.writeStr("page page0"); 
          delay(200);
          asm volatile("jmp 0x00");
        }
      }
      if (!reset){
        myNex.writeStr("page page1");
        myNex.writeStr("page page1");
      }
    }
    if (((score_rouge > 9) || (score_bleu > 9)) && boucle_score ){ 
      boucle_score = false;
      uint32_t plusGrand_a = tableau_vitesse_a[0]; 
      uint32_t plusGrand_b = tableau_vitesse_b[0];

      for (int i = 1; i < 12; i++) {
        if (tableau_vitesse_a[i] > plusGrand_a) {
          plusGrand_a = tableau_vitesse_a[i];
        }
        if (tableau_vitesse_b[i] > plusGrand_b) {
          plusGrand_b = tableau_vitesse_b[i];
        }
      }
      
      Serial.print("plusGrand_a");
      Serial.print(plusGrand_a);
      Serial.print("   plusGrand_b");
      Serial.println(plusGrand_b);

      for (int i = 1; i < 10; i++) { // PROBLEME BIZARRE 
         Serial.print(tableau_vitesse_a[i]);
       }
      
      if (score_rouge > score_bleu){
        myNex.writeStr("page page5");
        myNex.writeStr("page page5");
        myNex.writeStr("page page5");
      }
      else {
        myNex.writeStr("page page4");
        myNex.writeStr("page page4");
        myNex.writeStr("page page4");
      }
      myNex.writeNum("n4.val", plusGrand_a );
      myNex.writeNum("n5.val", plusGrand_b );
    }
  }
  
  myNex.writeNum("n0.val", score_bleu);
  myNex.writeNum("n1.val", score_rouge);
  myNex.writeNum("n2.val", speed_a );
  myNex.writeNum("n3.val", speed_b );
  
}

void infrarouge_b() {
  int tempsActuel = millis();
  if (tempsActuel - derniereActivation > 300) { 
    derniereActivation = tempsActuel;
    //tableau_vitesse_a[score_bleu] = speed;
    score_bleu ++ ; 
    Serial.print("score_bleu");
    Serial.println(score_bleu);
  }
}

void infrarouge_a() {
  int tempsActuel = millis();
  if (tempsActuel - derniereActivation > 300) { 
    derniereActivation = tempsActuel;
    //tableau_vitesse_b[score_rouge] = speed;
    score_rouge ++ ; 
    Serial.print("score_rouge");
    Serial.println(score_rouge);
  }
}

void sensor1ISR() {
  sensor1Time = millis();
  //Serial.print(analogRead(sensor1Pin));
 Serial.print(" sensor1Time : ");
 Serial.println(sensor1Time);
}

void sensor2ISR_a() {
  sensor2Time = millis();
  Serial.print(" sensor2Time : ");
  Serial.println(sensor2Time);
  if (sensor1Time != 0 && sensor2Time != 0) {
    int duration = sensor2Time - sensor1Time;
    //Serial.print(" duration : ");
    //Serial.println(duration);

    speed_a = ((distance * 3.6) / (duration)) * 1000;
    //speed = int(speed);
    Serial.print("Vitesse : ");
    Serial.print(speed_a);
    Serial.println(" km/h");

    sensor1Time = 0;
    sensor2Time = 0;
    tableau_vitesse_a [score_bleu] = speed_a;
  }
}

void sensor2ISR_b() {
  sensor2Time = millis();
  Serial.print(" sensor2Time_b : ");
  Serial.println(sensor2Time);
  if (sensor1Time != 0 && sensor2Time != 0) {
    unsigned long duration = sensor2Time - sensor1Time;
    //Serial.print(" duration : ");
    //Serial.println(duration);

    speed_b = ((distance * 3.6) / (duration)) * 1000;
    //speed = int(speed);
    Serial.print("Vitesse : ");
    Serial.print(speed_b);
    Serial.println(" km/h");

    sensor1Time = 0;
    sensor2Time = 0;
    tableau_vitesse_b [score_rouge] = speed_b;
  }
}


