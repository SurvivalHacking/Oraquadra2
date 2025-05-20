// 18/02/2025
// OraQuadra V2.0  -  By Davide Gatti SURVIVAL HACKING  www.survivalhacking.it
//
// Sketch completamente riscritto e ottimizzato per famiglia esp32 by Paolo Sambi
// Nuovo quadrante by Luca Beltramio
//
// Pulsante 1 = per lampeggio secondi sulla lettera E
// Pulsante 2 = per la cambio preset
// Pulsante 1 + Pulsante 2 premuti per 5 secondi = reset configurazione WiFi
// addon: cambio colori con alexa 
//        ota update sketch
//        setup ora automatica
//        effetto matrix
//        wifimanager ORAQUADRA_AP 192.168.4.1
//        ottimizzazione creazione parole per ora e minuti
// 
// 21/02/2025
// Rimappatura nuovo quadrante e aggiunta gestione nuove WORD
// Corretta gestione del comando di alexa SPEGNI/ACCENDI ORAQUADRA (ora rimane sempre sepnto e non si riaccende al cambio del minuto)
// Aggiunta gestione pulsante 1 per accensione/spegnimento blink dei secondi (attiva blink secondi in modalità NON marix)
// Sistemazione troncature minuti quando è il primo minuto della decade   21 = VENT UN / 31 = TRENT UN   
//
// 20/05/2025
// aggiunta la possibilità di usare pulsanti normali oltre a quelli touch
// BUTTON_LOGIC_INVERTED = 1 per pulsanti normali NA verso GND
// BUTTON_LOGIC_INVERTED = 0 per pulsanti touch

// Mappatura matrice
// S-015 O-014 N-013 O-012 U-011 L-010 E-009 Y-008 O-007 R-006 E-005 X-004 Z-003 E-002 R-001 O-000
// V-016 E-017 N-018 T-019 I-020 T-021 R-022 E-023 D-024 I-025 C-026 I-027 O-028 T-029 T-030 O-031
// E-047 C-046 Q-045 U-044 A-043 T-042 T-041 O-040 R-039 D-038 I-037 C-036 I-035 S-034 E-033 I-032
// N-048 I-049 U-050 N-051 D-052 I-053 C-054 I-055 Q-056 U-057 A-058 T-059 T-060 R-061 O-062 O-063
// T-079 N-078 I-077 J-076 V-075 E-074 N-073 T-072 U-071 N-070 O-069 D-068 I-067 E-066 C-065 I-064
// I-080 Q-081 N-082 S-083 E-084 D-085 I-086 C-087 I-088 A-089 S-090 S-091 E-092 T-093 T-094 E-095
// D-111 U-110 D-109 O-108 D-107 I-106 C-105 I-104 A-103 N-102 N-101 O-100 V-099 E-098 L-097 F-096
// U-112 E-113 I-114 H-115 E-116 L-117 P-118 Q-119 U-120 A-121 R-122 A-123 N-124 T-125 A-126 X-127
// E-143 R-142 C-141 K-140 U-139 V-138 E-137 N-136 T-135 I-134 T-133 R-132 E-131 N-130 T-129 A-128
// G-144 R-145 I-146 N-147 C-148 I-149 N-150 Q-151 U-152 A-153 N-154 T-155 A-156 U-157 N-158 O-159
// S-175 E-174 D-173 I-172 C-171 I-170 D-169 O-168 D-167 I-166 C-165 I-164 O-163 T-162 T-161 O-160
// D-176 I-177 E-178 C-179 I-180 Q-181 U-182 A-183 T-184 T-185 O-186 R-187 D-188 I-189 C-190 I-191
// Q-207 U-206 A-205 T-204 T-203 R-202 O-201 Q-200 U-199 I-198 N-197 D-196 I-195 C-194 I-193 O-192
// A-208 R-209 T-210 R-211 E-212 D-213 I-214 C-215 I-216 A-217 S-218 S-219 E-220 T-221 T-222 E-223
// U-239 N-238 D-237 I-236 C-235 I-234 A-233 N-232 N-231 O-230 V-229 E-228 O-227 S-226 E-225 I-224
// C-240 I-241 N-242 Q-243 U-244 E-245 D-246 U-247 E-248 U-249 M-250 I-251 N-252 U-253 T-254 I-255

// Parte 1: Include e definizioni di base
#include <FastLED.h>     // https://github.com/FastLED/FastLED 
#include <WiFi.h> 
#include <ESPmDNS.h>
#include <ArduinoOTA.h>
#include <ezTime.h>      // https://github.com/ropg/ezTime
#include <Espalexa.h>    // https://github.com/Aircoookie/Espalexa
#include <EEPROM.h>      // https://github.com/jwrw/ESP_EEPROM 
#include <WiFiManager.h> // https://github.com/tzapu/WiFiManager

// Abilita la logica invertita per i pulsanti.
// Definisci BUTTON_LOGIC_INVERTED come 1 per logica invertita (pull-up, premi per GND).
// Definisci BUTTON_LOGIC_INVERTED come 0 o commenta per logica normale (pull-down, premi per VCC).
#define BUTTON_LOGIC_INVERTED 0

// Configurazione ESP32-S3
//#if CONFIG_ARDUINO_RUNNING_CORE
//#undef CONFIG_ARDUINO_LOOP_STACK_SIZE
//#define CONFIG_ARDUINO_LOOP_STACK_SIZE 16384
//#endif

// Pin e configurazione LED ESP32C3
#define LED_PIN      5     // Pin per matrice LED
#define BUTTON_MODE  7     // Pulsante modi
#define BUTTON_SEC   6     // Pulsante lampeggio secondo

// Pin e configurazione LED ESP32+batt
//#define LED_PIN      14     // Pin per matrice LED
//#define BUTTON_MODE  26     // Pulsante modi
//#define BUTTON_SEC   33     // Pulsante lampeggio secondo

#define NUM_LEDS    256   // Totale LED matrice
#define LED_TYPE    WS2812B
#define COLOR_ORDER GRB
#define BRIGHTNESS_DAY   64
#define BRIGHTNESS_NIGHT 32
#define MATRIX_WIDTH  16
#define MATRIX_HEIGHT 16

