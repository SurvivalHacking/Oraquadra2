// OraQuadra V1.2.2  -  By Davide Gatti SURVIVAL HACKING  www.survivalhacking.it
// interfaccia web e alcuni effetti aggiunti da Marco Prunca
// Sketch completamente riscritto e ottimizzato per famiglia esp32 by Paolo Sambi
// Nuovo quadrante by Luca Beltramio
//
// Pulsante 1 = per lampeggio secondi sulla lettera E
// Pulsante 2 = per la cambio preset
// Pulsante 1 + Pulsante 2 premuti per 5 secondi = reset configurazione WiFi
// addon: setup ora automatica
//        effetto matrix
//        wifimanager ORAQUADRA_AP 192.168.4.1
//        ottimizzazione creazione parole per ora e minuti
//        interfaccia web per controllo completo
// 
// V1.0.1 - 21/02/2025
// Rimappatura nuovo quadrante e aggiunta gestione nuove WORD
// Sistemazione troncature minuti quando è il primo minuto della decade   21 = VENT UN / 31 = TRENT UN   
//
// V1.0.2 - 20/05/2025
// aggiunta la possibilità di usare pulsanti normali oltre a quelli touch
// BUTTON_LOGIC_INVERTED = 1 per pulsanti normali NA verso GND
// BUTTON_LOGIC_INVERTED = 0 per pulsanti touch
//
// V1.1.0 - 1/11/2025 by Marco Prunca
// aggiunto effetto MOTO con scia arcobaleno 
// MODIFICATO: la scia colora le scritte dell'orario man mano che passa
// aggiunto effetto GALAGA - sparo dal basso che colpisce le lettere
//
// V1.2.1 - 20/11/2025 - By Marco Prunca
// aggiunto effetto PACMAN - Pacman che insegue fantasmi colorando le scritte
// aggiunta interfaccia web per il controllo completo
//
// V1.2.2 - 22/11/2025 - By Davide Gatti
// aggiunto effetto DIGITALE - orologio digitale su schermo oraquadra
// aggiunta modalità giorno/sera/notte per la regolazione della luminosità su tre fasce orarie
// aggiunte scritte scorrevoli per informare l'utente sullo stato della connettività WIFI
// sistemati alcuni bug relativi a vari effetti e memorizzazione parametri di configurazione
// Sitemato grazie a Alessndro Spagnoletti e Paolo Sambinello l'effetto matrix che ora è corretto !!!!
//
//
// V1.2.3 - 27/11/2025 - By Marco Prunca
// Sistemati vari bug che facevano funzionare male la parte WEB
//
// V1.2.4 - 02/12-2025 - by Leotech
// Aggiunta funzione Testo scorrevole da web server
// Aggiunta possibilità di attivare/disattivare il quadrante digitale che si attiva ogni 6 sec per rendere visibile l'ora per postazioni più lontane. 
// L'attivazione/disattivazione del quadrante digitale che si alterna agli effetti selezionati può avvenire o da web server o mediante la pressione prolungata (6 sec) di uno dei 2 pulsanti. 
// Aggiunta palette di colori avanzata per selezionare molteplici più colori del testo scorrevole o delle parole di alcuni effetti.
// 
//
//
//
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
#include <EEPROM.h>      // https://github.com/jwrw/ESP_EEPROM 
#include <WiFiManager.h> // https://github.com/tzapu/WiFiManager
#include <WebServer.h>   // Per interfaccia web
#include <ArduinoJson.h> // Per interfaccia web

// Abilita la logica invertita per i pulsanti.
// Definisci BUTTON_LOGIC_INVERTED come 1 per pulsanti NA verso GND (pull-up, premi per GND).
// Definisci BUTTON_LOGIC_INVERTED come 0 per pulsanti touch (pull-down, premi per VCC).
#define BUTTON_LOGIC_INVERTED 0

// Configurazione ESP32-S3
//#if CONFIG_ARDUINO_RUNNING_CORE
//#undef CONFIG_ARDUINO_LOOP_STACK_SIZE
//#define CONFIG_ARDUINO_LOOP_STACK_SIZE 16384
//#endif

// Pin e configurazione LED ESP32C3
#define LED_PIN      5     // Pin per matrice LED
#define BUTTON_MODE  6     // Pulsante modi
#define BUTTON_SEC   7     // Pulsante lampeggio secondo

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
#define MODE_DIGITALE 9 // Nuova modalità DIGITALE
#define MODE_SCROLLTEXT 10 // Testo scorrevole custom via web
#define NUM_MODES   10  

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
#define GALAGA_SPEED 30         // Velocità dell'effetto (ms entre i frame)
#define GALAGA_WAIT_AFTER 5000  // Attesa dopo il completamento (5 secondi)
#define GALAGA_MAX_SHOTS 8      // Numero massimo di spari contemporanei
#define GALAGA_SHOT_LENGTH 3    // Lunghezza dello sparo

// Strutture e costanti per effetto PACMAN
#define PACMAN_SPEED 50           // Velocità dell'effetto (ms entre i frame) - RIPRISTINATA a 50
#define PACMAN_WAIT_AFTER 5000    // Attesa dopo il completamento (5 secondi)
#define NUM_GHOSTS 4              // Numero di fantasmi

// Strutture e costanti per EEPROM
#define EEPROM_SIZE 64
#define EEPROM_DIGITAL_OVERLAY_ADDR 40
#define EEPROM_CONFIGURED_MARKER 0x55
#define EEPROM_PRESET_ADDR 1
#define EEPROM_BLINK_ADDR 2
#define EEPROM_BRIGHTNESS_ADDR 9
#define EEPROM_COLOR_R_ADDR 10
#define EEPROM_COLOR_G_ADDR 11
#define EEPROM_COLOR_B_ADDR 12
#define EEPROM_COLOR_PRESET_ADDR 13
#define EEPROM_DAY_BRIGHTNESS_ADDR 14
#define EEPROM_NIGHT_BRIGHTNESS_ADDR 15
#define EEPROM_DAY_START_HOUR_ADDR 16
#define EEPROM_DAY_START_MINUTE_ADDR 17
#define EEPROM_NIGHT_START_HOUR_ADDR 18     
#define EEPROM_NIGHT_START_MINUTE_ADDR 19
#define EEPROM_SLEEP_START_HOUR_ADDR 20
#define EEPROM_SLEEP_START_MINUTE_ADDR 21
#define EEPROM_SLEEP_ENABLED_ADDR 22   
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

// ===== FORWARD DECLARATIONS =====
void displayWordToTarget(const uint8_t* word);
void displayMinutesToTarget(uint8_t minutes);
void initDrop(Drop &drop);
void initMotoEffect();
void initGalagaEffect();
void initPacmanEffect();
uint16_t xyToLED(uint8_t x, uint8_t y);

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
bool randomColor = {false};


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

// Variabili per effetto DIGITALE
// Variabili per effetto SCROLLTEXT
String customScrollText = "";
bool scrollTextInitialized = false;
int scrollTextOffset = 0;
int scrollTextTotalWidth = 0;
uint32_t lastScrollTextStep = 0;
uint16_t scrollTextSpeedMs = 50;

// Overlay digitale automatico (dopo fine effetto + 1.5s -> 2.5s digitale -> ritorna all'effetto precedente)
bool digitalOverlayActive = false;        // true quando siamo in MODE_DIGITALE per overlay
bool waitingForDigital = false;           // true quando l'effetto ha finito e stiamo aspettando 1.5s
uint32_t digitalOverlayStart = 0;         // quando è iniziato il digitale
uint32_t digitalDelayStart = 0;           // quando è finito l'effetto (inizio attesa 1.5s)
uint32_t lastEffectChangeTime = 0;        // quando è stato selezionato l'effetto corrente
uint8_t savedModeBeforeDigital = MODE_FAST; // effetto da ripristinare dopo overlay
uint32_t lastDigitaleUpdate = 0;
#define DIGITALE_UPDATE_INTERVAL 500      // Aggiorna ogni 500ms
#define DIGITALE_DELAY_AFTER_EFFECT 1500  // Attesa dopo fine effetto
#define DIGITALE_OVERLAY_DURATION 2500    // Durata visibile del digitale
#define DEFAULT_EFFECT_CYCLE_MS 6000      // Fallback per effetti continui senza "fine"

// Definizione array LED e controller
CRGB leds[NUM_LEDS];
CRGB matrixWordColor = CRGB::Blue;
Timezone myTZ;

// Server web per interfaccia
WebServer server(80);
bool webInitialized = false;

// Variabili globali stato
uint8_t currentMode = MODE_FAST;
uint8_t currentHour = 0;
uint8_t currentMinute = 0;
uint8_t currentSecond = 0;
uint8_t currentBlink = 0;
uint8_t prevHour = 255;
uint8_t prevMinute = 255;
uint8_t Presets;
bool displayOff = false;  // Sostituisce alexaOff
bool digitalOverlayEnabled = false;
uint8_t dayStartHour = 7;        // Ora inizio giorno default: 07:00
uint8_t dayStartMinute = 0;
uint8_t nightStartHour = 22;     // Ora inizio notte default: 22:00
uint8_t nightStartMinute = 0;
uint8_t sleepStartHour = 23;
uint8_t sleepStartMinute = 0;
bool sleepModeEnabled = false;
uint8_t dayBrightness = 204;     // 80% di 255
uint8_t nightBrightness = 26;    // 10% di 255

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
const uint8_t PROGMEM WORD_VENTI[] = {16,17,18,19,20,8};
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
const uint8_t PROGMEM WORD_MUN[] = {157,158,159,8};
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
       presetIndex = (presetIndex + 1) % 6; // Modificato da 7 a 6 (rimosso viola)
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
   CRGB::Green,
   CRGB::Yellow,
   CRGB(255,165,0) // Arancione
};

ClockColors clockColors;


