#include <Adafruit_NeoPixel.h>
#ifdef __AVR__
 #include <avr/power.h> // Required for 16 MHz Adafruit Trinket
#endif

const int pinboutonLumiere_a = 21; 
const int pinboutonLumiere_b = 19 ; // le bouton b a eu bcp de problème bizarre. 
const int capteur_infra_rouge_a = 2;
const int capteur_infra_rouge_b = 3;
volatile int numero_programme_a = -1 ;
volatile unsigned long derniereActivation = 0;

#define LED_PIN  8
#define LED_COUNT 40
uint32_t blue = 0x0000FF;
uint32_t white = 0xFFFFFF;
uint32_t red = 0xFF0000;

Adafruit_NeoPixel strip(LED_COUNT, LED_PIN, NEO_GRB + NEO_KHZ800);

void setup() {
  Serial.begin(9600);
  strip.begin();           // INITIALIZE NeoPixel strip object (REQUIRED)
  strip.show();            // Turn OFF all pixels ASAP
  strip.setBrightness(255); // Set BRIGHTNESS (max = 255)
  
  pinMode(pinboutonLumiere_a, INPUT_PULLUP);
  pinMode(pinboutonLumiere_b, INPUT_PULLUP);
  pinMode(capteur_infra_rouge_a, INPUT_PULLUP);
  pinMode(capteur_infra_rouge_b, INPUT_PULLUP);

  attachInterrupt(digitalPinToInterrupt(pinboutonLumiere_a), programme_led_a, FALLING);
  attachInterrupt(digitalPinToInterrupt(pinboutonLumiere_b), programme_led_b, FALLING);
  attachInterrupt(digitalPinToInterrupt(capteur_infra_rouge_a), bute_bleu, FALLING);
  attachInterrupt(digitalPinToInterrupt(capteur_infra_rouge_b), bute_rouge, FALLING);
}

void loop() {
  /*
  Serial.print("numero_programme_a   ");
  Serial.print(numero_programme_a);
  Serial.print("   ");
  Serial.println(digitalRead(pinboutonLumiere_a));
  */
  //debug();
  if (numero_programme_a == 0) {
    //Serial.println("dans le if programme 0");
    programme_0_a(5);
  }
  if (numero_programme_a == 1) {
    programme_1_a(2);
  }
  if (numero_programme_a == 2) {
    strip.clear();
    programme_2_a(15);
  }
  if (numero_programme_a == 3) {
    programme_3_a(5);
  }
  if (numero_programme_a >= 4) {
    numero_programme_a = 0;
  }
}


void programme_led_a () {
  int tempsActuel = millis();
   if (tempsActuel - derniereActivation > 1000) { 
    derniereActivation = tempsActuel;
    numero_programme_a ++;
    Serial.print("programme_led_a ");
    Serial.print("numero_programme_a ");
    Serial.println(numero_programme_a);
   }
}


void programme_led_b () {
  int tempsActuel = millis();
   if (tempsActuel - derniereActivation > 1000) { 
    derniereActivation = tempsActuel;
    numero_programme_a ++;
    Serial.println("programme_led probleme");
   }
}

void bute_rouge (){
  strip.fill(blue);
  strip.show();
  Chronometre(50);
  strip.clear();
}

void bute_bleu (){
  strip.fill(red);
  strip.show();
  Chronometre(50);
  strip.clear();
}

void programme_0_a(int wait){
  strip.fill(white);
  strip.show();
}

void programme_1_a(int wait) {
    for(long firstPixelHue = 0; firstPixelHue < 65424; firstPixelHue += 256) {
    strip.rainbow(firstPixelHue);
    strip.show(); // Update strip with new contents
    //Serial.println(firstPixelHue);
    Chronometre(wait);
    //debug();
  }
}

void programme_2_a(int wait){
    Serial.println("dans programme_2");
    for(int i=0; i<LED_COUNT/3; i++) { // For each pixel...
    strip.setPixelColor(i,blue);
    strip.show();   // Send the updated pixel colors to the hardware.
    Chronometre(wait); // Pause before next pass through loop
    }
    for (int i=LED_COUNT/3; i<2*LED_COUNT/3; i++) {
      strip.setPixelColor(i,white);
    strip.show();   // Send the updated pixel colors to the hardware.
    Chronometre(wait);   
  }
    for (int i=2*LED_COUNT/3; i<LED_COUNT; i++) {
      strip.setPixelColor(i,red);
    strip.show();   // Send the updated pixel colors to the hardware.
    Chronometre(wait);
  }
}

void programme_3_a(int wait){
  strip.clear();
}

void Chronometre(int wait){
  int timer = 0;
    while ( timer < wait){
    Serial.println("timer");
    timer++;
    //Serial.print("numero_programme_a   ");
    //Serial.print(numero_programme_a);
    //Serial.print("   ");
    //Serial.println(digitalRead(pinboutonLumiere_a));
    //debug();
    }
  timer = 0;
}

void debug (){
  if (digitalRead(pinboutonLumiere_a) == 0){
    for (int i = 0; i < 100; i++) {
  Serial.println("Erreur");
    }
  }
}