// Modalità visualizzazione
#define MODE_FADE   0
#define MODE_SLOW   1
#define MODE_FAST   2
#define MODE_MATRIX 3
#define MODE_MATRIX2 4
#define NUM_MODES   5  

// Strutture e costanti per effetto Matrix
#define MATRIX_BASE_SPEED   0.15f   // Ridotto da 0.8f
#define MATRIX_SPEED_VAR    0.01f   // Ridotto da 1.0f a 0.1f e poi a 0.01f per minore variabilità
#define MATRIX_START_Y_MIN  -3.0f
#define MATRIX_START_Y_MAX   0.0f
#define MATRIX_TRAIL_LENGTH  13     // da 3 portato 1 13
#define NUM_DROPS           32      
#define MATRIX2_BASE_SPEED  0.15f   // Velocità base più lenta per Matrix2
#define MATRIX2_SPEED_VAR   0.1f    // Minore variazione per Matrix2

// Strutture e costanti per EEPROM
#define EEPROM_SIZE 10  // Dimensione EEPROM necessaria
#define EEPROM_CONFIGURED_MARKER 0x55
#define EEPROM_PRESET_ADDR 1
#define EEPROM_BLINK_ADDR 2

#define GIORNO 64     // full on
#define NOTTE 32       // half on

uint8_t currentPreset;  // Preset corrente
int intBrightness;
uint8_t gHue = 0;
struct Drop {
    uint8_t x;
    float y;
    float speed;
    uint8_t intensity;
    bool active;
    bool isMatrix2;  // Nuovo campo per distinguere il tipo di effetto
};

// Struttura per tenere traccia dello stato delle gocce nel Matrix2
struct Matrix2State {
    bool completed;
    uint32_t completionTime;
    bool needsReset;
};

Matrix2State matrix2State = {false, 0, true};

Drop drops[NUM_DROPS];
bool matrixInitialized = false;
bool targetPixels[NUM_LEDS] = {false};
bool activePixels[NUM_LEDS] = {false};

// Definizione array LED e controller
CRGB leds[NUM_LEDS];
CRGB matrixWordColor = CRGB::Blue;  // Colore default per le parole
Timezone myTZ;
Espalexa espalexa;

// Variabili globali stato
uint8_t currentMode = MODE_FAST;
uint8_t currentHour = 0;
uint8_t currentMinute = 0;
uint8_t currentSecond = 0;
uint8_t currentBlink = 0;
uint8_t prevHour = 255;
uint8_t prevMinute = 255;
uint8_t Presets;            // Preset corrente
uint8_t alexaOff = 0;

//Definizione matrice parole in PROGMEM
const uint8_t PROGMEM WORD_SONO_LE[] = {15,14,13,12,10,9,7,6,5,8};
const uint8_t PROGMEM WORD_MINUTI[] = {250,251,252,253,254,255,8};

// Parole ore
const uint8_t PROGMEM WORD_UNA[] = {57,70,89,8};
const uint8_t PROGMEM WORD_DUE[] = {111,112,143,8};
const uint8_t PROGMEM WORD_TRE[] = {21,22,23,8};
const uint8_t PROGMEM WORD_QUATTRO[] = {56,57,58,59,60,61,62,8};
const uint8_t PROGMEM WORD_CINQUE[] = {46,49,78,81,110,113,8};
const uint8_t PROGMEM WORD_SEI[] = {34,33,32,8};
const uint8_t PROGMEM WORD_SETTE[] = {91,92,93,94,95,8};
const uint8_t PROGMEM WORD_OTTO[] = {28,29,30,31,8};
const uint8_t PROGMEM WORD_NOVE[] = {101,100,99,98,8};
const uint8_t PROGMEM WORD_DIECI[] = {68,67,66,65,64,8};
const uint8_t PROGMEM WORD_UNDICI[] = {50,51,52,53,54,55,8};
const uint8_t PROGMEM WORD_DODICI[] = {109,108,107,106,105,104,8};
const uint8_t PROGMEM WORD_TREDICI[] = {21,22,23,24,25,26,27,8};
const uint8_t PROGMEM WORD_QUATTORDICI[] = {45,44,43,42,41,40,39,38,37,36,35,8};
const uint8_t PROGMEM WORD_QUINDICI[] = {45,50,77,82,109,114,141,146,8};
const uint8_t PROGMEM WORD_SEDICI[] = {83,84,85,86,87,88,8};
const uint8_t PROGMEM WORD_DICIASSETTE[] = {85,86,87,88,89,90,91,92,93,94,95,8};
const uint8_t PROGMEM WORD_DICIOTTO[] = {24,25,26,27,28,29,30,31,8};
const uint8_t PROGMEM WORD_DICIANNOVE[] = {107,106,105,104,103,102,101,100,99,98,8};
const uint8_t PROGMEM WORD_VENTI[] = {16,47,48,79,80,8};
const uint8_t PROGMEM WORD_VENTUNO[] = {75,74,73,72,71,70,69,8};
const uint8_t PROGMEM WORD_VENTIDUE[] = {16,47,48,79,80,111,112,143,8};
const uint8_t PROGMEM WORD_VENTITRE[] = {16,17,18,19,20,21,22,23,8};
const uint8_t PROGMEM WORD_ZERO[] = {3,2,1,0,8};

// Array delle parole ore per lookup veloce
const uint8_t* const PROGMEM HOUR_WORDS[] = {
   WORD_ZERO,        // 0
   WORD_UNA,         // 1
   WORD_DUE,         // 2
   WORD_TRE,         // 3
   WORD_QUATTRO,     // 4
   WORD_CINQUE,      // 5
   WORD_SEI,         // 6
   WORD_SETTE,       // 7
   WORD_OTTO,        // 8
   WORD_NOVE,        // 9
   WORD_DIECI,       // 10
   WORD_UNDICI,      // 11
   WORD_DODICI,      // 12
   WORD_TREDICI,     // 13
   WORD_QUATTORDICI, // 14
   WORD_QUINDICI,    // 15
   WORD_SEDICI,      // 16
   WORD_DICIASSETTE, // 17
   WORD_DICIOTTO,    // 18
   WORD_DICIANNOVE,  // 19
   WORD_VENTI,       // 20
   WORD_VENTUNO,     // 21
   WORD_VENTIDUE,    // 22
   WORD_VENTITRE     // 23
};

