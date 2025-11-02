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
//
// 30/10/2025
// aggiunto effetto MOTO con scia arcobaleno 
// MODIFICATO: la scia colora le scritte dell'orario man mano che passa
//
// 31/10/2025
// aggiunto effetto GALAGA - sparo dal basso che colpisce le lettere
//
// 01/11/2025
// aggiunto effetto PACMAN

//
// [Data corrente]
// aggiunto effetto PACMAN - Pacman che insegue fantasmi colorando le scritte

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
#define MODE_TRON   5
#define MODE_MOTO   6
#define MODE_GALAGA 7  // Nuova modalità GALAGA
#define MODE_PACMAN 8  // Nuova modalità PACMAN
#define NUM_MODES   9  // Aggiorna a 9

// Strutture e costanti per effetto Matrix
#define MATRIX_BASE_SPEED   0.15f
#define MATRIX_SPEED_VAR    0.01f
#define MATRIX_START_Y_MIN  -3.0f
#define MATRIX_START_Y_MAX   0.0f
#define MATRIX_TRAIL_LENGTH  13
#define NUM_DROPS           32      
#define MATRIX2_BASE_SPEED  0.15f
#define MATRIX2_SPEED_VAR   0.1f

// Strutture e costanti per effetto MOTO
#define MOTO_SPEED 30           // Velocità dell'effetto (ms tra i frame)
#define MOTO_TRAIL_LENGTH 8     // Lunghezza della scia
#define MOTO_WAIT_AFTER 5000    // Attesa dopo il completamento (5 secondi)

// Strutture e costanti per effetto GALAGA
#define GALAGA_SPEED 30         // Velocità dell'effetto (ms tra i frame)
#define GALAGA_WAIT_AFTER 5000  // Attesa dopo il completamento (5 secondi)
#define GALAGA_MAX_SHOTS 8      // Numero massimo di spari contemporanei
#define GALAGA_SHOT_LENGTH 3    // Lunghezza dello sparo

// Strutture e costanti per effetto PACMAN
#define PACMAN_SPEED 50           // Velocità dell'effetto (ms tra i frame) - RIPRISTINATA a 50
#define PACMAN_WAIT_AFTER 5000    // Attesa dopo il completamento (5 secondi)
#define NUM_GHOSTS 4              // Numero di fantasmi

// Strutture e costanti per EEPROM
#define EEPROM_SIZE 10
#define EEPROM_CONFIGURED_MARKER 0x55
#define EEPROM_PRESET_ADDR 1
#define EEPROM_BLINK_ADDR 2

#define GIORNO 64     // full on
#define NOTTE 32       // half on

uint8_t currentPreset;
int intBrightness;
uint8_t gHue = 0;
struct Drop {
    uint8_t x;
    float y;
    float speed;
    uint8_t intensity;
    bool active;
    bool isMatrix2;
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

// Variabili per effetto TRON
bool tronGrid[NUM_LEDS] = {false};
uint8_t tronIntensity[NUM_LEDS] = {0};
uint32_t lastTronUpdate = 0;
const uint8_t TRON_UPDATE_INTERVAL = 30; // ms
uint32_t tronCycleStart = 0;
const uint32_t TRON_EFFECT_DURATION = 5000;
const uint32_t TEXT_ONLY_DURATION = 10000;
bool tronEffectActive = true;

// Variabili per effetto MOTO
uint32_t lastMotoUpdate = 0;
uint16_t motoCurrentPosition = 0;
bool motoEffectActive = false;
uint32_t motoCompletionTime = 0;
CRGB motoTrailColors[MOTO_TRAIL_LENGTH];
uint8_t motoTrailHue = 0;
uint8_t motoTextHue = 0;
CRGB motoTextColor = CRGB::White;
bool motoTextPixels[NUM_LEDS] = {false};

// Variabili per effetto GALAGA
uint32_t lastGalagaUpdate = 0;
bool galagaEffectActive = false;
uint32_t galagaCompletionTime = 0;
uint8_t galagaTextHue = 0;
CRGB galagaTextColor = CRGB::White;
bool galagaTextPixels[NUM_LEDS] = {false};
uint8_t galagaShotHue = 0;

// Struttura per gli spari GALAGA
struct GalagaShot {
    int8_t x;           // Posizione X corrente
    int8_t y;           // Posizione Y corrente
    int8_t targetX;     // Posizione X target (dove ci sono lettere da colpire)
    int8_t targetY;     // Posizione Y target
    int8_t dx;          // Direzione X (-1, 0, 1)
    int8_t dy;          // Direzione Y (-1, 0, 1)
    bool active;        // Se lo sparo è attivo
    uint8_t speed;      // Velocità dello sparo
    uint32_t lastMove;  // Ultimo momento in cui si è mosso
};

GalagaShot galagaShots[GALAGA_MAX_SHOTS];

// Variabili per effetto PACMAN
uint32_t lastPacmanUpdate = 0;
bool pacmanEffectActive = false;
uint32_t pacmanCompletionTime = 0;
uint8_t pacmanTextHue = 0;
CRGB pacmanTextColor = CRGB::White;
bool pacmanTextPixels[NUM_LEDS] = {false};
int16_t pacmanPosition = -2; // Posizione iniziale di Pacman (fuori dallo schermo a sinistra)
int16_t ghostPositions[NUM_GHOSTS] = {0}; // Posizioni dei fantasmi
CRGB ghostColors[NUM_GHOSTS] = {CRGB::Red, CRGB(255, 182, 255), CRGB::Cyan, CRGB(255, 165, 0)}; // Rosso, Rosa, Ciano, Arancio

// Definizione array LED e controller
CRGB leds[NUM_LEDS];
CRGB matrixWordColor = CRGB::Blue;
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
uint8_t Presets;
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
   WORD_ZERO,
   WORD_UNA,
   WORD_DUE,
   WORD_TRE,
   WORD_QUATTRO,
   WORD_CINQUE,
   WORD_SEI,
   WORD_SETTE,
   WORD_OTTO,
   WORD_NOVE,
   WORD_DIECI,
   WORD_UNDICI,
   WORD_DODICI,
   WORD_TREDICI,
   WORD_QUATTORDICI,
   WORD_QUINDICI,
   WORD_SEDICI,
   WORD_DICIASSETTE,
   WORD_DICIOTTO,
   WORD_DICIANNOVE,
   WORD_VENTI,
   WORD_VENTUNO,
   WORD_VENTIDUE,
   WORD_VENTITRE
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
   nullptr,
   WORD_MUNO,
   WORD_MDUE,
   WORD_MTRE,
   WORD_MQUATTRO,
   WORD_MCINQUE,
   WORD_MSEI,
   WORD_MSETTE,
   WORD_MOTTO,
   WORD_MNOVE,
   WORD_MDIECI,
   WORD_MUNDICI,
   WORD_MDODICI,
   WORD_MTREDICI,
   WORD_MQUATTORDICI,
   WORD_MQUINDICI,
   WORD_MSEDICI,
   WORD_MDICIASSETTE,
   WORD_MDICIOTTO,
   WORD_MDICIANNOVE
};

