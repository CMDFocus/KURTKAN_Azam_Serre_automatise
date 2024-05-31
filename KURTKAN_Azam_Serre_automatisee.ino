// ****************************************************************************
// KURTKAN_Serre_auto.ino
// Projet Serre Autonome 2023-2024
// 6ème électronique - INRACI
// Hardware : Feather M0 Express + 1602 LCD + SSD1306 (petit écran) + KY-018 (Cap lum) + SG90 (Servomoteur) + AHT10 (Capt t°/hum) + KY-019 (Relais)
// 18-03-2024
// ****************************************************************************


// ******************************** LIBRAIRIES ********************************

#include <Adafruit_SSD1306.h>
#include <Servo.h>
#include <Wire.h>  // Librairie I²C
#include <Adafruit_NeoPixel.h>
#include <LiquidCrystal_I2C.h>

// ************************ DÉFINITIONS DES CONSTANTES ************************

#define BP_A 13
#define BP_B 11
#define BP_C 10

#define NEOPIXEL_PIN 8

#define SignalPin A0

#define RELAIS1 A1 // Éclairage
#define RELAIS2 A2 // Chauffage
#define RELAIS2 A3 // Fenêtre

#define SERVO A4

// Définition des constantes pour le capt t°/hum

#define Adresse_AHT10 0x38
#define AHT10_INIT_CMD 0xE1
#define AHT10_TRIGGER_MEASURMENT_CMD 0xAC  // Ordre de conversion
#define AHT10_DATA0 0x33
#define AHT10_DATA1 0x00
#define AHT10_SOFT_RESET_CMD 0xBA
#define AHT10_MEASUREMENT_DELAY 80
#define AHT10_POWER_ON_DELAY 40
#define AHT10_CMD_DELAY 350
#define AHT10_SOFT_RESET_DELAY 20

/////////////////////////

// ************************ DÉFINITIONS DES VARIABLES *************************

int pos;  // pour servo

// Variable pour le capt t°/hum
byte DataBuffer[6];

float Raw_humidite;
float Raw_temperature;
float temperature;
float humidite;
int Tentier;
int decimal;

// ************************** DÉFINITIONS DES OBJETS **************************

LiquidCrystal_I2C lcd(0x27, 16, 2);
Adafruit_SSD1306 display = Adafruit_SSD1306(128, 32, &Wire);
Adafruit_NeoPixel NeoPixel(1, NEOPIXEL_PIN, NEO_GRB);
Servo myservo;

///////////////////////////////////////////////////////////////////////////////


void setup() {
  NeoPixel.begin();
  Serial.begin(9600);
  myservo.attach(SERVO);

  pinMode(RELAIS1, OUTPUT);
  pinMode(RELAIS2, OUTPUT);

  pinMode(BP_A, INPUT_PULLUP);
  pinMode(BP_B, INPUT_PULLUP);
  pinMode(BP_C, INPUT_PULLUP);

  initAHT10();

  display.begin(SSD1306_SWITCHCAPVCC, 0x3C);
  display.display();
  display.clearDisplay();
  display.display();

  lcd.init();
  lcd.backlight();
  lcd.setCursor(0, 0);

  servoXdegre(0);

  display.setTextSize(1);
  display.setTextColor(SSD1306_WHITE);
  display.setRotation(0);
  display.setCursor(0, 0);

  display.display();
  display.clearDisplay();
}

///////////////////////////////////////////////////////////////////////////////


void loop() {


  // Partie menu
test

  //////////////////////////////////////////////////
  // Capt Lum

  int ValeurBrute = analogRead(SignalPin);
  float Volt = ValeurBrute * (3.3 / 1023) * 1000;

  // display.printf("Lum:%.2f mV\n", Volt);
  lcd.setCursor(0, 0);
  //lcd.printf("L:%2f mV", Volt);
  lcd.print("L: ");
  lcd.print(Volt);
  lcd.print(" mV");
  //Serial.println(Volt);

  // Capt T°Hum
  /// Partie Humidité
  mesureHum();

  lcd.setCursor(0, 1);
  //lcd.printf("H=%.0f,+-2%\n", humidite);
  lcd.print("H: ");
  lcd.print(humidite);
  lcd.print(" % +-2%");


  /// Partie Température
  lcd.setCursor(11, 1);

  Raw_temperature = ((uint32_t)(DataBuffer[3] & 0x0F) << 16) | ((uint16_t)DataBuffer[4] << 8) | DataBuffer[5];  // Calcule de la valeur brute de température
  temperature = Raw_temperature * 0.000191 - 50;                                                                // Calcule de l'humidité avec les valeurs brutes
  int Tentier = int(temperature);                                                                               // Conversion de la partie entière de la température en un nombre entier
  int decimal = int((temperature - Tentier) * 10);                                                              // Avoir 1 chiffre après la virgule

  //lcd.printf("T=%d.C+-0.3C\n", Tentier);
  lcd.print("T:");
  lcd.print(Tentier);


  // relais
  //  digitalWrite(RELAIS1, HIGH);
  //  digitalWrite(RELAIS1, LOW);

  // Partie servo
  // Partie bouton
  if (!digitalRead(BP_A)) {
    servoXdegre(0);
  }
  if (!digitalRead(BP_B)) {
    digitalWrite(RELAIS1, HIGH);
    digitalWrite(RELAIS2, HIGH);
  } else {
    digitalWrite(RELAIS1, LOW);
    digitalWrite(RELAIS2, LOW);
  }
  if (!digitalRead(BP_C)) {
    servoXdegre(180);
  }

  delay(500);
}