// Definizioni minute words in PROGMEM
const uint8_t PROGMEM WORD_E[] = {116,8};
const uint8_t PROGMEM WORD_MUNO[] = {157,158,159,8};
const uint8_t PROGMEM WORD_MUN[] = {239,238,8};
const uint8_t PROGMEM WORD_MDUE[] = {246,247,248,8};
const uint8_t PROGMEM WORD_MTRE[] = {210,211,212,8};
const uint8_t PROGMEM WORD_MQUATTRO[] = {207,206,205,204,203,202,201,8};
const uint8_t PROGMEM WORD_MCINQUE[] = {240,241,242,243,244,245,8};
const uint8_t PROGMEM WORD_MSEI[] = {226,225,224,8};
const uint8_t PROGMEM WORD_MSETTE[] = {219,220,221,222,223,8};
const uint8_t PROGMEM WORD_MOTTO[] = {163,162,161,160,8};
const uint8_t PROGMEM WORD_MNOVE[] = {231,230,229,228,8};
const uint8_t PROGMEM WORD_MDIECI[] = {176,177,178,179,180,8};
const uint8_t PROGMEM WORD_MUNDICI[] = {239,238,237,236,235,234,8};
const uint8_t PROGMEM WORD_MDODICI[] = {169,168,167,166,165,164,8};
const uint8_t PROGMEM WORD_MTREDICI[] = {210,211,212,213,214,215,216,8};
const uint8_t PROGMEM WORD_MQUATTORDICI[] = {181,182,183,184,185,186,187,188,189,190,191,8};
const uint8_t PROGMEM WORD_MQUINDICI[] = {200,199,198,197,196,195,194,193,8};
const uint8_t PROGMEM WORD_MSEDICI[] = {175,174,173,172,171,170,8};
const uint8_t PROGMEM WORD_MDICIASSETTE[] = {213,214,215,216,217,218,219,220,221,222,223,8};
const uint8_t PROGMEM WORD_MDICIOTTO[] = {167,166,165,164,163,162,161,160,8};
const uint8_t PROGMEM WORD_MDICIANNOVE[] = {237,236,235,234,233,232,231,230,229,228,8};
const uint8_t PROGMEM WORD_MVENT[] = {138,137,136,135,8};
const uint8_t PROGMEM WORD_MVENTI[] = {138,137,136,135,134,8};
const uint8_t PROGMEM WORD_MTRENT[] = {133,132,131,130,129,8};
const uint8_t PROGMEM WORD_MTRENTA[] = {133,132,131,130,129,128,8};
const uint8_t PROGMEM WORD_MQUARANT[] = {119,120,121,122,123,124,125,8};
const uint8_t PROGMEM WORD_MQUARANTA[] = {119,120,121,122,123,124,125,126,8};
const uint8_t PROGMEM WORD_MCINQUANT[] = {148,149,150,151,152,153,154,155,8};
const uint8_t PROGMEM WORD_MCINQUANTA[] = {148,149,150,151,152,153,154,155,156,8};

// Array di lookup per i minuti 1-19
const uint8_t* const PROGMEM MINUTE_WORDS[] = {
   nullptr,              // 0
   WORD_MUNO,           // 1
   WORD_MDUE,           // 2
   WORD_MTRE,           // 3
   WORD_MQUATTRO,       // 4
   WORD_MCINQUE,        // 5
   WORD_MSEI,           // 6
   WORD_MSETTE,         // 7
   WORD_MOTTO,          // 8
   WORD_MNOVE,          // 9
   WORD_MDIECI,         // 10
   WORD_MUNDICI,        // 11
   WORD_MDODICI,        // 12
   WORD_MTREDICI,       // 13
   WORD_MQUATTORDICI,   // 14
   WORD_MQUINDICI,      // 15
   WORD_MSEDICI,        // 16
   WORD_MDICIASSETTE,   // 17
   WORD_MDICIOTTO,      // 18
   WORD_MDICIANNOVE     // 19
};

// Struttura per le decine
struct MinuteTens {
   const uint8_t* normal;
   const uint8_t* truncated;
};

// Array di lookup per le decine
const MinuteTens PROGMEM TENS_WORDS[] = {
   {WORD_MVENTI, WORD_MVENT},       // 20
   {WORD_MTRENTA, WORD_MTRENT},     // 30
   {WORD_MQUARANTA, WORD_MQUARANT}, // 40
   {WORD_MCINQUANTA, WORD_MCINQUANT} // 50
};

// Struttura per gestione colori
struct ClockColors {
   CRGB current;
   uint8_t brightness;
   static const CRGB presets[];
   uint8_t presetIndex;
   
   ClockColors() : current(CRGB::White), brightness(BRIGHTNESS_DAY), presetIndex(0) {}
   
   void nextPreset() {
       presetIndex = (presetIndex + 1) % 5;
       current = presets[presetIndex];
   }
   
   CRGB getColor() const {
       return CRGB(scale8(current.r, brightness),
                  scale8(current.g, brightness),
                  scale8(current.b, brightness));
   }
};

const CRGB ClockColors::presets[] = {
   CRGB::White,  // Bianco
   CRGB::Blue,   // Blu
   CRGB::Red,    // Rosso
   CRGB(60,0,255), // Viola
   CRGB::Green ,   // Verde
   CRGB(random8(), random8(), random8())  // Colore casuale
};

ClockColors clockColors;

// Funzioni di gestione LED e parole
void displayWord(const uint8_t* word, CRGB color) {
   uint8_t idx = 0;
   uint8_t pixel;
   while((pixel = pgm_read_byte(&word[idx])) != 8) {
       leds[pixel] = color;
       idx++;
   }
}