// Font 5x7 per caratteri - definito in PROGMEM per risparmiare RAM
const uint8_t PROGMEM font5x7[][5] = {
  {0x00, 0x00, 0x00, 0x00, 0x00}, // SPACE (32)
  {0x00, 0x00, 0x5F, 0x00, 0x00}, // ! (33)
  {0x00, 0x07, 0x00, 0x07, 0x00}, // " (34)
  {0x14, 0x7F, 0x14, 0x7F, 0x14}, // # (35)
  {0x24, 0x2A, 0x7F, 0x2A, 0x12}, // $ (36)
  {0x23, 0x13, 0x08, 0x64, 0x62}, // % (37)
  {0x36, 0x49, 0x55, 0x22, 0x50}, // & (38)
  {0x00, 0x05, 0x03, 0x00, 0x00}, // ' (39)
  {0x00, 0x1C, 0x22, 0x41, 0x00}, // ( (40)
  {0x00, 0x41, 0x22, 0x1C, 0x00}, // ) (41)
  {0x14, 0x08, 0x3E, 0x08, 0x14}, // * (42)
  {0x08, 0x08, 0x3E, 0x08, 0x08}, // + (43)
  {0x00, 0x50, 0x30, 0x00, 0x00}, // , (44)
  {0x08, 0x08, 0x08, 0x08, 0x08}, // - (45)
  {0x00, 0x60, 0x60, 0x00, 0x00}, // . (46)
  {0x20, 0x10, 0x08, 0x04, 0x02}, // / (47)
  {0x3E, 0x51, 0x49, 0x45, 0x3E}, // 0 (48)
  {0x00, 0x42, 0x7F, 0x40, 0x00}, // 1 (49)
  {0x42, 0x61, 0x51, 0x49, 0x46}, // 2 (50)
  {0x21, 0x41, 0x45, 0x4B, 0x31}, // 3 (51)
  {0x18, 0x14, 0x12, 0x7F, 0x10}, // 4 (52)
  {0x27, 0x45, 0x45, 0x45, 0x39}, // 5 (53)
  {0x3C, 0x4A, 0x49, 0x49, 0x30}, // 6 (54)
  {0x01, 0x71, 0x09, 0x05, 0x03}, // 7 (55)
  {0x36, 0x49, 0x49, 0x49, 0x36}, // 8 (56)
  {0x06, 0x49, 0x49, 0x29, 0x1E}, // 9 (57)
  {0x00, 0x36, 0x36, 0x00, 0x00}, // : (58)
  {0x00, 0x56, 0x36, 0x00, 0x00}, // ; (59)
  {0x08, 0x14, 0x22, 0x41, 0x00}, // < (60)
  {0x14, 0x14, 0x14, 0x14, 0x14}, // = (61)
  {0x00, 0x41, 0x22, 0x14, 0x08}, // > (62)
  {0x02, 0x01, 0x51, 0x09, 0x06}, // ? (63)
  {0x32, 0x49, 0x79, 0x41, 0x3E}, // @ (64)
  {0x7E, 0x11, 0x11, 0x11, 0x7E}, // A (65)
  {0x7F, 0x49, 0x49, 0x49, 0x36}, // B (66)
  {0x3E, 0x41, 0x41, 0x41, 0x22}, // C (67)
  {0x7F, 0x41, 0x41, 0x22, 0x1C}, // D (68)
  {0x7F, 0x49, 0x49, 0x49, 0x41}, // E (69)
  {0x7F, 0x09, 0x09, 0x09, 0x01}, // F (70)
  {0x3E, 0x41, 0x49, 0x49, 0x7A}, // G (71)
  {0x7F, 0x08, 0x08, 0x08, 0x7F}, // H (72)
  {0x00, 0x41, 0x7F, 0x41, 0x00}, // I (73)
  {0x20, 0x40, 0x41, 0x3F, 0x01}, // J (74)
  {0x7F, 0x08, 0x14, 0x22, 0x41}, // K (75)
  {0x7F, 0x40, 0x40, 0x40, 0x40}, // L (76)
  {0x7F, 0x02, 0x0C, 0x02, 0x7F}, // M (77)
  {0x7F, 0x04, 0x08, 0x10, 0x7F}, // N (78)
  {0x3E, 0x41, 0x41, 0x41, 0x3E}, // O (79)
  {0x7F, 0x09, 0x09, 0x09, 0x06}, // P (80)
  {0x3E, 0x41, 0x51, 0x21, 0x5E}, // Q (81)
  {0x7F, 0x09, 0x19, 0x29, 0x46}, // R (82)
  {0x46, 0x49, 0x49, 0x49, 0x31}, // S (83)
  {0x01, 0x01, 0x7F, 0x01, 0x01}, // T (84)
  {0x3F, 0x40, 0x40, 0x40, 0x3F}, // U (85)
  {0x1F, 0x20, 0x40, 0x20, 0x1F}, // V (86)
  {0x3F, 0x40, 0x38, 0x40, 0x3F}, // W (87)
  {0x63, 0x14, 0x08, 0x14, 0x63}, // X (88)
  {0x07, 0x08, 0x70, 0x08, 0x07}, // Y (89)
  {0x61, 0x51, 0x49, 0x45, 0x43}, // Z (90)
};

// Funzione per ottenere un carattere dal font
void getCharBitmap(char c, uint8_t* bitmap) {
  if (c >= 32 && c <= 90) {
    for (int i = 0; i < 5; i++) {
      bitmap[i] = pgm_read_byte(&font5x7[c - 32][i]);
    }
  } else {
    // Carattere non supportato - spazio vuoto
    for (int i = 0; i < 5; i++) {
      bitmap[i] = 0x00;
    }
  }
}

// Funzione per disegnare un carattere sulla matrice in una posizione specifica
void drawChar5x7(uint8_t x, uint8_t y, char c, CRGB color) {
  uint8_t charBitmap[5];
  getCharBitmap(c, charBitmap);
  
  for (int col = 0; col < 5; col++) {
    int screenX = x + col;
    if (screenX >= 0 && screenX < MATRIX_WIDTH) {
      for (int row = 0; row < 7; row++) {
        if (charBitmap[col] & (1 << row)) {
          int screenY = y + row;
          if (screenY >= 0 && screenY < MATRIX_HEIGHT) {
            uint16_t ledPos = xyToLED(screenX, screenY);
            if (ledPos < NUM_LEDS) {
              leds[ledPos] = color;
            }
          }
        }
      }
    }
  }
}

// Funzione per disegnare una cifra centrata in una area
void drawDigitCentered(uint8_t x, uint8_t y, uint8_t digit, CRGB color) {
  char digitChar = '0' + digit;
  drawChar5x7(x, y, digitChar, color);
}

// Funzione per disegnare i due punti tra ore e minuti
void drawColon(uint8_t x, uint8_t y, CRGB color, bool blink) {
  // I due punti sono composti da 2 pixel uno sopra e uno sotto
  if (!blink || (millis() / 500) % 2 == 0) {
    uint16_t topPos = xyToLED(x, y + 2);
    uint16_t bottomPos = xyToLED(x, y + 4);
    if (topPos < NUM_LEDS) leds[topPos] = color;
    if (bottomPos < NUM_LEDS) leds[bottomPos] = color;
  }
}

// Funzione per convertire un pixel (x,y) in posizione LED sulla matrice
uint16_t xyToLED(uint8_t x, uint8_t y) {
  // La matrice è mappata in serpentina, ma invertita rispetto allo standard
  if (y % 2 == 0) {
    // Riga pari: da destra a sinistra
    return y * MATRIX_WIDTH + (MATRIX_WIDTH - 1 - x);
  } else {
    // Riga dispari: da sinistra a destra
    return y * MATRIX_WIDTH + x;
  }
}

// Funzione per visualizzare testo scorrevole
void scrollText(String text, CRGB color, uint16_t scrollSpeed) {
  text.toUpperCase(); // Converti tutto in maiuscolo
  
  // Aggiungi spazi all'inizio e alla fine per l'effetto di entrata/uscita
  text = "    " + text + "    ";
  
  int textLength = text.length();
  int totalWidth = textLength * 6; // 5 pixel per carattere + 1 di spaziatura
  
  // Scorri il testo da destra a sinistra
  for (int offset = MATRIX_WIDTH; offset > -totalWidth; offset--) {
    FastLED.clear();
    
    // Disegna ogni carattere
    for (int charIndex = 0; charIndex < textLength; charIndex++) {
      char c = text.charAt(charIndex);
      uint8_t charBitmap[5];
      getCharBitmap(c, charBitmap);
      
      int charX = offset + (charIndex * 6);
      
      // Disegna il carattere se è visibile
      if (charX + 5 >= 0 && charX < MATRIX_WIDTH) {
        for (int col = 0; col < 5; col++) {
          int screenX = charX + col;
          
          if (screenX >= 0 && screenX < MATRIX_WIDTH) {
            for (int row = 0; row < 7; row++) {
              if (charBitmap[col] & (1 << row)) {
                // Centra verticalmente (matrice 16 pixel, font 7 pixel)
                int screenY = row + 4; // Offset di 4 pixel dall'alto
                if (screenY < MATRIX_HEIGHT) {
                  uint16_t ledPos = xyToLED(screenX, screenY);
                  leds[ledPos] = color;
                }
              }
            }
          }
        }
      }
    }
    
    FastLED.show();
    delay(scrollSpeed);
    yield(); // Permette al WiFi di gestire le sue operazioni
  }
}

// Versione NON bloccante del testo scorrevole per modalità custom
void initScrollText() {
  String t = customScrollText;
  if (t.length() == 0) t = " ";
  t.toUpperCase();
  // padding per entrata/uscita
  t = "    " + t + "    ";
  scrollTextTotalWidth = t.length() * 6;
  scrollTextOffset = MATRIX_WIDTH;
  scrollTextInitialized = true;
}

void resetScrollText() { /* reset interno testo scorrevole, se necessario */ }

void updateScrollText() {
  if (!scrollTextInitialized) initScrollText();
  if (millis() - lastScrollTextStep < scrollTextSpeedMs) return;
  lastScrollTextStep = millis();

  String t = customScrollText;
  if (t.length() == 0) t = " ";
  t.toUpperCase();
  t = "    " + t + "    ";

  FastLED.clear();
  int textLength = t.length();
  for (int charIndex = 0; charIndex < textLength; charIndex++) {
    char c = t.charAt(charIndex);
    uint8_t charBitmap[5];
    getCharBitmap(c, charBitmap);
    int charX = scrollTextOffset + (charIndex * 6);
    if (charX + 5 >= 0 && charX < MATRIX_WIDTH) {
      for (int col = 0; col < 5; col++) {
        int screenX = charX + col;
        if (screenX >= 0 && screenX < MATRIX_WIDTH) {
          for (int row = 0; row < 7; row++) {
            if (charBitmap[col] & (1 << row)) {
              int screenY = row + 4; // riga centrale come nel WiFi
              if (screenY >= 0 && screenY < MATRIX_HEIGHT) {
                uint16_t ledPos = xyToLED(screenX, screenY);
                if (ledPos < NUM_LEDS) leds[ledPos] = clockColors.getColor();
              }
            }
          }
        }
      }
    }
  }

  FastLED.show();
  scrollTextOffset--;
  if (scrollTextOffset < -scrollTextTotalWidth) {
    scrollTextOffset = MATRIX_WIDTH; // ciclo continuo
  }
}




// =============================================================================
// FUNZIONI INTERFACCIA WEB
// =============================================================================

// Forward declarations aggiunte per stabilità compilazione
void applyPreset(uint8_t preset);
void checkButtons();
void updateMatrix();
void updateMatrix2();
void updateTronEffect();
void updateMotoEffect();
void updateScrollText();
void resetScrollText();
void initScrollText();
void handleSetScrollText();
void handleSetDigitalOverlay();
void showCurrentTime();

void setupWebServer() {
    server.on("/", HTTP_GET, handleRoot);
    server.on("/status", HTTP_GET, handleStatus);
    server.on("/setMode", HTTP_POST, handleSetMode);
    server.on("/setBlink", HTTP_POST, handleSetBlink);
    server.on("/setColor", HTTP_POST, handleSetColor);
    server.on("/setBrightness", HTTP_POST, handleSetBrightness);
    server.on("/setDayNightSettings", HTTP_POST, handleSetDayNightSettings);
    server.on("/setDigitalOverlay", HTTP_POST, handleSetDigitalOverlay);
    server.on("/setScrollText", HTTP_GET, handleSetScrollText);
    server.on("/power", HTTP_POST, handlePower);
    server.on("/reset", HTTP_POST, handleReset);
    server.onNotFound(handleNotFound);
    
    server.begin();
    webInitialized = true;
    Serial.println("Server web avviato su http://" + WiFi.localIP().toString());
   // Reset timer overlay digitale automatico ad ogni cambio preset
   lastEffectChangeTime = millis();
   digitalOverlayActive = false;
   scrollTextInitialized = false;
}