///////////////////////////////////////////////////////////////////////////////

// ************************** Fonctions personalisée **************************

void servoXdegre(int pos) {
  myservo.write(pos);
}

void mesureHum() {
  Wire.beginTransmission(Adresse_AHT10);     // Début de la transmission(Adresse)
  Wire.write(AHT10_TRIGGER_MEASURMENT_CMD);  // Démarrer une nouvelle mesure
  Wire.write(AHT10_DATA0);                   // 1er donnée lu à partir du capteur associé à °C ou hum%
  Wire.write(AHT10_DATA1);                   // 2e donnée lu à partir du capteur associé à °C ou hum%
  Wire.endTransmission();                    // Fin de la tranmission
  delay(AHT10_CMD_DELAY);

  Wire.requestFrom(Adresse_AHT10, 6);  // Attends une réponse de Adresse, réponse en 6 octet
  while (Wire.available()) {           // Boucle qui lit les données dispos du bus I2C jusqu'à ce qu'il n'y en ait plus
    for (int i = 0; i < 6; i++) {      // Boucle pour les 6 valeurs
      DataBuffer[i] = Wire.read();     // Insertion des valeurs récupéré dans le bus I2C
    }
  }

  Raw_humidite = (((uint32_t)DataBuffer[1] << 16) | ((uint16_t)DataBuffer[2] << 8) | (DataBuffer[3])) >> 4;  // Calcule de la valeur brute d'humidité
  humidite = Raw_humidite * 0.000095;                                                                        // Calcule de l'humidité avec les valeurs brutes
  if (humidite < 1) humidite = 0;                                                                            // Ajuster la valeur d'humidité à 0% si hum=1%
  if (humidite > 99) humidite = 100;                                                                         // Ajuster la valeur d'humidité à 100% si hum=99%
}

void initAHT10() {
  Wire.begin();            // Initialisation de la communication I2C
  Wire.setClock(100000L);  // Configure la CLK de la com I2C 100kHz (L = entiers longs => nombres entiers sur un plus grand nombre de bits par rapport aux entiers standards)
  delay(AHT10_POWER_ON_DELAY);

  Wire.beginTransmission(Adresse_AHT10);  // Début de la transmission(Adresse)
  Wire.write(AHT10_SOFT_RESET_CMD);       // Reset software
  Wire.endTransmission();                 // Fin de la tranmission
  delay(AHT10_SOFT_RESET_DELAY);          // Temps d'attente pour un reset logiciel

  Wire.beginTransmission(Adresse_AHT10);
  Wire.write(AHT10_INIT_CMD);
  Wire.write(AHT10_TRIGGER_MEASURMENT_CMD);
  Wire.endTransmission();
  delay(AHT10_CMD_DELAY);
}

bool gestionChauffage(int t, int tmin) {
  if (t < tmin) {
    return 1;
  } else return 0;
}

bool gestionFroid(int t, int tmax) {
  if (t > tmax) {
    return 1;
  } else return 0;
}

bool gestionLedON(int lum, int lmin) {
  if (lum < lmin) {
    return 1;
  } else return 0;
}

bool gestionLedOFF(int lum, int lmax) {
  if (lum > lmax) {
    return 1;
  } else return 0;
}

bool gestionHumON(int hum, int hmin) {
  if (hum < hmin) {
    return 1;
  } else return 0;
}

bool gestionHumOFF(int hum, int hmax) {
  if (hum > hmax) {
    return 1;
  } else return 0;
}

///////////////////////////////////////////////////////////////////////////////