void fadeWord(const uint8_t* word, CRGB color, uint8_t steps = 8) {
   for(uint8_t step = 0; step < steps; step++) {
       uint8_t brightness = map(step, 0, steps-1, 0, 255);
       CRGB fadeColor = color;
       fadeColor.nscale8(brightness);
       displayWord(word, fadeColor);
       FastLED.show();
       delay(50);
       yield();
   }
}

// Setup principale
void setup() {
   Serial.begin(115200);
   Serial.println("START");
   EEPROM.begin(EEPROM_SIZE);
   
   // Leggi configurazione
   if (EEPROM.read(0) != EEPROM_CONFIGURED_MARKER) {
       // Prima configurazione
       EEPROM.write(0, EEPROM_CONFIGURED_MARKER);
       EEPROM.write(EEPROM_PRESET_ADDR, 0);  // Preset default
       EEPROM.write(EEPROM_BLINK_ADDR, 1);  // Seconds Blink default default
       EEPROM.commit();  // Importante per ESP32!
   }
   
   // Carica preset salvato
   currentPreset = EEPROM.read(EEPROM_PRESET_ADDR);
   currentBlink = EEPROM.read(EEPROM_BLINK_ADDR);
   applyPreset(currentPreset);                   
    
   // Configurazione pin in base alla logica dei pulsanti
   #if BUTTON_LOGIC_INVERTED == 1
     // Logica invertita: pull-up, premuto = GND (LOW)
     pinMode(BUTTON_MODE, INPUT_PULLUP);
     pinMode(BUTTON_SEC, INPUT_PULLUP);
   #else
     // Logica normale: pull-down, premuto = VCC (HIGH)
     pinMode(BUTTON_MODE, INPUT_PULLDOWN); // O semplicemente INPUT se si usa un resistore esterno di pull-down
     pinMode(BUTTON_SEC, INPUT_PULLDOWN);
   #endif
   
   // Inizializzazione FastLED
   FastLED.addLeds<LED_TYPE, LED_PIN, COLOR_ORDER>(leds, NUM_LEDS)
          .setCorrection(TypicalLEDStrip);
   FastLED.setBrightness(intBrightness);
   FastLED.clear();
   FastLED.show();
   
   // Configurazione WiFiManager
   WiFiManager wm;
   
   // Imposta timeout del portale di configurazione (opzionale)
   wm.setConfigPortalTimeout(180); // 3 minuti
   
   // Personalizza il portale WiFi
   wm.setTitle("ORAQUADRA WiFi Setup");
   
   // Mostra LED blu durante la configurazione
   fill_solid(leds, NUM_LEDS, CRGB::Black);
   leds[0] = CRGB::Blue;
   FastLED.show();
   
   // Tentativo di connessione o avvio del portale di configurazione
   bool res = wm.autoConnect("ORAQUADRA_AP");

   if(!res) {
       Serial.println("Fallimento connessione");
       // Mostra errore con LED rosso
       fill_solid(leds, NUM_LEDS, CRGB::Red);
       FastLED.show();
       delay(3000);
       ESP.restart();
   } 
   else {
       // Connesso!
       Serial.println("WiFi Connesso!");
       // Mostra successo con LED verde
       fill_solid(leds, NUM_LEDS, CRGB::Green);
       FastLED.show();
       delay(1000);
       FastLED.clear();
       FastLED.show();
   }
   
   // Configurazione post-connessione WiFi
   WiFi.setSleep(false);
   WiFi.setAutoReconnect(true);
   
   if(WiFi.status() == WL_CONNECTED) {
       setupOTA();
       espalexa.addDevice("ORAQUADRA", colorChanged);
       espalexa.begin();
       
       // Configurazione ezTime
       setServer("pool.ntp.org");
       waitForSync(10);
       myTZ.setLocation(F("Europe/Rome"));
   }
   
   updateDisplay();
}
void resetWiFi() {
    WiFiManager wm;
    wm.resetSettings();
    ESP.restart();
}
// Setup OTA
void setupOTA() {
   ArduinoOTA.setHostname("ORAQUADRA");
   
   ArduinoOTA.onStart([]() {
       FastLED.clear();
       FastLED.show();
   });
   
   ArduinoOTA.onProgress([](unsigned int progress, unsigned int total) {
       uint8_t percentComplete = (progress / (total / 100));
       fill_solid(leds, map(percentComplete, 0, 100, 0, NUM_LEDS), CRGB::Blue);
       FastLED.show();
   });
   
   ArduinoOTA.begin();
}

// Aggiornamento display principale
void updateDisplay() {
   FastLED.clear();
   CRGB currentColor = clockColors.getColor();
   
   // "SONO LE" sempre del colore base
   displayWord(WORD_SONO_LE, currentColor);
   
   // Mostra ora corrente
//   uint8_t hour12 = currentHour % 12;
   const uint8_t* hourWord = (const uint8_t*)pgm_read_ptr(&HOUR_WORDS[currentHour]);
   
   switch(currentMode) {
       case MODE_FADE:
           fadeWord(hourWord, currentColor);
           if(currentMinute > 0) {
               fadeWord(WORD_E, currentColor);
               showMinutes(currentMinute, currentColor);
               fadeWord(WORD_MINUTI, currentColor);
           }
           break;
           
       case MODE_SLOW: {
           // Colori random per ogni parte
           CRGB randomHourColor = CRGB(random8(), random8(), random8());
           displayWord(hourWord, randomHourColor);
           
           if(currentMinute > 0) {
               CRGB randomEColor = CRGB(random8(), random8(), random8());
               CRGB randomMinutesColor = CRGB(random8(), random8(), random8());
               CRGB randomMinutiColor = CRGB(random8(), random8(), random8());
               
               displayWord(WORD_E, randomEColor);
               showMinutes(currentMinute, randomMinutesColor);
               displayWord(WORD_MINUTI, randomMinutiColor);
           }
           FastLED.show();
           delay(500);
           break;
       }
       
       default:
           displayWord(hourWord, currentColor);
           if(currentMinute > 0) {
               displayWord(WORD_E, currentColor);
               showMinutes(currentMinute, currentColor);
               displayWord(WORD_MINUTI, currentColor);
           }
   }
   
   // Animazione secondi solo se ci sono minuti e non in modo slow
   if(currentMinute > 0 && currentMode != MODE_SLOW) {
       showSeconds(currentSecond, currentColor);
   }
   
   FastLED.show();
}

