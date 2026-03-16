// OraQuadra V1.2.5  -  By Davide Gatti SURVIVAL HACKING  www.survivalhacking.it
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
// V1.2.3 - 27/11/2025 - By Marco Prunca
// Sistemati vari bug che facevano funzionare male la parte WEB
//
// V1.2.4 - 02/12/2025 - By Leotech e Paolo Sambinello
// Aggiunta funzione Testo scorrevole da web server
// Aggiunta possibilità di attivare/disattivare il quadrante digitale che si attiva ogni 6 sec per rendere visibile l'ora per postazioni più lontane. 
// L'attivazione/disattivazione del quadrante digitale che si alterna agli effetti selezionati può avvenire o da web server o mediante la pressione prolungata (6 sec) di uno dei 2 pulsanti. 
// Aggiunta palette di colori avanzata per selezionare molteplici più colori del testo scorrevole o delle parole di alcuni effetti.
// Aggiunti possibilità di selezionare varie dimensioni di font
// Aggiunto effetto rainbow
// Sistemati e migliorati effetti goccie a tron
//
// V1.2.5 - 07/12/2025 - By Davide Gatti
// Gestione testo scorrevole come entità a parte e non come preset
// Aggiunta palette colori per il testo scorrevole
// Aggiunta modalità per mostrare ora in formato testuale
// Sistemata modalità notturna e luminosità giornaliera del testo scorrevole.
//
// V1.2.6 - 10/12/2025 - By leotech
// Sistemato un problema che impediva la memorizzazione della velocità di scorrimento del testo con la data
//
// V1.2.7 - 23/12/2025 - By Davide Gatti
// Sistemato un problema che a volte mostrava un ora sbagliata in quanto non conteggiava correttamente l'ora legale.
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
#include <FastLED.h>     // https://github.com/FastLED/FastLED (you need to install version 3.10.2 or lower)
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
#define BUTTON_LOGIC_INVERTED 1

// Pin e configurazione LED ESP32C3
#define LED_PIN      5     // Pin per matrice LED
#define BUTTON_MODE  6     // Pulsante modi
#define BUTTON_SEC   7     // Pulsante lampeggio secondo

#define NUM_LEDS    256   // Totale LED matrice
#define LED_TYPE    WS2812B
#define COLOR_ORDER GRB
#define BRIGHTNESS_DAY   64
#define BRIGHTNESS_NIGHT 32
#define MATRIX_WIDTH  16
#define MATRIX_HEIGHT 16

// Modalità visualizzazione / Display modes
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
#define MODE_DROP 10    // Effetto goccia con onde concentriche
#define MODE_RAINBOW 11 // Effetto arcobaleno graduale
#define NUM_MODES   11  

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

// Strutture e costanti per EEPROM / EEPROM structures and constants
#define EEPROM_SIZE 512
#define EEPROM_DIGITAL_OVERLAY_ADDR 40
#define EEPROM_DIGITAL_START_ADDR 41
#define EEPROM_DIGITAL_DURATION_ADDR 42
#define EEPROM_SCROLL_PAUSE_ADDR 43
#define EEPROM_SCROLL_SPEED_ADDR       44   // LOW BYTE
#define EEPROM_SCROLL_SPEED_ADDR_H     23   // HIGH BYTE (NUOVO → FIX BUG 306ms)
#define EEPROM_SCROLL_SHOW_DATE_ADDR 45
#define EEPROM_SCROLL_SHOW_TIME_ADDR 46
#define EEPROM_SCROLL_SHOW_TEXT_ADDR 47     // Mostra testo personalizzato
#define EEPROM_SCROLL_TEXT_LEN_ADDR 48      // Lunghezza del testo (1 byte, max 200)
#define EEPROM_SCROLL_TEXT_START_ADDR 49    // Inizio testo scorrevole (49-248, max 200 caratteri)
#define EEPROM_SCROLL_TEXT_MAX_LEN 200
#define EEPROM_SCROLL_FONT_SIZE_ADDR 249    // Dimensione font testo scorrevole (1=piccolo, 2=medio, 3=grande)
// Impostazioni per modalità: 250-349 (20 preset × 5 byte = 100 byte)
// Ogni preset: Marker(1) + R(1) + G(1) + B(1) + Rainbow(1) = 5 byte
#define EEPROM_MODE_SETTINGS_START 250
#define EEPROM_MODE_SETTINGS_SIZE 5         // Byte per ogni modalità
#define EEPROM_MODE_MARKER 0xAA             // Marker per indicare che la modalità è stata salvata
#define NUM_MODES 20                        // Numero totale di preset (0-19)
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
#define EEPROM_SCROLL_SHOW_PRESET_ADDR 350    // Mostra preset durante pausa
#define EEPROM_SCROLL_TEXTUAL_DATETIME_ADDR 351 // Data/ora testuale
#define EEPROM_SCROLL_ENABLED_ADDR 352
#define EEPROM_SCROLLTEXT_R_ADDR 353
#define EEPROM_SCROLLTEXT_G_ADDR 354
#define EEPROM_SCROLLTEXT_B_ADDR 355
#define EEPROM_SCROLLTEXT_RAINBOW_ADDR 356
#define GIORNO 64     // full on
#define NOTTE 32       // half on


bool scrollShowPresetDuringPause = true;      // Mostra preset durante pausa
bool scrollTextualDateTime = false;           // Mostra data/ora in formato testuale
uint8_t savedPresetBeforeScroll = MODE_FAST;  // Preset da ripristinare dopo scroll
bool scrollEnabled = false;                   // Scroll abilitato
uint8_t scrollEnabledRaw = 0;                 // Scroll abilitato RAW


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
void showCurrentTime();
void displayWord(const uint8_t* word, CRGB color);
void showMinutes(uint8_t minutes, CRGB color);
void showSeconds(uint8_t seconds, CRGB color);
void handleSetScrollEnabled(); // handler per settaggio scroll ON/OFF via web

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
bool randomColor = false;


// Variabili per effetto TRON
#define NUM_TRON_BIKES 3
#define TRON_TRAIL_LENGTH 12
#define TRON_UPDATE_INTERVAL 80  // ms tra ogni movimento

struct TronBike {
    int8_t x;
    int8_t y;
    int8_t dir;        // 0=su, 1=destra, 2=giù, 3=sinistra
    CRGB color;
    bool active;
    uint8_t trailX[TRON_TRAIL_LENGTH];
    uint8_t trailY[TRON_TRAIL_LENGTH];
    uint8_t trailLen;
};

TronBike tronBikes[NUM_TRON_BIKES];
uint8_t tronIntensity[NUM_LEDS] = {0};
uint32_t lastTronUpdate = 0;
uint32_t tronCycleStart = 0;
const uint32_t TRON_EFFECT_DURATION = 8000;
const uint32_t TEXT_ONLY_DURATION = 10000;
bool tronEffectActive = true;
bool tronNeedsInit = true;

// Variabili per effetto GOCCIA (onde concentriche)
#define DROP_UPDATE_INTERVAL 60      // ms tra i frame (più lento)
#define DROP_MAX_RADIUS 24           // raggio massimo onde
#define DROP_WAVE_SPEED 0.4f         // velocità espansione (più lento)

struct DropState {
    bool active = false;
    bool completed = false;
    float radius = 0;                // raggio corrente dell'onda
    uint8_t centerX = 8;             // centro X della goccia
    uint8_t centerY = 8;             // centro Y della goccia
    uint32_t lastUpdate = 0;
    uint32_t startTime = 0;
    CRGB waveColor;
    uint8_t waveCount = 0;           // numero di onde create
} dropState;

// Variabili per effetto RAINBOW
uint8_t rainbowHue = 0;
uint32_t lastRainbowUpdate = 0;
#define RAINBOW_SPEED 50  // ms tra aggiornamenti
bool textRainbowMode = false;  // true = colore rainbow per il testo (ogni lettera colore diverso)

// Variabili per effetto MOTO
uint32_t lastMotoUpdate = 0;
uint16_t motoCurrentPosition = 0;
bool motoEffectActive = false;
bool motoCompleted = false;
uint32_t motoCompletionTime = 0;
CRGB motoTrailColors[MOTO_TRAIL_LENGTH];
uint8_t motoTrailHue = 0;
uint8_t motoTextHue = 0;
CRGB motoTextColor = CRGB::White;
bool motoTextPixels[NUM_LEDS] = {false};
// Posizione e direzione moto per zigzag
int8_t motoX = 0;
int8_t motoY = 0;
int8_t motoDirX = 1;    // direzione orizzontale
int8_t motoDirY = 1;    // direzione verticale (per quando cambia riga)
uint16_t motoTrailX[MOTO_TRAIL_LENGTH];
uint16_t motoTrailY[MOTO_TRAIL_LENGTH];
uint8_t motoTrailLen = 0;

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

// Variabili per effetto SLOW (non-bloccante)
struct SlowEffectState {
    bool active = false;
    bool completed = false;     // true quando l'effetto è finito, aspetta cambio minuto
    uint8_t step = 0;
    uint8_t targetBrightness = 0;
    uint8_t increment = 1;
    uint32_t lastStepTime = 0;
    bool holdPhase = false;
    uint32_t holdStartTime = 0;
    CRGB sonoColor;
    CRGB hourColor;
    CRGB eColor;
    CRGB minutesColor;
    CRGB minutiColor;
} slowState;

// Variabili per effetto FADE (non-bloccante)
struct FadeEffectState {
    bool active = false;
    bool completed = false;     // true quando l'effetto è finito, aspetta cambio minuto
    uint8_t phase = 0;          // 0=sono, 1=hour, 2=e, 3=minutes, 4=minuti, 5=done
    uint8_t step = 0;
    uint32_t lastStepTime = 0;
    static const uint8_t STEPS = 50;  // 50 step per fade graduale (~2.5 sec per parola)
} fadeState;

// Variabili per effetto SCROLLTEXT
String customScrollText = "";
bool scrollTextInitialized = false;
int scrollTextOffset = 0;
int scrollTextTotalWidth = 0;
uint32_t lastScrollTextStep = 0;
uint16_t scrollTextSpeedMs = 50;          // Velocità iniziale del testo scorrevole (ms tra i frame)
uint8_t scrollPauseSeconds = 3;           // Pausa in secondi tra un passaggio e l'altro
bool scrollPauseActive = false;           // true quando siamo in pausa tra i passaggi
uint32_t scrollPauseStart = 0;            // Quando è iniziata la pausa
uint8_t lastPresetDuringPause = 255;      // preset durante la pausa
uint32_t pauseDelayStart = 0;             // 
bool pauseDelayActive = false;            // 

// Colore specifico per il testo scorrevole / Specific color for scrolling text
CRGB scrollTextColor = CRGB::White;       // 
bool scrollTextRainbow = false;           // 

bool scrollShowText = true;               // Mostra testo personalizzato nel testo scorrevole
bool scrollShowDate = false;              // Mostra data nel testo scorrevole
bool scrollShowTime = false;              // Mostra ora nel testo scorrevole
uint8_t scrollFontSize = 2;               // Dimensione font: 1=piccolo(3x5), 2=medio(5x7), 3=grande(8x11)

// Overlay digitale automatico (dopo fine effetto + 1.5s -> 2.5s digitale -> ritorna all'effetto precedente)
bool digitalOverlayActive = false;        // true quando siamo in MODE_DIGITALE per overlay
bool waitingForDigital = false;           // true quando l'effetto ha finito e stiamo aspettando 1.5s
bool manualModeSelected = false;          // true quando modalità selezionata manualmente (no overlay)
uint32_t digitalOverlayStart = 0;         // quando è iniziato il digitale
uint32_t digitalDelayStart = 0;           // quando è finito l'effetto (inizio attesa 1.5s)
uint32_t lastEffectChangeTime = 0;        // quando è stato selezionato l'effetto corrente
uint8_t savedModeBeforeDigital = MODE_FAST; // effetto da ripristinare dopo overlay
uint32_t lastDigitaleUpdate = 0;
#define DIGITALE_UPDATE_INTERVAL 500      // Aggiorna ogni 500ms
uint8_t digitaleInterval = 30;            // Ogni quanti secondi appare l'overlay digitale
uint8_t digitaleDuration = 5;             // Per quanti secondi rimane visibile
uint32_t lastDigitaleTrigger = 0;         // Ultimo momento in cui è partito l'overlay
#define DEFAULT_EFFECT_CYCLE_MS 45000     // Fallback per effetti continui senza "fine" (45 secondi)

// Definizione array LED e controller
CRGB leds[NUM_LEDS];
CRGB matrixWordColor = CRGB::Blue;
Timezone myTZ;

// Server web per interfaccia
WebServer server(80);
bool webInitialized = false;

// Variabili globali stato / Global state variables
uint8_t currentMode = MODE_FAST;
uint8_t currentHour = 0;
uint8_t currentMinute = 0;
uint8_t currentSecond = 0;
uint8_t currentBlink = 0;
uint8_t prevHour = 255;
uint8_t prevMinute = 255;
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

//Definizione matrice parole in PROGMEM / Word matrix definition in PROGMEM
const uint8_t PROGMEM WORD_SONO_LE[] = {15,14,13,12,10,9,7,6,5,8};
const uint8_t PROGMEM WORD_MINUTI[] = {250,251,252,253,254,255,8};

// Parole ore / Hour words
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

// Array delle parole ore per lookup veloce / Hour words array for fast lookup
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

// Definizioni minute words in PROGMEM / Minute words definitions in PROGMEM
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

// Array di lookup per i minuti / Lookup array for minutes
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

// Struttura per le decine / Structure for tens
struct MinuteTens {
   const uint8_t* normal;
   const uint8_t* truncated;
};

// Array di lookup per le decines / Lookup array for tens
const MinuteTens PROGMEM TENS_WORDS[] = {
   {WORD_MVENTI, WORD_MVENT},
   {WORD_MTRENTA, WORD_MTRENT},
   {WORD_MQUARANTA, WORD_MQUARANT},
   {WORD_MCINQUANTA, WORD_MCINQUANT}
};