void handleRoot() {
    String html = R"rawliteral(
<!DOCTYPE html>
<html lang="it">
<head>
    <meta charset="UTF-8">
    <meta name="viewport" content="width=device-width, initial-scale=1.0">
    <title>OraQuadra2 Plus</title>
    <style>
        :root {
            --primary: #2196F3;
            --primary-dark: #1976D2;
            --secondary: #FF9800;
            --success: #4CAF50;
            --danger: #F44336;
            --dark: #333;
            --light: #f4f4f4;
        }
        
        * {
            margin: 0;
            padding: 0;
            box-sizing: border-box;
        }
        
        body {
            font-family: 'Segoe UI', Tahoma, Geneva, Verdana, sans-serif;
            background: #000000;
            color: var(--light);
            min-height: 100vh;
            padding: 20px;
        }
        
        .container {
            max-width: 1200px;
            margin: 0 auto;
            background: rgba(255, 255, 255, 0.1);
            backdrop-filter: blur(10px);
            border-radius: 15px;
            padding: 30px;
            box-shadow: 0 8px 32px rgba(0, 0, 0, 0.3);
            border: 1px solid rgba(255, 255, 255, 0.2);
        }
        
        header {
            text-align: center;
            margin-bottom: 30px;
            padding-bottom: 20px;
            border-bottom: 1px solid rgba(255, 255, 255, 0.2);
        }
        
        h1 {
            font-size: 2.5rem;
            margin-bottom: 10px;
            text-shadow: 2px 2px 4px rgba(0, 0, 0, 0.5);
            color: white;
        }
        
        .subtitle {
            font-size: 1.1rem;
            opacity: 0.9;
            color: #cccccc;
        }
        
        .status-card {
            background: rgba(255, 255, 255, 0.15);
            border-radius: 10px;
            padding: 20px;
            margin-bottom: 25px;
            display: flex;
            justify-content: space-between;
            align-items: center;
            flex-wrap: wrap;
            gap: 15px;
            border: 1px solid rgba(255, 255, 255, 0.1);
        }
        
        .status-item {
            display: flex;
            flex-direction: column;
            align-items: center;
            min-width: 120px;
        }
        
        .status-label {
            font-size: 0.9rem;
            opacity: 0.8;
            margin-bottom: 5px;
            color: #cccccc;
        }
        
        .status-value {
            font-size: 1.3rem;
            font-weight: bold;
        }
        
        .status-time {
            color: #FFD700 !important; /* Oro per l'orario */
        }
        
        .status-mode {
            color: #FF9800 !important; /* Arancione per la modalità */
        }
        
        .status-color {
            color: #4CAF50 !important; /* Verde per il colore */
        }
        
        .status-blink {
            color: #2196F3 !important; /* Blu per il lampeggio */
        }
        
        .status-brightness {
            color: #9C27B0 !important; /* Viola per la luminosità */
        }
        
        .status-power {
            color: #F44336 !important; /* Rosso per lo stato */
        }
        
        .control-section {
            background: rgba(255, 255, 255, 0.15);
            border-radius: 10px;
            padding: 25px;
            margin-bottom: 25px;
            border: 1px solid rgba(255, 255, 255, 0.1);
        }
        
        .day-night-settings {
            margin-top: 20px;
            padding: 20px;
            background: rgba(255, 255, 255, 0.1);
            border-radius: 8px;
            border: 1px solid rgba(255, 255, 255, 0.2);
        }
        
        .time-brightness-row {
            display: flex;
            gap: 20px;
            align-items: center;
            margin-bottom: 20px;
            flex-wrap: wrap;
        }
        
        .time-input {
            display: flex;
            align-items: center;
            gap: 10px;
            min-width: 250px;
        }
        
        .time-input label {
            color: white;
            font-weight: bold;
            min-width: 130px;
        }
        
        .time-input input[type="time"] {
            padding: 8px;
            border-radius: 4px;
            border: 1px solid rgba(255, 255, 255, 0.3);
            background: rgba(255, 255, 255, 0.1);
            color: white;
            font-size: 1rem;
        }
        
        .brightness-control-inline {
            flex: 1;
            min-width: 250px;
        }
        
        .brightness-control-inline label {
            display: block;
            color: white;
            margin-bottom: 5px;
            font-weight: bold;
        }
        
        .brightness-control-inline .brightness-slider {
            width: 100%;
        }
        
        @media (max-width: 768px) {
            .time-brightness-row {
                flex-direction: column;
                align-items: stretch;
            }
            
            .time-input {
                width: 100%;
            }
            
            .brightness-control-inline {
                width: 100%;
            }
        }

        h2 {
            font-size: 1.5rem;
            margin-bottom: 20px;
            display: flex;
            align-items: center;
            gap: 10px;
            color: white;
        }
        
        h2 i {
            color: var(--secondary);
        }
        
        .mode-grid {
            display: grid;
            grid-template-columns: repeat(auto-fill, minmax(200px, 1fr));
            gap: 15px;
            margin-bottom: 20px;
        }
        
        .mode-btn {
            background: rgba(255, 255, 255, 0.2);
            border: 1px solid rgba(255, 255, 255, 0.3);
            border-radius: 8px;
            padding: 15px;
            color: white;
            font-size: 1rem;
            cursor: pointer;
            transition: all 0.3s ease;
            text-align: center;
        }
        
        .mode-btn:hover {
            background: rgba(255, 255, 255, 0.3);
            transform: translateY(-2px);
        }
        
        .mode-btn.active {
            background: var(--primary);
            box-shadow: 0 4px 15px rgba(33, 150, 243, 0.4);
            border-color: var(--primary);
        }
        
        .color-grid {
            display: grid;
            grid-template-columns: repeat(auto-fill, minmax(80px, 1fr));
            gap: 10px;
            margin-bottom: 20px;
        }
        
        .color-btn {
            width: 60px;
            height: 60px;
            border: 2px solid rgba(255, 255, 255, 0.5);
            border-radius: 50%;
            cursor: pointer;
            transition: all 0.3s ease;
            box-shadow: 0 4px 8px rgba(0, 0, 0, 0.2);
        }
        
        .color-btn:hover {
            transform: scale(1.1);
            border-color: white;
        }
        
        .toggle-container {
            display: flex;
            align-items: center;
            gap: 20px;
            margin-bottom: 20px;
            width: 100%;
            padding: 10px 0;
        }
        
        .toggle {
            position: relative;
            display: inline-block;
            width: 80px;
            height: 40px;
            flex-shrink: 0;
        }
        
        .toggle input {
            opacity: 0;
            width: 0;
            height: 0;
        }
        
        .slider {
            position: absolute;
            cursor: pointer;
            top: 0;
            left: 0;
            right: 0;
            bottom: 0;
            background-color: #ccc;
            transition: .4s;
            border-radius: 34px;
        }
        
        .slider:before {
            position: absolute;
            content: "";
            height: 32px;
            width: 32px;
            left: 4px;
            bottom: 4px;
            background-color: white;
            transition: .4s;
            border-radius: 50%;
        }
        
        input:checked + .slider {
            background-color: var(--success);
        }
        
        input:checked + .slider:before {
            transform: translateX(40px);
        }
        
        .slider-text {
            font-size: 1.1rem;
            color: white;
            flex: 1;
            min-width: 200px;
            text-align: left;
            white-space: nowrap;
            overflow: hidden;
            text-overflow: ellipsis;
        }
        
        .action-buttons {
            display: flex;
            gap: 15px;
            flex-wrap: wrap;
            margin-top: 20px;
        }
        
        .btn {
            padding: 12px 25px;
            border: none;
            border-radius: 8px;
            font-size: 1rem;
            cursor: pointer;
            transition: all 0.3s ease;
            font-weight: bold;
            display: flex;
            align-items: center;
            gap: 8px;
        }
        
        .btn-primary {
            background: var(--primary);
            color: white;
        }
        
        .btn-primary:hover {
            background: var(--primary-dark);
            transform: translateY(-2px);
        }
        
        .btn-danger {
            background: var(--danger);
            color: white;
        }
        
        .btn-danger:hover {
            background: #d32f2f;
            transform: translateY(-2px);
        }
        
        .btn-warning {
            background: var(--secondary);
            color: white;
        }
        
        .btn-warning:hover {
            background: #F57C00;
            transform: translateY(-2px);
        }
        
        .btn-success {
            background: var(--success);
            color: white;
        }
        
        .btn-success:hover {
            background: #388E3C;
            transform: translateY(-2px);
        }
        
        .notification {
            position: fixed;
            top: 20px;
            right: 20px;
            padding: 15px 25px;
            border-radius: 8px;
            color: white;
            font-weight: bold;
            box-shadow: 0 4px 15px rgba(0, 0, 0, 0.2);
            transform: translateX(150%);
            transition: transform 0.3s ease;
            z-index: 1000;
        }
        
        .notification.show {
            transform: translateX(0);
        }
        
        .notification.success {
            background: var(--success);
        }
        
        .notification.error {
            background: var(--danger);
        }
        
        .brightness-control {
            margin-top: 15px;
        }
        
        .brightness-slider {
            width: 100%;
            margin: 10px 0;
        }
        
        .brightness-value {
            text-align: center;
            font-weight: bold;
            color: white;
        }
        
        .night-mode-settings {
            margin-top: 15px;
            padding: 15px;
            background: rgba(255, 255, 255, 0.1);
            border-radius: 8px;
            border: 1px solid rgba(255, 255, 255, 0.2);
        }
        
        .time-input {
            display: flex;
            align-items: center;
            gap: 10px;
            margin-bottom: 10px;
        }
        
        .time-input label {
            min-width: 80px;
            color: white;
        }
        
        .time-input input {
            padding: 8px;
            border-radius: 4px;
            border: 1px solid rgba(255, 255, 255, 0.3);
            background: rgba(255, 255, 255, 0.1);
            color: white;
        }
        
        @media (max-width: 768px) {
            .mode-grid {
                grid-template-columns: repeat(auto-fill, minmax(150px, 1fr));
            }
            
            .status-card {
                flex-direction: column;
                align-items: flex-start;
            }
            
            .status-item {
                flex-direction: row;
                justify-content: space-between;
                width: 100%;
            }
            
            .toggle-container {
                gap: 15px;
            }
            
            .toggle {
                width: 70px;
                height: 35px;
            }
            
            .slider:before {
                height: 27px;
                width: 27px;
            }
            
            input:checked + .slider:before {
                transform: translateX(35px);
            }
            
            .slider-text {
                font-size: 1rem;
                min-width: 150px;
                white-space: normal;
            }
        }
    </style>
</head>
<body>
    <div class="container">
        <header>
            <h1>OraQuadra2 Plus</h1>
            <p class="subtitle">Gestisci tutti gli effetti del orologio LED</p>
        </header>
        
        <div class="status-card">
            <div class="status-item">
                <span class="status-label">ORA ATTUALE</span>
                <span class="status-value status-time" id="currentTime">--:--:--</span>
            </div>
            <div class="status-item">
                <span class="status-label">MODALITÀ</span>
                <span class="status-value status-mode" id="currentMode">--</span>
            </div>
            <div class="status-item">
                <span class="status-label">COLORE</span>
                <span class="status-value status-color" id="currentColor">--</span>
            </div>
            <div class="status-item">
                <span class="status-label">LAMPEGGIO</span>
                <span class="status-value status-blink" id="currentBlink">--</span>
            </div>
            <div class="status-item">
                <span class="status-label">LUMINOSITÀ</span>
                <span class="status-value status-brightness" id="currentBrightness">--</span>
            </div>
            <div class="status-item">
                <span class="status-label">STATO</span>
                <span class="status-value status-power" id="currentPower">--</span>
            </div>
        </div>
        
        <div class="control-section">
            <h2>🎭 Modalità Visualizzazione</h2>
            <div class="mode-grid" id="modeButtons">
                <!-- I pulsanti delle modalità verranno generati dinamicamente -->
            </div>
        </div>
        
        <div class="control-section">
            <h2>🎨 Colori Predefiniti</h2>
            <div class="color-grid" id="colorButtons">
                <!-- I pulsanti dei colori verranno generati dinamicamente -->
            </div>
            <!-- Palette colori avanzata -->
            <div class="control-section">
                <h2>🎨 Palette Colori Avanzata</h2>
                <div style="display:flex;align-items:center;gap:20px;flex-wrap:wrap;">
                    <input type="color" id="advancedColorPicker" value="#ff8800"
                           style="width:80px;height:40px;border:none;background:transparent;cursor:pointer;">
                    <button class="btn btn-primary" onclick="applyAdvancedColor()">Applica</button>
                </div>
            </div>


        </div>

        <div class="control-section">
            <h2>⚙️ Impostazioni</h2>
            
            <div class="toggle-container">
                <label class="toggle">
                    <input type="checkbox" id="blinkToggle">
                    <span class="slider"></span>
                </label>
                <span class="slider-text">Lampeggio Secondi</span>
            </div>
            
            <div class="toggle-container">
                <label class="toggle">
                    <input type="checkbox" id="digitalOverlayToggle">
                    <span class="slider"></span>
                </label>
                <span class="slider-text">Mostra orologio digitale automatico</span>
            </div>

            <div class="day-night-settings">
                <h3 style="color: white; margin-bottom: 15px;">☀️ Luminosità Giorno/Sera/Notte</h3>
                
                <div class="time-brightness-row">
                    <div class="time-input">
                        <label for="dayStartTime">🌅 Inizio Giorno:</label>
                        <input type="time" id="dayStartTime" value="07:00">
                    </div>
                    <div class="brightness-control-inline">
                        <label>Luminosità: <span id="dayBrightnessValue">80</span>%</label>
                        <input type="range" min="10" max="100" value="80" 
                               class="brightness-slider" id="dayBrightnessSlider">
                    </div>
                </div>
                
                <div class="time-brightness-row">
                    <div class="time-input">
                        <label for="nightStartTime">🌆 Inizio Sera:</label>
                        <input type="time" id="nightStartTime" value="22:00">
                    </div>
                    <div class="brightness-control-inline">
                        <label>Luminosità: <span id="nightBrightnessValue">10</span>%</label>
                        <input type="range" min="1" max="100" value="10" 
                               class="brightness-slider" id="nightBrightnessSlider">
                    </div>
                </div>

                <div class="time-brightness-row" style="border-top: 1px solid rgba(255,255,255,0.2); padding-top: 20px; margin-top: 20px;">
                    <div class="time-input">
                        <label for="sleepStartTime">🌙 Spegnimento Notturno:</label>
                        <input type="time" id="sleepStartTime" value="23:00">
                    </div>
                    <div class="brightness-control-inline" style="display: flex; align-items: center; gap: 15px;">
                        <label class="toggle" style="margin: 0;">
                            <input type="checkbox" id="sleepModeToggle">
                            <span class="slider"></span>
                        </label>
                        <span style="color: white; font-weight: bold;">Abilita Spegnimento</span>
                    </div>
                </div>

                <button class="btn btn-success" onclick="saveDayNightSettings()" 
                        style="margin-top: 15px;">
                    💾 Salva Impostazioni Giorno/Sera/Notte
                </button>
            </div>
            
            <div class="toggle-container" style="margin-top: 20px;">
                <label class="toggle">
                    <input type="checkbox" id="powerToggle">
                    <span class="slider"></span>
                </label>
                <span class="slider-text">Accendi/Spegni Display</span>
            </div>
            
            <div class="action-buttons">
                <button class="btn btn-primary" onclick="updateStatus()">
                    🔄 Aggiorna Stato
                </button>
                <button class="btn btn-warning" onclick="resetWiFi()">
                    🔌 Reset WiFi
                </button>
                <button class="btn btn-danger" onclick="rebootDevice()">
                    🔄 Riavvia Dispositivo
                </button>
            </div>
        
        <div class="control-section">
            <h2>📝 Testo Scorrevole</h2>
            <div style="margin-bottom: 15px;">
                <input type="text" id="scrollTextInput" placeholder="Inserisci testo da scorrere..." 
                style="width:100%;padding:12px;font-size:1rem;border-radius:8px;border:1px solid rgba(255,255,255,0.3);
                background:rgba(255,255,255,0.1);color:white;">
            </div>
            <button class="btn btn-primary" onclick="sendScrollText()">📡 Invia Testo Scorrevole</button>
        </div>

    </div>
    </div>
    
    <div class="notification" id="notification"></div>
    
    <script>
    // Modalità disponibili
    const modes = [
        { id: 0, name: "Lento Casuale" },
        { id: 3, name: "Lento Arancione" },
        { id: 7, name: "Lento Blu" },
        { id: 10, name: "Veloce Giallo" },
        { id: 11, name: "Veloce Bianco" },
        { id: 12, name: "Veloce Ciano" },
        { id: 1, name: "Sfumato Blu" },
        { id: 6, name: "Sfumato Verde" },
        { id: 9, name: "Sfumato Rosa" },
        { id: 2, name: "Matrix Verde" },
        { id: 4, name: "Matrix Blu" },
        { id: 5, name: "Matrix Giallo" },
        { id: 8, name: "Matrix Ciano" },
        { id: 13, name: "Effetto TRON" },
        { id: 14, name: "Effetto TRON 2" },
        { id: 15, name: "Effetto Moto" },
        { id: 16, name: "Effetto Galaga" },
        { id: 17, name: "Effetto Pacman" },
        { id: 18, name: "Digitale" }
    ];
    
    // Colori disponibili
    const colors = [
        { name: "Bianco", value: "FFFFFF" },
        { name: "Blu", value: "0000FF" },
        { name: "Rosso", value: "FF0000" },
        { name: "Verde", value: "00FF00" },
        { name: "Giallo", value: "FFFF00" },
        { name: "Arancione", value: "FFA500" }
    ];
    
    // Variabili globali
    let userIsEditingDayNight = false;
    let editingTimeout = null;
    let brightnessUpdateTimeout = null;
    
    // Inizializzazione
    document.addEventListener('DOMContentLoaded', function() {
        console.log('Inizializzazione pagina...');
        
        initializeModeButtons();
        initializeColorButtons();
        updateStatus();
        
        // Aggiorna l'orario ogni secondo
        setInterval(updateTime, 1000);
        
        // Aggiorna lo stato ogni 5 secondi
        setInterval(updateStatusIfNotEditing, 5000);
        
        // Event listener base
        document.getElementById('blinkToggle').addEventListener('change', toggleBlink);
        document.getElementById('digitalOverlayToggle').addEventListener('change', toggleDigitalOverlay);
        document.getElementById('powerToggle').addEventListener('change', togglePower);
        
        // Event listener per orari giorno/sera/notte
        document.getElementById('dayStartTime').addEventListener('focus', startEditingDayNight);
        document.getElementById('dayStartTime').addEventListener('change', keepEditingDayNight);
        document.getElementById('dayStartTime').addEventListener('blur', scheduleEndEditing);
        
        document.getElementById('nightStartTime').addEventListener('focus', startEditingDayNight);
        document.getElementById('nightStartTime').addEventListener('change', keepEditingDayNight);
        document.getElementById('nightStartTime').addEventListener('blur', scheduleEndEditing);

        document.getElementById('sleepStartTime').addEventListener('focus', startEditingDayNight);
        document.getElementById('sleepStartTime').addEventListener('change', keepEditingDayNight);
        document.getElementById('sleepStartTime').addEventListener('blur', scheduleEndEditing);

        document.getElementById('sleepModeToggle').addEventListener('change', keepEditingDayNight);

        // Event listener per slider giorno
        document.getElementById('dayBrightnessSlider').addEventListener('mousedown', startEditingDayNight);
        document.getElementById('dayBrightnessSlider').addEventListener('touchstart', startEditingDayNight);
        document.getElementById('dayBrightnessSlider').addEventListener('input', onDayBrightnessChange);
        document.getElementById('dayBrightnessSlider').addEventListener('mouseup', scheduleEndEditing);
        document.getElementById('dayBrightnessSlider').addEventListener('touchend', scheduleEndEditing);
        
        // Event listener per slider sera
        document.getElementById('nightBrightnessSlider').addEventListener('mousedown', startEditingDayNight);
        document.getElementById('nightBrightnessSlider').addEventListener('touchstart', startEditingDayNight);
        document.getElementById('nightBrightnessSlider').addEventListener('input', onNightBrightnessChange);
        document.getElementById('nightBrightnessSlider').addEventListener('mouseup', scheduleEndEditing);
        document.getElementById('nightBrightnessSlider').addEventListener('touchend', scheduleEndEditing);
        
        console.log('Inizializzazione completata');
    });
    
    // Funzioni per gestire editing
    function startEditingDayNight() {
        userIsEditingDayNight = true;
        if (editingTimeout) {
            clearTimeout(editingTimeout);
            editingTimeout = null;
        }
        console.log('Utente sta modificando - polling disabilitato');
    }
    
    function keepEditingDayNight() {
        userIsEditingDayNight = true;
        if (editingTimeout) {
            clearTimeout(editingTimeout);
        }
        editingTimeout = setTimeout(endEditingDayNight, 3000);
    }
    
    function scheduleEndEditing() {
        if (editingTimeout) {
            clearTimeout(editingTimeout);
        }
        editingTimeout = setTimeout(endEditingDayNight, 2000);
    }
    
    function endEditingDayNight() {
        userIsEditingDayNight = false;
        editingTimeout = null;
        console.log('Editing terminato - polling riabilitato');
    }
    
    function updateStatusIfNotEditing() {
        if (!userIsEditingDayNight) {
            updateStatus();
        } else {
            console.log('Aggiornamento status saltato - utente sta modificando');
        }
    }
    
    // Funzione per rilevare fascia oraria
    function isCurrentlyDayTime() {
        const now = new Date();
        const currentHour = now.getHours();
        const currentMinute = now.getMinutes();
        const currentTimeMinutes = currentHour * 60 + currentMinute;
        
        const dayStart = document.getElementById('dayStartTime').value;
        const nightStart = document.getElementById('nightStartTime').value;
        
        if (!dayStart || !nightStart) return true;
        
        const dayParts = dayStart.split(':');
        const nightParts = nightStart.split(':');
        
        const dayHour = parseInt(dayParts[0]);
        const dayMin = parseInt(dayParts[1]);
        const nightHour = parseInt(nightParts[0]);
        const nightMin = parseInt(nightParts[1]);
        
        const dayStartMinutes = dayHour * 60 + dayMin;
        const nightStartMinutes = nightHour * 60 + nightMin;
        
        if (dayStartMinutes > nightStartMinutes) {
            return !((currentTimeMinutes >= nightStartMinutes) && (currentTimeMinutes < dayStartMinutes));
        } else {
            return !((currentTimeMinutes >= nightStartMinutes) || (currentTimeMinutes < dayStartMinutes));
        }
    }
    
    // Funzione per aggiornare luminosità in tempo reale
    function updateBrightnessRealtime(brightness) {
        fetch('/setBrightness', {
            method: 'POST',
            headers: {
                'Content-Type': 'application/x-www-form-urlencoded',
            },
            body: 'brightness=' + brightness
        })
        .then(function(response) {
            return response.text();
        })
        .then(function(data) {
            console.log('Luminosità aggiornata in tempo reale: ' + brightness + '%');
        })
        .catch(function(error) {
            console.error('Errore aggiornamento luminosità:', error);
        });
    }
    
    // Handler slider giorno
    function onDayBrightnessChange() {
        const slider = document.getElementById('dayBrightnessSlider');
        const brightness = slider.value;
        document.getElementById('dayBrightnessValue').textContent = brightness;
        
        keepEditingDayNight();
        
        if (isCurrentlyDayTime()) {
            if (brightnessUpdateTimeout) {
                clearTimeout(brightnessUpdateTimeout);
            }
            brightnessUpdateTimeout = setTimeout(function() {
                updateBrightnessRealtime(brightness);
            }, 100);
        } else {
            console.log('Fascia SERA - nessun aggiornamento in tempo reale');
        }
    }
    
    // Handler slider sera
    function onNightBrightnessChange() {
        const slider = document.getElementById('nightBrightnessSlider');
        const brightness = slider.value;
        document.getElementById('nightBrightnessValue').textContent = brightness;
        
        keepEditingDayNight();
        
        if (!isCurrentlyDayTime()) {
            if (brightnessUpdateTimeout) {
                clearTimeout(brightnessUpdateTimeout);
            }
            brightnessUpdateTimeout = setTimeout(function() {
                updateBrightnessRealtime(brightness);
            }, 100);
        } else {
            console.log('Fascia GIORNO - nessun aggiornamento in tempo reale');
        }
    }
    
    function initializeModeButtons() {
        const container = document.getElementById('modeButtons');
        container.innerHTML = '';
        
        modes.forEach(function(mode) {
            const button = document.createElement('button');
            button.className = 'mode-btn';
            button.textContent = mode.name;
            button.dataset.mode = mode.id;
            button.addEventListener('click', function() {
                setMode(mode.id);
            });
            container.appendChild(button);
        });
    }
    
    function initializeColorButtons() {
        const container = document.getElementById('colorButtons');
        container.innerHTML = '';
        
        colors.forEach(function(color) {
            const button = document.createElement('button');
            button.className = 'color-btn';
            button.style.backgroundColor = '#' + color.value;
            button.title = color.name;
            button.addEventListener('click', function() {
                setColor(color.value);
            });
            container.appendChild(button);
        });
    }
    
    function updateTime() {
        const now = new Date();
        const timeString = now.toLocaleTimeString('it-IT');
        document.getElementById('currentTime').textContent = timeString;
    }
    
    function updateStatus() {
        fetch('/status')
            .then(function(response) {
                return response.json();
            })
            .then(function(data) {
                console.log('Status ricevuto:', data);
                
                document.getElementById('currentMode').textContent = data.modeName;
                document.getElementById('currentColor').textContent = data.colorName;
                document.getElementById('currentBlink').textContent = data.blink ? 'attivo' : 'disattivo';
                document.getElementById('currentBrightness').textContent = data.brightness + '%';
                document.getElementById('currentPower').textContent = data.power ? 'acceso' : 'spento';
                
                document.getElementById('blinkToggle').checked = data.blink;
                document.getElementById('powerToggle').checked = data.power;
                if (data.digitalOverlayEnabled !== undefined) {
                    document.getElementById('digitalOverlayToggle').checked = data.digitalOverlayEnabled;
                }
                
                if (!userIsEditingDayNight) {
                    if (data.dayStartTime) {
                        document.getElementById('dayStartTime').value = data.dayStartTime;
                    }
                    if (data.nightStartTime) {
                        document.getElementById('nightStartTime').value = data.nightStartTime;
                    }
                    if (data.sleepStartTime) {
                        document.getElementById('sleepStartTime').value = data.sleepStartTime;
                    }
                    if (data.sleepModeEnabled !== undefined) {
                        document.getElementById('sleepModeToggle').checked = data.sleepModeEnabled;
                    }
                    if (data.dayBrightness !== undefined) {
                        document.getElementById('dayBrightnessSlider').value = data.dayBrightness;
                        document.getElementById('dayBrightnessValue').textContent = data.dayBrightness;
                    }
                    if (data.nightBrightness !== undefined) {
                        document.getElementById('nightBrightnessSlider').value = data.nightBrightness;
                        document.getElementById('nightBrightnessValue').textContent = data.nightBrightness;
                    }
                }
                
                document.querySelectorAll('.mode-btn').forEach(function(btn) {
                    const isActive = parseInt(btn.dataset.mode) === data.mode;
                    if (isActive) {
                        btn.classList.add('active');
                    } else {
                        btn.classList.remove('active');
                    }
                });
            })
            .catch(function(error) {
                console.error('Errore nel recupero dello stato:', error);
                showNotification('Errore nel recupero dello stato', 'error');
            });
    }
    
    function setMode(modeId) {
        fetch('/setMode', {
            method: 'POST',
            headers: {
                'Content-Type': 'application/x-www-form-urlencoded',
            },
            body: 'mode=' + modeId
        })
        .then(function(response) {
            return response.text();
        })
        .then(function(data) {
            showNotification('Modalità cambiata con successo', 'success');
            updateStatus();
        })
        .catch(function(error) {
            console.error('Errore nel cambio modalità:', error);
            showNotification('Errore nel cambio modalità', 'error');
        });
    }
    
    function setColor(colorHex) {
        fetch('/setColor', {
            method: 'POST',
            headers: {
                'Content-Type': 'application/x-www-form-urlencoded',
            },
            body: 'color=' + colorHex
        })
        .then(function(response) {
            return response.text();
        })
        .then(function(data) {
            showNotification('Colore cambiato con successo', 'success');
            updateStatus();
        })
        .catch(function(error) {
            console.error('Errore nel cambio colore:', error);
            showNotification('Errore nel cambio colore', 'error');
        });
    }
    
    function toggleBlink() {
        const blinkState = document.getElementById('blinkToggle').checked;
        
        fetch('/setBlink', {
            method: 'POST',
            headers: {
                'Content-Type': 'application/x-www-form-urlencoded',
            },
            body: 'blink=' + (blinkState ? '1' : '0')
        })
        .then(function(response) {
            return response.text();
        })
        .then(function(data) {
            showNotification('Lampeggio ' + (blinkState ? 'attivato' : 'disattivato'), 'success');
            updateStatus();
        })
        .catch(function(error) {
            console.error('Errore nel cambio stato lampeggio:', error);
            showNotification('Errore nel cambio stato lampeggio', 'error');
        });
    }
    
    
    function toggleDigitalOverlay() {
        const enabled = document.getElementById('digitalOverlayToggle').checked;
        fetch('/setDigitalOverlay', {
            method: 'POST',
            headers: {
                'Content-Type': 'application/x-www-form-urlencoded',
            },
            body: 'enable=' + (enabled ? '1' : '0')
        })
        .then(function(response) {
            return response.text();
        })
        .then(function(data) {
            showNotification('Digitale automatico ' + (enabled ? 'attivato' : 'disattivato'), 'success');
        })
        .catch(function(error) {
            console.error('Errore nel cambio stato digitale automatico:', error);
            showNotification('Errore nel cambio stato digitale automatico', 'error');
        });
    }

function saveDayNightSettings() {
        const dayStart = document.getElementById('dayStartTime').value;
        const nightStart = document.getElementById('nightStartTime').value;
        const sleepStart = document.getElementById('sleepStartTime').value;
        const dayBright = document.getElementById('dayBrightnessSlider').value;
        const nightBright = document.getElementById('nightBrightnessSlider').value;
        const sleepEnabled = document.getElementById('sleepModeToggle').checked;
        
        fetch('/setDayNightSettings', {
            method: 'POST',
            headers: {
                'Content-Type': 'application/x-www-form-urlencoded',
            },
            body: 'dayStartTime=' + dayStart + 
                '&nightStartTime=' + nightStart + 
                '&sleepStartTime=' + sleepStart +
                '&dayBrightness=' + dayBright + 
                '&nightBrightness=' + nightBright +
                '&sleepModeEnabled=' + (sleepEnabled ? '1' : '0')
        })
        .then(function(response) {
            return response.text();
        })
        .then(function(data) {
            showNotification('Impostazioni giorno/sera/notte salvate', 'success');
            endEditingDayNight();
            setTimeout(function() {
                updateStatus();
            }, 500);
        })
        .catch(function(error) {
            showNotification('Errore nel salvataggio', 'error');
        });
    }    
    function togglePower() {
        const powerState = document.getElementById('powerToggle').checked;
        
        fetch('/power', {
            method: 'POST',
            headers: {
                'Content-Type': 'application/x-www-form-urlencoded',
            },
            body: 'power=' + (powerState ? '1' : '0')
        })
        .then(function(response) {
            return response.text();
        })
        .then(function(data) {
            showNotification('Display ' + (powerState ? 'acceso' : 'spento'), 'success');
            updateStatus();
        })
        .catch(function(error) {
            console.error('Errore nel cambio stato display:', error);
            showNotification('Errore nel cambio stato display', 'error');
        });
    }
    
    function resetWiFi() {
        if (confirm('Sei sicuro di voler resettare la configurazione WiFi? Il dispositivo si riavvierà.')) {
            fetch('/reset', {
                method: 'POST'
            })
            .then(function(response) {
                return response.text();
            })
            .then(function(data) {
                showNotification('WiFi resettato, riconnettiti al WiFi ORAQUADRA2_Plus_AP', 'success');
                setTimeout(function() {
                    window.location.reload();
                }, 3000);
            })
            .catch(function(error) {
                console.error('Errore nel reset WiFi:', error);
                showNotification('Errore nel reset WiFi', 'error');
            });
        }
    }
    
    function rebootDevice() {
        if (confirm('Sei sicuro di voler riavviare il dispositivo?')) {
            fetch('/reset', {
                method: 'POST'
            })
            .then(function(response) {
                return response.text();
            })
            .then(function(data) {
                showNotification('Dispositivo in riavvio...', 'success');
                setTimeout(function() {
                    window.location.reload();
                }, 3000);
            })
            .catch(function(error) {
                console.error('Errore nel riavvio:', error);
                showNotification('Errore nel riavvio', 'error');
            });
        }
    }
    
    function showNotification(message, type) {
        const notification = document.getElementById('notification');
        notification.textContent = message;
        notification.className = 'notification ' + type + ' show';
        
        setTimeout(function() {
            notification.classList.remove('show');
        }, 3000);
    }

function sendScrollText() {
    const txt = document.getElementById('scrollTextInput').value.trim();
    if (txt.length === 0) { showNotification("Inserisci un testo!", "error"); return; }
    fetch('/setScrollText?text=' + encodeURIComponent(txt))
    .then(r => r.text())
    .then(resp => {
        if (resp === "OK") showNotification("Testo inviato!", "success");
        else showNotification("Errore invio testo", "error");
    })
    .catch(err => showNotification("Errore di rete", "error"));
}


// ==== Palette Colori Avanzata ====
function applyAdvancedColor() {
    const picker = document.getElementById('advancedColorPicker');
    if (!picker) return;
    const hex = picker.value.replace('#','');
    const formData = new FormData();
    formData.append('color', hex);
    fetch('/setColor', { method:'POST', body:formData })
        .then(()=>showNotification("Colore applicato: #"+hex.toUpperCase(),"success"))
        .catch(()=>showNotification("Errore applicazione colore","error"));
}

</script>


</body>
</html>
)rawliteral";

    server.send(200, "text/html", html);
}