// Gestione minuti
void showMinutes(uint8_t minutes, CRGB color) {
   // Array di puntatori alle parole dei minuti in PROGMEM
   static const uint8_t* const minuteWords[] PROGMEM = {
       nullptr,           // 0
       WORD_MUNO,         // 1
       WORD_MDUE,         // 2 
       WORD_MTRE,         // 3
       WORD_MQUATTRO,     // 4
       WORD_MCINQUE,      // 5
       WORD_MSEI,         // 6
       WORD_MSETTE,       // 7
       WORD_MOTTO,        // 8
       WORD_MNOVE,        // 9
       WORD_MDIECI,       // 10
       WORD_MUNDICI,      // 11
       WORD_MDODICI,      // 12
       WORD_MTREDICI,     // 13
       WORD_MQUATTORDICI, // 14
       WORD_MQUINDICI,    // 15
       WORD_MSEDICI,      // 16
       WORD_MDICIASSETTE, // 17
       WORD_MDICIOTTO,    // 18
       WORD_MDICIANNOVE   // 19
   };

   if(minutes <= 0) return;

   if(minutes <= 19) {
       displayWord((const uint8_t*)pgm_read_ptr(&minuteWords[minutes]), color);
   } else {
       // Gestione decine
       uint8_t tens = minutes / 10;
       uint8_t ones = minutes % 10;
       
       switch(tens) {
           case 2: // 20-29
               displayWord(ones == 1 || ones == 8 ? WORD_MVENT : WORD_MVENTI, color);
               break;
           case 3: // 30-39
               displayWord(ones == 1 || ones == 8 ? WORD_MTRENT : WORD_MTRENTA, color);
               break;
           case 4: // 40-49
               displayWord(ones == 1 || ones == 8 ? WORD_MQUARANT : WORD_MQUARANTA, color);
               break;
           case 5: // 50-59
               displayWord(ones == 1 || ones == 8 ? WORD_MCINQUANT : WORD_MCINQUANTA, color);
               break;
       }
       
       if(ones > 0) {
           if (ones == 1) {
                displayWord(WORD_MUN, color);
           } else {
                displayWord((const uint8_t*)pgm_read_ptr(&minuteWords[ones]), color);
           }    
       }
   }
}

// Animazione secondi con effetto rainbow
  void showSecondsM(uint8_t seconds, CRGB color) {
    if(currentMinute >= 1) {
    uint8_t segment = seconds / 10;  // 0-5
    
      for(uint8_t i = 0; i < 6; i++) {
          if(i <= segment) {
              if(i == segment) {
                  // Effetto rainbow pulsante per il segmento corrente
                  uint8_t hue = beat8(60); // Velocità pulsazione
                  leds[250 + i] = (seconds % 2 == 0) ? 
                      CHSV(hue, 255, 255) : color;
              } else {
                  // Segmenti precedenti colorati normalmente
                  leds[250 + i] = color;
              }
          } else {
              // Segmenti successivi spenti
              leds[250 + i] = color;
          }
      }
    }
  }

// Animazione secondi con effetto rainbow
  void showSeconds(uint8_t seconds, CRGB color) {
    if(currentBlink == 1) {        // se devono lampeggiare i secondi
      for(uint8_t i = 0; i < 6; i++) {
        // Effetto rainbow pulsante per il segmento corrente
        uint8_t hue = beat8(60); // Velocità pulsazione
        leds[116] = (seconds % 2 == 0) ? 
        CHSV(hue, 255, 255) : color;
      }
    } else {
        leds[116] = color; 
    }
  }


// Loop principale
void loop() {
   static uint32_t lastUpdate = 0;
   static uint32_t lastButtonCheck = 0;
   static uint32_t lastMatrixUpdate = 0;
   uint32_t currentMillis = millis();
  
   // Gestione OTA e network
   if(WiFi.status() == WL_CONNECTED) {
       ArduinoOTA.handle();
       events(); // ezTime events
   }
  
   // Check pulsanti ogni 50ms
   if(currentMillis - lastButtonCheck > 50) {
       checkButtons();
       lastButtonCheck = currentMillis;
   }

   // Aggiornamento orario ogni secondo se connesso
   if(WiFi.status() == WL_CONNECTED && currentMillis - lastUpdate > 1000) {
       currentHour = myTZ.hour();
       currentMinute = myTZ.minute();
//       currentMinute=21; // ############################ debug forzatura minuti per test
       currentSecond = myTZ.second();
       espalexa.loop();   
       // Gestione luminosità giorno/notte
       clockColors.brightness = (currentHour < 7 || currentHour >= 19) ? 
                               BRIGHTNESS_NIGHT : BRIGHTNESS_DAY;
       
       lastUpdate = currentMillis;
   }

   // Gestione luminosità giorno/notte
   if((myTZ.hour() < 7) || (myTZ.hour() >= 19)) {  // se nella fascia oraria dalle 19 alle 7 abbassa la luminosità
       intBrightness = NOTTE;               // setta la luminosità notturna
   } else {
       intBrightness = GIORNO;              // setta la luminosità giorno
   }
   FastLED.setBrightness(intBrightness);     // imposta la luminosità nella matrice neopixel
   FastLED.show(); 

   // Gestione display in base alla modalità
   if (alexaOff==0) {  // se è stato spento con alexa, non aggiornare l'orario
    if(currentMode == MODE_MATRIX) {
        if(currentMillis - lastMatrixUpdate > 16) { // ~60fps
            updateMatrix();
            lastMatrixUpdate = currentMillis;
        }
    } else if(currentMode == MODE_MATRIX2) {
        if(currentMillis - lastMatrixUpdate > 16) {
            updateMatrix2();
            lastMatrixUpdate = currentMillis;
        }
    } else {
        // Aggiorna display se necessario
        if(currentHour != prevHour || currentMinute != prevMinute) {
            updateDisplay();
            prevHour = currentHour;
            prevMinute = currentMinute;
        } else {
            // Aggiorna solo i secondi
            if(currentMinute >= 1) {
                showSeconds(currentSecond, clockColors.getColor());
                FastLED.show();
            }
        }
    }
  }
   yield();
}