// Struttura per le decine
struct MinuteTens {
   const uint8_t* normal;
   const uint8_t* truncated;
};

// Array di lookup per le decines
const MinuteTens PROGMEM TENS_WORDS[] = {
   {WORD_MVENTI, WORD_MVENT},
   {WORD_MTRENTA, WORD_MTRENT},
   {WORD_MQUARANTA, WORD_MQUARANT},
   {WORD_MCINQUANTA, WORD_MCINQUANT}
};

// Struttura per gestione colori
struct ClockColors {
   CRGB current;
   uint8_t brightness;
   static const CRGB presets[];
   uint8_t presetIndex;
   
   ClockColors() : current(CRGB::White), brightness(BRIGHTNESS_DAY), presetIndex(0) {}
   
   void nextPreset() {
       presetIndex = (presetIndex + 1) % 7;
       current = presets[presetIndex];
   }
   
   CRGB getColor() const {
       return CRGB(scale8(current.r, brightness),
                  scale8(current.g, brightness),
                  scale8(current.b, brightness));
   }
};

const CRGB ClockColors::presets[] = {
   CRGB::White,
   CRGB::Blue,
   CRGB::Red,
   CRGB(60,0,255),
   CRGB::Green,
   CRGB::Yellow,
   CRGB::White
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

// Funzioni per effetto GALAGA
void createNewGalagaShot();
void findTargetForShot(GalagaShot &shot);
void calculateShotDirection(GalagaShot &shot);

void initGalagaEffect() {
    FastLED.clear();
    galagaEffectActive = true;
    galagaCompletionTime = 0;
    galagaTextHue = random8();
    galagaTextColor = CHSV(galagaTextHue, 255, 255);
    
    // Reset array pixel della scritta
    memset(galagaTextPixels, 0, sizeof(galagaTextPixels));
    
    // Inizializza gli spari
    for (int i = 0; i < GALAGA_MAX_SHOTS; i++) {
        galagaShots[i].active = false;
        galagaShots[i].lastMove = 0;
    }
    
    // Ricalcola i target pixel per l'orario corrente
    memset(targetPixels, 0, sizeof(targetPixels));
    displayWordToTarget(WORD_SONO_LE);
    
    const uint8_t* hourWord = (const uint8_t*)pgm_read_ptr(&HOUR_WORDS[currentHour]);
    displayWordToTarget(hourWord);
    
    if (currentMinute > 0) {
        displayWordToTarget(WORD_E);
        displayMinutesToTarget(currentMinute);
        displayWordToTarget(WORD_MINUTI);
    }
    
    // Crea i primi spari
    for (int i = 0; i < 3; i++) {
        createNewGalagaShot();
    }
}

void createNewGalagaShot() {
    for (int i = 0; i < GALAGA_MAX_SHOTS; i++) {
        if (!galagaShots[i].active) {
            // Posizione iniziale casuale in basso
            galagaShots[i].x = random8(MATRIX_WIDTH);
            galagaShots[i].y = MATRIX_HEIGHT - 1;
            
            // Trova un target tra le lettere non ancora colpite
            findTargetForShot(galagaShots[i]);
            
            galagaShots[i].active = true;
            galagaShots[i].speed = random8(2, 5);
            galagaShots[i].lastMove = millis();
            break;
        }
    }
}

void findTargetForShot(GalagaShot &shot) {
    // Cerca lettere non ancora colpite
    for (int y = 0; y < MATRIX_HEIGHT; y++) {
        for (int x = 0; x < MATRIX_WIDTH; x++) {
            uint16_t pos = y * MATRIX_WIDTH + x;
            if (targetPixels[pos] && !galagaTextPixels[pos]) {
                // 30% di probabilità di scegliere questo target
                if (random8(100) < 30) {
                    shot.targetX = x;
                    shot.targetY = y;
                    calculateShotDirection(shot);
                    return;
                }
            }
        }
    }
    
    // Se non trova target, va dritto verso l'alto
    shot.targetX = shot.x;
    shot.targetY = 0;
    shot.dx = 0;
    shot.dy = -1;
}

void calculateShotDirection(GalagaShot &shot) {
    int8_t diffX = shot.targetX - shot.x;
    int8_t diffY = shot.targetY - shot.y;
    
    // Normalizza la direzione
    shot.dx = (diffX > 0) ? 1 : (diffX < 0) ? -1 : 0;
    shot.dy = (diffY > 0) ? 1 : (diffY < 0) ? -1 : 0;
    
    // Se il target è sopra, priorità alla salita
    if (diffY < 0) {
        shot.dy = -1;
        // Movimento orizzontale solo se necessario
        if (abs(diffX) > 2) {
            shot.dx = (diffX > 0) ? 1 : -1;
        } else {
            shot.dx = 0;
        }
    }
}

void updateGalagaEffect() {
    uint32_t currentMillis = millis();
    
    // Gestione attesa dopo il completamento
    if (galagaCompletionTime != 0 && currentMillis - galagaCompletionTime < GALAGA_WAIT_AFTER) {
        // Durante l'attesa, mostra solo la scritta dell'orario che è stata colorata
        FastLED.clear();
        for (uint16_t i = 0; i < NUM_LEDS; i++) {
            if (galagaTextPixels[i]) {
                leds[i] = galagaTextColor;
            }
        }
        FastLED.show();
        
        // Crea occasionalmente nuovi spari durante l'attesa per effetto decorativo
        if (random8(100) < 10) {
            createNewGalagaShot();
        }
        return;
    }
    
    // Reset per nuovo ciclo
    if (galagaCompletionTime != 0 && currentMillis - galagaCompletionTime >= GALAGA_WAIT_AFTER) {
        initGalagaEffect();
        return;
    }
    
    // Aggiornamento periodico
    if (currentMillis - lastGalagaUpdate < GALAGA_SPEED) return;
    lastGalagaUpdate = currentMillis;
    
    // Crea nuovi spari occasionalmente
    if (random8(100) < 15) {
        createNewGalagaShot();
    }
    
    // Cancella tutto
    FastLED.clear();
    
    bool allShotsInactive = true;
    int activeShots = 0;
    
    // Aggiorna e disegna tutti gli spari attivi
    for (int i = 0; i < GALAGA_MAX_SHOTS; i++) {
        if (galagaShots[i].active) {
            allShotsInactive = false;
            activeShots++;
            
            // Muovi lo sparo secondo la direzione
            if (currentMillis - galagaShots[i].lastMove > (100 / galagaShots[i].speed)) {
                galagaShots[i].x += galagaShots[i].dx;
                galagaShots[i].y += galagaShots[i].dy;
                galagaShots[i].lastMove = currentMillis;
                
                // Controlla bordi
                if (galagaShots[i].x < 0) {
                    galagaShots[i].x = 0;
                    galagaShots[i].dx = 1;
                } else if (galagaShots[i].x >= MATRIX_WIDTH) {
                    galagaShots[i].x = MATRIX_WIDTH - 1;
                    galagaShots[i].dx = -1;
                }
                
                if (galagaShots[i].y < 0) {
                    galagaShots[i].active = false;
                    continue;
                }
                
                // Se lo sparo ha raggiunto il target o è vicino, trova nuovo target
                if (abs(galagaShots[i].x - galagaShots[i].targetX) <= 1 && 
                    abs(galagaShots[i].y - galagaShots[i].targetY) <= 1) {
                    findTargetForShot(galagaShots[i]);
                }
            }
            
            // Disegna lo sparo (linea di pixel)
            for (int j = 0; j < GALAGA_SHOT_LENGTH; j++) {
                int drawX = galagaShots[i].x;
                int drawY = galagaShots[i].y - j;
                
                if (drawY >= 0 && drawY < MATRIX_HEIGHT) {
                    uint16_t pos = drawY * MATRIX_WIDTH + drawX;
                    
                    // Colore sparo: bianco con effetto arcobaleno
                    uint8_t shotHue = (galagaShotHue + (i * 30)) % 255;
                    leds[pos] = CHSV(shotHue, 255, 255);
                    
                    // Se lo sparo colpisce una lettera non colorata, colorala
                    if (targetPixels[pos] && !galagaTextPixels[pos]) {
                        galagaTextPixels[pos] = true;
                    }
                }
            }
        }
    }
    
    galagaShotHue += 2;
    
    // Disegna i pixel della scritta che sono stati colorati
    for (uint16_t i = 0; i < NUM_LEDS; i++) {
        if (galagaTextPixels[i]) {
            leds[i] = galagaTextColor;
        }
    }
    
    // Controlla se tutte le lettere sono state colorate
    bool allLettersColored = true;
    for (uint16_t i = 0; i < NUM_LEDS; i++) {
        if (targetPixels[i] && !galagaTextPixels[i]) {
            allLettersColored = false;
            break;
        }
    }
    
    // Se tutte le lettere sono colorate e non ci sono spari attivi, completa l'effetto
    if (allLettersColored && activeShots == 0 && galagaCompletionTime == 0) {
        galagaCompletionTime = currentMillis;
    }
    
    // Se ci sono ancora lettere da colorare ma pochi spari attivi, creane di nuovi
    if (!allLettersColored && activeShots < 3 && random8(100) < 30) {
        createNewGalagaShot();
    }
    
    FastLED.show();
}

// Funzioni per effetto PACMAN
void initPacmanEffect() {
    FastLED.clear();
    pacmanEffectActive = true;
    pacmanCompletionTime = 0;
    pacmanTextHue = random8();
    pacmanTextColor = CRGB::White; // Testo bianco come richiesto
    
    // Reset array pixel della scritta
    memset(pacmanTextPixels, 0, sizeof(pacmanTextPixels));
    
    // Inizializza posizioni - Pacman davanti ai fantasmi
    pacmanPosition = -2; // Inizia fuori dallo schermo a sinistra
    
    // Posiziona i fantasmi dietro a Pacman con spaziatura
    for (int i = 0; i < NUM_GHOSTS; i++) {
        ghostPositions[i] = pacmanPosition - 4 - (i * 3); // Fantasmi dietro a Pacman
    }
    
    // Ricalcola i target pixel per l'orario corrente
    memset(targetPixels, 0, sizeof(targetPixels));
    displayWordToTarget(WORD_SONO_LE);
    
    const uint8_t* hourWord = (const uint8_t*)pgm_read_ptr(&HOUR_WORDS[currentHour]);
    displayWordToTarget(hourWord);
    
    if (currentMinute > 0) {
        displayWordToTarget(WORD_E);
        displayMinutesToTarget(currentMinute);
        displayWordToTarget(WORD_MINUTI);
    }
}

void updatePacmanEffect() {
    uint32_t currentMillis = millis();
    
    // Gestione attesa dopo il completamento (5 secondi)
    if (pacmanCompletionTime != 0 && currentMillis - pacmanCompletionTime < PACMAN_WAIT_AFTER) {
        // Durante l'attesa, mostra solo la scritta dell'orario che è stata colorata
        FastLED.clear();
        for (uint16_t i = 0; i < NUM_LEDS; i++) {
            if (pacmanTextPixels[i]) {
                leds[i] = pacmanTextColor;
            }
        }
        FastLED.show();
        return;
    }
    
    // Reset per nuovo ciclo
    if (pacmanCompletionTime != 0 && currentMillis - pacmanCompletionTime >= PACMAN_WAIT_AFTER) {
        initPacmanEffect();
        return;
    }
    
    // Aggiornamento periodico
    if (currentMillis - lastPacmanUpdate < PACMAN_SPEED) return;
    lastPacmanUpdate = currentMillis;
    
    // Cancella tutto
    FastLED.clear();
    
    // Muovi Pacman e fantasmi
    pacmanPosition++;
    
    for (int i = 0; i < NUM_GHOSTS; i++) {
        ghostPositions[i]++;
    }
    
    // Disegna Pacman (2 LED gialli in orizzontale)
    if (pacmanPosition >= 0 && pacmanPosition < NUM_LEDS) {
        // Calcola le posizioni per i 2 LED di Pacman
        int16_t pacmanLeft = pacmanPosition;
        int16_t pacmanRight = pacmanPosition + 1;
        
        // Disegna Pacman solo se le posizioni sono valide
        if (pacmanLeft >= 0 && pacmanLeft < NUM_LEDS) leds[pacmanLeft] = CRGB::Yellow;
        if (pacmanRight >= 0 && pacmanRight < NUM_LEDS) leds[pacmanRight] = CRGB::Yellow;
        
        // Marca i pixel della scritta che Pacman passa sopra
        if (pacmanLeft >= 0 && pacmanLeft < NUM_LEDS && targetPixels[pacmanLeft]) {
            pacmanTextPixels[pacmanLeft] = true;
        }
        if (pacmanRight >= 0 && pacmanRight < NUM_LEDS && targetPixels[pacmanRight]) {
            pacmanTextPixels[pacmanRight] = true;
        }
    }
    
    // Disegna fantasmi (1 LED ciascuno)
    for (int i = 0; i < NUM_GHOSTS; i++) {
        if (ghostPositions[i] >= 0 && ghostPositions[i] < NUM_LEDS) {
            leds[ghostPositions[i]] = ghostColors[i];
            
            // Marca i pixel della scritta che i fantasmi passano sopra
            if (targetPixels[ghostPositions[i]]) {
                pacmanTextPixels[ghostPositions[i]] = true;
            }
        }
    }
    
    // Disegna i pixel della scritta che sono stati colorati
    for (uint16_t i = 0; i < NUM_LEDS; i++) {
        if (pacmanTextPixels[i]) {
            leds[i] = pacmanTextColor;
        }
    }
    
    // Se Pacman esce dallo schermo a destra, completa l'effetto
    if (pacmanPosition >= NUM_LEDS && pacmanCompletionTime == 0) {
        pacmanCompletionTime = currentMillis;
    }
    
    FastLED.show();
}

// Setup principale
void setup() {
   Serial.begin(115200);
   Serial.println("START");
   EEPROM.begin(EEPROM_SIZE);
   
   // Leggi configurazione
   if (EEPROM.read(0) != EEPROM_CONFIGURED_MARKER) {
       EEPROM.write(0, EEPROM_CONFIGURED_MARKER);
       EEPROM.write(EEPROM_PRESET_ADDR, 0);
       EEPROM.write(EEPROM_BLINK_ADDR, 1);
       EEPROM.commit();
   }
   
   // Carica preset salvato
   currentPreset = EEPROM.read(EEPROM_PRESET_ADDR);
   currentBlink = EEPROM.read(EEPROM_BLINK_ADDR);
   applyPreset(currentPreset);                   
    
   // Configurazione pin in base alla logica dei pulsanti
   #if BUTTON_LOGIC_INVERTED == 1
     pinMode(BUTTON_MODE, INPUT_PULLUP);
     pinMode(BUTTON_SEC, INPUT_PULLUP);
   #else
     pinMode(BUTTON_MODE, INPUT_PULLDOWN);
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
   wm.setConfigPortalTimeout(180);
   wm.setTitle("ORAQUADRA WiFi Setup");
   
   // Mostra LED blu durante la configurazione
   fill_solid(leds, NUM_LEDS, CRGB::Black);
   leds[0] = CRGB::Blue;
   FastLED.show();
   
   bool res = wm.autoConnect("ORAQUADRA_AP");

   if(!res) {
       Serial.println("Fallimento connessione");
       fill_solid(leds, NUM_LEDS, CRGB::Red);
       FastLED.show();
       delay(3000);
       ESP.restart();
   } 
   else {
       Serial.println("WiFi Connesso!");
       fill_solid(leds, NUM_LEDS, CRGB::Green);
       FastLED.show();
       delay(1000);
       FastLED.clear();
       FastLED.show();
   }
   
   WiFi.setSleep(false);
   WiFi.setAutoReconnect(true);
   
   if(WiFi.status() == WL_CONNECTED) {
       setupOTA();
       espalexa.addDevice("ORAQUADRA", colorChanged, 255);
       espalexa.begin();
       setServer("pool.ntp.org");
       waitForSync(10);
       myTZ.setLocation(F("Europe/Rome"));
       Serial.println("Alexa configurata - Cerca 'ORAQUADRA' nella app Alexa");
   }
   
   updateDisplay();
}

void resetWiFi() {
    WiFiManager wm;
    wm.resetSettings();
    ESP.restart();
}

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

void updateDisplay() {
   FastLED.clear();
   CRGB currentColor = clockColors.getColor();
   
   displayWord(WORD_SONO_LE, currentColor);
   
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
   
   if(currentMinute > 0 && currentMode != MODE_SLOW) {
       showSeconds(currentSecond, currentColor);
   }
   
   FastLED.show();
}

void showMinutes(uint8_t minutes, CRGB color) {
   static const uint8_t* const minuteWords[] PROGMEM = {
       nullptr,
       WORD_MUNO,
       WORD_MDUE,
       WORD_MTRE,
       WORD_MQUATTRO,
       WORD_MCINQUE,
       WORD_MSEI,
       WORD_MSETTE,
       WORD_MOTTO,
       WORD_MNOVE,
       WORD_MDIECI,
       WORD_MUNDICI,
       WORD_MDODICI,
       WORD_MTREDICI,
       WORD_MQUATTORDICI,
       WORD_MQUINDICI,
       WORD_MSEDICI,
       WORD_MDICIASSETTE,
       WORD_MDICIOTTO,
   WORD_MDICIANNOVE
   };

   if(minutes <= 0) return;

   if(minutes <= 19) {
       displayWord((const uint8_t*)pgm_read_ptr(&minuteWords[minutes]), color);
   } else {
       uint8_t tens = minutes / 10;
       uint8_t ones = minutes % 10;
       
       switch(tens) {
           case 2:
               displayWord(ones == 1 || ones == 8 ? WORD_MVENT : WORD_MVENTI, color);
               break;
           case 3:
               displayWord(ones == 1 || ones == 8 ? WORD_MTRENT : WORD_MTRENTA, color);
               break;
           case 4:
               displayWord(ones == 1 || ones == 8 ? WORD_MQUARANT : WORD_MQUARANTA, color);
               break;
           case 5:
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

void showSecondsM(uint8_t seconds, CRGB color) {
    if(currentMinute >= 1) {
    uint8_t segment = seconds / 10;
    
      for(uint8_t i = 0; i < 6; i++) {
          if(i <= segment) {
              if(i == segment) {
                  uint8_t hue = beat8(60);
                  leds[250 + i] = (seconds % 2 == 0) ? 
                      CHSV(hue, 255, 255) : color;
              } else {
                  leds[250 + i] = color;
              }
          } else {
              leds[250 + i] = color;
          }
      }
    }
}

void showSeconds(uint8_t seconds, CRGB color) {
    if(currentBlink == 1) {
        uint8_t hue = beat8(60);
        leds[116] = (seconds % 2 == 0) ? 
        CHSV(hue, 255, 255) : color;
    } else {
        leds[116] = color; 
    }
}

void loop() {
   static uint32_t lastUpdate = 0;
   static uint32_t lastButtonCheck = 0;
   static uint32_t lastMatrixUpdate = 0;
   uint32_t currentMillis = millis();
  
   if(WiFi.status() == WL_CONNECTED) {
       ArduinoOTA.handle();
       events();
       espalexa.loop();
   }
  
   if(currentMillis - lastButtonCheck > 50) {
       checkButtons();
       lastButtonCheck = currentMillis;
   }

   if(WiFi.status() == WL_CONNECTED && currentMillis - lastUpdate > 1000) {
       currentHour = myTZ.hour();
       currentMinute = myTZ.minute();
       currentSecond = myTZ.second();
       
       clockColors.brightness = (currentHour < 7 || currentHour >= 19) ? 
                               BRIGHTNESS_NIGHT : BRIGHTNESS_DAY;
       
       lastUpdate = currentMillis;
   }

   if((myTZ.hour() < 7) || (myTZ.hour() >= 19)) {
       intBrightness = NOTTE;
   } else {
       intBrightness = GIORNO;
   }
   FastLED.setBrightness(intBrightness);
   FastLED.show(); 

   if (alexaOff==0) {
    if(currentMode == MODE_MATRIX) {
        if(currentMillis - lastMatrixUpdate > 16) {
            updateMatrix();
            lastMatrixUpdate = currentMillis;
        }
    } else if(currentMode == MODE_MATRIX2) {
        if(currentMillis - lastMatrixUpdate > 16) {
            updateMatrix2();
            lastMatrixUpdate = currentMillis;
        }
    } else if(currentMode == MODE_TRON) {
        updateTronEffect();
    } else if(currentMode == MODE_MOTO) {
        updateMotoEffect();
    } else if(currentMode == MODE_GALAGA) {
        updateGalagaEffect();
    } else if(currentMode == MODE_PACMAN) {
        updatePacmanEffect();
    } else {
        if(currentHour != prevHour || currentMinute != prevMinute) {
            updateDisplay();
            prevHour = currentHour;
            prevMinute = currentMinute;
        } else {
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
   static uint32_t bothPressStart = 0;
   const uint32_t DEBOUNCE_TIME = 300;
   const uint32_t RESET_WIFI_TIME = 5000;

  #if BUTTON_LOGIC_INVERTED == 1
     bool isModeButtonPressed = !digitalRead(BUTTON_MODE);
     bool isColorButtonPressed = !digitalRead(BUTTON_SEC);
   #else
     bool isModeButtonPressed = digitalRead(BUTTON_MODE);
     bool isColorButtonPressed = digitalRead(BUTTON_SEC);
   #endif

   if(isModeButtonPressed && isColorButtonPressed) {
       if(bothPressStart == 0) {
           bothPressStart = millis();
           Serial.println("Iniziato conteggio per reset WiFi...");
       }
       
       if(millis() - bothPressStart >= RESET_WIFI_TIME) {
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
       bothPressStart = 0;
       
       if(isModeButtonPressed) {
           if(millis() - lastBothPress > DEBOUNCE_TIME) {
               currentPreset = (currentPreset + 1) % 18;  // Aggiornato a 18 preset (0-17) per includere Pacman
               applyPreset(currentPreset);
               EEPROM.write(EEPROM_PRESET_ADDR, currentPreset);
               EEPROM.commit();
               lastBothPress = millis();
           }
       }
       if(isColorButtonPressed) {
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
       case 0:
           currentMode = MODE_SLOW;
           clockColors.current = CRGB(random8(), random8(), random8());
           break;
           
       case 1:
           currentMode = MODE_FADE;
           clockColors.current = CRGB::Blue;
           break;
           
       case 2:
           currentMode = MODE_MATRIX2;
           matrixWordColor = CRGB::Green;
           matrix2State.needsReset = true;
           break;
           
       case 3:
           currentMode = MODE_SLOW;
           clockColors.current = CRGB(60,0,255);
           break;
           
       case 4:
           currentMode = MODE_MATRIX2;
           matrixWordColor = CRGB::Blue;
           matrix2State.needsReset = true;
           break;
           
       case 5:
           currentMode = MODE_MATRIX;
           matrixWordColor = CRGB::Yellow;
           break;
           
       case 6:
           currentMode = MODE_FADE;
           clockColors.current = CRGB::Green;
           break;
           
       case 7:
           currentMode = MODE_SLOW;
           clockColors.current = CRGB(255,165,0);
           break;
           
       case 8:
           currentMode = MODE_MATRIX;
           matrixWordColor = CRGB::Cyan;
           break;
           
       case 9:
           currentMode = MODE_FADE;
           clockColors.current = CRGB(255,20,147);
           break;
           
       case 10:
           currentMode = MODE_FAST;
           clockColors.current = CRGB::Yellow;
           break;
           
       case 11:
           currentMode = MODE_FAST;
           clockColors.current = CRGB::White;
           break;
           
       case 12:
           currentMode = MODE_FAST;
           clockColors.current = CRGB(0,255,255);
           break;
           
       case 13:
           currentMode = MODE_TRON;
           tronCycleStart = 0;
           tronEffectActive = true;
           memset(tronIntensity, 0, sizeof(tronIntensity));
           break;
           
       case 14:
           currentMode = MODE_TRON;
           tronCycleStart = 0;
           tronEffectActive = true;
           memset(tronIntensity, 0, sizeof(tronIntensity));
           break;
           
       case 15:
           currentMode = MODE_MOTO;
           initMotoEffect();
           break;

       case 16:
           currentMode = MODE_GALAGA;
           galagaTextHue = random8();
           initGalagaEffect();
           break;

       case 17:
           currentMode = MODE_PACMAN;
           initPacmanEffect();
           break;
           
       default:
           currentMode = MODE_SLOW;
           clockColors.current = CRGB(random8(), random8(), random8());
           break;
   }
   
   FastLED.setBrightness(intBrightness);
   updateDisplay();
}

void initMatrix() {
   memset(targetPixels, 0, sizeof(targetPixels));
   memset(activePixels, 0, sizeof(activePixels));
   
   displayWordToTarget(WORD_SONO_LE);
   
   uint8_t hour12 = currentHour % 12;
   const uint8_t* hourWord = (const uint8_t*)pgm_read_ptr(&HOUR_WORDS[hour12]);
   displayWordToTarget(hourWord);
   
   if(currentMinute > 0) {
       displayWordToTarget(WORD_E);
       displayMinutesToTarget(currentMinute);
       displayWordToTarget(WORD_MINUTI);
   }
   
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

   if(currentHour != lastHour || currentMinute != lastMinute) {
       FastLED.clear();
       memset(targetPixels, 0, sizeof(targetPixels));
       memset(activePixels, 0, sizeof(activePixels));
       
       displayWordToTarget(WORD_SONO_LE);
       const uint8_t* hourWord = (const uint8_t*)pgm_read_ptr(&HOUR_WORDS[currentHour]);
       displayWordToTarget(hourWord);
       
       if(currentMinute > 0) {
           displayWordToTarget(WORD_E);
           displayMinutesToTarget(currentMinute);
           displayWordToTarget(WORD_MINUTI);
       }
       
       for(int i = 0; i < NUM_DROPS; i++) {
           drops[i].isMatrix2 = false;
           initDrop(drops[i]);
       }
       
       lastHour = currentHour;
       lastMinute = currentMinute;
   }
   
   FastLED.clear();
   
   for(uint8_t i = 0; i < NUM_DROPS; i++) {
       Drop &drop = drops[i];
       if(!drop.active) continue;
       
       uint16_t pos = ((int)drop.y * MATRIX_WIDTH) + drop.x;
       
       if(drop.y >= 0 && drop.y < MATRIX_HEIGHT && pos < NUM_LEDS) {
           if(targetPixels[pos] && !activePixels[pos]) {
               activePixels[pos] = true;
               leds[pos] = matrixWordColor;
           } 
           else if(!targetPixels[pos]) {
               uint8_t intensity = 255 - ((int)drop.y * 16);
               leds[pos] = CRGB(0, intensity, 0);
               
               for(int trail = 1; trail <= MATRIX_TRAIL_LENGTH; trail++) {
                   int trailPos = pos - (MATRIX_WIDTH * trail);
                   if(trailPos >= 0 && !targetPixels[trailPos]) {
                       leds[trailPos] = CRGB(0, intensity/(trail*2), 0);
                   }
               }
           }
       }
       
       drop.y += drop.speed;
       
       if(drop.y >= MATRIX_HEIGHT) {
           drop.isMatrix2 = false;
           initDrop(drop);
       }
   }
   
   for(uint16_t i = 0; i < NUM_LEDS; i++) {
       if(targetPixels[i] && activePixels[i]) {
           leds[i] = matrixWordColor;
       }
   }
   
   FastLED.show();
}

void displayMinutesToTarget(uint8_t minutes) {
   if(minutes <= 0) return;

   if(minutes <= 19) {
       const uint8_t* minuteWord = (const uint8_t*)pgm_read_ptr(&MINUTE_WORDS[minutes]);
       displayWordToTarget(minuteWord);
   } else {
       uint8_t tens = (minutes / 10) - 2;
       uint8_t ones = minutes % 10;
       const MinuteTens* tensWords = &TENS_WORDS[tens];
       
       const uint8_t* decinaWord;
       if(ones == 8 || (ones == 1 && minutes >= 21)) {
           decinaWord = (const uint8_t*)pgm_read_ptr(&tensWords->truncated);
       } else {
           decinaWord = (const uint8_t*)pgm_read_ptr(&tensWords->normal);
       }
       
       displayWordToTarget(decinaWord);
       
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
   Serial.print("Comando Alexa ricevuto - Brightness: ");
   Serial.print(brightness);
   Serial.print(", RGB: ");
   Serial.println(rgb, HEX);

   if (brightness == 0) {
       alexaOff = 1;  
       FastLED.clear();
       FastLED.show();
       Serial.println("Alexa: ORAQUADRA spento");
       return;
   } else {
       alexaOff = 0;
       Serial.println("Alexa: ORAQUADRA acceso");
   } 

   CRGB newColor;
   newColor.r = (rgb >> 16) & 0xFF;
   newColor.g = (rgb >> 8) & 0xFF;
   newColor.b = rgb & 0xFF;

   clockColors.current = newColor;
   clockColors.brightness = brightness;
   
   FastLED.setBrightness(intBrightness);
 
   if(currentMode == MODE_MATRIX || currentMode == MODE_MATRIX2) {
       matrixWordColor = newColor;
   }

   updateDisplay();
   Serial.println("Colore Alexa applicato con successo");
}

void paintWordSlow(const uint8_t arrWord[], CRGB color) {
   uint8_t i = 0;
   uint8_t pixel;
   
   while((pixel = pgm_read_byte(&arrWord[i])) != 8 && i < NUM_LEDS) {
       leds[pixel] = color;
       FastLED.show();
       delay(500);
       yield();
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
       yield();
       i++;
   }
}

void updateMatrix2() {
    static uint8_t lastHour = 255;
    static uint8_t lastMinute = 255;
    
    if(currentHour != lastHour || currentMinute != lastMinute || matrix2State.needsReset) {
        FastLED.clear();
        memset(targetPixels, 0, sizeof(targetPixels));
        memset(activePixels, 0, sizeof(activePixels));
        
        displayWordToTarget(WORD_SONO_LE);
        const uint8_t* hourWord = (const uint8_t*)pgm_read_ptr(&HOUR_WORDS[currentHour]);
        displayWordToTarget(hourWord);
        
        if(currentMinute > 0) {
            displayWordToTarget(WORD_E);
            displayMinutesToTarget(currentMinute);
            displayWordToTarget(WORD_MINUTI);
        }
        
        for(int i = 0; i < NUM_DROPS; i++) {
            drops[i].isMatrix2 = true;
            initDrop(drops[i]);
        }
        
        lastHour = currentHour;
        lastMinute = currentMinute;
        matrix2State.completed = false;
        matrix2State.needsReset = false;
    }

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

    for(uint8_t i = 0; i < NUM_DROPS; i++) {
        Drop &drop = drops[i];
        if(!drop.active) continue;
        
        uint16_t pos = ((int)drop.y * MATRIX_WIDTH) + drop.x;
        
        if(drop.y >= 0 && drop.y < MATRIX_HEIGHT && pos < NUM_LEDS) {
            if(targetPixels[pos] && !activePixels[pos]) {
                activePixels[pos] = true;
                leds[pos] = matrixWordColor;
            } 
            else if(!targetPixels[pos]) {
                uint8_t intensity = 255 - ((int)drop.y * 16);
                leds[pos] = CRGB(0, intensity, 0);
                
                for(int trail = 1; trail <= MATRIX_TRAIL_LENGTH; trail++) {
                    int trailPos = pos - (MATRIX_WIDTH * trail);
                    if(trailPos >= 0 && !targetPixels[trailPos]) {
                        leds[trailPos] = CRGB(0, intensity/(trail*2), 0);
                    }
                }
            }
        }
        
        drop.y += drop.speed;
        
        if(drop.y >= MATRIX_HEIGHT) {
            if(!matrix2State.completed) {
                drop.isMatrix2 = true;
                initDrop(drop);
            } else {
                drop.active = false;
            }
        }
    }

    for(uint16_t i = 0; i < NUM_LEDS; i++) {
        if(targetPixels[i] && !activePixels[i]) {
            allTargetPixelsActive = false;
            break;
        }
    }

    if(allTargetPixelsActive && !matrix2State.completed) {
        matrix2State.completed = true;
        matrix2State.completionTime = millis();
    }

    for(uint16_t i = 0; i < NUM_LEDS; i++) {
        if(targetPixels[i] && activePixels[i]) {
            leds[i] = matrixWordColor;
        }
    }

    FastLED.show();
}

void updateTronEffect() {
    static uint8_t trailHue = 0;
    static uint8_t cycleCounter = 0;
    
    uint32_t currentMillis = millis();
    if(tronCycleStart == 0) {
        tronCycleStart = currentMillis;
    }
    
    uint32_t cycleElapsed = currentMillis - tronCycleStart;
    
    if(tronEffectActive && cycleElapsed > TRON_EFFECT_DURATION) {
        tronEffectActive = false;
        tronCycleStart = currentMillis;
        FastLED.clear();
        showCurrentTime();
        return;
    } else if(!tronEffectActive && cycleElapsed > TEXT_ONLY_DURATION) {
        tronEffectActive = true;
        tronCycleStart = currentMillis;
        for(uint16_t i = 0; i < NUM_LEDS; i++) {
            tronIntensity[i] = 0;
        }
    }
    
    if(currentMillis - lastTronUpdate < TRON_UPDATE_INTERVAL) return;
    lastTronUpdate = currentMillis;
    
    FastLED.clear();
    
    if(tronEffectActive) {
        for(uint16_t i = 0; i < NUM_LEDS; i++) {
            if(tronIntensity[i] > 0) {
                tronIntensity[i] -= 10;
                if(tronIntensity[i] < 0) tronIntensity[i] = 0;
            }
        }
        
        cycleCounter++;
        if(cycleCounter >= 3) {
            cycleCounter = 0;
            trailHue += 3;
            
            for(uint8_t i = 0; i < 6; i++) {
                uint8_t startX = random8(MATRIX_WIDTH);
                uint8_t startY = random8(MATRIX_HEIGHT);
                
                uint8_t vertLength = random8(2, 6);
                for(uint8_t len = 0; len < vertLength; len++) {
                    if(startY + len < MATRIX_HEIGHT) {
                        uint16_t pos = ((startY + len) * MATRIX_WIDTH) + startX;
                        tronIntensity[pos] = 255;
                    }
                }
                
                uint8_t horzLength = random8(2, 6);
                for(uint8_t len = 0; len < horzLength; len++) {
                    if(startX + len < MATRIX_WIDTH) {
                        uint16_t pos = (startY * MATRIX_WIDTH) + (startX + len);
                        tronIntensity[pos] = 255;
                    }
                }
                
                if(random8(100) < 30) {
                    uint8_t diagLength = random8(2, 4);
                    for(uint8_t len = 0; len < diagLength; len++) {
                        if(startX + len < MATRIX_WIDTH && startY + len < MATRIX_HEIGHT) {
                            uint16_t pos = ((startY + len) * MATRIX_WIDTH) + (startX + len);
                            tronIntensity[pos] = 200;
                        }
                    }
                }
            }
        }
        
        for(uint16_t i = 0; i < NUM_LEDS; i++) {
            if(tronIntensity[i] > 0) {
                uint8_t hue = trailHue + (i % 24);
                leds[i] = CHSV(hue, 255, tronIntensity[i]);
            }
        }
        
        showCurrentTime();
        
    } else {
        showCurrentTime();
    }
    
    FastLED.show();
}

void showCurrentTime() {
    CRGB textColor = CRGB::White;
    
    displayWord(WORD_SONO_LE, textColor);
    
    const uint8_t* hourWord = (const uint8_t*)pgm_read_ptr(&HOUR_WORDS[currentHour]);
    displayWord(hourWord, textColor);
    
    if(currentMinute > 0) {
        displayWord(WORD_E, textColor);
        showMinutes(currentMinute, textColor);
        displayWord(WORD_MINUTI, textColor);
    }
    
    if(currentMinute > 0 && currentBlink == 1) {
        showSeconds(currentSecond, textColor);
    }
}

// Funzioni per effetto MOTO
void initMotoEffect() {
    FastLED.clear();
    motoCurrentPosition = 0;
    motoEffectActive = true;
    motoCompletionTime = 0;
    motoTrailHue = 0;
    motoTextHue = random8();
    motoTextColor = CHSV(motoTextHue, 255, 255);
    
    // Reset array pixel della scritta
    memset(motoTextPixels, 0, sizeof(motoTextPixels));
    
    for (int i = 0; i < MOTO_TRAIL_LENGTH; i++) {
        motoTrailColors[i] = CRGB::Black;
    }
    
    memset(targetPixels, 0, sizeof(targetPixels));
    displayWordToTarget(WORD_SONO_LE);
    
    const uint8_t* hourWord = (const uint8_t*)pgm_read_ptr(&HOUR_WORDS[currentHour]);
    displayWordToTarget(hourWord);
    
    if (currentMinute > 0) {
        displayWordToTarget(WORD_E);
        displayMinutesToTarget(currentMinute);
        displayWordToTarget(WORD_MINUTI);
    }
}

void updateMotoEffect() {
    uint32_t currentMillis = millis();
    
    // Gestione attesa dopo il completamento
    if (motoCurrentPosition >= NUM_LEDS && currentMillis - motoCompletionTime < MOTO_WAIT_AFTER) {
        // Durante l'attesa, mostra solo la scritta dell'orario che è stata colorata
        FastLED.clear();
        for (uint16_t i = 0; i < NUM_LEDS; i++) {
            if (motoTextPixels[i]) {
                leds[i] = motoTextColor;
            }
        }
        FastLED.show();
        return;
    }
    
    // Reset per nuovo ciclo
    if (motoCurrentPosition >= NUM_LEDS && currentMillis - motoCompletionTime >= MOTO_WAIT_AFTER) {
        motoCurrentPosition = 0;
        motoCompletionTime = 0;
        memset(motoTextPixels, 0, sizeof(motoTextPixels));
        
        // Cambia colore della scritta ad ogni ciclo
        motoTextHue += 42;
        motoTextColor = CHSV(motoTextHue, 255, 255);
        
        // Ricalcola i target pixel per l'orario corrente
        memset(targetPixels, 0, sizeof(targetPixels));
        displayWordToTarget(WORD_SONO_LE);
        
        const uint8_t* hourWord = (const uint8_t*)pgm_read_ptr(&HOUR_WORDS[currentHour]);
        displayWordToTarget(hourWord);
        
        if (currentMinute > 0) {
            displayWordToTarget(WORD_E);
            displayMinutesToTarget(currentMinute);
            displayWordToTarget(WORD_MINUTI);
        }
    }
    
    // Aggiornamento periodico
    if (currentMillis - lastMotoUpdate < MOTO_SPEED) return;
    lastMotoUpdate = currentMillis;
    
    // Aggiorna i colori della scia (effetto arcobaleno rotante)
    for (int i = MOTO_TRAIL_LENGTH - 1; i > 0; i--) {
        motoTrailColors[i] = motoTrailColors[i - 1];
    }
    motoTrailColors[0] = CHSV(motoTrailHue, 255, 255);
    motoTrailHue += 8;
    
    // Cancella tutto
    FastLED.clear();
    
    // Disegna la scia corrente
    if (motoCurrentPosition < NUM_LEDS) {
        for (int i = 0; i < MOTO_TRAIL_LENGTH; i++) {
            uint16_t pos = motoCurrentPosition - i;
            if (pos < NUM_LEDS) {
                // Calcola l'intensità (più luminosa davanti, più scura dietro)
                uint8_t intensity = 255 - (i * (255 / MOTO_TRAIL_LENGTH));
                CRGB trailColor = motoTrailColors[i];
                trailColor.nscale8(intensity);
                leds[pos] = trailColor;
                
                // Se questo pixel fa parte della scritta dell'orario, coloralo e segnalo come colorato
                if (targetPixels[pos]) {
                    motoTextPixels[pos] = true;
                }
            }
        }
        
        motoCurrentPosition++;
        
        // Se abbiamo completato la matrice, imposta il tempo di completamento
        if (motoCurrentPosition >= NUM_LEDS && motoCompletionTime == 0) {
            motoCompletionTime = currentMillis;
        }
    }
    
    // Disegna i pixel della scritta che sono stati colorati (rimangono accesi)
    for (uint16_t i = 0; i < NUM_LEDS; i++) {
        if (motoTextPixels[i]) {
            leds[i] = motoTextColor;
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
   
   hue++;
}

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