void handleStatus() {
    StaticJsonDocument<512> doc;
    
    // Nome della modalità corrente
    String modeName = "Sconosciuta";
    switch(currentPreset) {
        case 0: modeName = "Lento Casuale"; break;
        case 1: modeName = "Sfumato Blu"; break;
        case 2: modeName = "Matrix Verde"; break;
        case 3: modeName = "Lento Arancione"; break;
        case 4: modeName = "Matrix Blu"; break;
        case 5: modeName = "Matrix Giallo"; break;
        case 6: modeName = "Sfumato Verde"; break;
        case 7: modeName = "Lento Blu"; break;
        case 8: modeName = "Matrix Ciano"; break;
        case 9: modeName = "Sfumato Rosa"; break;
        case 10: modeName = "Veloce Giallo"; break;
        case 11: modeName = "Veloce Bianco"; break;
        case 12: modeName = "Veloce Ciano"; break;
        case 13: modeName = "Effetto TRON"; break;
        case 14: modeName = "Effetto TRON 2"; break;
        case 15: modeName = "Effetto Moto"; break;
        case 16: modeName = "Effetto Galaga"; break;
        case 17: modeName = "Effetto Pacman"; break;
        case 18: modeName = "Digitale"; break;
        case 19: modeName = "Scroll Text"; break;
        default: modeName = "Sconosciuta"; break;
    }
    
    // Nome del colore corrente
    String colorName = "Personalizzato";
    CRGB current = clockColors.current;
    if (current == CRGB::White) colorName = "Bianco";
    else if (current == CRGB::Blue) colorName = "Blu";
    else if (current == CRGB::Red) colorName = "Rosso";
    else if (current == CRGB::Green) colorName = "Verde";
    else if (current == CRGB::Yellow) colorName = "Giallo";
    else if (current == CRGB(255,165,0)) colorName = "Arancione";
    
    // Formatta gli orari
    char dayStart[6], nightStart[6], sleepStart[6];
    snprintf(dayStart, sizeof(dayStart), "%02d:%02d", dayStartHour, dayStartMinute);
    snprintf(nightStart, sizeof(nightStart), "%02d:%02d", nightStartHour, nightStartMinute);
    snprintf(sleepStart, sizeof(sleepStart), "%02d:%02d", sleepStartHour, sleepStartMinute);
  
    // Debug
    Serial.println("=== INVIO STATUS ===");
    Serial.print("dayStartTime: ");
    Serial.println(dayStart);
    Serial.print("nightStartTime: ");
    Serial.println(nightStart);
    Serial.print("dayBrightness: ");
    Serial.println(map(dayBrightness, 0, 255, 0, 100));
    Serial.print("nightBrightness: ");
    Serial.println(map(nightBrightness, 0, 255, 0, 100));
    Serial.print("sleepStartTime: ");
    Serial.println(sleepStart);
    Serial.print("sleepModeEnabled: ");
    Serial.println(sleepModeEnabled);

    doc["mode"] = currentPreset;
    doc["modeName"] = modeName;
    doc["colorName"] = colorName;
    doc["blink"] = (currentBlink == 1);
    doc["brightness"] = map(intBrightness, 0, 255, 0, 100);
    doc["power"] = !displayOff;
    doc["dayStartTime"] = dayStart;
    doc["nightStartTime"] = nightStart;
    doc["sleepStartTime"] = sleepStart;
    doc["sleepModeEnabled"] = sleepModeEnabled;
    doc["dayBrightness"] = map(dayBrightness, 0, 255, 0, 100);
    doc["nightBrightness"] = map(nightBrightness, 0, 255, 0, 100);
    doc["digitalOverlayEnabled"] = digitalOverlayEnabled;
    doc["hour"] = currentHour;
    doc["minute"] = currentMinute;
    doc["second"] = currentSecond;
    
    String response;
    serializeJson(doc, response);
    
    Serial.print("JSON inviato: ");
    Serial.println(response);
    Serial.println("===================");
    
    server.send(200, "application/json", response);
}