void checkButtons() {
   static uint32_t lastModePress = 0;
   static uint32_t lastColorPress = 0;
   static uint32_t lastBothPress = 0;
   static uint32_t bothPressStart = 0;  // Aggiunta questa variabile statica
   const uint32_t DEBOUNCE_TIME = 300;
   const uint32_t RESET_WIFI_TIME = 5000; // 5 secondi per il reset WiFi
   

  // Lettura dei pulsanti in base alla logica definita
   #if BUTTON_LOGIC_INVERTED == 1
     // Logica invertita: il pulsante è premuto se digitalRead restituisce LOW
     bool isModeButtonPressed = !digitalRead(BUTTON_MODE);
     bool isColorButtonPressed = !digitalRead(BUTTON_SEC);
   #else
     // Logica normale: il pulsante è premuto se digitalRead restituisce HIGH
     bool isModeButtonPressed = digitalRead(BUTTON_MODE);
     bool isColorButtonPressed = digitalRead(BUTTON_SEC);
   #endif

   // Controlla se entrambi i pulsanti sono premuti per il reset WiFi
   if(isModeButtonPressed && isColorButtonPressed) {
       static uint32_t bothPressStart = 0;
       if(bothPressStart == 0) {
           bothPressStart = millis();
           Serial.println("Iniziato conteggio per reset WiFi...");
       }
       
       // Se i pulsanti sono stati premuti per 5 secondi
       if(millis() - bothPressStart >= RESET_WIFI_TIME) {
           // Indica il reset con LED rosso lampeggiante
           for(int i = 0; i < 5; i++) {
               fill_solid(leds, NUM_LEDS, CRGB::Red);
               FastLED.show();
               delay(100);
               FastLED.clear();
               FastLED.show();
               delay(100);
           }
           resetWiFi();
       }
   } else {
       // Reset del timer se i pulsanti vengono rilasciati
       bothPressStart = 0;
       
       // Gestione normale dei pulsanti
       if(digitalRead(BUTTON_MODE)) {
           if(millis() - lastBothPress > DEBOUNCE_TIME) {
               currentPreset = (currentPreset + 1) % 11;
               applyPreset(currentPreset);
               EEPROM.write(EEPROM_PRESET_ADDR, currentPreset);
               EEPROM.commit();
               lastBothPress = millis();
           }
       }
       if(digitalRead(BUTTON_SEC)) {
           if(millis() - lastBothPress > DEBOUNCE_TIME) {
               if (currentBlink == 0) {
                currentBlink = 1;
               } else {
                currentBlink = 0;
               }
               EEPROM.write(EEPROM_BLINK_ADDR, currentBlink);
               EEPROM.commit();
               lastBothPress = millis();
           }
       }

   }
}

void applyPreset(uint8_t preset) {
   switch(preset) {
       case 0:  // Normale - Solo orario
           currentMode = MODE_SLOW;
           clockColors.current = CRGB(random8(), random8(), random8());
           break;
           
       case 1:  // Fade con colore blu
           currentMode = MODE_FADE;
           clockColors.current = CRGB::Blue;
           break;
           
       case 2:  // Matrix con parole verdi
           currentMode = MODE_MATRIX2;
           matrixWordColor = CRGB::Green;
           matrix2State.needsReset = true;
           break;
           
       case 3:  // Lento con colore viola
           currentMode = MODE_SLOW;
           clockColors.current = CRGB(60,0,255);
           break;
           
       case 4:  // Rainbow mode
           currentMode = MODE_MATRIX2;
           matrixWordColor = CRGB::Blue;
           matrix2State.needsReset = true;
           break;
           
       case 5:  // Matrix con parole gialle
           currentMode = MODE_MATRIX;
           matrixWordColor = CRGB::Yellow;
           break;
           
       case 6:  // Fade con verde
           currentMode = MODE_FADE;
           clockColors.current = CRGB::Green;
           break;
           
       case 7:  // Lento con arancione
           currentMode = MODE_SLOW;
           clockColors.current = CRGB(255,165,0);
           break;
           
       case 8:  // Matrix con parole ciano
           currentMode = MODE_MATRIX;
           matrixWordColor = CRGB::Cyan;
           break;
           
       case 9:  // Fade con rosa
           currentMode = MODE_FADE;
           clockColors.current = CRGB(255,20,147);
           break;
           
       case 10:  // Veloce con colore acqua
           currentMode = MODE_FAST;
           clockColors.current = CRGB(0,255,255);
           break;
           
       default:  // Default a preset 0
           currentMode = MODE_SLOW;
           clockColors.current = CRGB(random8(), random8(), random8());
           break;
   }
   
   // Aggiorna display dopo il cambio preset
   FastLED.setBrightness(intBrightness);
   updateDisplay();
}

// Inizializzazione effetto Matrix
void initMatrix() {
   memset(targetPixels, 0, sizeof(targetPixels));
   memset(activePixels, 0, sizeof(activePixels));
   
   // Imposta target pixels per l'ora corrente
   displayWordToTarget(WORD_SONO_LE);
   
   // Ora corrente
   uint8_t hour12 = currentHour % 12;
   const uint8_t* hourWord = (const uint8_t*)pgm_read_ptr(&HOUR_WORDS[hour12]);
   displayWordToTarget(hourWord);
   
   if(currentMinute > 0) {
       displayWordToTarget(WORD_E);
       displayMinutesToTarget(currentMinute);
       displayWordToTarget(WORD_MINUTI);
   }
   
   // Inizializza gocce
   for(int i = 0; i < NUM_DROPS; i++) {
       initDrop(drops[i]);
   }
   
   matrixInitialized = true;
}