// Struttura per gestione colori / Color management structure
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
       if (textRainbowMode) {
           // In modalità rainbow, restituisce un colore che cambia nel tempo / In rainbow mode, returns a time-changing color
           return CHSV(rainbowHue, 255, 255);
       }
       return CRGB(scale8(current.r, brightness),
                  scale8(current.g, brightness),
                  scale8(current.b, brightness));
   }

   // Colore rainbow basato su posizione / Rainbow color based on position
   CRGB getRainbowColor(uint8_t position) const {
       uint8_t hue = (position * 25) % 256;
       return CHSV(hue, 255, 255);
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

// ============================================================================
// FUNZIONI PER GESTIONE IMPOSTAZIONI PER MODALITÀ
// ============================================================================

// Salva le impostazioni correnti / Save current settings
void saveModeSettings(uint8_t mode) {
    if (mode >= NUM_MODES) return;
    uint16_t baseAddr = EEPROM_MODE_SETTINGS_START + (mode * EEPROM_MODE_SETTINGS_SIZE);
    EEPROM.write(baseAddr, EEPROM_MODE_MARKER);  // Marker per indicare che è stato salvato
    EEPROM.write(baseAddr + 1, clockColors.current.r);
    EEPROM.write(baseAddr + 2, clockColors.current.g);
    EEPROM.write(baseAddr + 3, clockColors.current.b);
    EEPROM.write(baseAddr + 4, textRainbowMode ? 1 : 0);
    EEPROM.commit();
    Serial.print("Salvate impostazioni per modalita ");
    Serial.print(mode);
    Serial.print(" - R=");
    Serial.print(clockColors.current.r);
    Serial.print(" G=");
    Serial.print(clockColors.current.g);
    Serial.print(" B=");
    Serial.print(clockColors.current.b);
    Serial.print(" Rainbow=");
    Serial.println(textRainbowMode);
}

// per la modalità specificata / for specified mode
void loadModeSettings(uint8_t mode) {
    if (mode >= NUM_MODES) return;
    uint16_t baseAddr = EEPROM_MODE_SETTINGS_START + (mode * EEPROM_MODE_SETTINGS_SIZE);

    uint8_t marker = EEPROM.read(baseAddr);

    // Verifica se questa modalità ha impostazioni salvate / Check if this mode has saved settings
    if (marker != EEPROM_MODE_MARKER) {
        // Nessuna impostazione salvata / No saved settings
        Serial.print("Nessuna impostazione salvata per modalita ");
        Serial.println(mode);
        return;
    }

    uint8_t r = EEPROM.read(baseAddr + 1);
    uint8_t g = EEPROM.read(baseAddr + 2);
    uint8_t b = EEPROM.read(baseAddr + 3);
    uint8_t rainbow = EEPROM.read(baseAddr + 4);
    clockColors.current = CRGB(r, g, b);
    textRainbowMode = (rainbow == 1);

}

// Font 5x7 per caratteri - definito in PROGMEM per risparmiare RAM / Font 5x7 for characters - defined in PROGMEM to save RAM
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

// Font 3x5 per caratteri piccoli - definito in PROGMEM per risparmiare RAM / Font 3x5 for small characters - defined in PROGMEM to save RAM
const uint8_t PROGMEM font3x5[][3] = {
  {0x00, 0x00, 0x00}, // SPACE (32)
  {0x00, 0x17, 0x00}, // ! (33)
  {0x03, 0x00, 0x03}, // " (34)
  {0x1F, 0x0A, 0x1F}, // # (35)
  {0x16, 0x1F, 0x0D}, // $ (36)
  {0x19, 0x04, 0x13}, // % (37)
  {0x0A, 0x15, 0x1A}, // & (38)
  {0x00, 0x03, 0x00}, // ' (39)
  {0x00, 0x0E, 0x11}, // ( (40)
  {0x11, 0x0E, 0x00}, // ) (41)
  {0x0A, 0x04, 0x0A}, // * (42)
  {0x04, 0x0E, 0x04}, // + (43)
  {0x10, 0x08, 0x00}, // , (44)
  {0x04, 0x04, 0x04}, // - (45)
  {0x00, 0x10, 0x00}, // . (46)
  {0x18, 0x04, 0x03}, // / (47)
  {0x1F, 0x11, 0x1F}, // 0 (48)
  {0x12, 0x1F, 0x10}, // 1 (49)
  {0x1D, 0x15, 0x17}, // 2 (50)
  {0x15, 0x15, 0x1F}, // 3 (51)
  {0x07, 0x04, 0x1F}, // 4 (52)
  {0x17, 0x15, 0x1D}, // 5 (53)
  {0x1F, 0x15, 0x1D}, // 6 (54)
  {0x01, 0x1D, 0x03}, // 7 (55)
  {0x1F, 0x15, 0x1F}, // 8 (56)
  {0x17, 0x15, 0x1F}, // 9 (57)
  {0x00, 0x0A, 0x00}, // : (58)
  {0x10, 0x0A, 0x00}, // ; (59)
  {0x04, 0x0A, 0x11}, // < (60)
  {0x0A, 0x0A, 0x0A}, // = (61)
  {0x11, 0x0A, 0x04}, // > (62)
  {0x01, 0x15, 0x03}, // ? (63)
  {0x0E, 0x15, 0x16}, // @ (64)
  {0x1E, 0x05, 0x1E}, // A (65)
  {0x1F, 0x15, 0x0A}, // B (66)
  {0x0E, 0x11, 0x11}, // C (67)
  {0x1F, 0x11, 0x0E}, // D (68)
  {0x1F, 0x15, 0x15}, // E (69)
  {0x1F, 0x05, 0x05}, // F (70)
  {0x0E, 0x11, 0x1D}, // G (71)
  {0x1F, 0x04, 0x1F}, // H (72)
  {0x11, 0x1F, 0x11}, // I (73)
  {0x18, 0x10, 0x1F}, // J (74)
  {0x1F, 0x04, 0x1B}, // K (75)
  {0x1F, 0x10, 0x10}, // L (76)
  {0x1F, 0x02, 0x1F}, // M (77)
  {0x1F, 0x0E, 0x1F}, // N (78)
  {0x0E, 0x11, 0x0E}, // O (79)
  {0x1F, 0x05, 0x02}, // P (80)
  {0x0E, 0x19, 0x1E}, // Q (81)
  {0x1F, 0x05, 0x1A}, // R (82)
  {0x12, 0x15, 0x09}, // S (83)
  {0x01, 0x1F, 0x01}, // T (84)
  {0x0F, 0x10, 0x0F}, // U (85)
  {0x07, 0x18, 0x07}, // V (86)
  {0x1F, 0x08, 0x1F}, // W (87)
  {0x1B, 0x04, 0x1B}, // X (88)
  {0x03, 0x1C, 0x03}, // Y (89)
  {0x19, 0x15, 0x13}, // Z (90)
};

// Funzione per ottenere un carattere dal font piccolo 3x5 / Function to get a character from small 3x5 font
void getCharBitmapSmall(char c, uint8_t* bitmap) {
  if (c >= 32 && c <= 90) {
    for (int i = 0; i < 3; i++) {
      bitmap[i] = pgm_read_byte(&font3x5[c - 32][i]);
    }
  } else {
    for (int i = 0; i < 3; i++) {
      bitmap[i] = 0x00;
    }
  }
}

// Funzione per ottenere un carattere dal font medio 5x7 / Function to get a character from medium 5x7 font
void getCharBitmap(char c, uint8_t* bitmap) {
  if (c >= 32 && c <= 90) {
    for (int i = 0; i < 5; i++) {
      bitmap[i] = pgm_read_byte(&font5x7[c - 32][i]);
    }
  } else {
    // Carattere non supportato - spazio vuoto / Unsupported character - empty space
    for (int i = 0; i < 5; i++) {
      bitmap[i] = 0x00;
    }
  }
}

// Funzione per disegnare un carattere sulla matrice in una posizione specifica / Function to draw a character on matrix at specific position
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

// Funzione per disegnare una cifra centrata in una area / Function to draw a centered digit in an area
void drawDigitCentered(uint8_t x, uint8_t y, uint8_t digit, CRGB color) {
  char digitChar = '0' + digit;
  drawChar5x7(x, y, digitChar, color);
}

// Funzione per disegnare i due punti tra ore e minuti / Function to draw colon between hours and minutes
void drawColon(uint8_t x, uint8_t y, CRGB color, bool blink) {
  // I due punti sono composti da 2 pixel / Colon is composed of 2 pixels
  if (!blink || (millis() / 500) % 2 == 0) {
    uint16_t topPos = xyToLED(x, y + 2);
    uint16_t bottomPos = xyToLED(x, y + 4);
    if (topPos < NUM_LEDS) leds[topPos] = color;
    if (bottomPos < NUM_LEDS) leds[bottomPos] = color;
  }
}

// Funzione per convertire un pixel / Function to convert a pixel
uint16_t xyToLED(uint8_t x, uint8_t y) {
  // La matrice è mappata in serpentina / Matrix is mapped in serpentine
  if (y % 2 == 0) {
    // Riga pari: da destra a sinistra / Even row: right to left
    return y * MATRIX_WIDTH + (MATRIX_WIDTH - 1 - x);
  } else {
    // Riga dispari: da sinistra a destra / Odd row: left to right
    return y * MATRIX_WIDTH + x;
  }
}

// Funzione per visualizzare testo scorrevole con dimensione font dinamica / Function to display scrolling text with dynamic font size
void scrollText(String text, CRGB color, uint16_t scrollSpeed) {
  text.toUpperCase(); // Converti tutto in maiuscolo

  // Aggiungi spazi all'inizio e alla fine per l'effetto di entrata/uscita / Add spaces at start and end for entry/exit effect
  text = "    " + text + "    ";

  int textLength = text.length();

  // Parametri in base alla dimensione del font / Parameters based on font size
  int charWidth, charHeight, charSpacing, vertOffset;

  int scale = 1;
  switch (scrollFontSize) {
    case 1: // Piccolo 3x5
      charWidth = 3;
      charHeight = 5;
      charSpacing = 4; // 3 + 1
      vertOffset = 5;  // Centra verticalmente
      break;
    case 3: // Grande (font 5x7 scalato 2x)
      charWidth = 5;
      charHeight = 7;
      charSpacing = 6;
      vertOffset = 1;  // Centra verticalmente (16-14)/2 = 1
      scale = 2;
      break;
    default: // Medio 5x7
      charWidth = 5;
      charHeight = 7;
      charSpacing = 6; // 5 + 1
      vertOffset = 4;  // Centra verticalmente
      break;
  }

  int effectiveCharSpacing = charSpacing * scale;
  int totalWidth = textLength * effectiveCharSpacing;

  // Scorri il testo da destra a sinistra / Scroll text from right to left
  for (int offset = MATRIX_WIDTH; offset > -totalWidth; offset--) {
    FastLED.clear();

    // Disegna ogni carattere / Draw each character
    for (int charIndex = 0; charIndex < textLength; charIndex++) {
      char c = text.charAt(charIndex);
      int charX = offset + (charIndex * effectiveCharSpacing);

      if (scrollFontSize == 1) {
        // Font piccolo 3x5 / Small 3x5 font
        uint8_t charBitmap[3];
        getCharBitmapSmall(c, charBitmap);

        if (charX + charWidth >= 0 && charX < MATRIX_WIDTH) {
          for (int col = 0; col < charWidth; col++) {
            int screenX = charX + col;
            if (screenX >= 0 && screenX < MATRIX_WIDTH) {
              for (int row = 0; row < charHeight; row++) {
                if (charBitmap[col] & (1 << row)) {
                  int screenY = row + vertOffset;
                  if (screenY < MATRIX_HEIGHT) {
                    uint16_t ledPos = xyToLED(screenX, screenY);
                    leds[ledPos] = color;
                  }
                }
              }
            }
          }
        }
      } else if (scrollFontSize == 3) {
        // Font grande (5x7 scalato 2x = 10x14)
        uint8_t charBitmap[5];
        getCharBitmap(c, charBitmap);
        int effectiveCharWidth = charWidth * scale;

        if (charX + effectiveCharWidth >= 0 && charX < MATRIX_WIDTH) {
          for (int col = 0; col < charWidth; col++) {
            for (int sx = 0; sx < scale; sx++) {
              int screenX = charX + (col * scale) + sx;
              if (screenX >= 0 && screenX < MATRIX_WIDTH) {
                for (int row = 0; row < charHeight; row++) {
                  if (charBitmap[col] & (1 << row)) {
                    for (int sy = 0; sy < scale; sy++) {
                      int screenY = (row * scale) + sy + vertOffset;
                      if (screenY >= 0 && screenY < MATRIX_HEIGHT) {
                        uint16_t ledPos = xyToLED(screenX, screenY);
                        leds[ledPos] = color;
                      }
                    }
                  }
                }
              }
            }
          }
        }
      } else {
        // Font medio 5x7 / Medium 5x7 font
        uint8_t charBitmap[5];
        getCharBitmap(c, charBitmap);

        if (charX + charWidth >= 0 && charX < MATRIX_WIDTH) {
          for (int col = 0; col < charWidth; col++) {
            int screenX = charX + col;
            if (screenX >= 0 && screenX < MATRIX_WIDTH) {
              for (int row = 0; row < charHeight; row++) {
                if (charBitmap[col] & (1 << row)) {
                  int screenY = row + vertOffset;
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
    }

    FastLED.show();
    delay(scrollSpeed);
    yield(); // Permette al WiFi di gestire le sue operazioni
  }
}

// Nomi dei giorni della settimana in italiano / Italian weekday names
const char* giorniSettimana[] = {"DOMENICA", "LUNEDI'", "MARTEDI'", "MERCOLEDI'", "GIOVEDI'", "VENERDI'", "SABATO"};
// Nomi dei mesi in italiano / Italian month names
const char* mesiAnno[] = {"GENNAIO", "FEBBRAIO", "MARZO", "APRILE", "MAGGIO", "GIUGNO",
                          "LUGLIO", "AGOSTO", "SETTEMBRE", "OTTOBRE", "NOVEMBRE", "DICEMBRE"};

// Funzione helper per costruire una stringa testuale dell'ora / Helper function to build textual time string
String buildTextualTime() {
    String result = "SONO LE ORE ";
    
    const char* hourNames[] = {
        "ZERO", "UNA", "DUE", "TRE", "QUATTRO", "CINQUE", "SEI", "SETTE",
        "OTTO", "NOVE", "DIECI", "UNDICI", "DODICI", "TREDICI", "QUATTORDICI",
        "QUINDICI", "SEDICI", "DICIASSETTE", "DICIOTTO", "DICIANNOVE", "VENTI",
        "VENTUNO", "VENTIDUE", "VENTITRE'"
    };
    
    // USA currentHour invece di hour()
    result += hourNames[currentHour];
    
    // USA currentMinute invece di minute()
    int m = currentMinute;
    if (m > 0) {
        result += " E ";
        
        if (m <= 19) {
            const char* minuteNames[] = {
                "", "UNO", "DUE", "TRE", "QUATTRO", "CINQUE", "SEI", "SETTE",
                "OTTO", "NOVE", "DIECI", "UNDICI", "DODICI", "TREDICI", "QUATTORDICI",
                "QUINDICI", "SEDICI", "DICIASSETTE", "DICIOTTO", "DICIANNOVE"
            };
            result += minuteNames[m];
        } else {
            uint8_t tens = m / 10;
            uint8_t ones = m % 10;
            
            const char* tensNames[] = {"", "", "VENTI", "TRENTA", "QUARANTA", "CINQUANTA"};
            
            if (ones == 1 || ones == 8) {
                const char* truncTensNames[] = {"", "", "VENT", "TRENT", "QUARANT", "CINQUANT"};
                result += truncTensNames[tens];
            } else {
                result += tensNames[tens];
            }
            
            if (ones > 0) {
                const char* onesNames[] = {
                    "", "UNO", "DUE", "TRE", "QUATTRO", "CINQUE", "SEI", "SETTE", "OTTO", "NOVE"
                };
                if (ones == 1 && m >= 21) {
                    result += "UN";
                } else {
                    result += onesNames[ones];
                }
            }
        }
        
        result += " MINUTI";
    }
    
    return result;
}

String buildTextualDate() {
    // USA myTZ.weekday(), myTZ.day(), myTZ.month(), myTZ.year()
    // invece di weekday(), day(), month(), year()
    int giornoSett = myTZ.weekday() - 1;
    if (giornoSett < 0) giornoSett = 0;
    if (giornoSett > 6) giornoSett = 6;

    int meseIdx = myTZ.month() - 1;
    if (meseIdx < 0) meseIdx = 0;
    if (meseIdx > 11) meseIdx = 11;

    String result = giorniSettimana[giornoSett];
    result += " ";
    
    int d = myTZ.day();
    const char* dayNames[] = {
        "", "UNO", "DUE", "TRE", "QUATTRO", "CINQUE", "SEI", "SETTE", "OTTO", "NOVE", "DIECI",
        "UNDICI", "DODICI", "TREDICI", "QUATTORDICI", "QUINDICI", "SEDICI", "DICIASSETTE",
        "DICIOTTO", "DICIANNOVE", "VENTI", "VENTUNO", "VENTIDUE", "VENTITRE", "VENTIQUATTRO",
        "VENTICINQUE", "VENTISEI", "VENTISETTE", "VENTOTTO", "VENTINOVE", "TRENTA", "TRENTUNO"
    };
    
    result += dayNames[d];
    result += " ";
    result += mesiAnno[meseIdx];
    result += " ";
    result += String(myTZ.year());
    
    return result;
}

// Funzione helper per costruire il testo completo con data/ora / Helper function to build complete text with date/time
String buildScrollDisplayText() {
    String parts[3];
    int numParts = 0;

    // Aggiungi data se abilitata / Add date if enabled
    if (scrollShowDate) {
        if (scrollTextualDateTime) {
            parts[numParts++] = buildTextualDate();
        } else {
            // Formato numerico / Numeric format
            int giornoSett = myTZ.weekday() - 1;
            if (giornoSett < 0) giornoSett = 0;
            if (giornoSett > 6) giornoSett = 6;
            
            String dateStr = giorniSettimana[giornoSett];
            dateStr += " ";
            if (myTZ.day() < 10) dateStr += "0";
            dateStr += String(myTZ.day());
            dateStr += "/";
            if (myTZ.month() < 10) dateStr += "0";
            dateStr += String(myTZ.month());
            dateStr += "/";
            dateStr += String(myTZ.year());
            parts[numParts++] = dateStr;
        }
    }

    // Aggiungi ora se abilitata / Add time if enabled
    if (scrollShowTime) {
        if (scrollTextualDateTime) {
            parts[numParts++] = buildTextualTime();
        } else {
            // Formato numerico / Numeric format
            char timeStr[6];
            snprintf(timeStr, sizeof(timeStr), "%02d:%02d", currentHour, currentMinute);
            parts[numParts++] = String(timeStr);
        }
    }

    // Aggiungi testo personalizzato / Add custom text
    if (scrollShowText && customScrollText.length() > 0) {
        parts[numParts++] = customScrollText;
    }

    // Costruisci il risultato unendo le parti con / Build result joining parts with
    String result = "";
    for (int i = 0; i < numParts; i++) {
        if (i > 0) result += " - ";
        result += parts[i];
    }

    if (result.length() == 0) result = " ";

    return result;
}

// Versione NON bloccante del testo scorrevole / NON-blocking version of scrolling text
void initScrollText() {
  // Usa buildScrollDisplayText per includere data/ora se attivi / Use buildScrollDisplayText to include date/time if active
  String t = buildScrollDisplayText();
  if (t.length() == 0) t = " ";
  t.toUpperCase();
  scrollTextTotalWidth = t.length() * 6;
  scrollTextOffset = MATRIX_WIDTH;
  scrollTextInitialized = true;
}

void resetScrollText() { /* reset interno testo scorrevole, se necessario */ }

void updateScrollText() {
    if (displayOff) {
        FastLED.clear();
        FastLED.show();
        return;
    }
   
    if (!scrollEnabled) {
        if (scrollTextInitialized) {
            scrollTextInitialized = false;
            scrollTextOffset = MATRIX_WIDTH;
        }
        return;
    }
    
    if (!scrollTextInitialized) {
        initScrollText();
    }
    
    // Se siamo in pausa tra i passaggi / If we are in pause between passes
    if (scrollPauseActive) {
        if (millis() - scrollPauseStart > (uint32_t)scrollPauseSeconds * 1000) {
            // Pausa finita / Pause finished
            scrollPauseActive = false;
            scrollTextOffset = MATRIX_WIDTH;
            scrollTextInitialized = false;
            lastPresetDuringPause = 255;
            pauseDelayActive = false;
        } else if (!pauseDelayActive) {
            // secondi / seconds
            if (millis() - scrollPauseStart < 500) {
                FastLED.clear();
                FastLED.show();
                return;
            } else {
                pauseDelayActive = true;  // Dopo 0,5 sec, mostra preset
            }
        }
    return;
}

    if (millis() - lastScrollTextStep < scrollTextSpeedMs) return;
    lastScrollTextStep = millis();

    // Costruisci il testo completo con data/ora se abilitati / Build complete text with date/time if enabled
    String t = buildScrollDisplayText();
    t.toUpperCase();

    // Parametri in base alla dimensione del font / Parameters based on font size
    int charWidth, charHeight, charSpacing, vertOffset;
    int scale = 1;

    switch (scrollFontSize) {
        case 1: // Piccolo 3x5
            charWidth = 3;
            charHeight = 5;
            charSpacing = 4;
            vertOffset = 5;
            break;
        case 3: // Grande
            charWidth = 5;
            charHeight = 7;
            charSpacing = 6;
            vertOffset = 1;
            scale = 2;
            break;
        default: // Medio 5x7
            charWidth = 5;
            charHeight = 7;
            charSpacing = 6;
            vertOffset = 4;
            break;
    }

    int effectiveCharSpacing = charSpacing * scale;
    scrollTextTotalWidth = t.length() * effectiveCharSpacing;

    FastLED.clear();
    int textLength = t.length();



    for (int charIndex = 0; charIndex < textLength; charIndex++) {
        char c = t.charAt(charIndex);
        int charX = scrollTextOffset + (charIndex * effectiveCharSpacing);

        CRGB charColor;
        if (scrollTextRainbow) {  // 
            uint8_t hue = (charIndex * 25) % 256;
            charColor = CHSV(hue, 255, 255);
        } else {
            charColor = scrollTextColor;  //  Usa colore specifico testo
        }

        if (scrollFontSize == 1) {
            uint8_t charBitmap[3];
            getCharBitmapSmall(c, charBitmap);

            if (charX + charWidth >= 0 && charX < MATRIX_WIDTH) {
                for (int col = 0; col < charWidth; col++) {
                    int screenX = charX + col;
                    if (screenX >= 0 && screenX < MATRIX_WIDTH) {
                        for (int row = 0; row < charHeight; row++) {
                            if (charBitmap[col] & (1 << row)) {
                                int screenY = row + vertOffset;
                                if (screenY >= 0 && screenY < MATRIX_HEIGHT) {
                                    uint16_t ledPos = xyToLED(screenX, screenY);
                                    if (ledPos < NUM_LEDS) leds[ledPos] = charColor;
                                }
                            }
                        }
                    }
                }
            }
        } else if (scrollFontSize == 3) {
            uint8_t charBitmap[5];
            getCharBitmap(c, charBitmap);
            int effectiveCharWidth = charWidth * scale;

            if (charX + effectiveCharWidth >= 0 && charX < MATRIX_WIDTH) {
                for (int col = 0; col < charWidth; col++) {
                    for (int sx = 0; sx < scale; sx++) {
                        int screenX = charX + (col * scale) + sx;
                        if (screenX >= 0 && screenX < MATRIX_WIDTH) {
                            for (int row = 0; row < charHeight; row++) {
                                if (charBitmap[col] & (1 << row)) {
                                    for (int sy = 0; sy < scale; sy++) {
                                        int screenY = (row * scale) + sy + vertOffset;
                                        if (screenY >= 0 && screenY < MATRIX_HEIGHT) {
                                            uint16_t ledPos = xyToLED(screenX, screenY);
                                            if (ledPos < NUM_LEDS) leds[ledPos] = charColor;
                                        }
                                    }
                                }
                            }
                        }
                    }
                }
            }
        } else {
            uint8_t charBitmap[5];
            getCharBitmap(c, charBitmap);

            if (charX + charWidth >= 0 && charX < MATRIX_WIDTH) {
                for (int col = 0; col < charWidth; col++) {
                    int screenX = charX + col;
                    if (screenX >= 0 && screenX < MATRIX_WIDTH) {
                        for (int row = 0; row < charHeight; row++) {
                            if (charBitmap[col] & (1 << row)) {
                                int screenY = row + vertOffset;
                                if (screenY >= 0 && screenY < MATRIX_HEIGHT) {
                                    uint16_t ledPos = xyToLED(screenX, screenY);
                                    if (ledPos < NUM_LEDS) leds[ledPos] = charColor;
                                }
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
        if (scrollPauseSeconds > 0) {
            scrollPauseActive = true;
            scrollPauseStart = millis();
        } else {
            scrollTextOffset = MATRIX_WIDTH;
            scrollTextInitialized = false;
        }
    }
}




// =============================================================================
// FUNZIONI INTERFACCIA WEB / WEB INTERFACE FUNCTIONS
// =============================================================================

// Forward declarations aggiunte per stabilità compilazione / Forward declarations added for compilation stability
void applyPreset(uint8_t preset);
void checkButtons();
void updateMatrix();
void updateMatrix2();
void updateSlowEffect();
void updateFadeEffect();
void initTronBikes();
bool tronPosIsFree(int8_t x, int8_t y, uint8_t bikeIndex);
int8_t tronFindSafeDir(uint8_t bikeIndex);
void updateTronEffect();
void updateDropEffect();
void updateMotoEffect();
void updateScrollText();
void resetScrollText();
void initScrollText();
String buildScrollDisplayText();
void handleSetScrollText();
void handleSetScrollFontSize();
void handleSetScrollOptions();
void handleSetDigitalOverlay();
void showCurrentTime();

// ===== Web handlers: forward declarations (aggiunte per evitare errori di compilazione) =====
void handleRoot();
void handleStatus();
void handleSetMode();
void handleSetBlink();
void handleSetColor();
void handleSetRainbow();
void handleSetBrightness();
void handleSetDayNightSettings();
void handleSetDigitalOverlay();
void handleSetDigitalTiming();
void handleSetScrollText();
void handleSetScrollPause();
void handleSetScrollFontSize();
void handleSetScrollOptions();
void handleSetScrollShowPreset();
void handleSetScrollTextualDateTime();
void handleSetScrollSpeed();
void handlePower();
void handleReset();
void handleNotFound();
void handleSetScrollShowPreset(); // presente nelle route
// Se hai altri handler custom aggiungi qui la loro signature / If you have other custom handlers add their signature here



void setupWebServer() {
    server.on("/", HTTP_GET, handleRoot);
    server.on("/status", HTTP_GET, handleStatus);
    server.on("/setMode", HTTP_POST, handleSetMode);
    server.on("/setBlink", HTTP_POST, handleSetBlink);
    server.on("/setColor", HTTP_POST, handleSetColor);
    server.on("/setRainbow", HTTP_POST, handleSetRainbow);
    server.on("/setBrightness", HTTP_POST, handleSetBrightness);
    server.on("/setDayNightSettings", HTTP_POST, handleSetDayNightSettings);
    server.on("/setDigitalOverlay", HTTP_POST, handleSetDigitalOverlay);
    server.on("/setDigitalTiming", HTTP_POST, handleSetDigitalTiming);
    server.on("/setScrollText", HTTP_POST, handleSetScrollText);
    server.on("/setScrollPause", HTTP_POST, handleSetScrollPause);
    server.on("/setScrollFontSize", HTTP_POST, handleSetScrollFontSize);
    server.on("/setScrollOptions", HTTP_POST, handleSetScrollOptions);
    server.on("/setScrollShowPreset", HTTP_POST, handleSetScrollShowPreset);
    server.on("/setScrollTextualDateTime", HTTP_POST, handleSetScrollTextualDateTime);
    server.on("/setScrollSpeed", HTTP_POST, handleSetScrollSpeed); // Nuovo endpoint per velocità
    server.on("/setScrollEnabled", HTTP_GET, handleSetScrollEnabled);
    server.on("/setScrollTextColor", HTTP_POST, handleSetScrollTextColor);
    server.on("/setScrollTextRainbow", HTTP_POST, handleSetScrollTextRainbow);
    server.on("/power", HTTP_POST, handlePower);
    server.on("/reset", HTTP_POST, handleReset);
    server.onNotFound(handleNotFound);
    
    server.begin();
    webInitialized = true;
    Serial.println("Server web avviato su http://" + WiFi.localIP().toString());
   // preset / preset
   lastEffectChangeTime = millis();
   digitalOverlayActive = false;
   scrollTextInitialized = false;
}


const char webpage[] PROGMEM = R"rawliteral(    

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
        .status-scroll {
            color: #00BCD4 !important; /* Azzurro/Ciano per lo scroll */
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
            body {
                padding: 10px;
            }

            .container {
                padding: 15px;
            }

            h1 {
                font-size: 1.8rem;
            }

            h2 {
                font-size: 1.2rem;
            }

            .mode-grid {
                grid-template-columns: repeat(2, 1fr);
                gap: 10px;
            }

            .mode-btn {
                padding: 12px 8px;
                font-size: 0.9rem;
            }

            .status-card {
                flex-direction: column;
                align-items: flex-start;
                padding: 15px;
            }

            .status-item {
                flex-direction: row;
                justify-content: space-between;
                width: 100%;
            }

            .toggle-container {
                gap: 15px;
                flex-wrap: wrap;
            }

            .toggle {
                width: 60px;
                height: 30px;
                flex-shrink: 0;
            }

            .slider:before {
                height: 22px;
                width: 22px;
            }

            input:checked + .slider:before {
                transform: translateX(30px);
            }

            .slider-text {
                font-size: 0.9rem;
                min-width: 0;
                flex: 1;
                white-space: normal;
                word-break: break-word;
            }

            .control-section {
                padding: 15px;
            }

            .color-grid {
                grid-template-columns: repeat(4, 1fr);
                gap: 8px;
            }

            .color-btn {
                width: 45px;
                height: 45px;
            }

            .btn {
                padding: 10px 15px;
                font-size: 0.9rem;
                width: 100%;
                justify-content: center;
            }

            .action-buttons {
                flex-direction: column;
            }

            .day-night-settings {
                padding: 10px;
            }

            .digital-timing-settings {
                padding: 10px;
            }

            .digital-timing-settings > div {
                flex-direction: column;
                gap: 10px;
            }

            /* Impostazioni giorno/notte su mobile */
            .day-night-settings {
                padding: 12px !important;
            }

            .day-night-settings h3 {
                font-size: 1.1rem !important;
                margin-bottom: 12px !important;
            }

            .time-brightness-row {
                flex-direction: column !important;
                gap: 12px !important;
                margin-bottom: 15px !important;
                padding-bottom: 15px;
                border-bottom: 1px solid rgba(255,255,255,0.1);
            }

            .time-brightness-row:last-of-type {
                border-bottom: none;
                margin-bottom: 10px !important;
            }

            .time-input {
                width: 100% !important;
                min-width: auto !important;
                flex-direction: row !important;
                justify-content: space-between !important;
            }

            .time-input label {
                min-width: auto !important;
                flex: 1;
                font-size: 0.95rem !important;
            }

            .time-input input[type="time"] {
                width: 110px !important;
                font-size: 16px !important;
                padding: 10px !important;
            }

            .brightness-control-inline {
                width: 100% !important;
                min-width: auto !important;
            }

            .brightness-control-inline label {
                font-size: 0.9rem !important;
                margin-bottom: 8px !important;
            }

            .brightness-slider {
                width: 100% !important;
                height: 10px !important;
                margin: 5px 0 !important;
            }

            /* Toggle spegnimento notturno su mobile */
            .time-brightness-row .brightness-control-inline[style*="display: flex"] {
                flex-wrap: wrap !important;
                justify-content: flex-start !important;
            }
        }

        @media (max-width: 400px) {
            .mode-grid {
                grid-template-columns: 1fr;
            }
            .color-grid {
                grid-template-columns: repeat(3, 1fr);
            }
        }

        /* Mobile responsive */
        input[type="range"] { width: 100%; }
        input[type="number"], input[type="text"], input[type="time"] {
            font-size: 16px;
            max-width: 100%;
            box-sizing: border-box;
        }
        .mode-category { margin-bottom: 12px; }
        .mode-category .mode-btn { flex: 1 1 auto; min-width: 70px; }
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
                <div class="status-label">Orario</div>
                <div class="status-value status-time" id="currentTime">--:--:--</div>
            </div>
            <div class="status-item">
                <div class="status-label">Modalità</div>
                <div class="status-value status-mode" id="currentMode">---</div>
            </div>
            <div class="status-item">
                <div class="status-label">Colore</div>
                <div class="status-value status-color" id="currentColor">---</div>
            </div>
            <div class="status-item">
                <div class="status-label">Lampeggio</div>
                <div class="status-value status-blink" id="currentBlink">---</div>
            </div>
            <div class="status-item">
                <div class="status-label">Luminosità</div>
                <div class="status-value status-brightness" id="currentBrightness">---</div>
            </div>
            <div class="status-item">
                <div class="status-label">Stato</div>
                <div class="status-value status-power" id="currentPower">---</div>
            </div>
            <div class="status-item">
                <div class="status-label">Scroll</div>
                <div class="status-value status-scroll" id="currentScroll">---</div>
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
                           style="width:80px;height:40px;border:none;background:transparent;cursor:pointer;"
                           oninput="applyAdvancedColor()">
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

            <div class="digital-timing-settings" style="margin-top:15px;padding:15px;background:rgba(255,255,255,0.1);border-radius:8px;">
                <h4 style="color:white;margin-bottom:10px;">⏱️ Tempistica Orologio Digitale</h4>
                <div style="display:flex;gap:20px;flex-wrap:wrap;align-items:center;">
                    <div>
                        <label style="color:#ccc;">Appare ogni (sec):</label>
                        <input type="number" id="digitalInterval" min="5" max="120" value="30"
                               style="width:60px;padding:5px;border-radius:4px;border:none;"
                               onfocus="startEditingDigital()" onchange="scheduleEndEditingDigital()" oninput="startEditingDigital()">
                    </div>
                    <div>
                        <label style="color:#ccc;">Per (sec):</label>
                        <input type="number" id="digitalDuration" min="5" max="59" value="10"
                               style="width:60px;padding:5px;border-radius:4px;border:none;"
                               onfocus="startEditingDigital()" onchange="scheduleEndEditingDigital()" oninput="startEditingDigital()">
                    </div>
                    <button class="btn btn-primary" onclick="saveDigitalTiming()">Salva</button>
                </div>
                <p style="color:#888;font-size:12px;margin-top:8px;">Animazioni partono al cambio minuto</p>
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
        </div>

        <div class="control-section">
            <div style="display: flex; justify-content: space-between; align-items: center; margin-bottom: 20px;">
                <h2 style="margin: 0;">📝 Testo Scorrevole</h2>
                <label class="toggle">
                <input type="checkbox" id="scrollEnabledToggle" onchange="handleScrollEnabledChange()">
                <span class="slider"></span>
                </label>
            </div>
            <div style="margin-bottom: 15px;">
                <input type="text" id="scrollTextInput" placeholder="Inserisci testo da scorrere..." maxlength="200"
                style="width:100%;padding:12px;font-size:1rem;border-radius:8px;border:1px solid rgba(255,255,255,0.3);
                background:rgba(255,255,255,0.1);color:white;" oninput="updateCharCount()">
                <div style="display:flex;justify-content:space-between;font-size:0.8rem;margin-top:4px;">
                    <span style="color:#888;">Totale stimato: <span id="totalCharCount" style="color:#4CAF50;">0</span> caratteri</span>
                    <span><span id="charCount" style="color:#888;">0</span><span style="color:#888;">/200</span></span>
                </div>
            </div>
            
            <div style="display:flex;gap:15px;align-items:center;margin-bottom:15px;flex-wrap:wrap;">
                <label style="color:#ccc;">Testo scorrevole ogni (sec):</label>
                <input type="number" id="scrollPause" min="5" max="60" value="5"
                    style="width:60px;padding:5px;border-radius:4px;border:none;">
                <label style="color:#ccc;display:flex;align-items:center;gap:8px;cursor:pointer;margin-left:10px;">
                    <input type="checkbox" id="scrollShowPreset" onchange="saveScrollShowPreset()" checked>
                    <span>🕐 Mostra oraquadra quando non si vede il testo scorrevole</span>
                </label>
             </div>
            
            <div style="margin-bottom:15px;">
                <label style="color:#ccc;display:block;margin-bottom:8px;">⚡ Velocità scorrimento: <span id="scrollSpeedValue" style="color:#4CAF50;font-weight:bold;">50</span> ms</label>
                <input type="range" id="scrollSpeedSlider" min="10" max="500" value="50" step="5"
                    style="width:100%;cursor:pointer;" onchange="saveScrollSpeed()" oninput="updateScrollSpeedLabel(this.value)">
                <div style="display:flex;justify-content:space-between;color:#888;font-size:0.8rem;">
                    <span>Veloce (10ms)</span><span>Normale (50ms)</span><span>Lento (500ms)</span>
                </div>
            </div>
            
            <div style="margin-bottom:15px;">
                <label style="color:#ccc;display:block;margin-bottom:8px;">Dimensione Font: <span id="fontSizeLabel">Medio</span></label>
                <input type="range" id="scrollFontSize" min="1" max="3" value="2"
                    style="width:100%;cursor:pointer;" onchange="saveScrollFontSize(this.value)" oninput="updateFontSizeLabel(this.value)">
                <div style="display:flex;justify-content:space-between;color:#888;font-size:0.8rem;">
                    <span>Piccolo</span><span>Medio</span><span>Grande</span>
                </div>
            </div>

            <div style="margin-top: 15px; margin-bottom: 25px;">
                <label style="color: white; font-weight: bold;">Colore Testo Scorrevole:</label>
                <div style="display: flex; gap: 10px; margin-top: 10px; align-items: center;">
                    <input type="color" id="scrollTextColorPicker" value="#FFFFFF" style="width: 60px; height: 40px; cursor: pointer;">
                    <button onclick="applyScrollTextColor()" class="mode-btn" style="padding: 10px 20px;">Applica Colore</button>
                    <button onclick="setScrollTextRainbow()" class="mode-btn" style="padding: 10px 20px; background: linear-gradient(90deg, red, orange, yellow, green, blue, violet);">Rainbow</button>
                </div>
            </div>
            
            <div style="background:rgba(255,255,255,0.1);padding:15px;border-radius:8px;margin-bottom:15px;">
                <h4 style="color:white;margin-bottom:10px;">📋 Opzioni Visualizzazione</h4>
                <div style="display:flex;gap:20px;margin-bottom:10px;flex-wrap:wrap;">
                    <label style="color:#ccc;display:flex;align-items:center;gap:8px;cursor:pointer;">
                        <input type="checkbox" id="scrollShowText" onchange="saveScrollOptions();updateTotalCharCount()" checked>
                        <span>📝 Mostra Testo</span>
                    </label>
                    <label style="color:#ccc;display:flex;align-items:center;gap:8px;cursor:pointer;">
                        <input type="checkbox" id="scrollShowDate" onchange="saveScrollOptions();updateTotalCharCount()">
                        <span>📅 Mostra Data</span>
                    </label>
                    <label style="color:#ccc;display:flex;align-items:center;gap:8px;cursor:pointer;">
                        <input type="checkbox" id="scrollShowTime" onchange="saveScrollOptions();updateTotalCharCount()">
                        <span>🕐 Mostra Ora</span>
                    </label>
                </div>
                <div style="display:flex;gap:10px;align-items:center;margin-top:10px;">
                    <label style="color:#ccc;display:flex;align-items:center;gap:8px;cursor:pointer;">
                        <input type="checkbox" id="scrollTextualDateTime" onchange="saveScrollTextualDateTime()">
                        <span>💬 Mostra data/ora testuale</span>
                    </label>
                    <span style="color:#888;font-size:0.85rem;">(Es: "SONO LE OTTO E DODICI MINUTI")</span>
                </div>
            </div>
            
            <button class="btn btn-primary" onclick="saveScrollOptions(); saveScrollText();">Salva impostazioni Testo Scorrevole</button>
        </div>

    <div class="notification" id="notification"></div>
    
    <script>
    // Modalità disponibili organizzate per categoria
    const modeCategories = [
        {
            name: "Lento",
            icon: "🐢",
            modes: [
                { id: 0, name: "Casuale" },
                { id: 3, name: "Arancione" },
                { id: 7, name: "Blu" }
            ]
        },
        {
            name: "Veloce",
            icon: "⚡",
            modes: [
                { id: 10, name: "Giallo" },
                { id: 11, name: "Bianco" },
                { id: 12, name: "Ciano" }
            ]
        },
        {
            name: "Sfumato",
            icon: "🌈",
            modes: [
                { id: 1, name: "Blu" },
                { id: 6, name: "Verde" },
                { id: 9, name: "Rosa" }
            ]
        },
        {
            name: "Matrix",
            icon: "💊",
            modes: [
                { id: 2, name: "Verde" },
                { id: 4, name: "Blu" },
                { id: 5, name: "Giallo" },
                { id: 8, name: "Ciano" }
            ]
        },
        {
            name: "Effetti Speciali",
            icon: "✨",
            modes: [
                { id: 13, name: "TRON" },
                { id: 14, name: "Goccia" },
                { id: 15, name: "Moto" },
                { id: 16, name: "Galaga" },
                { id: 17, name: "Pacman" }
            ]
        },
        {
            name: "Altri",
            icon: "🔢",
            modes: [
                { id: 18, name: "Digitale" }
            ]
        }
    ];
    
    // Colori disponibili
    const colors = [
        { name: "Bianco", value: "FFFFFF" },
        { name: "Blu", value: "0000FF" },
        { name: "Rosso", value: "FF0000" },
        { name: "Verde", value: "00FF00" },
        { name: "Giallo", value: "FFFF00" },
        { name: "Arancione", value: "FFA500" },
        { name: "Rainbow", value: "RAINBOW", isRainbow: true }
    ];
    
    // Variabili globali
    let userIsEditingDayNight = false;
    let userIsEditingDigital = false;
    let userIsEditingScrollText = false;
    let userIsEditingScrollPause = false;
    let editingTimeout = null;
    let digitalEditingTimeout = null;
    let scrollTextEditingTimeout = null;
    let scrollPauseEditingTimeout = null;
    let brightnessUpdateTimeout = null;
    
    // Inizializzazione
    document.addEventListener('DOMContentLoaded', function() {
        console.log('Inizializzazione pagina...');
        
        initializeModeButtons();
        initializeColorButtons();
        updateStatus();
        
        // Aggiorna l'orario ogni secondo
        setInterval(updateTime, 1000);
        
        // secondi / seconds
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

        // Event listener per campo testo scorrevole
        document.getElementById('scrollTextInput').addEventListener('focus', startEditingScrollText);
        document.getElementById('scrollTextInput').addEventListener('input', keepEditingScrollText);
        document.getElementById('scrollTextInput').addEventListener('blur', scheduleEndEditingScrollText);

        // Event listener per campo pausa
        document.getElementById('scrollPause').addEventListener('focus', startEditingScrollPause);
        document.getElementById('scrollPause').addEventListener('input', keepEditingScrollPause);
        document.getElementById('scrollPause').addEventListener('blur', scheduleEndEditingScrollPause);

        // Event listener per slider velocità
        document.getElementById('scrollSpeedSlider').addEventListener('input', updateScrollSpeedLabel);

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

    function startEditingDigital() {
        userIsEditingDigital = true;
        if (digitalEditingTimeout) {
            clearTimeout(digitalEditingTimeout);
            digitalEditingTimeout = null;
        }
    }

    function scheduleEndEditingDigital() {
        if (digitalEditingTimeout) {
            clearTimeout(digitalEditingTimeout);
        }
        digitalEditingTimeout = setTimeout(function() {
            userIsEditingDigital = false;
            digitalEditingTimeout = null;
        }, 5000);
    }

    // Funzioni per gestire editing testo scorrevole
    function startEditingScrollText() {
        userIsEditingScrollText = true;
        if (scrollTextEditingTimeout) {
            clearTimeout(scrollTextEditingTimeout);
            scrollTextEditingTimeout = null;
        }
    }

    function keepEditingScrollText() {
        userIsEditingScrollText = true;
        if (scrollTextEditingTimeout) {
            clearTimeout(scrollTextEditingTimeout);
        }
        scrollTextEditingTimeout = setTimeout(function() {
            userIsEditingScrollText = false;
            scrollTextEditingTimeout = null;
        }, 10000);
    }

    function scheduleEndEditingScrollText() {
        if (scrollTextEditingTimeout) {
            clearTimeout(scrollTextEditingTimeout);
        }
        scrollTextEditingTimeout = setTimeout(function() {
            userIsEditingScrollText = false;
            scrollTextEditingTimeout = null;
        }, 3000);
    }

    // Funzioni per gestire editing pausa
    function startEditingScrollPause() {
        userIsEditingScrollPause = true;
        if (scrollPauseEditingTimeout) {
            clearTimeout(scrollPauseEditingTimeout);
            scrollPauseEditingTimeout = null;
        }
    }

    function keepEditingScrollPause() {
        userIsEditingScrollPause = true;
        if (scrollPauseEditingTimeout) {
            clearTimeout(scrollPauseEditingTimeout);
        }
        scrollPauseEditingTimeout = setTimeout(function() {
            userIsEditingScrollPause = false;
            scrollPauseEditingTimeout = null;
        }, 10000);
    }

    function scheduleEndEditingScrollPause() {
        if (scrollPauseEditingTimeout) {
            clearTimeout(scrollPauseEditingTimeout);
        }
        scrollPauseEditingTimeout = setTimeout(function() {
            userIsEditingScrollPause = false;
            scrollPauseEditingTimeout = null;
        }, 3000);
    }

    function updateStatusIfNotEditing() {
        if (!userIsEditingDayNight && !userIsEditingScrollText && !userIsEditingScrollPause) {
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

        modeCategories.forEach(function(category) {
            // Crea il contenitore della categoria
            const categoryDiv = document.createElement('div');
            categoryDiv.className = 'mode-category';
            categoryDiv.style.cssText = 'margin-bottom:15px;';

            // Titolo categoria
            const categoryTitle = document.createElement('div');
            categoryTitle.className = 'category-title';
            categoryTitle.style.cssText = 'color:#888;font-size:0.85rem;margin-bottom:8px;padding-left:5px;';
            categoryTitle.textContent = category.icon + ' ' + category.name;
            categoryDiv.appendChild(categoryTitle);

            // Griglia pulsanti
            const buttonsGrid = document.createElement('div');
            buttonsGrid.style.cssText = 'display:flex;flex-wrap:wrap;gap:8px;';

            category.modes.forEach(function(mode) {
                const button = document.createElement('button');
                button.className = 'mode-btn';
                button.textContent = mode.name;
                button.dataset.mode = mode.id;
                button.addEventListener('click', function() {
                    setMode(mode.id);
                });
                buttonsGrid.appendChild(button);
            });

            categoryDiv.appendChild(buttonsGrid);
            container.appendChild(categoryDiv);
        });
    }
    
    function initializeColorButtons() {
        const container = document.getElementById('colorButtons');
        container.innerHTML = '';

        colors.forEach(function(color) {
            const button = document.createElement('button');
            button.className = 'color-btn';
            button.title = color.name;

            if (color.isRainbow) {
                // Pulsante Rainbow con gradiente
                button.style.background = 'linear-gradient(90deg, red, orange, yellow, green, blue, violet)';
                button.addEventListener('click', function() {
                    setRainbow();
                });
            } else {
                button.style.backgroundColor = '#' + color.value;
                button.addEventListener('click', function() {
                    setColor(color.value);
                });
            }
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
                if (data.scrollEnabled !== undefined) {
                    document.getElementById('currentScroll').textContent = data.scrollEnabled ? 'attivo' : 'disattivo';
                    document.getElementById('scrollEnabledToggle').checked = data.scrollEnabled == 1;
                }
                document.getElementById('blinkToggle').checked = data.blink;
                document.getElementById('powerToggle').checked = data.power;
                if (data.digitalOverlayEnabled !== undefined) {
                    document.getElementById('digitalOverlayToggle').checked = data.digitalOverlayEnabled;
                }
                if (!userIsEditingDigital) {
                    if (data.digitalInterval !== undefined) {
                        document.getElementById('digitalInterval').value = data.digitalInterval;
                    }
                    if (data.digitalDuration !== undefined) {
                        document.getElementById('digitalDuration').value = data.digitalDuration;
                    }
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
                if (data.scrollPause !== undefined && !userIsEditingScrollPause) {
                    document.getElementById('scrollPause').value = data.scrollPause;
                }
                if (data.scrollManualSpeed !== undefined) {
                    document.getElementById('scrollSpeedSlider').value = data.scrollManualSpeed;
                    document.getElementById('scrollSpeedValue').textContent = data.scrollManualSpeed;
                }
                if (data.scrollShowText !== undefined) {
                    document.getElementById('scrollShowText').checked = data.scrollShowText;
                }
                if (data.scrollShowDate !== undefined) {
                    document.getElementById('scrollShowDate').checked = data.scrollShowDate;
                }
                if (data.scrollShowTime !== undefined) {
                    document.getElementById('scrollShowTime').checked = data.scrollShowTime;
                }
                if (data.scrollShowPreset !== undefined) {
                    document.getElementById('scrollShowPreset').checked = data.scrollShowPreset;
                }
                if (data.scrollTextualDateTime !== undefined) {
                    document.getElementById('scrollTextualDateTime').checked = data.scrollTextualDateTime;
                }
                if (data.scrollFontSize !== undefined) {
                    document.getElementById('scrollFontSize').value = data.scrollFontSize;
                    updateFontSizeLabel(data.scrollFontSize);
                }
                if (data.scrollText !== undefined && data.scrollText.length > 0 && !userIsEditingScrollText) {
                    document.getElementById('scrollTextInput').value = data.scrollText;
                    updateCharCount();
                }
                updateTotalCharCount();

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

    function setRainbow() {
        fetch('/setRainbow', {
            method: 'POST'
        })
        .then(function(response) {
            return response.text();
        })
        .then(function(data) {
            showNotification('Colore Rainbow attivato', 'success');
            updateStatus();
        })
        .catch(function(error) {
            console.error('Errore attivazione Rainbow:', error);
            showNotification('Errore attivazione Rainbow', 'error');
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

    function saveDigitalTiming() {
        const interval = document.getElementById('digitalInterval').value;
        const duration = document.getElementById('digitalDuration').value;

        fetch('/setDigitalTiming', {
            method: 'POST',
            headers: {
                'Content-Type': 'application/x-www-form-urlencoded',
            },
            body: 'interval=' + interval + '&duration=' + duration
        })
        .then(function(response) {
            return response.text();
        })
        .then(function(data) {
            showNotification('Tempistica digitale salvata', 'success');
        })
        .catch(function(error) {
            console.error('Errore salvataggio tempistica:', error);
            showNotification('Errore salvataggio tempistica', 'error');
        });
    }

function applyScrollTextColor() {
    const picker = document.getElementById('scrollTextColorPicker');
    const hex = picker.value.replace('#', '');
    
    fetch('/setScrollTextColor', {
        method: 'POST',
        headers: {'Content-Type': 'application/x-www-form-urlencoded'},
        body: 'color=' + hex
    })
    .then(r => r.text())
    .then(() => showNotification('Colore testo applicato: #' + hex.toUpperCase(), 'success'))
    .catch(() => showNotification('Errore applicazione colore', 'error'));
}

function setScrollTextRainbow() {
    fetch('/setScrollTextRainbow', {method: 'POST'})
    .then(r => r.text())
    .then(() => showNotification('Rainbow testo attivato', 'success'))
    .catch(() => showNotification('Errore attivazione rainbow', 'error'));
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

function saveScrollText() {
    const text = document.getElementById('scrollTextInput').value;
    
    fetch('/setScrollText', {
        method: 'POST',
        headers: { 'Content-Type': 'application/x-www-form-urlencoded' },
        body: 'text=' + encodeURIComponent(text)
    })
    .then(r => r.text())
    .then(() => {
        showNotification('✅ Testo scorrevole salvato! Verrà mostrato quando lo scroll è attivo.', 'success');
    })
    .catch(() => showNotification('Errore salvataggio testo', 'error'));
}

function handleScrollEnabledChange() {
    const enabled = document.getElementById('scrollEnabledToggle').checked;
    fetch('/setScrollEnabled?state=' + (enabled ? '1' : '0'))
        .then(r => r.json())
        .then(data => {
            if (data.scrollEnabled) {
                showNotification('✅ Testo scorrevole ATTIVATO', 'success');
            } else {
                showNotification('⏸️ Testo scorrevole DISATTIVATO - Mostra preset corrente', 'info');
            }
        })
        .catch(e => showNotification('Errore attivazione scroll', 'error'));
}

function saveScrollPause() {
    const pause = document.getElementById('scrollPause').value;
    fetch('/setScrollPause', {
        method: 'POST',
        headers: { 'Content-Type': 'application/x-www-form-urlencoded' },
        body: 'pause=' + pause
    })
    .then(r => r.text())
    .then(resp => {
        showNotification("Pausa salvata: " + pause + " sec", "success");
    })
    .catch(err => showNotification("Errore salvataggio pausa", "error"));
}

function saveScrollSpeed() {
    const speed = document.getElementById('scrollSpeedSlider').value;
    fetch('/setScrollSpeed', {
        method: 'POST',
        headers: { 'Content-Type': 'application/x-www-form-urlencoded' },
        body: 'speed=' + speed
    })
    .then(r => r.text())
    .then(resp => {
        showNotification("Velocità salvata: " + speed + " ms", "success");
    })
    .catch(err => showNotification("Errore salvataggio velocità", "error"));
}

function updateScrollSpeedLabel(value) {
    const slider = document.getElementById('scrollSpeedSlider');
    const displayValue = slider.value;
    document.getElementById('scrollSpeedValue').textContent = displayValue;
}

function saveScrollOptions() {
    const showText = document.getElementById('scrollShowText').checked ? 1 : 0;
    const showDate = document.getElementById('scrollShowDate').checked ? 1 : 0;
    const showTime = document.getElementById('scrollShowTime').checked ? 1 : 0;
    const pause = document.getElementById('scrollPause').value;
    fetch('/setScrollOptions', {
        method: 'POST',
        headers: { 'Content-Type': 'application/x-www-form-urlencoded' },
        body: `showText=${showText}&showDate=${showDate}&showTime=${showTime}&pause=${pause}`  
    })
    .then(r => r.text())
    .then(resp => {
        let msg = [];
        if (showText) msg.push("Testo");
        if (showDate) msg.push("Data");
        if (showTime) msg.push("Ora");
        showNotification(msg.length > 0 ? "Attivato: " + msg.join(", ") : "Tutto disattivato", "success");
    })
    .catch(err => showNotification("Errore salvataggio opzioni", "error"));
}

function saveScrollShowPreset() {
    const showPreset = document.getElementById('scrollShowPreset').checked ? 1 : 0;
    fetch('/setScrollShowPreset', {
        method: 'POST',
        headers: { 'Content-Type': 'application/x-www-form-urlencoded' },
        body: 'showPreset=' + showPreset
    })
    .then(r => r.text())
    .then(resp => {
        showNotification(showPreset ? "Preset attivo durante pausa" : "Preset disattivato durante pausa", "success");
    })
    .catch(err => showNotification("Errore salvataggio", "error"));
}

function saveScrollTextualDateTime() {
    const textual = document.getElementById('scrollTextualDateTime').checked ? 1 : 0;
    fetch('/setScrollTextualDateTime', {
        method: 'POST',
        headers: { 'Content-Type': 'application/x-www-form-urlencoded' },
        body: 'textual=' + textual
    })
    .then(r => r.text())
    .then(resp => {
        showNotification(textual ? "Data/ora testuale attivata" : "Data/ora numerica attivata", "success");
        updateTotalCharCount();  // Ricalcola stima caratteri
    })
    .catch(err => showNotification("Errore salvataggio", "error"));
}

function updateFontSizeLabel(value) {
    const labels = ['', 'Piccolo', 'Medio', 'Grande'];
    document.getElementById('fontSizeLabel').textContent = labels[value] || 'Medio';
}

function updateCharCount() {
    const input = document.getElementById('scrollTextInput');
    const count = input.value.length;
    const charCountEl = document.getElementById('charCount');
    charCountEl.textContent = count;
    if (count >= 200) {
        charCountEl.style.color = '#ff4444';
        showNotification('Hai raggiunto il limite massimo di 200 caratteri!', 'error');
    } else if (count >= 180) {
        charCountEl.style.color = '#ffaa00';
    } else {
        charCountEl.style.color = '#888';
    }
    updateTotalCharCount();
}

function updateTotalCharCount() {
    let total = 0;
    let parts = 0;
    const showText = document.getElementById('scrollShowText').checked;
    const showDate = document.getElementById('scrollShowDate').checked;
    const showTime = document.getElementById('scrollShowTime').checked;
    const textual = document.getElementById('scrollTextualDateTime').checked;
    const textLen = document.getElementById('scrollTextInput').value.length;

    // Data
    if (showDate) {
        if (textual) {
            // Es: "MERCOLEDI UNO DICEMBRE DUEMILAVENTICINQUE" = ~45 caratteri
            total += 45;
        } else {
            // Es: "MERCOLEDI 01/12/2025" = ~21 caratteri
            total += 21;
        }
        parts++;
    }
    
    // Ora
    if (showTime) {
        if (textual) {
            // Es: "SONO LE OTTO E DODICI MINUTI" = ~30 caratteri (media)
            total += 30;
        } else {
            // "HH:MM" = 5 caratteri
            total += 5;
        }
        parts++;
    }
    
    if (showText && textLen > 0) {
        total += textLen;
        parts++;
    }
    
    // Separatori " - " (3 caratteri) tra le parti
    if (parts > 1) {
        total += (parts - 1) * 3;
    }

    const totalEl = document.getElementById('totalCharCount');
    totalEl.textContent = total;
    if (total > 250) {
        totalEl.style.color = '#ff4444';
    } else if (total > 200) {
        totalEl.style.color = '#ffaa00';
    } else {
        totalEl.style.color = '#4CAF50';
    }
}

function saveScrollFontSize(size) {
    fetch('/setScrollFontSize', {
        method: 'POST',
        headers: { 'Content-Type': 'application/x-www-form-urlencoded' },
        body: 'fontSize=' + size
    })
    .then(r => r.text())
    .then(resp => {
        const labels = ['', 'Piccolo', 'Medio', 'Grande'];
        showNotification("Dimensione font: " + labels[size], "success");
    })
    .catch(err => showNotification("Errore salvataggio dimensione font", "error"));
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

// --- gestione toggle testo scorrevole ---
function setScrollControlsEnabled(enabled) {
  // lista dei selector dei controlli di scroll (adatta i selettori ai tuoi input reali)
  const controls = document.querySelectorAll('.scroll-control'); // aggiungi classe .scroll-control a tutti gli input del pannello scroll
  controls.forEach(c => c.disabled = !enabled);
  document.getElementById('scrollEnabledLabel').textContent = enabled ? 'ON' : 'OFF';
  document.getElementById('scrollEnabledToggle').checked = enabled;
}

// invoca /status per impostare lo stato iniziale
function refreshStatus() {
  fetch('/status').then(r => r.json()).then(data => {
    // ... altri aggiornamenti status ...
    if (typeof data.scrollEnabled !== 'undefined') {
      setScrollControlsEnabled(data.scrollEnabled == 1);
    }
  });
}

// evento toggle
document.getElementById('scrollEnabledToggle').addEventListener('change', function() {
  var state = this.checked ? '1' : '0';
  fetch('/setScrollEnabled?state=' + state)
    .then(r => r.json())
    .then(j => {
      if (j && j.scrollEnabled !== undefined) {
        setScrollControlsEnabled(j.scrollEnabled == 1);
      }
    })
    .catch(e => {
      console.error('Errore setScrollEnabled', e);
    });
});

// al caricamento della pagina
document.addEventListener('DOMContentLoaded', (event) => {
  // assicurati che i controlli del pannello testo abbiano la classe .scroll-control
  refreshStatus();
});


</script>


</body>
</html>
)rawliteral";



void handleRoot() {
    server.send_P(200, "text/html", webpage);
}


//    server.send(200, "text/html", html);
//}

void handleStatus() {
    StaticJsonDocument<1024> doc;
    
    // Nome della modalità corrente / Current mode name
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
        case 14: modeName = "Effetto Goccia"; break;
        case 15: modeName = "Effetto Moto"; break;
        case 16: modeName = "Effetto Galaga"; break;
        case 17: modeName = "Effetto Pacman"; break;
        case 18: modeName = "Digitale"; break;
        case 19: modeName = "Scroll Text"; break;
        default: modeName = "Sconosciuta"; break;
    }
    
    // Nome del colore corrente / Current color name
    String colorName = "Personalizzato";
    CRGB current = clockColors.current;
    if (current == CRGB::White) colorName = "Bianco";
    else if (current == CRGB::Blue) colorName = "Blu";
    else if (current == CRGB::Red) colorName = "Rosso";
    else if (current == CRGB::Green) colorName = "Verde";
    else if (current == CRGB::Yellow) colorName = "Giallo";
    else if (current == CRGB(255,165,0)) colorName = "Arancione";
    
    // Formatta gli orari / Format times
    char dayStart[6], nightStart[6], sleepStart[6];
    snprintf(dayStart, sizeof(dayStart), "%02d:%02d", dayStartHour, dayStartMinute);
    snprintf(nightStart, sizeof(nightStart), "%02d:%02d", nightStartHour, nightStartMinute);
    snprintf(sleepStart, sizeof(sleepStart), "%02d:%02d", sleepStartHour, sleepStartMinute);
  
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
    doc["digitalInterval"] = digitaleInterval;
    doc["digitalDuration"] = digitaleDuration;
    doc["scrollPause"] = scrollPauseSeconds;
    doc["scrollManualSpeed"] = scrollTextSpeedMs;  // Velocità manuale del testo scorrevole
    doc["scrollFontSize"] = scrollFontSize;
    doc["scrollShowText"] = scrollShowText;
    doc["scrollShowDate"] = scrollShowDate;
    doc["scrollShowTime"] = scrollShowTime;
    doc["scrollShowPreset"] = scrollShowPresetDuringPause;
    doc["scrollTextualDateTime"] = scrollTextualDateTime;
    doc["scrollEnabled"] = scrollEnabled ? 1 : 0;
    doc["rainbowMode"] = textRainbowMode;
    // Colore corrente in formato hex / Current color in hex format
    char colorHex[7];
    sprintf(colorHex, "%02X%02X%02X", clockColors.current.r, clockColors.current.g, clockColors.current.b);
    doc["currentColor"] = colorHex;
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
        if (newMode < 20) {
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
            textRainbowMode = false;  // Disattiva rainbow quando si sceglie un colore

            // Reset effetti per applicare subito il nuovo colore
            slowState.active = false;
            slowState.completed = false;
            fadeState.active = false;
            fadeState.completed = false;

            // NON salvare in EEPROM - cambio temporaneo
            // Il colore originale verrà ripristinato quando si preme di nuovo il pulsante effetto

            // preset / preset
            uint8_t presetIndex = 255; // Valore per "colore personalizzato"
            if (newColor == CRGB::White) presetIndex = 0;
            else if (newColor == CRGB::Blue) presetIndex = 1;
            else if (newColor == CRGB::Red) presetIndex = 2;
            else if (newColor == CRGB::Green) presetIndex = 3;
            else if (newColor == CRGB::Yellow) presetIndex = 4;
            else if (newColor == CRGB(255,165,0)) presetIndex = 5; // Arancione

            clockColors.presetIndex = presetIndex;

            updateDisplay();
            server.send(200, "text/plain", "OK");
            return;
        }
    }
    server.send(400, "text/plain", "Invalid color");
}

void handleSetRainbow() {
    // Attiva colore Rainbow per il testo (ogni lettera un colore diverso)
    textRainbowMode = true;
    // NON salvare in EEPROM - cambio temporaneo
    server.send(200, "text/plain", "OK");
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
        
        // Salva in EEPROM / Save to EEPROM
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

void handleSetDigitalTiming() {
    if (server.hasArg("interval") && server.hasArg("duration")) {
        uint8_t interval = server.arg("interval").toInt();
        uint8_t duration = server.arg("duration").toInt();

        // Validazione / Validation
        if (interval < 5) interval = 5;
        if (interval > 120) interval = 120;
        if (duration < 10) duration = 10;
        if (duration > 59) duration = 59;

        digitaleInterval = interval;
        digitaleDuration = duration;

        // Salva in EEPROM / Save to EEPROM
        EEPROM.write(EEPROM_DIGITAL_START_ADDR, digitaleInterval);
        EEPROM.write(EEPROM_DIGITAL_DURATION_ADDR, digitaleDuration);
        EEPROM.commit();

        server.send(200, "text/plain", "OK");
        return;
    }
    server.send(400, "text/plain", "Invalid parameters");
}

void handleSetScrollPause() {
    if (server.hasArg("pause")) {
        uint8_t pause = server.arg("pause").toInt();
        if (pause >= 5 && pause <= 600) pause = 5;

        scrollPauseSeconds = pause;

        // Salva in EEPROM / Save to EEPROM
        EEPROM.write(EEPROM_SCROLL_PAUSE_ADDR, scrollPauseSeconds);
        EEPROM.commit();

        server.send(200, "text/plain", "OK");
        return;
    }
    server.send(400, "text/plain", "Invalid parameters");
}

// Nuova funzione per gestire la velocità del testo scorrevole
void handleSetScrollSpeed() {
    if (server.hasArg("speed")) {
        uint16_t speed = server.arg("speed").toInt();
        
        // Validazione / Validation
        if (speed < 10) speed = 10;
        if (speed > 500) speed = 500;

        scrollTextSpeedMs = speed;

        // Salva in EEPROM / Save to EEPROM
        EEPROM.write(EEPROM_SCROLL_SPEED_ADDR, speed & 0xFF); // Byte basso
        EEPROM.write(EEPROM_SCROLL_SPEED_ADDR_H, (speed >> 8) & 0xFF); // Byte alto
        EEPROM.commit();

        Serial.print("Velocità testo scorrevole impostata a: ");
        Serial.print(speed);
        Serial.println(" ms");

        server.send(200, "text/plain", "OK");
        return;
    }
    server.send(400, "text/plain", "Invalid parameters");
}

void handleSetScrollEnabled() {
    if (server.hasArg("state")) {
        String s = server.arg("state");
        bool newState = (s == "1" || s.equalsIgnoreCase("true"));

        // Aggiorna runtime e EEPROM
        scrollEnabled = newState ? true : false;
        EEPROM.write(EEPROM_SCROLL_ENABLED_ADDR, scrollEnabled ? 1 : 0);
        EEPROM.commit();

        // Se disabilitato, ferma immediatamente lo scroll e mostra orologio
        if (!scrollEnabled) {
            // Forza terminazione stato scroll
            scrollPauseActive = false;
            scrollTextInitialized = false;
            scrollTextOffset = MATRIX_WIDTH;
            // preset / preset
            FastLED.clear();
            showCurrentTime(); // usa la tua funzione esistente per mostrare l'orologio
            FastLED.show();
        }

        // Risposta JSON
        StaticJsonDocument<128> doc;
        doc["ok"] = true;
        doc["scrollEnabled"] = scrollEnabled;
        String out;
        serializeJson(doc, out);
        server.send(200, "application/json", out);
        return;
    }
    server.send(400, "text/plain", "missing state");
}

void handleSetScrollFontSize() {
    if (server.hasArg("fontSize")) {
        uint8_t fontSize = server.arg("fontSize").toInt();
        if (fontSize < 1) fontSize = 1;
        if (fontSize > 3) fontSize = 3;

        scrollFontSize = fontSize;

        // Salva in EEPROM / Save to EEPROM
        EEPROM.write(EEPROM_SCROLL_FONT_SIZE_ADDR, scrollFontSize);
        EEPROM.commit();

        // Reinizializza il testo scorrevole per applicare il nuovo font
        scrollTextInitialized = false;

        server.send(200, "text/plain", "OK");
        return;
    }
    server.send(400, "text/plain", "Invalid parameters");
}

void handleSetScrollOptions() {
   // ✅ DEBUG: stampa TUTTI gli argomenti ricevuti
    Serial.println("=== handleSetScrollOptions chiamata ===");
    Serial.print("Numero argomenti: ");
    Serial.println(server.args());
    
    for (int i = 0; i < server.args(); i++) {
        Serial.print("Argomento ");
        Serial.print(i);
        Serial.print(": ");
        Serial.print(server.argName(i));
        Serial.print(" = ");
        Serial.println(server.arg(i));
    }
    
    Serial.print("hasArg('pause'): ");
    Serial.println(server.hasArg("pause") ? "SI" : "NO");
    // ✅ FINE DEBUG
    if (server.hasArg("showText") && server.hasArg("showDate") && server.hasArg("showTime")) {
        scrollShowText = server.arg("showText").toInt() == 1;
        scrollShowDate = server.arg("showDate").toInt() == 1;
        scrollShowTime = server.arg("showTime").toInt() == 1;
        Serial.println("✅ Opzioni scroll salvati: ");  

        // ✅ Salva anche la pausa se presente
        if (server.hasArg("pause")) {
            uint8_t pause = server.arg("pause").toInt();
            if (pause >= 1) {
                scrollPauseSeconds = pause;
                EEPROM.write(EEPROM_SCROLL_PAUSE_ADDR, scrollPauseSeconds);
                Serial.print("✅ Pausa salvata: ");  
                Serial.println(scrollPauseSeconds);  
            }
        }
        
        scrollTextInitialized = false;
        
        EEPROM.write(EEPROM_SCROLL_SHOW_TEXT_ADDR, scrollShowText ? 1 : 0);
        EEPROM.write(EEPROM_SCROLL_SHOW_DATE_ADDR, scrollShowDate ? 1 : 0);
        EEPROM.write(EEPROM_SCROLL_SHOW_TIME_ADDR, scrollShowTime ? 1 : 0);
        EEPROM.commit();
        
        server.send(200, "text/plain", "OK");
        return;
    }
    server.send(400, "text/plain", "Invalid parameters");
}

void handleSetScrollShowPreset() {
    if (server.hasArg("showPreset")) {
        scrollShowPresetDuringPause = server.arg("showPreset").toInt() == 1;

        EEPROM.write(EEPROM_SCROLL_SHOW_PRESET_ADDR, scrollShowPresetDuringPause ? 1 : 0);
        EEPROM.commit();

        server.send(200, "text/plain", "OK");
        return;
    }
    server.send(400, "text/plain", "Invalid parameters");
}

void handleSetScrollTextualDateTime() {
    if (server.hasArg("textual")) {
        scrollTextualDateTime = server.arg("textual").toInt() == 1;

        scrollTextInitialized = false;  // Reinizializza per applicare il cambiamento

        EEPROM.write(EEPROM_SCROLL_TEXTUAL_DATETIME_ADDR, scrollTextualDateTime ? 1 : 0);
        EEPROM.commit();

        server.send(200, "text/plain", "OK");
        return;
    }
    server.send(400, "text/plain", "Invalid parameters");
}

void handleSetScrollText() {
    if (server.hasArg("text")) {
        String rawText = server.arg("text");
        Serial.print("Raw text ricevuto: [");
        Serial.print(rawText);
        Serial.print("] len=");
        Serial.println(rawText.length());

        // Decodifica URL encoding
        rawText.replace("+", " ");
        rawText.replace("%20", " ");
        rawText.replace("%2C", ",");
        rawText.replace("%2E", ".");
        rawText.replace("%21", "!");
        rawText.replace("%3F", "?");
        rawText.replace("%27", "'");
        rawText.replace("%2D", "-");
        rawText.toUpperCase();

        Serial.print("Dopo decode/upper: [");
        Serial.print(rawText);
        Serial.println("]");

        // Filtra solo caratteri ASCII stampabili (32-126) e sostituisci accentate
        customScrollText = "";
        for (unsigned int i = 0; i < rawText.length(); i++) {
            char c = rawText.charAt(i);
            // Sostituisci lettere accentate comuni (UTF-8)
            if (c == (char)0xC3) { // Primo byte UTF-8 per lettere accentate
                i++;
                if (i < rawText.length()) {
                    char c2 = rawText.charAt(i);
                    if (c2 == (char)0x80 || c2 == (char)0xA0) c = 'A'; // À à
                    else if (c2 == (char)0x88 || c2 == (char)0xA8) c = 'E'; // È è
                    else if (c2 == (char)0x89 || c2 == (char)0xA9) c = 'E'; // É é
                    else if (c2 == (char)0x8C || c2 == (char)0xAC) c = 'I'; // Ì ì
                    else if (c2 == (char)0x92 || c2 == (char)0xB2) c = 'O'; // Ò ò
                    else if (c2 == (char)0x99 || c2 == (char)0xB9) c = 'U'; // Ù ù
                    else c = '?';
                    customScrollText += c;
                }
            } else if (c >= 32 && c <= 126) {
                // Converti minuscole in maiuscole
                if (c >= 'a' && c <= 'z') c = c - 32;
                customScrollText += c;
            }
            // Ignora altri caratteri non ASCII
        }

        scrollTextInitialized = false;
        digitalOverlayActive = false; // disattiva overlay se eravamo in digitale
        lastEffectChangeTime = millis();

        // Salva il testo in EEPROM
        uint8_t textLen = customScrollText.length();
        if (textLen > EEPROM_SCROLL_TEXT_MAX_LEN) textLen = EEPROM_SCROLL_TEXT_MAX_LEN;
        EEPROM.write(EEPROM_SCROLL_TEXT_LEN_ADDR, textLen);
        for (uint8_t i = 0; i < textLen; i++) {
            EEPROM.write(EEPROM_SCROLL_TEXT_START_ADDR + i, customScrollText.charAt(i));
        }
        // preset / preset
        EEPROM.write(EEPROM_PRESET_ADDR, currentPreset);
        EEPROM.commit();

        Serial.print("Testo ricevuto: ");
        Serial.println(customScrollText);
        Serial.print("Lunghezza: ");
        Serial.println(textLen);

        server.send(200, "text/plain", "OK");
        return;
    }
    server.send(400, "text/plain", "Missing text");
}

void handleSetScrollTextColor() {
    if (server.hasArg("color")) {
        String colorHex = server.arg("color");
        if (colorHex.length() == 6) {
            long colorValue = strtol(colorHex.c_str(), NULL, 16);
            scrollTextColor.r = (colorValue >> 16) & 0xFF;
            scrollTextColor.g = (colorValue >> 8) & 0xFF;
            scrollTextColor.b = colorValue & 0xFF;
            scrollTextRainbow = false;
            
            // Salva in EEPROM / Save to EEPROM
            EEPROM.write(EEPROM_SCROLLTEXT_R_ADDR, scrollTextColor.r);
            EEPROM.write(EEPROM_SCROLLTEXT_G_ADDR, scrollTextColor.g);
            EEPROM.write(EEPROM_SCROLLTEXT_B_ADDR, scrollTextColor.b);
            EEPROM.write(EEPROM_SCROLLTEXT_RAINBOW_ADDR, 0);
            EEPROM.commit();
            
            server.send(200, "text/plain", "OK");
            return;
        }
    }
    server.send(400, "text/plain", "Invalid color");
}

void handleSetScrollTextRainbow() {
    scrollTextRainbow = true;
    
    // Salva in EEPROM / Save to EEPROM
    EEPROM.write(EEPROM_SCROLLTEXT_RAINBOW_ADDR, 1);
    EEPROM.commit();
    
    server.send(200, "text/plain", "OK");
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
// FUNZIONI PRINCIPALI DEL SKETCH / MAIN SKETCH FUNCTIONS
// =============================================================================

// Funzione per verificare se siamo nel periodo di modalità serale / Function to check if we are in evening mode period
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

// Funzione per verificare se siamo nel periodo di spegnimento notturno / Function to check if we are in night shutdown period
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

// Funzioni di gestione LED e parole / LED and word management functions
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
        // se necessario / if needed
        if (abs(diffX) > 2) {
            shot.dx = (diffX > 0) ? 1 : -1;
        } else {
            shot.dx = 0;
        }
    }
}

void updateGalagaEffect() {
    uint32_t currentMillis = millis();

    // Se non attivo, mostra solo orario
    if (!galagaEffectActive) {
        FastLED.clear();
        showCurrentTime();
        FastLED.show();
        return;
    }

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
    
    // Dopo l'attesa, disattiva effetto (aspetta cambio minuto)
    if (galagaCompletionTime != 0 && currentMillis - galagaCompletionTime >= GALAGA_WAIT_AFTER) {
        galagaEffectActive = false;
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
// Variabile per tracciare l'ultimo minuto in cui Pacman è partito
static uint8_t pacmanLastMinute = 255;

void initPacmanEffect() {
    FastLED.clear();
    pacmanEffectActive = true;
    pacmanCompletionTime = 0;
    pacmanTextHue = random8();

    // Salva il minuto corrente per sapere quando ripartire
    pacmanLastMinute = currentMinute;

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

    // Se l'effetto è completato, mostra l'orario statico
    // L'overlay digitale e la reinizializzazione sono gestiti da handleDigitalOverlay()
    if (pacmanCompletionTime != 0) {
        // Mostra l'orario colorato da Pacman
        FastLED.clear();
        for (uint16_t i = 0; i < NUM_LEDS; i++) {
            if (pacmanTextPixels[i]) {
                leds[i] = pacmanTextColor;
            }
        }

        // secondi / seconds
        if (currentBlink == 1) {
            showSeconds(currentSecond, pacmanTextColor);
        }

        FastLED.show();
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
        // secondi / seconds
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
   // se necessario / if needed
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
       EEPROM.write(EEPROM_SLEEP_START_HOUR_ADDR, 23);  // Corretto: 23:00 come default
       EEPROM.write(EEPROM_SLEEP_START_MINUTE_ADDR, 0);
       EEPROM.write(EEPROM_SLEEP_ENABLED_ADDR, 0);
       EEPROM.write(EEPROM_DAY_BRIGHTNESS_ADDR, 204);    // 80%
       EEPROM.write(EEPROM_NIGHT_BRIGHTNESS_ADDR, 26);   // 10%
       EEPROM.write(EEPROM_DIGITAL_OVERLAY_ADDR, 1);     // overlay digitale ATTIVO di default
       EEPROM.write(EEPROM_DIGITAL_START_ADDR, 30);       // ogni 30 secondi
       EEPROM.write(EEPROM_DIGITAL_DURATION_ADDR, 10);    // dura 10 secondi
       
       // per testo scorrevole / for scrolling text
       EEPROM.write(EEPROM_SCROLL_PAUSE_ADDR, 3);        // pausa 3 secondi
       uint16_t defaultSpeed = 50;                      // velocità default 50ms
       EEPROM.write(EEPROM_SCROLL_SPEED_ADDR, defaultSpeed & 0xFF);
       EEPROM.write(EEPROM_SCROLL_SPEED_ADDR + 1, (defaultSpeed >> 8) & 0xFF);
       EEPROM.write(EEPROM_SCROLL_SHOW_TEXT_ADDR, 1);    // mostra testo
       EEPROM.write(EEPROM_SCROLL_SHOW_DATE_ADDR, 0);    // non mostra data
       EEPROM.write(EEPROM_SCROLL_SHOW_TIME_ADDR, 0);    // non mostra ora
       EEPROM.write(EEPROM_SCROLL_FONT_SIZE_ADDR, 2);    // font medio
       EEPROM.write(EEPROM_SCROLL_SHOW_PRESET_ADDR, 1);  // mostra preset durante pausa
       EEPROM.write(EEPROM_SCROLL_TEXTUAL_DATETIME_ADDR, 0);  // data/ora numerica
       EEPROM.write(EEPROM_SCROLL_ENABLED_ADDR, 0); // default scroll disabilitato

       EEPROM.commit();
       
       Serial.println("EEPROM inizializzata con valori default");
   }
   
   // =========================================================================
   // FASE 2: CARICAMENTO VALORI DA EEPROM
   // =========================================================================
   
   // preset / preset
   currentPreset = EEPROM.read(EEPROM_PRESET_ADDR);
   currentBlink = EEPROM.read(EEPROM_BLINK_ADDR);
   
   // Carica luminosità salvata (questa non verrà più usata, ma la teniamo per compatibilità)
   uint8_t savedBrightness = EEPROM.read(EEPROM_BRIGHTNESS_ADDR);
   
   // Carica colore salvato
   uint8_t savedR = EEPROM.read(EEPROM_COLOR_R_ADDR);
   uint8_t savedG = EEPROM.read(EEPROM_COLOR_G_ADDR);
   uint8_t savedB = EEPROM.read(EEPROM_COLOR_B_ADDR);
   uint8_t savedPreset = EEPROM.read(EEPROM_COLOR_PRESET_ADDR);
   uint8_t scrollR = EEPROM.read(EEPROM_SCROLLTEXT_R_ADDR);
   uint8_t scrollG = EEPROM.read(EEPROM_SCROLLTEXT_G_ADDR);
   uint8_t scrollB = EEPROM.read(EEPROM_SCROLLTEXT_B_ADDR);
   if(scrollR != 255 || scrollG != 255 || scrollB != 255) {
       scrollTextColor = CRGB(scrollR, scrollG, scrollB);
   }
   scrollTextRainbow = EEPROM.read(EEPROM_SCROLLTEXT_RAINBOW_ADDR) == 1;

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
   digitaleInterval = EEPROM.read(EEPROM_DIGITAL_START_ADDR);
   digitaleDuration = EEPROM.read(EEPROM_DIGITAL_DURATION_ADDR);
   scrollPauseSeconds = EEPROM.read(EEPROM_SCROLL_PAUSE_ADDR);
   
   // byte / byte
   scrollTextSpeedMs = EEPROM.read(EEPROM_SCROLL_SPEED_ADDR) | (EEPROM.read(EEPROM_SCROLL_SPEED_ADDR_H) << 8);
   if (scrollTextSpeedMs < 10 || scrollTextSpeedMs > 500) {
       scrollTextSpeedMs = 50; // default
   }
   
   scrollFontSize = EEPROM.read(EEPROM_SCROLL_FONT_SIZE_ADDR);
   scrollShowText = (EEPROM.read(EEPROM_SCROLL_SHOW_TEXT_ADDR) == 1);
   scrollShowDate = (EEPROM.read(EEPROM_SCROLL_SHOW_DATE_ADDR) == 1);
   scrollShowTime = (EEPROM.read(EEPROM_SCROLL_SHOW_TIME_ADDR) == 1);

   scrollShowPresetDuringPause = (EEPROM.read(EEPROM_SCROLL_SHOW_PRESET_ADDR) == 1);
   scrollTextualDateTime = (EEPROM.read(EEPROM_SCROLL_TEXTUAL_DATETIME_ADDR) == 1);
   scrollEnabledRaw = EEPROM.read(EEPROM_SCROLL_ENABLED_ADDR);
   scrollEnabled = (scrollEnabledRaw == 1);  // convertire in bool

   // Se EEPROM non inizializzata, default true per showPreset
   if (EEPROM.read(EEPROM_SCROLL_SHOW_PRESET_ADDR) == 255) scrollShowPresetDuringPause = true;
   
   // Se EEPROM non inizializzata per scrollShowText, default true
   if (EEPROM.read(EEPROM_SCROLL_SHOW_TEXT_ADDR) == 255) scrollShowText = true;
   // Validazione / Validation
   if (digitaleInterval < 5 || digitaleInterval > 120) digitaleInterval = 30;
   if (digitaleDuration < 10 || digitaleDuration > 59) digitaleDuration = 10;
   if (scrollPauseSeconds > 60) scrollPauseSeconds = 3;
   if (scrollFontSize < 1 || scrollFontSize > 3) scrollFontSize = 2;  // Default: medio

   // Carica testo scorrevole da EEPROM (PRIMA di applyPreset per averlo disponibile)
   uint8_t textLen = EEPROM.read(EEPROM_SCROLL_TEXT_LEN_ADDR);
   if (textLen > 0 && textLen <= EEPROM_SCROLL_TEXT_MAX_LEN) {
       customScrollText = "";
       for (uint8_t i = 0; i < textLen; i++) {
           char c = EEPROM.read(EEPROM_SCROLL_TEXT_START_ADDR + i);
           if (c >= 32 && c <= 126) {  // Solo caratteri stampabili
               customScrollText += c;
           }
       }
       Serial.print("Testo scorrevole caricato: ");
       Serial.println(customScrollText);
   }

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
   Serial.print("Velocità testo scorrevole: ");
   Serial.print(scrollTextSpeedMs);
   Serial.println(" ms");
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
       // currentHour = myTZ.hour();
       // currentMinute = myTZ.minute();
       // currentSecond = myTZ.second();
       
       updateCurrentTimeFromTZ();

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

   // colore, rainbow / color, rainbow
   // per sovrascrivere i valori di default con quelli salvati dall'utente
   loadModeSettings(currentPreset);

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
            // Gestito da updateFadeEffect() nel loop principale
            return;

  case MODE_SLOW:
            // Gestito da updateSlowEffect() nel loop principale
            return;


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
            return motoCompleted;
        case MODE_DROP:
            return dropState.completed;
        case MODE_SLOW:
            return slowState.completed;
        case MODE_FADE:
            return fadeState.completed;
        case MODE_PACMAN:
            // Pacman è completato quando pacmanCompletionTime è diverso da 0
            return (pacmanCompletionTime != 0);
        case MODE_GALAGA:
            return !galagaEffectActive;
        case MODE_TRON:
            return !tronEffectActive;
        case MODE_DIGITALE:
            // Già in digitale, non serve overlay
            return false;
        case MODE_MATRIX:
        case MODE_FAST:
        case MODE_RAINBOW:
            // Effetti continui senza fine: usa il fallback
            return (now - lastEffectChangeTime >= DEFAULT_EFFECT_CYCLE_MS);
        default:
            return (now - lastEffectChangeTime >= DEFAULT_EFFECT_CYCLE_MS);
    }
}

// Gestione overlay digitale
// - Effetto parte al secondo 0
// - Quando effetto finisce, mostra orario statico
// - Dopo intervallo impostato, appare orologio digitale per durata impostata
// - Ciclo continua fino al secondo 59, poi al secondo 0 riparte effetto
bool effectWasRunning = false;  // Per tracciare se l'effetto era in esecuzione
uint32_t effectCompletedTime = 0;  // Quando l'effetto ha completato il suo ciclo
bool showingDigitalOverlay = false;  // True quando stiamo mostrando l'overlay digitale

void handleDigitalOverlay() {
    // Se la modalità è stata selezionata manualmente, non fare overlay
    if (manualModeSelected) {
        showingDigitalOverlay = false;
        return;
    }

    // Se overlay digitale disabilitato
    if (!digitalOverlayEnabled) {
        showingDigitalOverlay = false;
        return;
    }

    uint32_t now = millis();

    // Controlla stato effetto (usando la modalità originale se siamo in overlay)
    uint8_t modeToCheck = showingDigitalOverlay ? savedModeBeforeDigital : currentMode;

    // Salva temporaneamente currentMode per controllare effectCycleCompleted
    uint8_t originalMode = currentMode;
    currentMode = modeToCheck;
    bool effectCompleted = effectCycleCompleted();
    currentMode = originalMode;

    // Al secondo 0, se l'effetto era completato, deve ripartire
    if (currentSecond == 0 && effectCompletedTime != 0) {
        // Reset per far ripartire l'effetto
        if (showingDigitalOverlay) {
            currentMode = savedModeBeforeDigital;
        }
        showingDigitalOverlay = false;
        effectWasRunning = false;
        effectCompletedTime = 0;

        // Reinizializza l'effetto specifico
        if (currentMode == MODE_PACMAN) {
            initPacmanEffect();
        } else if (currentMode == MODE_MOTO) {
            initMotoEffect();
        } else if (currentMode == MODE_GALAGA) {
            initGalagaEffect();
        }
        return;
    }

    // Se siamo in overlay digitale attivo
    if (showingDigitalOverlay) {
        // Termina dopo la durata impostata
        if (now - digitalOverlayStart >= (uint32_t)digitaleDuration * 1000) {
            currentMode = savedModeBeforeDigital;
            showingDigitalOverlay = false;
            lastDigitaleTrigger = now;  // Reset timer per prossimo ciclo
        }
        return;
    }

    // Effetto in esecuzione
    if (!effectCompleted) {
        effectWasRunning = true;
        effectCompletedTime = 0;
        return;
    }

    // Effetto completato - salva il momento (solo la prima volta)
    if (effectCompletedTime == 0) {
        effectCompletedTime = now;
        lastDigitaleTrigger = now;
    }

    // Effetto completato, controlla se è ora di mostrare orologio digitale
    if (now - lastDigitaleTrigger >= (uint32_t)digitaleInterval * 1000) {
        savedModeBeforeDigital = currentMode;
        currentMode = MODE_DIGITALE;
        showingDigitalOverlay = true;
        digitalOverlayStart = now;
    }
}

void updateCurrentTimeFromTZ() {
    // Assicurati che myTZ sia già sincronizzato con NTP in setup()
    currentHour   = myTZ.hour();    // ora locale con fuso e ora legale
    currentMinute = myTZ.minute();
    currentSecond = myTZ.second();
}



void loop() {

// Aggiorna ezTime (NTP, DST) e le variabili usate dal display
    events();                 // ezTime
    updateCurrentTimeFromTZ();  // allinea currentHour/Minute/Second a myTZ

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
    if (scrollEnabled) {
        if(webInitialized) {
            server.handleClient();
        }
        updateScrollText();
        yield();
        
        // secondi / seconds
        if (scrollPauseActive && !pauseDelayActive) {
            return;  // Schermo spento per 2 secondi
        }
        
        // Se siamo in pausa MA il flag è disattivo, tieni schermo spento
        if (scrollPauseActive && !scrollShowPresetDuringPause) {
            FastLED.clear();
            FastLED.show();
            return;  // Schermo spento durante tutta la pausa
}

        // preset / preset
        if (scrollPauseActive && scrollShowPresetDuringPause && pauseDelayActive) {
            // Imposta il mode corretto se è cambiato
            if (lastPresetDuringPause != currentPreset) {
                lastPresetDuringPause = currentPreset;
                applyPreset(currentPreset);
                loadModeSettings(currentPreset);
            }
            // preset / preset
        } else if (!scrollPauseActive) {
            return;  // Durante lo scroll, blocca il resto
        }
    }
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
       uint8_t newHour = myTZ.hour();
       uint8_t newMinute = myTZ.minute();
       currentSecond = myTZ.second();

       // Controlla se è cambiato il minuto - resetta gli effetti
       if(newMinute != currentMinute || newHour != currentHour) {
           currentHour = newHour;
           currentMinute = newMinute;

           // Reset TUTTI gli effetti per farli ripartire allo scoccare del minuto
           slowState.active = false;
           slowState.completed = false;
           fadeState.active = false;
           fadeState.completed = false;
           dropState.active = false;
           dropState.completed = false;
           matrix2State.needsReset = true;
           matrix2State.completed = false;
           tronNeedsInit = true;
           tronEffectActive = true;
           tronCycleStart = 0;
           galagaEffectActive = false;
           pacmanEffectActive = false;
           motoEffectActive = false;
           motoCompleted = false;
           scrollTextInitialized = false;  // Reset testo scorrevole

           // Reinizializza effetti specifici se attivi
           if (currentMode == MODE_MOTO) {
               initMotoEffect();
           }
           if (currentMode == MODE_GALAGA) {
               initGalagaEffect();
           }
           if (currentMode == MODE_PACMAN) {
               initPacmanEffect();
           }
           if (currentMode == MODE_TRON) {
               memset(tronIntensity, 0, sizeof(tronIntensity));
           }
       }

       lastUpdate = currentMillis;
   }

    // Applica la luminosità solo se non siamo in modalità sleep
   if (!currentlySleepTime) {
       FastLED.setBrightness(intBrightness);
   }

if (!displayOff) {
    // Aggiorna il colore rainbow (cicla attraverso lo spettro)
    if (textRainbowMode && currentMillis - lastRainbowUpdate > RAINBOW_SPEED) {
        rainbowHue += 3;  // Incrementa per scorrere i colori
        lastRainbowUpdate = currentMillis;
    }

    // --- Disegno modalità corrente ---
    if(currentMode == MODE_MATRIX) {
        if(currentMillis - lastMatrixUpdate > 16) { updateMatrix(); lastMatrixUpdate = currentMillis; }
    }
    else if(currentMode == MODE_MATRIX2) {
        if(currentMillis - lastMatrixUpdate > 16) { updateMatrix2(); lastMatrixUpdate = currentMillis; }
    }
    else if(currentMode == MODE_SLOW) {
        updateSlowEffect();
    }
    else if(currentMode == MODE_FADE) {
        updateFadeEffect();
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
    else if(currentMode == MODE_DROP) {
        updateDropEffect();
    }
    else if(currentMode == MODE_MOTO) {
        updateMotoEffect();
    }
    else if(currentMode == MODE_RAINBOW) {
        updateRainbowEffect();
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

// [GESTIONE PULSANTI E TEMPI DI RILASCIO]

void checkButtons() {
    static uint32_t lastAction = 0;
    static uint32_t bothPressStart = 0;
    static uint32_t modePressStart = 0;
    static uint32_t colorPressStart = 0;

    static bool modeLongDone = false;
    static bool colorLongDone = false;

    // NUOVI FLAG (essenziali per correggere la tua logica)
    static bool modeWasPressed = false;
    static bool colorWasPressed = false;

    const uint32_t DEBOUNCE_TIME = 300;
    const uint32_t RESET_WIFI_TIME = 5000;
    const uint32_t DIGITAL_TOGGLE_TIME = 6000;

#if BUTTON_LOGIC_INVERTED == 1
    bool isMode = !digitalRead(BUTTON_MODE);
    bool isColor = !digitalRead(BUTTON_SEC);
#else
    bool isMode = digitalRead(BUTTON_MODE);
    bool isColor = digitalRead(BUTTON_SEC);
#endif

    uint32_t now = millis();

    /*
     * 1) PRESSIONE CONTEMPORANEA PER RESET WIFI
     */
    if (isMode && isColor) {
        if (bothPressStart == 0) {
            bothPressStart = now;
            Serial.println("Inizio conteggio reset WiFi...");
        }
        if (now - bothPressStart >= RESET_WIFI_TIME) {
            resetWiFi();
        }
        return;  // blocca tutto il resto
    } else {
        bothPressStart = 0;
    }

    /*
     * 2) GESTIONE PULSANTE MODE
     */
    if (isMode && !isColor) {

        if (!modeWasPressed) {              // appena premuto
            modePressStart = now;
            modeLongDone = false;
            modeWasPressed = true;
        }

        // pressione lunga
        if (!modeLongDone && (now - modePressStart >= DIGITAL_TOGGLE_TIME)) {
            digitalOverlayEnabled = !digitalOverlayEnabled;
            EEPROM.write(EEPROM_DIGITAL_OVERLAY_ADDR, digitalOverlayEnabled);
            EEPROM.commit();
            Serial.println(digitalOverlayEnabled ? "Overlay ABILITATO (MODE)" : "Overlay DISABILITATO (MODE)");
            modeLongDone = true;
        }

    } else {

        // rilascio del pulsante MODE
        if (modeWasPressed) {

            // pressione breve SOLO se non c'è stata lunga
            if (!modeLongDone && (now - modePressStart < DIGITAL_TOGGLE_TIME)
                && now - lastAction > DEBOUNCE_TIME) {

                currentPreset = (currentPreset + 1) % 19;
                applyPreset(currentPreset);
                EEPROM.write(EEPROM_PRESET_ADDR, currentPreset);
                EEPROM.commit();
                Serial.println("Preset -> " + String(currentPreset));

                lastAction = now;
            }
        }

        modeWasPressed = false;
        modePressStart = 0;
        modeLongDone = false;
    }


    /*
     * 3) GESTIONE PULSANTE COLOR
     */
    if (isColor && !isMode) {

        if (!colorWasPressed) {            // appena premuto
            colorPressStart = now;
            colorLongDone = false;
            colorWasPressed = true;
        }

        // pressione lunga
        if (!colorLongDone && (now - colorPressStart >= DIGITAL_TOGGLE_TIME)) {
            digitalOverlayEnabled = !digitalOverlayEnabled;
            EEPROM.write(EEPROM_DIGITAL_OVERLAY_ADDR, digitalOverlayEnabled);
            EEPROM.commit();
            Serial.println(digitalOverlayEnabled ? "Overlay ABILITATO (SEC)" : "Overlay DISABILITATO (SEC)");
            colorLongDone = true;
        }

    } else {

        // rilascio del pulsante COLOR
        if (colorWasPressed) {

            if (!colorLongDone && (now - colorPressStart < DIGITAL_TOGGLE_TIME)
                && now - lastAction > DEBOUNCE_TIME) {

                currentBlink = (currentBlink == 0) ? 1 : 0;
                EEPROM.write(EEPROM_BLINK_ADDR, currentBlink);
                EEPROM.commit();
                Serial.println(currentBlink ? "Blink ON" : "Blink OFF");

                lastAction = now;
            }
        }

        colorWasPressed = false;
        colorPressStart = 0;
        colorLongDone = false;
    }
}




void applyPreset(uint8_t preset) {
   // preset / preset
   digitalOverlayActive = false;
   waitingForDigital = false;
   lastEffectChangeTime = millis();

   // Reset stato effetti per ripartire da capo
   slowState.active = false;
   slowState.completed = false;
   fadeState.active = false;
   fadeState.completed = false;

   // Di default, l'overlay digitale automatico può funzionare
   // Verrà impostato a true solo per DIGITALE e PACMAN
   manualModeSelected = false;

   switch(preset) {
       case 0:
           currentMode = MODE_SLOW;
           randomColor = true;
           clockColors.current = CRGB(random8(), random8(), random8());
           slowState.active = false;
           slowState.completed = false;  // Parte subito
           break;

       case 1:
           currentMode = MODE_FADE;
           clockColors.current = CRGB::Blue;
           fadeState.active = false;
           fadeState.completed = false;  // Parte subito
           break;

       case 2:
           currentMode = MODE_MATRIX2;
           matrixWordColor = CRGB::Green;
           matrix2State.needsReset = true;
           matrix2State.completed = false;  // Parte subito
           break;

       case 3:
           currentMode = MODE_SLOW;
           randomColor = false;
           clockColors.current = CRGB(255,165,0); // slow arancione
           slowState.active = false;
           slowState.completed = false;  // Parte subito
           break;

       case 4:
           currentMode = MODE_MATRIX2;
           matrixWordColor = CRGB::Blue;
           matrix2State.needsReset = true;
           matrix2State.completed = false;  // Parte subito
           break;
           
       case 5:
           currentMode = MODE_MATRIX;
           matrixWordColor = CRGB::Yellow;
           break;
           
       case 6:
           currentMode = MODE_FADE;
           clockColors.current = CRGB::Green;
           fadeState.active = false;
           fadeState.completed = false;  // Parte subito
           break;

       case 7:
           currentMode = MODE_SLOW;
           randomColor = false;
           clockColors.current = CRGB::Blue; // slow blu
           slowState.active = false;
           slowState.completed = false;  // Parte subito
           break;
           
       case 8:
           currentMode = MODE_MATRIX;
           matrixWordColor = CRGB::Cyan;
           break;
           
       case 9:
           currentMode = MODE_FADE;
           clockColors.current = CRGB(255,20,147);
           fadeState.active = false;
           fadeState.completed = false;  // Parte subito
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
           tronNeedsInit = true;  // Parte subito
           tronEffectActive = true;
           tronCycleStart = 0;
           memset(tronIntensity, 0, sizeof(tronIntensity));
           break;

       case 14:
           currentMode = MODE_DROP;
           dropState.active = false;
           dropState.completed = false;  // Parte subito
           break;

       case 15:
           currentMode = MODE_MOTO;
           initMotoEffect();  // Parte subito
           break;

       case 16:
           currentMode = MODE_GALAGA;
           galagaTextHue = random8();
           initGalagaEffect();  // Parte subito
           break;

       case 17:
           currentMode = MODE_PACMAN;
           manualModeSelected = false;  // Permetti overlay digitale
           initPacmanEffect();
           break;

       case 18:
           currentMode = MODE_DIGITALE;
           manualModeSelected = true;  // Non interrompere con overlay
           lastDigitaleUpdate = 0;
           break;

       default:
           currentMode = MODE_SLOW;
           randomColor = true;
           clockColors.current = CRGB(random8(), random8(), random8());
           slowState.active = false;
           slowState.completed = false;  // Parte subito
           break;
   }
   
   FastLED.setBrightness(intBrightness);  // Mantiene la luminosità corretta
   updateDisplay();
   
   uint16_t baseAddr = EEPROM_MODE_SETTINGS_START + (preset * EEPROM_MODE_SETTINGS_SIZE);
   uint8_t marker = EEPROM.read(baseAddr);
    
   if (marker != EEPROM_MODE_MARKER) {
       // preset / preset
       saveModeSettings(preset);
       Serial.print("Impostazioni auto-salvate per preset: ");
       Serial.println(preset);
   }

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

// Funzione non-bloccante per effetto FADE
void updateFadeEffect() {
    uint32_t now = millis();
    CRGB currentColor = clockColors.getColor();
    const uint8_t* hourWord = (const uint8_t*)pgm_read_ptr(&HOUR_WORDS[currentHour]);

    // secondi / seconds
    if (fadeState.completed) {
        FastLED.clear();
        displayWord(WORD_SONO_LE, currentColor);
        displayWord(hourWord, currentColor);
        if (currentMinute > 0) {
            displayWord(WORD_E, currentColor);
            showMinutes(currentMinute, currentColor);
            displayWord(WORD_MINUTI, currentColor);
            showSeconds(currentSecond, currentColor);
        }
        FastLED.show();
        return;
    }

    // Inizializza l'effetto se non attivo
    if (!fadeState.active) {
        fadeState.active = true;
        fadeState.phase = 0;
        fadeState.step = 0;
        fadeState.lastStepTime = now;
        FastLED.clear();
    }

    // Attendi 50ms tra ogni step
    if (now - fadeState.lastStepTime < 50) {
        return;
    }
    fadeState.lastStepTime = now;

    // Calcola luminosità corrente
    uint8_t brightness = map(fadeState.step, 0, fadeState.STEPS - 1, 0, 255);
    CRGB fadeColor = currentColor;
    fadeColor.nscale8(brightness);

    // Mostra le parole già completate a piena luminosità
    FastLED.clear();

    // Parole già completate (piena luminosità)
    if (fadeState.phase > 0) {
        displayWord(WORD_SONO_LE, currentColor);
    }
    if (fadeState.phase > 1) {
        displayWord(hourWord, currentColor);
    }
    if (fadeState.phase > 2 && currentMinute > 0) {
        displayWord(WORD_E, currentColor);
    }
    if (fadeState.phase > 3 && currentMinute > 0) {
        showMinutes(currentMinute, currentColor);
    }
    if (fadeState.phase > 4 && currentMinute > 0) {
        displayWord(WORD_MINUTI, currentColor);
        showSeconds(currentSecond, currentColor);
    }

    // Parola corrente in fade
    switch (fadeState.phase) {
        case 0: // SONO LE
            displayWord(WORD_SONO_LE, fadeColor);
            break;
        case 1: // Ora
            displayWord(WORD_SONO_LE, currentColor);
            displayWord(hourWord, fadeColor);
            break;
        case 2: // E
            if (currentMinute > 0) {
                displayWord(WORD_E, fadeColor);
            }
            break;
        case 3: // Minuti numero
            if (currentMinute > 0) {
                showMinutes(currentMinute, fadeColor);
            }
            break;
        case 4: // MINUTI parola
            if (currentMinute > 0) {
                displayWord(WORD_MINUTI, fadeColor);
            }
            break;
    }

    FastLED.show();

    // Avanza step
    fadeState.step++;
    if (fadeState.step >= fadeState.STEPS) {
        fadeState.step = 0;
        fadeState.phase++;

        // Se non ci sono minuti, salta le fasi 2,3,4
        if (currentMinute == 0 && fadeState.phase == 2) {
            fadeState.phase = 5;
        }

        // Completato - aspetta cambio minuto
        if (fadeState.phase > 4) {
            fadeState.completed = true;
        }
    }
}

// Funzione non-bloccante per effetto SLOW
void updateSlowEffect() {
    uint32_t now = millis();
    const uint8_t* hourWord = (const uint8_t*)pgm_read_ptr(&HOUR_WORDS[currentHour]);

    // secondi / seconds
    if (slowState.completed) {
        FastLED.clear();
        displayWord(WORD_SONO_LE, slowState.sonoColor);
        displayWord(hourWord, slowState.hourColor);
        if (currentMinute > 0) {
            displayWord(WORD_E, slowState.eColor);
            showMinutes(currentMinute, slowState.minutesColor);
            displayWord(WORD_MINUTI, slowState.minutiColor);
            showSeconds(currentSecond, slowState.minutiColor);
        }
        FastLED.show();
        return;
    }

    // Inizializza l'effetto se non attivo
    if (!slowState.active) {
        slowState.active = true;
        slowState.step = 0;
        slowState.targetBrightness = intBrightness;
        slowState.increment = slowState.targetBrightness / 30;
        if (slowState.increment == 0) slowState.increment = 1;
        slowState.lastStepTime = now;
        slowState.holdPhase = false;

        // Imposta i colori
        if (randomColor) {
            slowState.sonoColor = CRGB(random8(), random8(), random8());
            slowState.hourColor = CRGB(random8(), random8(), random8());
            slowState.eColor = CRGB(random8(), random8(), random8());
            slowState.minutesColor = CRGB(random8(), random8(), random8());
            slowState.minutiColor = CRGB(random8(), random8(), random8());
        } else {
            slowState.sonoColor = clockColors.current;
            slowState.hourColor = clockColors.current;
            slowState.eColor = clockColors.current;
            slowState.minutesColor = clockColors.current;
            slowState.minutiColor = clockColors.current;
        }
    }

    // Fase di hold (mantieni acceso)
    if (slowState.holdPhase) {
        // secondi / seconds
        FastLED.clear();
        displayWord(WORD_SONO_LE, slowState.sonoColor);
        displayWord(hourWord, slowState.hourColor);
        if (currentMinute > 0) {
            displayWord(WORD_E, slowState.eColor);
            showMinutes(currentMinute, slowState.minutesColor);
            displayWord(WORD_MINUTI, slowState.minutiColor);
            showSeconds(currentSecond, slowState.minutiColor);
        }
        FastLED.show();

        if (now - slowState.holdStartTime >= 500) {
            // Effetto completato - aspetta cambio minuto
            slowState.completed = true;
        }
        return;
    }

    // Fase di fade-in
    if (now - slowState.lastStepTime >= 70) {
        slowState.lastStepTime = now;

        uint8_t currentBrightLevel = slowState.step * slowState.increment;
        if (currentBrightLevel > slowState.targetBrightness) {
            currentBrightLevel = slowState.targetBrightness;
        }

        FastLED.clear();

        uint8_t scaleFactor = map(currentBrightLevel, 0, slowState.targetBrightness, 0, 255);

        CRGB sonoFade = slowState.sonoColor;
        sonoFade.nscale8(scaleFactor);
        displayWord(WORD_SONO_LE, sonoFade);

        CRGB hourFade = slowState.hourColor;
        hourFade.nscale8(scaleFactor);
        displayWord(hourWord, hourFade);

        if (currentMinute > 0) {
            CRGB eFade = slowState.eColor;
            eFade.nscale8(scaleFactor);
            displayWord(WORD_E, eFade);

            CRGB minutesFade = slowState.minutesColor;
            minutesFade.nscale8(scaleFactor);
            showMinutes(currentMinute, minutesFade);

            CRGB minutiFade = slowState.minutiColor;
            minutiFade.nscale8(scaleFactor);
            displayWord(WORD_MINUTI, minutiFade);
        }

        FastLED.show();

        slowState.step++;

        // Se abbiamo raggiunto la luminosità target, passa alla fase hold
        if (currentBrightLevel >= slowState.targetBrightness) {
            slowState.holdPhase = true;
            slowState.holdStartTime = now;
        }
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
        // Mostra orario direttamente
        displayWord(WORD_SONO_LE, matrixWordColor);
        const uint8_t* hourWord = (const uint8_t*)pgm_read_ptr(&HOUR_WORDS[currentHour]);
        displayWord(hourWord, matrixWordColor);
        if (currentMinute > 0) {
            displayWord(WORD_E, matrixWordColor);
            showMinutes(currentMinute, matrixWordColor);
            displayWord(WORD_MINUTI, matrixWordColor);
            showSeconds(currentSecond, matrixWordColor);
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

// Inizializza le moto TRON in posizioni diverse
void initTronBikes() {
    // Colori delle moto (stile TRON)
    CRGB bikeColors[NUM_TRON_BIKES] = {
        CRGB(0, 200, 255),    // Ciano (protagonista)
        CRGB(255, 100, 0),    // Arancione
        CRGB(200, 0, 255)     // Viola
    };

    // Posizioni di partenza diverse (ai bordi della matrice)
    for(uint8_t i = 0; i < NUM_TRON_BIKES; i++) {
        tronBikes[i].active = true;
        tronBikes[i].color = bikeColors[i];
        tronBikes[i].trailLen = 0;

        switch(i) {
            case 0: // Parte da sinistra, va a destra
                tronBikes[i].x = 0;
                tronBikes[i].y = 4;
                tronBikes[i].dir = 1;
                break;
            case 1: // Parte da destra, va a sinistra
                tronBikes[i].x = MATRIX_WIDTH - 1;
                tronBikes[i].y = 11;
                tronBikes[i].dir = 3;
                break;
            case 2: // Parte dal basso, va su
                tronBikes[i].x = 8;
                tronBikes[i].y = MATRIX_HEIGHT - 1;
                tronBikes[i].dir = 0;
                break;
        }
    }

    // Reset intensità scie
    memset(tronIntensity, 0, sizeof(tronIntensity));
    tronNeedsInit = false;
}

// Controlla se una posizione è libera (no collisione)
bool tronPosIsFree(int8_t x, int8_t y, uint8_t bikeIndex) {
    if(x < 0 || x >= MATRIX_WIDTH || y < 0 || y >= MATRIX_HEIGHT) {
        return false;
    }

    uint16_t pos = xyToLED(x, y);
    if(pos >= NUM_LEDS) return false;

    // Controlla se c'è una scia
    if(tronIntensity[pos] > 50) {
        return false;
    }

    return true;
}

// Trova una direzione sicura per la moto
int8_t tronFindSafeDir(uint8_t bikeIndex) {
    TronBike &bike = tronBikes[bikeIndex];

    // Direzioni: 0=su, 1=destra, 2=giù, 3=sinistra
    int8_t dx[] = {0, 1, 0, -1};
    int8_t dy[] = {-1, 0, 1, 0};

    // Prima prova a continuare dritto
    int8_t newX = bike.x + dx[bike.dir];
    int8_t newY = bike.y + dy[bike.dir];
    if(tronPosIsFree(newX, newY, bikeIndex)) {
        // 70% continua dritto, 30% gira
        if(random8(100) < 70) {
            return bike.dir;
        }
    }

    // Prova le altre direzioni (no inversione)
    int8_t possibleDirs[3];
    uint8_t numPossible = 0;

    for(int8_t d = 0; d < 4; d++) {
        // Non tornare indietro
        if((d + 2) % 4 == bike.dir) continue;

        newX = bike.x + dx[d];
        newY = bike.y + dy[d];
        if(tronPosIsFree(newX, newY, bikeIndex)) {
            possibleDirs[numPossible++] = d;
        }
    }

    if(numPossible > 0) {
        return possibleDirs[random8(numPossible)];
    }

    // Nessuna direzione sicura - moto si ferma
    return -1;
}

void updateTronEffect() {
    uint32_t currentMillis = millis();

    // Se non attivo e non deve partire, mostra solo orario
    if (!tronEffectActive && !tronNeedsInit) {
        FastLED.clear();
        showCurrentTime();
        FastLED.show();
        return;
    }

    if(tronCycleStart == 0) {
        tronCycleStart = currentMillis;
    }

    uint32_t cycleElapsed = currentMillis - tronCycleStart;

    // Gestione ciclo effetto/testo
    if(tronEffectActive && cycleElapsed > TRON_EFFECT_DURATION) {
        tronEffectActive = false;
        tronCycleStart = currentMillis;
        tronNeedsInit = false;  // Non ripartire fino al cambio minuto
        FastLED.clear();
        showCurrentTime();
        FastLED.show();
        return;
    }

    // se necessario / if needed
    if(tronNeedsInit) {
        initTronBikes();
    }

    if(currentMillis - lastTronUpdate < TRON_UPDATE_INTERVAL) return;
    lastTronUpdate = currentMillis;

    FastLED.clear();

    if(tronEffectActive) {
        // Fade delle scie
        for(uint16_t i = 0; i < NUM_LEDS; i++) {
            if(tronIntensity[i] > 15) {
                tronIntensity[i] -= 15;
            } else {
                tronIntensity[i] = 0;
            }
        }

        // Muovi ogni moto
        for(uint8_t i = 0; i < NUM_TRON_BIKES; i++) {
            TronBike &bike = tronBikes[i];
            if(!bike.active) continue;

            // Trova direzione sicura
            int8_t newDir = tronFindSafeDir(i);

            if(newDir < 0) {
                // Moto bloccata - riposiziona
                bike.x = random8(MATRIX_WIDTH);
                bike.y = random8(MATRIX_HEIGHT);
                bike.dir = random8(4);
                bike.trailLen = 0;
                continue;
            }

            bike.dir = newDir;

            // Calcola nuova posizione
            int8_t dx[] = {0, 1, 0, -1};
            int8_t dy[] = {-1, 0, 1, 0};

            // Salva posizione nella scia
            if(bike.trailLen < TRON_TRAIL_LENGTH) {
                bike.trailX[bike.trailLen] = bike.x;
                bike.trailY[bike.trailLen] = bike.y;
                bike.trailLen++;
            } else {
                // Shift scia
                for(uint8_t t = 0; t < TRON_TRAIL_LENGTH - 1; t++) {
                    bike.trailX[t] = bike.trailX[t + 1];
                    bike.trailY[t] = bike.trailY[t + 1];
                }
                bike.trailX[TRON_TRAIL_LENGTH - 1] = bike.x;
                bike.trailY[TRON_TRAIL_LENGTH - 1] = bike.y;
            }

            // Aggiorna intensità scia nella posizione attuale
            uint16_t pos = xyToLED(bike.x, bike.y);
            if(pos < NUM_LEDS) {
                tronIntensity[pos] = 255;
            }

            // Muovi
            bike.x += dx[bike.dir];
            bike.y += dy[bike.dir];

            // Wrap around ai bordi
            if(bike.x < 0) bike.x = MATRIX_WIDTH - 1;
            if(bike.x >= MATRIX_WIDTH) bike.x = 0;
            if(bike.y < 0) bike.y = MATRIX_HEIGHT - 1;
            if(bike.y >= MATRIX_HEIGHT) bike.y = 0;
        }

        // Disegna scie con fade
        for(uint16_t i = 0; i < NUM_LEDS; i++) {
            if(tronIntensity[i] > 0) {
                // Trova a quale moto appartiene basandosi sulla posizione
                leds[i] = CHSV(160, 255, tronIntensity[i]); // Default ciano
            }
        }

        // Disegna moto (teste luminose)
        for(uint8_t i = 0; i < NUM_TRON_BIKES; i++) {
            TronBike &bike = tronBikes[i];
            if(!bike.active) continue;

            uint16_t pos = xyToLED(bike.x, bike.y);
            if(pos < NUM_LEDS) {
                leds[pos] = bike.color;
                leds[pos].maximizeBrightness();
            }

            // Disegna scia colorata
            for(uint8_t t = 0; t < bike.trailLen; t++) {
                uint16_t trailPos = xyToLED(bike.trailX[t], bike.trailY[t]);
                if(trailPos < NUM_LEDS) {
                    CRGB trailColor = bike.color;
                    trailColor.nscale8(180 - (t * 12));
                    leds[trailPos] = trailColor;
                }
            }
        }

        showCurrentTime();

    } else {
        showCurrentTime();
    }

    FastLED.show();
}

// Effetto GOCCIA - onde concentriche che rivelano l'orario
void updateDropEffect() {
    uint32_t now = millis();

    // secondi / seconds
    if (dropState.completed) {
        FastLED.clear();
        CRGB textColor = dropState.waveColor;
        displayWord(WORD_SONO_LE, textColor);
        const uint8_t* hourWord = (const uint8_t*)pgm_read_ptr(&HOUR_WORDS[currentHour]);
        displayWord(hourWord, textColor);
        if (currentMinute > 0) {
            displayWord(WORD_E, textColor);
            showMinutes(currentMinute, textColor);
            displayWord(WORD_MINUTI, textColor);
            showSeconds(currentSecond, textColor);
        }
        FastLED.show();
        return;
    }

    // Inizializza effetto
    if (!dropState.active) {
        dropState.active = true;
        dropState.radius = 0;
        dropState.centerX = 7 + random8(3);  // Centro leggermente casuale
        dropState.centerY = 7 + random8(3);
        dropState.startTime = now;
        dropState.lastUpdate = now;
        dropState.waveColor = CHSV(random8(), 200, 255);  // Colore casuale
        dropState.waveCount = 0;

        // Prepara i target pixel per l'orario
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

    // Aggiornamento frame
    if (now - dropState.lastUpdate < DROP_UPDATE_INTERVAL) {
        return;
    }
    dropState.lastUpdate = now;

    FastLED.clear();

    // Espandi il raggio
    dropState.radius += DROP_WAVE_SPEED;

    // Se l'onda è uscita, effetto completato (solo una volta)
    if (dropState.radius > DROP_MAX_RADIUS) {
        dropState.completed = true;
        return;
    }

    // Calcola il centro della matrice
    float cx = dropState.centerX;
    float cy = dropState.centerY;

    // Disegna le onde concentriche
    for (uint8_t y = 0; y < MATRIX_HEIGHT; y++) {
        for (uint8_t x = 0; x < MATRIX_WIDTH; x++) {
            // Calcola distanza dal centro
            float dx = x - cx;
            float dy = y - cy;
            float dist = sqrt(dx * dx + dy * dy);

            uint16_t pos = xyToLED(x, y);
            if (pos >= NUM_LEDS) continue;

            // Crea effetto onda (anello che si espande)
            float waveWidth = 2.5f;  // Larghezza dell'onda
            float waveDist = abs(dist - dropState.radius);

            if (waveDist < waveWidth) {
                // Intensità basata sulla distanza dall'onda
                uint8_t intensity = 255 - (uint8_t)(waveDist * 100);

                // Se è un pixel dell'orario, coloralo di più
                if (targetPixels[pos]) {
                    leds[pos] = dropState.waveColor;
                    leds[pos].nscale8(intensity);
                } else {
                    // Onda blu/ciano per l'acqua
                    CRGB waterColor = CHSV(160, 255, intensity / 2);
                    leds[pos] = waterColor;
                }
            }

            // I pixel dell'orario già "toccati" dalle onde rimangono accesi
            if (targetPixels[pos] && dist < dropState.radius - waveWidth) {
                // Fade in graduale per i pixel rivelati
                int calcIntensity = (int)((dropState.radius - dist) * 20);
                uint8_t revealIntensity = (calcIntensity > 255) ? 255 : (uint8_t)calcIntensity;
                CRGB textColor = dropState.waveColor;
                textColor.nscale8(revealIntensity);
                if (leds[pos].getAverageLight() < textColor.getAverageLight()) {
                    leds[pos] = textColor;
                }
            }
        }
    }

    // Effetto "impatto" al centro quando la goccia cade
    if (dropState.radius < 3) {
        uint16_t centerPos = xyToLED(dropState.centerX, dropState.centerY);
        if (centerPos < NUM_LEDS) {
            leds[centerPos] = CRGB::White;
        }
    }

    FastLED.show();
}

// Effetto RAINBOW - colori arcobaleno graduali che scorrono
void updateRainbowEffect() {
    uint32_t now = millis();

    if (now - lastRainbowUpdate >= RAINBOW_SPEED) {
        lastRainbowUpdate = now;
        rainbowHue++;  // Incrementa hue per far scorrere l'arcobaleno
    }

    FastLED.clear();

    // Mostra l'orario con colori arcobaleno graduali
    // Ogni parola ha un colore diverso basato sulla posizione + hue corrente
    uint8_t hueOffset = 0;

    // SONO LE
    displayWord(WORD_SONO_LE, CHSV(rainbowHue + hueOffset, 255, 255));
    hueOffset += 40;

    // ORA
    const uint8_t* hourWord = (const uint8_t*)pgm_read_ptr(&HOUR_WORDS[currentHour]);
    displayWord(hourWord, CHSV(rainbowHue + hueOffset, 255, 255));
    hueOffset += 40;

    if (currentMinute > 0) {
        // E
        displayWord(WORD_E, CHSV(rainbowHue + hueOffset, 255, 255));
        hueOffset += 40;

        // MINUTI
        showMinutes(currentMinute, CHSV(rainbowHue + hueOffset, 255, 255));
        hueOffset += 40;

        // MINUTI (parola)
        displayWord(WORD_MINUTI, CHSV(rainbowHue + hueOffset, 255, 255));
        hueOffset += 40;

        // Secondi
        if (currentBlink == 1) {
            showSeconds(currentSecond, CHSV(rainbowHue + hueOffset, 255, 255));
        }
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
// Variabili per tracciare l'ultimo orario visualizzato dall'effetto Moto
static uint8_t motoLastHour = 255;
static uint8_t motoLastMinute = 255;

void initMotoEffect() {
    FastLED.clear();
    motoCurrentPosition = 0;
    motoEffectActive = true;
    motoCompleted = false;
    motoCompletionTime = 0;
    motoTrailHue = 0;
    motoTextHue = random8();
    motoTextColor = CHSV(motoTextHue, 255, 255);
    motoTrailLen = 0;

    // Salva l'orario corrente per rilevare cambiamenti
    motoLastHour = currentHour;
    motoLastMinute = currentMinute;

    // Scegli angolo casuale di partenza (deve partire da un angolo per coprire tutto lo schermo)
    uint8_t corner = random8(4);
    switch(corner) {
        case 0: // Alto-sinistra, va verso destra e giù
            motoX = 0;
            motoY = 0;
            motoDirX = 1;
            motoDirY = 1;
            break;
        case 1: // Alto-destra, va verso sinistra e giù
            motoX = MATRIX_WIDTH - 1;
            motoY = 0;
            motoDirX = -1;
            motoDirY = 1;
            break;
        case 2: // Basso-sinistra, va verso destra e su
            motoX = 0;
            motoY = MATRIX_HEIGHT - 1;
            motoDirX = 1;
            motoDirY = -1;
            break;
        case 3: // Basso-destra, va verso sinistra e su
            motoX = MATRIX_WIDTH - 1;
            motoY = MATRIX_HEIGHT - 1;
            motoDirX = -1;
            motoDirY = -1;
            break;
    }

    // Reset array pixel della scritta
    memset(motoTextPixels, 0, sizeof(motoTextPixels));

    for (int i = 0; i < MOTO_TRAIL_LENGTH; i++) {
        motoTrailColors[i] = CRGB::Black;
        motoTrailX[i] = 0;
        motoTrailY[i] = 0;
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

    // secondi / seconds
    if (motoCompleted) {
        // Controlla se l'orario è cambiato - se sì, reinizializza per mostrare il nuovo orario
        if (motoLastHour != currentHour || motoLastMinute != currentMinute) {
            initMotoEffect();
            motoLastHour = currentHour;
            motoLastMinute = currentMinute;
            return;
        }

        FastLED.clear();
        for (uint16_t i = 0; i < NUM_LEDS; i++) {
            if (motoTextPixels[i]) {
                leds[i] = motoTextColor;
            }
        }
        showSeconds(currentSecond, motoTextColor);
        FastLED.show();
        return;
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

    // Salva la posizione corrente nella scia
    for (int i = MOTO_TRAIL_LENGTH - 1; i > 0; i--) {
        motoTrailX[i] = motoTrailX[i - 1];
        motoTrailY[i] = motoTrailY[i - 1];
    }
    motoTrailX[0] = motoX;
    motoTrailY[0] = motoY;
    if (motoTrailLen < MOTO_TRAIL_LENGTH) motoTrailLen++;

    // Segna il pixel corrente come visitato per l'orario
    uint16_t currentPos = xyToLED(motoX, motoY);
    if (currentPos < NUM_LEDS && targetPixels[currentPos]) {
        motoTextPixels[currentPos] = true;
    }

    // Cancella tutto
    FastLED.clear();

    // Disegna la scia
    for (int i = 0; i < motoTrailLen; i++) {
        uint16_t pos = xyToLED(motoTrailX[i], motoTrailY[i]);
        if (pos < NUM_LEDS) {
            uint8_t intensity = 255 - (i * (255 / MOTO_TRAIL_LENGTH));
            CRGB trailColor = motoTrailColors[i];
            trailColor.nscale8(intensity);
            leds[pos] = trailColor;
        }
    }

    // Disegna i pixel della scritta che sono stati colorati
    for (uint16_t i = 0; i < NUM_LEDS; i++) {
        if (motoTextPixels[i]) {
            leds[i] = motoTextColor;
        }
    }

    FastLED.show();

    // Movimento a zigzag
    motoCurrentPosition++;

    // Prossima posizione orizzontale
    int8_t nextX = motoX + motoDirX;

    // Controllo bordi orizzontali
    if (nextX < 0 || nextX >= MATRIX_WIDTH) {
        // Cambia direzione orizzontale
        motoDirX = -motoDirX;
        // Vai alla prossima riga
        motoY += motoDirY;

        // Controllo se abbiamo finito (usciti dalla matrice verticalmente)
        if (motoY < 0 || motoY >= MATRIX_HEIGHT) {
            motoCompleted = true;
            return;
        }
    } else {
        motoX = nextX;
    }
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