void handleSetMode() {
    if (server.hasArg("mode")) {
        uint8_t newMode = server.arg("mode").toInt();
        if (newMode < 20) { // 19 preset totali
            currentPreset = newMode;
            applyPreset(currentPreset);
            EEPROM.write(EEPROM_PRESET_ADDR, currentPreset);
            EEPROM.commit();
            server.send(200, "text/plain", "OK");
            return;
        }
    }
    server.send(400, "text/plain", "Invalid mode");
}

void handleSetBlink() {
    if (server.hasArg("blink")) {
        currentBlink = server.arg("blink").toInt();
        EEPROM.write(EEPROM_BLINK_ADDR, currentBlink);
        EEPROM.commit();
        updateDisplay(); // Aggiorna il display per applicare il cambio del lampeggio
        server.send(200, "text/plain", "OK");
        return;
    }
    server.send(400, "text/plain", "Invalid blink value");
}

void handleSetColor() {
    if (server.hasArg("color")) {
        String colorHex = server.arg("color");
        if (colorHex.length() == 6) {
            long colorValue = strtol(colorHex.c_str(), NULL, 16);
            CRGB newColor;
            newColor.r = (colorValue >> 16) & 0xFF;
            newColor.g = (colorValue >> 8) & 0xFF;
            newColor.b = colorValue & 0xFF;
            
            clockColors.current = newColor;
            
            // Salva in EEPROM
            EEPROM.write(EEPROM_COLOR_R_ADDR, newColor.r);
            EEPROM.write(EEPROM_COLOR_G_ADDR, newColor.g);
            EEPROM.write(EEPROM_COLOR_B_ADDR, newColor.b);
            
            // Determina quale preset corrisponde (se esiste)
            uint8_t presetIndex = 255; // Valore per "colore personalizzato"
            if (newColor == CRGB::White) presetIndex = 0;
            else if (newColor == CRGB::Blue) presetIndex = 1;
            else if (newColor == CRGB::Red) presetIndex = 2;
            else if (newColor == CRGB::Green) presetIndex = 3;
            else if (newColor == CRGB::Yellow) presetIndex = 4;
            else if (newColor == CRGB(255,165,0)) presetIndex = 5; // Arancione
            
            clockColors.presetIndex = presetIndex;
            EEPROM.write(EEPROM_COLOR_PRESET_ADDR, presetIndex);
            EEPROM.commit();
            
            updateDisplay();
            server.send(200, "text/plain", "OK");
            return;
        }
    }
    server.send(400, "text/plain", "Invalid color");
}