void initDrop(Drop &drop) {
    drop.x = random8(MATRIX_WIDTH);
    drop.y = random(MATRIX_START_Y_MIN, MATRIX_START_Y_MAX);
    if (drop.isMatrix2) {
        drop.speed = MATRIX2_BASE_SPEED + (random(100) / 100.0f * MATRIX2_SPEED_VAR);
    } else {
        drop.speed = MATRIX_BASE_SPEED + (random(100) / 100.0f * MATRIX_SPEED_VAR);
    }
    drop.active = true;
}

// Funzione per settare il tipo di effetto per una goccia
void setDropType(Drop &drop, bool isMatrix2) {
    drop.isMatrix2 = isMatrix2;
    initDrop(drop);
}

void displayWordToTarget(const uint8_t* word) {
   uint8_t idx = 0;
   uint8_t pixel;
   while((pixel = pgm_read_byte(&word[idx])) != 8) {
       targetPixels[pixel] = true;
       idx++;
   }
}

void updateMatrix() {
   static uint8_t lastHour = 255;
   static uint8_t lastMinute = 255;

   // Verifica cambio orario
   if(currentHour != lastHour || currentMinute != lastMinute) {

       // Reset completo
       FastLED.clear();
       memset(targetPixels, 0, sizeof(targetPixels));
       memset(activePixels, 0, sizeof(activePixels));
       
       // Imposta i nuovi target pixel
       displayWordToTarget(WORD_SONO_LE);
       const uint8_t* hourWord = (const uint8_t*)pgm_read_ptr(&HOUR_WORDS[currentHour]);
       displayWordToTarget(hourWord);
       
       if(currentMinute > 0) {
           displayWordToTarget(WORD_E);
           displayMinutesToTarget(currentMinute);
           displayWordToTarget(WORD_MINUTI);
       }
       
       // Reset gocce
       for(int i = 0; i < NUM_DROPS; i++) {
           drops[i].isMatrix2 = false;  // Imposta per Matrix normale
           initDrop(drops[i]);
       }
       
       lastHour = currentHour;
       lastMinute = currentMinute;
   }
   
   FastLED.clear();
   
   // Gestione effetto matrix
   for(uint8_t i = 0; i < NUM_DROPS; i++) {
       Drop &drop = drops[i];
       if(!drop.active) continue;
       
       uint16_t pos = ((int)drop.y * MATRIX_WIDTH) + drop.x;
       
       if(drop.y >= 0 && drop.y < MATRIX_HEIGHT && pos < NUM_LEDS) {
           // Attiva nuovi pixel target
           if(targetPixels[pos] && !activePixels[pos]) {
               activePixels[pos] = true;
               leds[pos] = matrixWordColor;
           } 
           // Effetto matrix sui pixel non target
           else if(!targetPixels[pos]) {
               uint8_t intensity = 255 - ((int)drop.y * 16);
               leds[pos] = CRGB(0, intensity, 0);
               
               // Scia
               for(int trail = 1; trail <= MATRIX_TRAIL_LENGTH; trail++) {
                   int trailPos = pos - (MATRIX_WIDTH * trail);
                   if(trailPos >= 0 && !targetPixels[trailPos]) {
                       leds[trailPos] = CRGB(0, intensity/(trail*2), 0);
                   }
               }
           }
       }
       
       // Aggiorna posizione goccia
       drop.y += drop.speed;
       
       // Reset goccia se fuori schermo
       if(drop.y >= MATRIX_HEIGHT) {
           drop.isMatrix2 = false;  // Mantiene Matrix normale
           initDrop(drop);
       }
   }
   
   // Mantieni pixel attivi
   for(uint16_t i = 0; i < NUM_LEDS; i++) {
       if(targetPixels[i] && activePixels[i]) {
           leds[i] = matrixWordColor;
       }
   }
   
   FastLED.show();
}

// Funzione per mostrare i minuti nell'effetto matrix
void displayMinutesToTarget(uint8_t minutes) {
   if(minutes <= 0) return;

   if(minutes <= 19) {
       // Uso diretto della lookup table per 1-19
       const uint8_t* minuteWord = (const uint8_t*)pgm_read_ptr(&MINUTE_WORDS[minutes]);
       displayWordToTarget(minuteWord);
   } else {
       // Gestione decine (20-59)
       uint8_t tens = (minutes / 10) - 2;  // -2 perché partiamo da 20
       uint8_t ones = minutes % 10;
       const MinuteTens* tensWords = &TENS_WORDS[tens];
       
       // Scegli tra forma normale e troncata
       const uint8_t* decinaWord;
       if(ones == 8 || (ones == 1 && minutes >= 21)) {
           decinaWord = (const uint8_t*)pgm_read_ptr(&tensWords->truncated);
       } else {
           decinaWord = (const uint8_t*)pgm_read_ptr(&tensWords->normal);
       }
       
       displayWordToTarget(decinaWord);
       
       // Se ci sono unità, mostra anche quelle
       if(ones > 0) {
          const uint8_t* onesWord = (const uint8_t*)pgm_read_ptr(&MINUTE_WORDS[ones]);
          if (ones == 1) {
             onesWord = (WORD_MUN);
          }    
          displayWordToTarget(onesWord);
       }
   }
}

void colorChanged(uint8_t brightness, uint32_t rgb) {

   if (brightness == 0) {
    alexaOff=1;  
    FastLED.clear();
    FastLED.show();
    return;
   } else {
    alexaOff=0;
   } 

   // Converte il colore RGB di Alexa nel formato CRGB di FastLED
   CRGB newColor;
   newColor.r = (rgb >> 16) & 0xFF;
   newColor.g = (rgb >> 8) & 0xFF;
   newColor.b = rgb & 0xFF;

   // Aggiorna i colori
   clockColors.current = newColor;
   clockColors.brightness = brightness;
   FastLED.setBrightness(intBrightness);
 
   // Se in modalità matrix, aggiorna anche il colore delle parole
   if(currentMode == MODE_MATRIX) {
       matrixWordColor = newColor;
   }

   updateDisplay();
}