void handleSetBrightness() {
    if (server.hasArg("brightness")) {
        int brightnessPercent = server.arg("brightness").toInt();
        intBrightness = map(brightnessPercent, 0, 100, 0, 255);
        
        // NON salvare in EEPROM - questo è solo un aggiornamento temporaneo
        // L'EEPROM verrà aggiornato solo quando si clicca "Salva Impostazioni Giorno/sera/Notte"
        
        FastLED.setBrightness(intBrightness);
        FastLED.show();
        
        Serial.print("Luminosità temporanea impostata a: ");
        Serial.print(brightnessPercent);
        Serial.println("%");
        
        server.send(200, "text/plain", "OK");
        return;
    }
    server.send(400, "text/plain", "Invalid brightness");
}

void handleSetDayNightSettings() {
    if (server.hasArg("dayStartTime") && server.hasArg("nightStartTime") && 
        server.hasArg("dayBrightness") && server.hasArg("nightBrightness") &&
        server.hasArg("sleepStartTime") && server.hasArg("sleepModeEnabled")) {
        
        String dayStart = server.arg("dayStartTime");
        String nightStart = server.arg("nightStartTime");
        String sleepStart = server.arg("sleepStartTime");
        int dayBright = server.arg("dayBrightness").toInt();
        int nightBright = server.arg("nightBrightness").toInt();
        bool sleepEnabled = (server.arg("sleepModeEnabled") == "true" || server.arg("sleepModeEnabled") == "1");
        
        // Parse orari
        dayStartHour = dayStart.substring(0, 2).toInt();
        dayStartMinute = dayStart.substring(3, 5).toInt();
        nightStartHour = nightStart.substring(0, 2).toInt();
        nightStartMinute = nightStart.substring(3, 5).toInt();
        sleepStartHour = sleepStart.substring(0, 2).toInt();
        sleepStartMinute = sleepStart.substring(3, 5).toInt();
        sleepModeEnabled = sleepEnabled;
        
        // Converti luminosità
        dayBrightness = map(dayBright, 0, 100, 0, 255);
        nightBrightness = map(nightBright, 0, 100, 0, 255);
        
        // Salva in EEPROM
        EEPROM.write(EEPROM_DAY_START_HOUR_ADDR, dayStartHour);
        EEPROM.write(EEPROM_DAY_START_MINUTE_ADDR, dayStartMinute);
        EEPROM.write(EEPROM_NIGHT_START_HOUR_ADDR, nightStartHour);
        EEPROM.write(EEPROM_NIGHT_START_MINUTE_ADDR, nightStartMinute);
        EEPROM.write(EEPROM_SLEEP_START_HOUR_ADDR, sleepStartHour);
        EEPROM.write(EEPROM_SLEEP_START_MINUTE_ADDR, sleepStartMinute);
        EEPROM.write(EEPROM_SLEEP_ENABLED_ADDR, sleepEnabled ? 1 : 0);
        EEPROM.write(EEPROM_DAY_BRIGHTNESS_ADDR, dayBrightness);
        EEPROM.write(EEPROM_NIGHT_BRIGHTNESS_ADDR, nightBrightness);
        
        EEPROM.commit();
        
        // Applica immediatamente
        if (isSleepTime()) {
            displayOff = true;
            FastLED.clear();
            FastLED.show();
        } else {
            displayOff = false;
            intBrightness = isNightTime() ? nightBrightness : dayBrightness;
            FastLED.setBrightness(intBrightness);
            updateDisplay();
        }
        
        server.send(200, "text/plain", "OK");
        return;
    }
    server.send(400, "text/plain", "Invalid parameters");
}

void handleSetDigitalOverlay() {
    if (server.hasArg("enable")) {
        String val = server.arg("enable");
        // Consideriamo "1" o "true" (case-insensitive) come ON
        val.toLowerCase();
        digitalOverlayEnabled = (val == "1" || val == "true");
        EEPROM.write(EEPROM_DIGITAL_OVERLAY_ADDR, digitalOverlayEnabled ? 1 : 0);
        EEPROM.commit();
        server.send(200, "text/plain", "OK");
        return;
    }
    server.send(400, "text/plain", "Invalid parameters");
}





void handleSetScrollText() {
    if (server.hasArg("text")) {
        customScrollText = server.arg("text");
        customScrollText.toUpperCase();
        scrollTextInitialized = false;
        digitalOverlayActive = false; // disattiva overlay se eravamo in digitale
        lastEffectChangeTime = millis();
        currentMode = MODE_SCROLLTEXT;
        server.send(200, "text/plain", "OK");
        return;
    }
    server.send(400, "text/plain", "Missing text");
}

void handlePower() {
    if (server.hasArg("power")) {
        displayOff = !server.arg("power").toInt(); // Inverti perché displayOff = true significa spento
        if (displayOff) {
            FastLED.clear();
            FastLED.show();
        } else {
            updateDisplay();
        }
        server.send(200, "text/plain", "OK");
        return;
    }
    server.send(400, "text/plain", "Invalid power value");
}

void handleReset() {
    server.send(200, "text/plain", "OK");
    delay(1000);
    ESP.restart();
}

void handleNotFound() {
    server.send(404, "text/plain", "Pagina non trovata");
}

// =============================================================================
// FUNZIONI PRINCIPALI DEL SKETCH
// =============================================================================

// Funzione per verificare se siamo nel periodo di modalità serale
bool isNightTime() {
    uint32_t currentTimeMinutes = currentHour * 60 + currentMinute;
    uint32_t dayStartMinutes = dayStartHour * 60 + dayStartMinute;
    uint32_t nightStartMinutes = nightStartHour * 60 + nightStartMinute;
    
    if (dayStartMinutes > nightStartMinutes) {
        return (currentTimeMinutes >= nightStartMinutes) && (currentTimeMinutes < dayStartMinutes);
    } else {
        return (currentTimeMinutes >= nightStartMinutes) || (currentTimeMinutes < dayStartMinutes);
    }
}

// Funzione per verificare se siamo nel periodo di spegnimento notturno
bool isSleepTime() {
    if (!sleepModeEnabled) return false;
    
    uint32_t currentTimeMinutes = currentHour * 60 + currentMinute;
    uint32_t sleepStartMinutes = sleepStartHour * 60 + sleepStartMinute;
    uint32_t dayStartMinutes = dayStartHour * 60 + dayStartMinute;
    
    if (dayStartMinutes > sleepStartMinutes) {
        return (currentTimeMinutes >= sleepStartMinutes) && (currentTimeMinutes < dayStartMinutes);
    } else {
        return (currentTimeMinutes >= sleepStartMinutes) || (currentTimeMinutes < dayStartMinutes);
    }
}

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

// [RESTANTE DEL CODICE RIMANE INVARIATO...]

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
    
    // Controlla se tutte le lettere sono state colorated
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


void updateDigitaleEffect() {
    uint32_t currentMillis = millis();
    
    // Aggiornamento periodico
    if (currentMillis - lastDigitaleUpdate < DIGITALE_UPDATE_INTERVAL) return;
    lastDigitaleUpdate = currentMillis;
    
    // Cancella tutto
    FastLED.clear();
    
    // Ottieni il colore corrente dal sistema
    CRGB displayColor = clockColors.getColor();
    
    // Calcola le cifre da visualizzare
    uint8_t hourTens = currentHour / 10;
    uint8_t hourOnes = currentHour % 10;
    uint8_t minuteTens = currentMinute / 10;
    uint8_t minuteOnes = currentMinute % 10;
    
    // Layout display:
    // Le ore occupano la parte alta (righe 1-7)
    // I minuti occupano la parte bassa (righe 9-15)
    // Il punto centrale nella riga 8 (centro display)
    
    // Posizioni orizzontali centrate per le cifre
    // ORE IN ALTO (righe 1-7)
    uint8_t hourY = 0;
    uint8_t hourX1 = 3;   // Prima cifra ora
    uint8_t hourX2 = 9;   // Seconda cifra ora
    
    drawDigitCentered(hourX1, hourY, hourTens, displayColor);
    drawDigitCentered(hourX2, hourY, hourOnes, displayColor);
    
    // MINUTI IN BASSO (righe 10-16)
    uint8_t minuteY = 9;
    uint8_t minuteX1 = 3;  // Prima cifra minuti
    uint8_t minuteX2 = 9;  // Seconda cifra minuti
    
    drawDigitCentered(minuteX1, minuteY, minuteTens, displayColor);
    drawDigitCentered(minuteX2, minuteY, minuteOnes, displayColor);
    
    // PUNTO LAMPEGGIANTE CENTRALE
    // Solo se il lampeggio è attivato (currentBlink == 1)
    if (currentBlink == 1) {
        // Lampeggia ogni secondo (basato sui secondi)
        if (currentSecond % 2 == 0) {
            // Punto centrale della matrice 16x16:  x=8,  y=8
            uint16_t centerPos = xyToLED(8, 8);
            if (centerPos < NUM_LEDS) {
                leds[centerPos] = displayColor;
            }
        }
    } else {
        // Se il lampeggio è disattivato, spegni il punto fisso
        uint16_t centerPos = xyToLED(8, 8);
        if (centerPos < NUM_LEDS) {
            leds[centerPos] =  CRGB::Black; 
        }
    }
    
    // Mostra tutto
    FastLED.show();
}