// Funzioni di display con FastLED
void paintWordSlow(const uint8_t arrWord[], CRGB color) {
   uint8_t i = 0;
   uint8_t pixel;
   
   while((pixel = pgm_read_byte(&arrWord[i])) != 8 && i < NUM_LEDS) {
       leds[pixel] = color;
       FastLED.show();
       delay(500);
       yield();  // Per ESP32
       i++;
   }
}

void paintWordFast(const uint8_t arrWord[], CRGB color) {
   uint8_t i = 0;
   uint8_t pixel;
   
   while((pixel = pgm_read_byte(&arrWord[i])) != 8 && i < NUM_LEDS) {
       leds[pixel] = color;
       i++;
   }
   FastLED.show();
}

void paintWordSpeed(const uint8_t arrWord[], CRGB color, int speed) {
   uint8_t i = 0;
   uint8_t pixel;
   
   while((pixel = pgm_read_byte(&arrWord[i])) != 8 && i < NUM_LEDS) {
       leds[pixel] = color;
       FastLED.show();
       delay(speed);
       yield();  // Per ESP32
       i++;
   }
}

// Funzione per l'effetto Matrix2
void updateMatrix2() {
    static uint8_t lastHour = 255;
    static uint8_t lastMinute = 255;
    
    // Verifica se è necessario resettare l'effetto
    if(currentHour != lastHour || currentMinute != lastMinute || matrix2State.needsReset) {
        // Reset completo
        FastLED.clear();
        memset(targetPixels, 0, sizeof(targetPixels));
        memset(activePixels, 0, sizeof(activePixels));
        
        // Imposta i nuovi target pixel
        displayWordToTarget(WORD_SONO_LE);
        const uint8_t* hourWord = (const uint8_t*)pgm_read_ptr(&HOUR_WORDS[currentHour]);
        displayWordToTarget(hourWord);
        
        if(currentMinute > 0) {
            displayWordToTarget(WORD_E);
            displayMinutesToTarget(currentMinute);
            displayWordToTarget(WORD_MINUTI);
        }
        
        // Reset gocce
        for(int i = 0; i < NUM_DROPS; i++) {
            drops[i].isMatrix2 = true;  // Imposta il flag Matrix2
            initDrop(drops[i]);
        }
        
        lastHour = currentHour;
        lastMinute = currentMinute;
        matrix2State.completed = false;
        matrix2State.needsReset = false;
    }

    // Se l'effetto è completato, mantieni solo le parole illuminate
    if(matrix2State.completed) {
        FastLED.clear();
        for(uint16_t i = 0; i < NUM_LEDS; i++) {
            if(targetPixels[i] && activePixels[i]) {
                leds[i] = matrixWordColor;
            }
        }
        FastLED.show();
        return;
    }

    FastLED.clear();
    bool allTargetPixelsActive = true;

    // Gestione effetto matrix
    for(uint8_t i = 0; i < NUM_DROPS; i++) {
        Drop &drop = drops[i];
        if(!drop.active) continue;
        
        uint16_t pos = ((int)drop.y * MATRIX_WIDTH) + drop.x;
        
        if(drop.y >= 0 && drop.y < MATRIX_HEIGHT && pos < NUM_LEDS) {
            // Attiva nuovi pixel target
            if(targetPixels[pos] && !activePixels[pos]) {
                activePixels[pos] = true;
                leds[pos] = matrixWordColor;
            } 
            // Effetto matrix sui pixel non target
            else if(!targetPixels[pos]) {
                uint8_t intensity = 255 - ((int)drop.y * 16);
                leds[pos] = CRGB(0, intensity, 0);
                
                // Scia
                for(int trail = 1; trail <= MATRIX_TRAIL_LENGTH; trail++) {
                    int trailPos = pos - (MATRIX_WIDTH * trail);
                    if(trailPos >= 0 && !targetPixels[trailPos]) {
                        leds[trailPos] = CRGB(0, intensity/(trail*2), 0);
                    }
                }
            }
        }
        
        // Aggiorna posizione goccia
        drop.y += drop.speed;
        
        // Reset goccia se fuori schermo
        if(drop.y >= MATRIX_HEIGHT) {
            if(!matrix2State.completed) {
                drop.isMatrix2 = true;  // Mantiene il flag Matrix2
                initDrop(drop);
            } else {
                drop.active = false;
            }
        }
    }

    // Verifica se tutti i pixel target sono stati attivati
    for(uint16_t i = 0; i < NUM_LEDS; i++) {
        if(targetPixels[i] && !activePixels[i]) {
            allTargetPixelsActive = false;
            break;
        }
    }

    // Se tutti i pixel target sono attivi, imposta il completamento
    if(allTargetPixelsActive && !matrix2State.completed) {
        matrix2State.completed = true;
        matrix2State.completionTime = millis();
    }

    // Mantieni pixel attivi
    for(uint16_t i = 0; i < NUM_LEDS; i++) {
        if(targetPixels[i] && activePixels[i]) {
            leds[i] = matrixWordColor;
        }
    }

    FastLED.show();
}

void rainbow(uint8_t wait) {
   static uint8_t hue = 0;
   
   for(uint16_t i = 0; i < NUM_LEDS; i++) {
       leds[i] = CHSV(hue + (i * 256 / NUM_LEDS), 255, 255);
   }
   
   FastLED.show();
   delay(wait);
   
   hue++;  // Ciclo dei colori
}

// Funzione Wheel convertita per FastLED
CRGB Wheel(byte WheelPos) {
   WheelPos = 255 - WheelPos;
   
   if(WheelPos < 85) {
       return CRGB(255 - WheelPos * 3, 0, WheelPos * 3);
   }
   
   if(WheelPos < 170) {
       WheelPos -= 85;
       return CRGB(0, WheelPos * 3, 255 - WheelPos * 3);
   }
   
   WheelPos -= 170;
   return CRGB(WheelPos * 3, 255 - WheelPos * 3, 0);
}