// Setup principale
void setup() {
   Serial.begin(115200);
   Serial.println("START");
   EEPROM.begin(EEPROM_SIZE);
   
   // =========================================================================
   // FASE 1: INIZIALIZZAZIONE EEPROM (se necessario)
   // =========================================================================
   if (EEPROM.read(0) != EEPROM_CONFIGURED_MARKER) {
       Serial.println("Inizializzazione EEPROM - Prima configurazione");
       
       EEPROM.write(0, EEPROM_CONFIGURED_MARKER);
       EEPROM.write(EEPROM_PRESET_ADDR, 0);
       EEPROM.write(EEPROM_BLINK_ADDR, 1);
       
       // Valori default per luminosità e colore
       EEPROM.write(EEPROM_BRIGHTNESS_ADDR, 128);
       EEPROM.write(EEPROM_COLOR_R_ADDR, 255);
       EEPROM.write(EEPROM_COLOR_G_ADDR, 255);
       EEPROM.write(EEPROM_COLOR_B_ADDR, 255);
       EEPROM.write(EEPROM_COLOR_PRESET_ADDR, 0);
       
       // Valori default per gestione giorno/sera/notte
       EEPROM.write(EEPROM_DAY_START_HOUR_ADDR, 7);
       EEPROM.write(EEPROM_DAY_START_MINUTE_ADDR, 0);
       EEPROM.write(EEPROM_NIGHT_START_HOUR_ADDR, 22);
       EEPROM.write(EEPROM_NIGHT_START_MINUTE_ADDR, 0);
       EEPROM.write(EEPROM_SLEEP_START_HOUR_ADDR, 19);
       EEPROM.write(EEPROM_SLEEP_START_MINUTE_ADDR, 0);
       EEPROM.write(EEPROM_SLEEP_ENABLED_ADDR, 0);
       EEPROM.write(EEPROM_DAY_BRIGHTNESS_ADDR, 204);    // 80%
       EEPROM.write(EEPROM_NIGHT_BRIGHTNESS_ADDR, 26);   // 10%
       EEPROM.write(EEPROM_DIGITAL_OVERLAY_ADDR, 1);     // overlay digitale ATTIVO di default
       
       EEPROM.commit();
       
       Serial.println("EEPROM inizializzata con valori default");
   }
   
   // =========================================================================
   // FASE 2: CARICAMENTO VALORI DA EEPROM
   // =========================================================================
   
   // Carica preset salvato
   currentPreset = EEPROM.read(EEPROM_PRESET_ADDR);
   currentBlink = EEPROM.read(EEPROM_BLINK_ADDR);
   
   // Carica luminosità salvata (questa non verrà più usata, ma la teniamo per compatibilità)
   uint8_t savedBrightness = EEPROM.read(EEPROM_BRIGHTNESS_ADDR);
   
   // Carica colore salvato
   uint8_t savedR = EEPROM.read(EEPROM_COLOR_R_ADDR);
   uint8_t savedG = EEPROM.read(EEPROM_COLOR_G_ADDR);
   uint8_t savedB = EEPROM.read(EEPROM_COLOR_B_ADDR);
   uint8_t savedPreset = EEPROM.read(EEPROM_COLOR_PRESET_ADDR);
   
   clockColors.current = CRGB(savedR, savedG, savedB);
   clockColors.presetIndex = savedPreset;
   
   // Carica impostazioni giorno/notte
   dayStartHour = EEPROM.read(EEPROM_DAY_START_HOUR_ADDR);
   dayStartMinute = EEPROM.read(EEPROM_DAY_START_MINUTE_ADDR);
   nightStartHour = EEPROM.read(EEPROM_NIGHT_START_HOUR_ADDR);
   nightStartMinute = EEPROM.read(EEPROM_NIGHT_START_MINUTE_ADDR);
   dayBrightness = EEPROM.read(EEPROM_DAY_BRIGHTNESS_ADDR);
   nightBrightness = EEPROM.read(EEPROM_NIGHT_BRIGHTNESS_ADDR);
   sleepStartHour = EEPROM.read(EEPROM_SLEEP_START_HOUR_ADDR);
   sleepStartMinute = EEPROM.read(EEPROM_SLEEP_START_MINUTE_ADDR);
   sleepModeEnabled = (EEPROM.read(EEPROM_SLEEP_ENABLED_ADDR) == 1);
   digitalOverlayEnabled = (EEPROM.read(EEPROM_DIGITAL_OVERLAY_ADDR) == 1);

   // Debug - stampa i valori caricati
   Serial.println("=== IMPOSTAZIONI CARICATE DA EEPROM ===");
   Serial.print("Preset: ");
   Serial.println(currentPreset);
   Serial.print("Blink: ");
   Serial.println(currentBlink);
   Serial.print("Giorno inizio: ");
   Serial.print(dayStartHour);
   Serial.print(":");
   Serial.println(dayStartMinute);
   Serial.print("Notte inizio: ");
   Serial.print(nightStartHour);
   Serial.print(":");
   Serial.println(nightStartMinute);
   Serial.print("Luminosità giorno: ");
   Serial.println(dayBrightness);
   Serial.print("Luminosità notte: ");
   Serial.println(nightBrightness);
   Serial.print("Spegnimento notturno: ");
   Serial.print(sleepStartHour);
   Serial.print(":");
   Serial.println(sleepStartMinute);
   Serial.print("Spegnimento abilitato: ");
   Serial.println(sleepModeEnabled);
   Serial.println("=====================================");
   
   // =========================================================================
   // FASE 3: DETERMINARE LA LUMINOSITÀ INIZIALE CORRETTA
   // =========================================================================
   
   // IMPORTANTE: Calcola subito se siamo in fascia giorno o notte
   // Questo richiede che l'ora sia disponibile, quindi lo faremo dopo WiFi
   // Per ora impostiamo un valore temporaneo ragionevole
   intBrightness = dayBrightness;  // Default temporaneo
   
   Serial.print("Luminosità iniziale temporanea: ");
   Serial.println(intBrightness);
   
   // =========================================================================
   // FASE 4: CONFIGURAZIONE PIN
   // =========================================================================
   
   #if BUTTON_LOGIC_INVERTED == 1
     pinMode(BUTTON_MODE, INPUT_PULLUP);
     pinMode(BUTTON_SEC, INPUT_PULLUP);
   #else
     pinMode(BUTTON_MODE, INPUT_PULLDOWN);
     pinMode(BUTTON_SEC, INPUT_PULLDOWN);
   #endif
   
   // =========================================================================
   // FASE 5: INIZIALIZZAZIONE FASTLED CON LUMINOSITÀ CORRETTA
   // =========================================================================
   
   FastLED.addLeds<LED_TYPE, LED_PIN, COLOR_ORDER>(leds, NUM_LEDS)
          .setCorrection(TypicalLEDStrip);
   
   // IMPORTANTE: Usa la luminosità caricata, NON BRIGHTNESS_DAY
   FastLED.setBrightness(intBrightness);
   
   Serial.print("FastLED inizializzato con luminosità: ");
   Serial.println(intBrightness);
   
   FastLED.clear();
   FastLED.show();
   
   // =========================================================================
   // FASE 6: CONFIGURAZIONE WIFI
   // =========================================================================
   
   WiFiManager wm;
   wm.setConfigPortalTimeout(180);
   wm.setTitle("ORAQUADRA2 WiFi Setup");
   
   bool portalOpened = false;
   
   wm.setAPCallback([&portalOpened](WiFiManager* myWiFiManager) {
       Serial.println("Portale di configurazione avviato");
       portalOpened = true;
       scrollText("COLLEGATI AL DISPOSITIVO VIA WIFI E CONFIGURA LA RETE", 
                  CRGB::White, 50);
   });
   
   fill_solid(leds, NUM_LEDS, CRGB::Blue);
   FastLED.show();
   
   Serial.println("Tentativo connessione WiFi...");
   
   bool res = wm.autoConnect("ORAQUADRA2_AP");
   
   if (!res) {
       Serial.println("Impossibile collegarsi alla rete");
       scrollText("IMPOSSIBILE COLLEGARSI ALLA RETE", CRGB::White, 50);
       delay(3000);
       ESP.restart();
   }

   Serial.println("WiFi Connesso!");
   Serial.print("Indirizzo IP: ");
   Serial.println(WiFi.localIP());
   
   String ipMessage = "IP " + WiFi.localIP().toString();
   scrollText(ipMessage, CRGB::White, 50);
   
   FastLED.clear();
   FastLED.show();
   
   WiFi.setSleep(false);
   WiFi.setAutoReconnect(true);
   
   // =========================================================================
   // FASE 7: SINCRONIZZAZIONE ORARIO E CALCOLO LUMINOSITÀ CORRETTA
   // =========================================================================
   
   if(WiFi.status() == WL_CONNECTED) {
       setupOTA();
       setServer("pool.ntp.org");
       waitForSync(10);
       myTZ.setLocation(F("Europe/Rome"));
       
       // ORA abbiamo l'orario! Calcoliamo la luminosità corretta
       currentHour = myTZ.hour();
       currentMinute = myTZ.minute();
       currentSecond = myTZ.second();
       
       Serial.print("Ora sincronizzata: ");
       Serial.print(currentHour);
       Serial.print(":");
       Serial.println(currentMinute);
       
       if (isSleepTime()) {
           Serial.println("Fascia oraria: SPEGNIMENTO NOTTURNO");
           displayOff = true;
           intBrightness = 0;
       } else {
           bool isNight = isNightTime();
           intBrightness = isNight ? nightBrightness : dayBrightness;
           
           Serial.print("Fascia oraria: ");
           Serial.println(isNight ? "SERA" : "GIORNO");
       }

       Serial.print("Luminosità corretta impostata a: ");
       Serial.println(intBrightness);
       
       // Applica la luminosità corretta
       FastLED.setBrightness(intBrightness);
       
       // Avvia il server web
       setupWebServer();
   }
   // =========================================================================
   // FASE 8: APPLICA IL PRESET SALVATO
   // =========================================================================
   
   Serial.print("Applicazione preset salvato: ");
   Serial.println(currentPreset);
   
   applyPreset(currentPreset);
   
   Serial.println("Setup completato!");
   Serial.println("=====================================");
   
   // Stampa riepilogo finale
   Serial.println("=== STATO FINALE ===");
   Serial.print("Preset: ");
   Serial.println(currentPreset);
   Serial.print("Luminosità: ");
   Serial.println(intBrightness);
   Serial.print("Colore RGB: ");
   Serial.print(clockColors.current.r);
   Serial.print(",");
   Serial.print(clockColors.current.g);
   Serial.print(",");
   Serial.println(clockColors.current.b);
   Serial.println("===================");
}

void resetWiFi() {
    Serial.println("Impossibile collegarsi alla rete");
    scrollText("RESET WIFI", CRGB::Red, 50);
    WiFiManager wm;
    wm.resetSettings();
    ESP.restart();
}

void setupOTA() {
   ArduinoOTA.setHostname("ORAQUADRA2");
   
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
   if (displayOff) {
       FastLED.clear();
       FastLED.show();
       return;
   }
   
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
            CRGB CasualeSonoColor = clockColors.current;
            CRGB CasualeHourColor = clockColors.current;
            CRGB CasualeEColor = clockColors.current;
            CRGB CasualeMinutesColor = clockColors.current;
            CRGB CasualeMinutiColor = clockColors.current;

            if (randomColor == true) {
                CasualeSonoColor = CRGB(random8(), random8(), random8());
                CasualeHourColor = CRGB(random8(), random8(), random8());
                CasualeEColor = CRGB(random8(), random8(), random8());
                CasualeMinutesColor = CRGB(random8(), random8(), random8());
                CasualeMinutiColor = CRGB(random8(), random8(), random8());
            } 
        
            // Effetto fade-in graduale fino alla luminosità di sistema
            uint8_t targetBrightness = intBrightness; // Usa la luminosità di sistema corrente
            uint8_t steps = 30;
            uint8_t increment = targetBrightness / steps;
            if (increment == 0) increment = 1; // Evita divisione per zero
            
            for(uint8_t brightness = 0; brightness <= targetBrightness; brightness += increment) {
                FastLED.clear();
                
                // Calcola il fattore di scala proporzionale (0-255)
                uint8_t scaleFactor = map(brightness, 0, targetBrightness, 0, 255);
                
                // Crea colori con luminosità progressiva

                CRGB sonoFade = CasualeSonoColor;
                sonoFade.nscale8(scaleFactor);
                displayWord(WORD_SONO_LE, sonoFade);

                CRGB hourFade = CasualeHourColor;
                hourFade.nscale8(scaleFactor);
                displayWord(hourWord, hourFade);
                
                if(currentMinute > 0) {
           
                    CRGB eFade = CasualeEColor;
                    eFade.nscale8(scaleFactor);
                    displayWord(WORD_E, eFade);
                    
                    CRGB minutesFade = CasualeMinutesColor;
                    minutesFade.nscale8(scaleFactor);
                    showMinutes(currentMinute, minutesFade);
                    
                    CRGB minutiFade = CasualeMinutiColor;
                    minutiFade.nscale8(scaleFactor);
                    displayWord(WORD_MINUTI, minutiFade);
                }
                
                FastLED.show();
                delay(70); // 50ms per step = ~1 secondo totale
                yield();
            }
            
            // Mantieni acceso per un po'
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

// Variabile per tracciare lo stato precedente del display
bool prevDisplayOff = false;


// Ritorna true quando l'effetto corrente ha completato un ciclo "naturale".
// Per gli effetti continui senza fine, usa un fallback a tempo.
bool effectCycleCompleted() {
    uint32_t now = millis();
    switch (currentMode) {
        case MODE_MATRIX2:
            return matrix2State.completed;
        case MODE_MOTO:
            return (motoCurrentPosition >= NUM_LEDS && motoCompletionTime > 0);
        // Aggiungi qui altri effetti con fine naturale se vuoi
        default:
            return (now - lastEffectChangeTime >= DEFAULT_EFFECT_CYCLE_MS);
    }
}

// Gestione overlay digitale: dopo fine ciclo effetto -> attesa 1.5s -> digitale 2.5s -> ritorno effetto precedente
void handleDigitalOverlay() {
    if (!digitalOverlayEnabled) {
        // Overlay digitale automatico disattivato
        return;
    }

    uint32_t now = millis();

    if (currentMode == MODE_DIGITALE) {
        if (now - digitalOverlayStart >= DIGITALE_OVERLAY_DURATION) {
            currentMode = savedModeBeforeDigital;
            digitalOverlayActive = false;
            waitingForDigital = false;
            lastEffectChangeTime = now;
        }
        return;
    }

    if (currentMode == MODE_SCROLLTEXT) return;

    if (!digitalOverlayActive) {
        if (!waitingForDigital) {
            if (effectCycleCompleted()) {
                waitingForDigital = true;
                digitalDelayStart = now;
            }
        } else {
            if (now - digitalDelayStart >= DIGITALE_DELAY_AFTER_EFFECT) {
                savedModeBeforeDigital = currentMode;
                currentMode = MODE_DIGITALE;
                digitalOverlayActive = true;
                digitalOverlayStart = now;
                waitingForDigital = false;
            }
        }
        return;
    }

    if (now - digitalOverlayStart >= DIGITALE_OVERLAY_DURATION) {
        digitalOverlayActive = false;
        currentMode = savedModeBeforeDigital;
        lastEffectChangeTime = now;
    }
}

void loop() {
    handleDigitalOverlay();
static uint32_t lastUpdate = 0;
   static uint32_t lastButtonCheck = 0;
   static uint32_t lastMatrixUpdate = 0;
   uint32_t currentMillis = millis();
  
   if(WiFi.status() == WL_CONNECTED) {
       ArduinoOTA.handle();
       events();
       
       // Gestisci le richieste del server web
       if(webInitialized) {
           server.handleClient();
       }
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



// Gestione luminosità giorno/sera e spegnimento notturno
static bool wasNightTime = false;
static bool wasSleepTime = false;
bool currentlyNightTime = isNightTime();
bool currentlySleepTime = isSleepTime();

   // Gestione spegnimento notturno automatico
   if (currentlySleepTime != wasSleepTime) {
       if (currentlySleepTime) {
           Serial.println("Entrata in modalità spegnimento notturno");
           displayOff = true;
           FastLED.clear();
           FastLED.show();
       } else {
           Serial.println("Uscita da modalità spegnimento notturno");
           displayOff = false;
           intBrightness = isNightTime() ? nightBrightness : dayBrightness;
           FastLED.setBrightness(intBrightness);
           updateDisplay();
       }
       wasSleepTime = currentlySleepTime;
   }
    // Aggiorna la luminosità solo quando cambia la fascia oraria (e non siamo in sleep)
   if (!currentlySleepTime && currentlyNightTime != wasNightTime) {
       intBrightness = currentlyNightTime ? nightBrightness : dayBrightness;
       FastLED.setBrightness(intBrightness);
       Serial.print("Cambio fascia oraria - nuova luminosità: ");
       Serial.println(intBrightness);
       wasNightTime = currentlyNightTime;
   }
    // Applica la luminosità solo se non siamo in modalità sleep
   if (!currentlySleepTime) {
       FastLED.setBrightness(intBrightness);
   }

if (!displayOff) {
    // --- Disegno modalità corrente ---
    if(currentMode == MODE_MATRIX) {
        if(currentMillis - lastMatrixUpdate > 16) { updateMatrix(); lastMatrixUpdate = currentMillis; }
    }
    else if(currentMode == MODE_MATRIX2) {
        if(currentMillis - lastMatrixUpdate > 16) { updateMatrix2(); lastMatrixUpdate = currentMillis; }
    }
    else if(currentMode == MODE_GALAGA) {
        updateGalagaEffect();
    }
    else if(currentMode == MODE_PACMAN) {
        updatePacmanEffect();
    }
    else if(currentMode == MODE_TRON) {
        updateTronEffect();
    }
    else if(currentMode == MODE_MOTO) {
        updateMotoEffect();
    }
    else if(currentMode == MODE_SCROLLTEXT) {
        updateScrollText();
    }
    else if(currentMode == MODE_DIGITALE) {
        updateDigitaleEffect();
    }
    else {
        updateDisplay();
    }
    yield();
}
   yield();
}

// [RESTANTE DEL CODICE RIMANE INVARIATO...]

void checkButtons() {
   static uint32_t lastModePress = 0;
   static uint32_t lastColorPress = 0;
   static uint32_t lastBothPress = 0;
   static uint32_t bothPressStart = 0;
   static uint32_t modePressStart = 0;
   static uint32_t colorPressStart = 0;
   const uint32_t DEBOUNCE_TIME = 300;
   const uint32_t RESET_WIFI_TIME = 5000;
   const uint32_t DIGITAL_TOGGLE_TIME = 6000; // pressione lunga per toggle overlay digitale

  #if BUTTON_LOGIC_INVERTED == 1
   bool isModeButtonPressed = !digitalRead(BUTTON_MODE);
   bool isColorButtonPressed = !digitalRead(BUTTON_SEC);
  #else
   bool isModeButtonPressed = digitalRead(BUTTON_MODE);
   bool isColorButtonPressed = digitalRead(BUTTON_SEC);
  #endif

   uint32_t now = millis();

   // 1) Gestione pressione contemporanea per reset WiFi (rimane prioritaria)
   if (isModeButtonPressed && isColorButtonPressed) {
       if (bothPressStart == 0) {
           bothPressStart = now;
           Serial.println("Iniziato conteggio per reset WiFi...");
       }
       if (now - bothPressStart >= RESET_WIFI_TIME) {
           resetWiFi();
       }
       // mentre sono entrambi premuti, non gestiamo altro
       return;
   } else {
       bothPressStart = 0;
   }

   // 2) Gestione pressione prolungata di UNO SOLO dei due pulsanti per toggle overlay digitale
   //    Se uno solo è premuto a lungo, toggliamo digitalOverlayEnabled
   if (isModeButtonPressed && !isColorButtonPressed) {
       if (modePressStart == 0) modePressStart = now;
       // Se tiene premuto per >= 6s e non è stato appena usato per qualcos'altro
       if (now - modePressStart >= DIGITAL_TOGGLE_TIME) {
           digitalOverlayEnabled = !digitalOverlayEnabled;
           EEPROM.write(EEPROM_DIGITAL_OVERLAY_ADDR, digitalOverlayEnabled ? 1 : 0);
           EEPROM.commit();
           Serial.print("Overlay digitale ");
           Serial.println(digitalOverlayEnabled ? "ABILITATO (da pulsante MODE)" : "DISABILITATO (da pulsante MODE)");
           // Evita retrigger continuo finché resta premuto
           modePressStart = now + 60000;
       }
   } else {
       // rilascio pulsante MODE
       modePressStart = 0;
   }

   if (isColorButtonPressed && !isModeButtonPressed) {
       if (colorPressStart == 0) colorPressStart = now;
       if (now - colorPressStart >= DIGITAL_TOGGLE_TIME) {
           digitalOverlayEnabled = !digitalOverlayEnabled;
           EEPROM.write(EEPROM_DIGITAL_OVERLAY_ADDR, digitalOverlayEnabled ? 1 : 0);
           EEPROM.commit();
           Serial.print("Overlay digitale ");
           Serial.println(digitalOverlayEnabled ? "ABILITATO (da pulsante SEC)" : "DISABILITATO (da pulsante SEC)");
           colorPressStart = now + 60000;
       }
   } else {
       colorPressStart = 0;
   }

   // 3) Gestione pressioni brevi (come prima)
   if (isModeButtonPressed && !isColorButtonPressed) {
       if (now - lastBothPress > DEBOUNCE_TIME && now - modePressStart < DIGITAL_TOGGLE_TIME) {
           // pressione breve: cambia preset
           currentPreset = (currentPreset + 1) % 19;  // 0-18, include Digitale
           applyPreset(currentPreset);
           EEPROM.write(EEPROM_PRESET_ADDR, currentPreset);
           EEPROM.commit();
           lastBothPress = now;
       }
   }

   if (isColorButtonPressed && !isModeButtonPressed) {
       if (now - lastBothPress > DEBOUNCE_TIME && now - colorPressStart < DIGITAL_TOGGLE_TIME) {
           // pressione breve: toggle lampeggio
           currentBlink = (currentBlink == 0) ? 1 : 0;
           EEPROM.write(EEPROM_BLINK_ADDR, currentBlink);
           EEPROM.commit();
           lastBothPress = now;
       }
   }
}


void applyPreset(uint8_t preset) {
   // reset overlay digitale quando cambio preset
   digitalOverlayActive = false;
   waitingForDigital = false;
   lastEffectChangeTime = millis();

   switch(preset) {
       case 0:
           currentMode = MODE_SLOW;
           randomColor = true ;
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
           randomColor= false;
           clockColors.current = CRGB(255,165,0); // slow arancione
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
           randomColor= false;
           clockColors.current = CRGB::Blue; // slow blu
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
       
       case 18:
           currentMode = MODE_DIGITALE;
           lastDigitaleUpdate = 0;
           break;     
       
       default:
           currentMode = MODE_SLOW;
           randomColor = true;
           clockColors.current = CRGB(random8(), random8(), random8());
           break;
   }
   
   FastLED.setBrightness(intBrightness);  // Mantiene la luminosità corretta
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

       int yInt = (int)drop.y;

       if(yInt >= 0 && yInt < MATRIX_HEIGHT) {
           uint16_t pos = xyToLED(drop.x, yInt);

           if(pos < NUM_LEDS) {
               if(targetPixels[pos] && !activePixels[pos]) {
                   activePixels[pos] = true;
                   leds[pos] = matrixWordColor;
               }
               else if(!targetPixels[pos]) {
                   uint8_t intensity = 255 - (yInt * 16);
                   leds[pos] = CRGB(0, intensity, 0);

                   for(int trail = 1; trail <= MATRIX_TRAIL_LENGTH; trail++) {
                       int trailY = yInt - trail;
                       if(trailY >= 0) {
                           uint16_t trailPos = xyToLED(drop.x, trailY);
                           if(trailPos < NUM_LEDS && !targetPixels[trailPos]) {
                               leds[trailPos] = CRGB(0, intensity/(trail*2), 0);
                           }
                       }
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

        int yInt = (int)drop.y;

        if(yInt >= 0 && yInt < MATRIX_HEIGHT) {
            uint16_t pos = xyToLED(drop.x, yInt);

            if(pos < NUM_LEDS) {
                if(targetPixels[pos] && !activePixels[pos]) {
                    activePixels[pos] = true;
                    leds[pos] = matrixWordColor;
                }
                else if(!targetPixels[pos]) {
                    uint8_t intensity = 255 - (yInt * 16);
                    leds[pos] = CRGB(0, intensity, 0);

                    for(int trail = 1; trail <= MATRIX_TRAIL_LENGTH; trail++) {
                        int trailY = yInt - trail;
                        if(trailY >= 0) {
                            uint16_t trailPos = xyToLED(drop.x, trailY);
                            if(trailPos < NUM_LEDS && !targetPixels[trailPos]) {
                                leds[trailPos] = CRGB(0, intensity/(trail*2), 0);
                            }
                        }
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
