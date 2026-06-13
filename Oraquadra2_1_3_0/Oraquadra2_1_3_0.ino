// OraQuadra V1.3.0  -  By Davide Gatti SURVIVAL HACKING  www.survivalhacking.it
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
// V1.2.8 - 28/02/2026 - By Davide Gatti
// Sistemato un bug che in assenza di testo scorrevole e di orologio dgitale, non faceva commutare i vari effetti al cambio minuto
// Sistemato un bug nell'effto moto dove in orari pieni tipo 22:00/12:00 dove non ci sono minuti, rimaneva accesa la E
//
// V1.2.9 - 10/06/2026
// Sistemata la luminosità che non rispecchiava lo slider (doppio scaling: colore già scalato al 25% prima della luminosità globale)
// Completato il fix della E accesa a ore piene negli effetti Moto e Pacman (showSeconds scriveva comunque il LED 116)
// Sistemato effetto Matrix che mostrava l'ora sbagliata dalle 12:00 in poi (usava il formato 12 ore su quadrante 24 ore)
// Sistemato /setScrollPause che forzava sempre la pausa a 5 secondi (condizione di validazione invertita)
// Sistemato salvataggio velocità scroll al primo avvio (high byte scritto a indirizzo EEPROM sbagliato)
// Sistemate le traiettorie degli spari Galaga specchiate sulle righe pari (mancava la conversione serpentina)
// Sistemato reset effetti al secondo 0 che veniva eseguito ripetutamente per un secondo intero
// Sistemato il bottone web "Reset WiFi" che riavviava soltanto senza cancellare le credenziali
// Sistemati errori JavaScript nella pagina web (elemento inesistente, doppio listener sul toggle scroll)
// Il campo testo scorrevole nella pagina web ora viene ripopolato dal valore salvato
// Aggiunto bottone web di Reset di Fabbrica con conferma (cancella EEPROM e WiFi)
// Tutte le conferme della pagina web (reset WiFi, riavvio, reset di fabbrica) ora usano modali personalizzate al posto delle dialog del browser
// Sistemato l'effetto Lento che con luminosità vicino al 100% ripeteva il fade-in all'infinito (overflow a 8 bit nel calcolo della rampa)
//
// V1.3.0 - 11/06/2026
// Integrati 9 nuovi effetti dalla Prunca Edition V1.2.6 di Marco Prunca:
// ARKANOID, NATALE, NEVE, LASER, FUOCO, BLACK HOLE, FIRE1, FIREWORK e DRIFT (nuovi preset 19-27)
// La lettera E dei secondi non si accende mai a ore piene in nessun effetto (guardia centralizzata in showSeconds)
// Le impostazioni colore dei preset 20+ sono salvate in una nuova area EEPROM (360-399)
// ATTENZIONE: lo sketch supera la partizione app di default, compilare con Partition Scheme "Minimal SPIFFS"
// Migliorie interfaccia web: pallino stato connessione (niente piu notifiche di errore a raffica),
// badge "attiva ora" sulla fascia giorno/sera corrente, bottone Salva che pulsa con modifiche pendenti,
// evidenziazione del colore attivo, sezioni collassabili, card stato compatta su mobile,
// tooltip descrittivi sugli effetti, favicon, gerarchia bottoni pericolosi rivista
// La pagina web e stata spostata in webpage.h: risolve definitivamente i problemi del
// generatore di prototipi Arduino con virgolette e apostrofi nel codice HTML/JS
// Aggiunti 11 effetti originali (preset 28-38): PLASMA, AURORA, RADAR, CIELO STELLATO,
// BATTITO, TETRIS, SNAKE, SABBIA, TEMPORALE, BOLLE e PONG
// Le categorie della pagina web sono state riorganizzate in Effetti Speciali / Giochi / Ambient
// Aggiunto effetto LAVA LAMP (preset 39) con blob metaball che si fondono e separano
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
// Effetti integrati dalla Prunca Edition V1.2.6
#define MODE_ARKANOID 12  // Pallina rimbalzante tipo Arkanoid
#define MODE_CHRISTMAS 13 // Effetto Natalizio con colori festivi e sparkle
#define MODE_SNOWFALL 14  // Neve che cade con cristalli ghiacciati
#define MODE_LASER 15     // Raggi laser dal basso che rivelano l'orario
#define MODE_FUOCO 16     // Fiamme animate dal basso con orario visibile
#define MODE_BLACKHOLE 17 // Black hole con particelle risucchiate verso il centro
#define MODE_FIRE1 18     // Fire2012 stile WLED - simulazione fuoco realistica
#define MODE_FIREWORK 19  // Fireworks 1D stile WLED - fuochi d'artificio esplosivi
#define MODE_DRIFT 20     // Drift stile WLED - particelle che driftano lentamente
// Nuovi effetti V1.3.0
#define MODE_PLASMA 21    // Onde plasma colorate in movimento continuo
#define MODE_AURORA 22    // Aurora boreale con Perlin noise
#define MODE_RADAR 23     // Fascio radar rotante che rivela le lettere
#define MODE_STARS 24     // Cielo stellato, orario come costellazione
#define MODE_HEARTBEAT 25 // Cuore pulsante con onde che rivelano
#define MODE_TETRIS 26    // Blocchi colorati che compongono le scritte
#define MODE_SNAKE 27     // Serpente che mangia le lettere
#define MODE_SAND 28      // Sabbia che riempie le lettere dal basso
#define MODE_STORM 29     // Pioggia e lampi che rivelano le scritte
#define MODE_BUBBLES 30   // Bolle che salgono e scoppiano sulle lettere
#define MODE_PONG 31      // Pong autonomo, la pallina rivela le lettere
#define MODE_LAVA 32      // Lava lamp con blob metaball che si fondono

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

// Strutture e costanti per effetto ARKANOID
#define ARKANOID_SPEED 40         // Velocità dell'effetto (ms tra i frame)
#define ARKANOID_BALL_SPEED 0.8f  // Velocità della pallina (pixel per frame)
#define ARKANOID_BALL_SIZE 1      // Dimensione pallina (1 LED)
// Costanti per effetto CHRISTMAS
#define CHRISTMAS_SPEED 80        // Velocità cambio colore (ms tra i frame)
#define CHRISTMAS_SPARKLE_CHANCE 15 // Probabilità di sparkle (su 255)
#define CHRISTMAS_NUM_LIGHTS 8    // Numero di lucine natalizie sparse
// Costanti per effetto SNOWFALL
#define SNOWFALL_SPEED 60         // Velocità dell'effetto (ms tra i frame)
#define SNOWFALL_NUM_FLAKES 20    // Numero di fiocchi di neve contemporanei
#define SNOWFALL_FALL_SPEED 0.3f  // Velocità caduta neve (pixel per frame)
#define SNOWFALL_NUM_XMAS_LIGHTS 12 // Numero di lucine natalizie intermittenti
// Costanti per effetto LASER
#define LASER_SPEED 30            // Velocità dell'effetto (ms tra i frame)
#define LASER_NUM_BEAMS 12        // Numero di raggi laser contemporanei
#define LASER_RISE_SPEED 0.4f     // Velocità di salita laser (pixel per frame)
#define LASER_MAX_DURATION 15000  // Durata massima effetto (15 secondi)
#define LASER_BEAM_LENGTH 5       // Lunghezza del raggio laser
// Costanti per effetto FUOCO
#define FUOCO_SPEED 40            // Velocità dell'effetto (ms tra i frame)
#define FUOCO_COOLING 55          // Quanto velocemente si raffredda il fuoco
#define FUOCO_SPARKING 120        // Probabilità di nuove scintille (0-255)
#define FUOCO_HEIGHT 14           // Altezza massima fiamme
#define FUOCO_MAX_DURATION 15000  // Durata massima effetto fuoco (15 secondi)
// Costanti per effetto BLACKHOLE
#define BLACKHOLE_SPEED 40        // Velocità dell'effetto (ms tra i frame)
#define BLACKHOLE_NUM_PARTICLES 40 // Numero di particelle risucchiate
#define BLACKHOLE_MAX_DURATION 15000 // Durata massima effetto (15 secondi)
#define BLACKHOLE_SPIRAL_SPEED 0.3f  // Velocità rotazione spirale
#define BLACKHOLE_PULL_SPEED 0.25f   // Velocità attrazione verso il centro
// Costanti per effetto FIRE1 (Fire2012)
#define FIRE1_SPEED 30            // Velocità dell'effetto (ms tra i frame)
#define FIRE1_COOLING 50          // Quanto velocemente si raffredda
#define FIRE1_SPARKING 120        // Probabilità di nuove scintille (0-255)
#define FIRE1_DURATION_MS 15000   // Durata dell'effetto FIRE1 (15 secondi)
#define FIRE1_HEIGHT 10           // Altezza effetto in righe
// Costanti per effetto FIREWORK
#define FIREWORK_SPEED 40         // Velocità dell'effetto (ms tra i frame)
#define FIREWORK_DURATION_MS 15000 // Durata dell'effetto FIREWORK (15 secondi)
#define MAX_FIREWORKS 6           // Numero massimo di fuochi contemporanei
#define MAX_PARTICLES 80          // Numero massimo di particelle totali
// Costanti per effetto DRIFT
#define DRIFT_SPEED 50            // Velocità dell'effetto (ms tra i frame)
#define DRIFT_DURATION_MS 15000   // Durata dell'effetto DRIFT (15 secondi)
#define MAX_DRIFT_BLOBS 8         // Numero massimo di blob che driftano
#define DRIFT_BLUR_AMOUNT 172     // Quantità di blur/fade (0-255)

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
#define NUM_MODES 40                        // Numero totale di preset (0-39)
#define EEPROM_MODE_SETTINGS_EXT 360        // Area estesa preset 20-39 (360-459): l'area storica finisce a 349 e 350-356 è già usata dallo scroll
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
void initArkanoidEffect();
void updateArkanoidEffect();
void initChristmasEffect();
void updateChristmasEffect();
void initSnowfallEffect();
void updateSnowfallEffect();
void initLaserEffect();
void updateLaserEffect();
void initFuocoEffect();
void updateFuocoEffect();
void initBlackHoleEffect();
void updateBlackHoleEffect();
void initFire1Effect();
void updateFire1Effect();
void initFireworkEffect();
void updateFireworkEffect();
void initDriftEffect();
void updateDriftEffect();
void initPlasmaEffect();
void updatePlasmaEffect();
void initAuroraEffect();
void updateAuroraEffect();
void initRadarEffect();
void updateRadarEffect();
void initStarsEffect();
void updateStarsEffect();
void initHeartbeatEffect();
void updateHeartbeatEffect();
void initTetrisEffect();
void updateTetrisEffect();
void initSnakeEffect();
void updateSnakeEffect();
void initSandEffect();
void updateSandEffect();
void initStormEffect();
void updateStormEffect();
void initBubblesEffect();
void updateBubblesEffect();
void initPongEffect();
void updatePongEffect();
void initLavaEffect();
void updateLavaEffect();
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
uint8_t pacmanLastMinute = 255;
uint8_t motoLastMinute = 255;
uint8_t galagaLastMinute = 255;
uint8_t tronLastMinute = 255;
int16_t pacmanPosition = -2; // Posizione iniziale di Pacman (fuori dallo schermo a sinistra)
int16_t ghostPositions[NUM_GHOSTS] = {0}; // Posizioni dei fantasmi
CRGB ghostColors[NUM_GHOSTS] = {CRGB::Red, CRGB(255, 182, 255), CRGB::Cyan, CRGB(255, 165, 0)}; // Rosso, Rosa, Ciano, Arancio

// Variabili per effetto ARKANOID
uint32_t lastArkanoidUpdate = 0;
bool arkanoidEffectActive = false;
uint32_t arkanoidCompletionTime = 0;
uint32_t arkanoidStartTime = 0;              // Tempo inizio effetto per timeout
float arkanoidBallX = 8.0f;          // Posizione X della pallina (centro schermo)
float arkanoidBallY = 8.0f;          // Posizione Y della pallina (centro schermo)
float arkanoidVelX = 0.8f;           // Velocità X della pallina
float arkanoidVelY = -0.6f;          // Velocità Y della pallina
bool arkanoidTextPixels[NUM_LEDS] = {false};  // Pixel delle lettere rivelati
CRGB arkanoidBallColor = CRGB::Yellow;        // Colore della pallina
CRGB arkanoidTextColor = CRGB::White;         // Colore delle lettere rivelate
uint8_t arkanoidTextHue = 0;                  // Hue per animazione colore
// Variabili per effetto CHRISTMAS
uint32_t lastChristmasUpdate = 0;
uint8_t christmasColorIndex = 0;              // Indice del colore corrente
uint8_t christmasLightPositions[CHRISTMAS_NUM_LIGHTS]; // Posizioni delle lucine sparse
uint8_t christmasLightHues[CHRISTMAS_NUM_LIGHTS];      // Hue delle lucine
const CRGB christmasColors[4] = {
    CRGB::Red,              // Rosso
    CRGB::Green,            // Verde
    CRGB::White,            // Bianco
    CRGB(255, 215, 0)       // Oro
};
// Variabili per effetto SNOWFALL
struct Snowflake {
    float x;              // Posizione X (con decimali per movimento fluido)
    float y;              // Posizione Y
    float speed;          // Velocità di caduta
    uint8_t brightness;   // Luminosità del fiocco
    bool active;          // Se il fiocco è attivo
};
Snowflake snowflakes[SNOWFALL_NUM_FLAKES];
uint32_t lastSnowfallUpdate = 0;
uint8_t snowAccumulation[MATRIX_WIDTH] = {0};  // Accumulo neve sul fondo (per ogni colonna)
uint8_t snowfallXmasLightPos[SNOWFALL_NUM_XMAS_LIGHTS];  // Posizioni lucine natalizie
uint8_t snowfallXmasLightHue[SNOWFALL_NUM_XMAS_LIGHTS];  // Hue lucine
bool snowfallXmasLightOn[SNOWFALL_NUM_XMAS_LIGHTS];      // Stato on/off lucine
uint32_t snowfallXmasLightTimer[SNOWFALL_NUM_XMAS_LIGHTS]; // Timer per intermittenza
uint8_t snowfallTextHue = 0;  // Hue per effetto cristallo ghiacciato sulle lettere
// Variabili per effetto LASER
struct LaserBeam {
    float x;              // Posizione X del raggio
    float y;              // Posizione Y corrente (parte dal basso, sale verso l'alto)
    float speed;          // Velocità di salita del raggio
    uint8_t hue;          // Colore del raggio
    bool active;          // Se il raggio è attivo
    bool hasTarget;       // Se il raggio ha un pixel target da rivelare
    uint8_t targetX;      // Coordinata X del target
    uint8_t targetY;      // Coordinata Y del target
};
LaserBeam laserBeams[LASER_NUM_BEAMS];
uint32_t lastLaserUpdate = 0;
uint32_t laserEffectStart = 0;
bool laserEffectActive = false;
uint32_t laserCompletionTime = 0;
bool laserTextPixels[NUM_LEDS] = {false};  // Pixel delle lettere rivelati dai laser
CRGB laserTextColor = CRGB::White;         // Colore delle lettere rivelate
uint8_t laserHue = 0;                      // Hue base per i raggi laser
// Variabili per effetto FUOCO
uint8_t fuocoHeat[NUM_LEDS];               // Array per simulazione calore fuoco
uint32_t lastFuocoUpdate = 0;
uint32_t fuocoStartTime = 0;               // Tempo inizio effetto per timeout
bool fuocoEffectActive = false;            // Se l'effetto è attivo
bool fuocoTextPixels[NUM_LEDS] = {false};  // Pixel delle lettere dell'orario
CRGB fuocoTextColor = CRGB::Cyan;          // Colore delle lettere (ciano per contrasto con fuoco)
// Variabili per effetto BLACKHOLE
struct BlackHoleParticle {
    float x;              // Posizione X
    float y;              // Posizione Y
    float angle;          // Angolo nella spirale
    float distance;       // Distanza dal centro
    uint8_t hue;          // Colore della particella
    uint8_t brightness;   // Luminosità
    bool active;          // Se la particella è attiva
    bool hasTarget;       // Se ha un pixel target da rivelare
    uint8_t targetX;      // Coordinata X del target
    uint8_t targetY;      // Coordinata Y del target
};
BlackHoleParticle blackholeParticles[BLACKHOLE_NUM_PARTICLES];
uint32_t lastBlackholeUpdate = 0;
uint32_t blackholeStartTime = 0;
bool blackholeEffectActive = false;
uint32_t blackholeCompletionTime = 0;
bool blackholeTextPixels[NUM_LEDS] = {false};
CRGB blackholeTextColor = CRGB::White;
uint8_t blackholeHue = 0;
float blackholeCenterX = 7.5f;  // Centro del buco nero (centro della matrice 16x16)
float blackholeCenterY = 7.5f;
// Variabili per effetto FIRE1 (Fire2012 style)
uint8_t fire1Heat[NUM_LEDS];          // Array per simulazione calore (fire2012)
uint32_t lastFire1Update = 0;
uint8_t fire1TongueHeight[MATRIX_WIDTH]; // Altezza variabile per ogni colonna (lingue di fuoco)
// Variabili e strutture per effetto FIREWORK
struct FireworkParticle {
    float x, y;           // Posizione
    float vx, vy;         // Velocità
    uint8_t brightness;   // Luminosità
    CRGB color;          // Colore della particella
    bool active;         // Particella attiva
    uint8_t lifetime;    // Tempo di vita rimanente
};
struct Firework {
    float x, y;          // Posizione esplosione
    CHSV color;          // Colore del fuoco d'artificio
    uint8_t state;       // 0=inattivo, 1=esplosione in corso
    uint32_t startTime;  // Tempo di inizio esplosione
    uint8_t particleStart; // Indice prima particella
    uint8_t particleCount; // Numero di particelle
};
Firework fireworks[MAX_FIREWORKS];
FireworkParticle fireworkParticles[MAX_PARTICLES];
uint32_t lastFireworkUpdate = 0;
uint32_t nextFireworkTime = 0;
uint32_t fireworkStartTime = 0;
bool fireworkEffectActive = false;
uint32_t fireworkCompletionTime = 0;
// Variabili e strutture per effetto DRIFT
struct DriftBlob {
    float x, y;           // Posizione
    float vx, vy;         // Velocità
    CHSV color;           // Colore del blob
    float size;           // Dimensione del blob
    bool active;          // Blob attivo
};
DriftBlob driftBlobs[MAX_DRIFT_BLOBS];
uint32_t lastDriftUpdate = 0;
uint8_t driftHue = 0;
uint32_t driftStartTime = 0;
bool driftEffectActive = false;
uint32_t driftCompletionTime = 0;

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
       // La luminosità è gestita globalmente da FastLED.setBrightness():
       // scalare anche qui il colore dimezzava due volte la luminosità percepita
       return current;
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

// Indirizzo EEPROM delle impostazioni di un preset: i primi 20 nell'area storica
// 250-349 (compatibile con le versioni precedenti), i nuovi nell'area estesa 360+
uint16_t modeSettingsAddr(uint8_t mode) {
    if (mode < 20) return EEPROM_MODE_SETTINGS_START + (mode * EEPROM_MODE_SETTINGS_SIZE);
    return EEPROM_MODE_SETTINGS_EXT + ((mode - 20) * EEPROM_MODE_SETTINGS_SIZE);
}

// Salva le impostazioni correnti / Save current settings
void saveModeSettings(uint8_t mode) {
    if (mode >= NUM_MODES) return;
    uint16_t baseAddr = modeSettingsAddr(mode);
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
    uint16_t baseAddr = modeSettingsAddr(mode);

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
            if (millis() - scrollPauseStart < 2000) {
                FastLED.clear();
                FastLED.show();
                return;
            } else {
                pauseDelayActive = true;  // Dopo 2 sec, mostra preset
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
void handleResetWiFi();
void handleFactoryReset();
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
    server.on("/resetWiFi", HTTP_POST, handleResetWiFi);
    server.on("/factoryReset", HTTP_POST, handleFactoryReset);
    server.onNotFound(handleNotFound);
    
    server.begin();
    webInitialized = true;
    Serial.println("Server web avviato su http://" + WiFi.localIP().toString());
   // preset / preset
   lastEffectChangeTime = millis();
   digitalOverlayActive = false;
   scrollTextInitialized = false;
}


#include "webpage.h"




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
        case 19: modeName = "Effetto Arkanoid"; break;
        case 20: modeName = "Effetto Natale"; break;
        case 21: modeName = "Effetto Neve"; break;
        case 22: modeName = "Effetto Laser"; break;
        case 23: modeName = "Effetto Fuoco"; break;
        case 24: modeName = "Effetto Black Hole"; break;
        case 25: modeName = "Effetto Camino"; break;
        case 26: modeName = "Effetto Firework"; break;
        case 27: modeName = "Effetto Drift"; break;
        case 28: modeName = "Effetto Plasma"; break;
        case 29: modeName = "Aurora Boreale"; break;
        case 30: modeName = "Effetto Radar"; break;
        case 31: modeName = "Cielo Stellato"; break;
        case 32: modeName = "Effetto Battito"; break;
        case 33: modeName = "Effetto Tetris"; break;
        case 34: modeName = "Effetto Snake"; break;
        case 35: modeName = "Effetto Sabbia"; break;
        case 36: modeName = "Effetto Temporale"; break;
        case 37: modeName = "Effetto Bolle"; break;
        case 38: modeName = "Effetto Pong"; break;
        case 39: modeName = "Lava Lamp"; break;
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
    doc["scrollText"] = customScrollText;
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
        if (newMode < 40) {
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
        if (pause < 5 || pause > 60) pause = 5;

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

// Cancella solo le credenziali WiFi e riavvia / Clear WiFi credentials only and restart
void handleResetWiFi() {
    server.send(200, "text/plain", "OK");
    delay(1000);
    WiFiManager wm;
    wm.resetSettings();
    delay(500);
    ESP.restart();
}

// Reset di fabbrica: cancella tutta la EEPROM e le credenziali WiFi, poi riavvia
// Factory reset: wipe whole EEPROM and WiFi credentials, then restart
void handleFactoryReset() {
    server.send(200, "text/plain", "OK");
    Serial.println("RESET DI FABBRICA richiesto da web");
    // Invalida il marker: al prossimo avvio setup() reinizializza tutti i default
    for (int i = 0; i < EEPROM_SIZE; i++) {
        EEPROM.write(i, 0xFF);
    }
    EEPROM.commit();
    delay(500);
    WiFiManager wm;
    wm.resetSettings();
    delay(500);
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

    // Orari identici: nessun periodo notte (altrimenti risulterebbe sempre notte)
    if (dayStartMinutes == nightStartMinutes) return false;

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

    // Orari identici: nessun periodo di spegnimento (altrimenti risulterebbe sempre attivo)
    if (dayStartMinutes == sleepStartMinutes) return false;

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
            // targetPixels usa indici fisici serpentina: converti le coordinate
            uint16_t pos = xyToLED(x, y);
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

    if (currentMinute != galagaLastMinute) {
        galagaLastMinute = currentMinute;
        initGalagaEffect();
        return;
    }


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
                    // Conversione serpentina: senza, gli spari risultavano specchiati sulle righe pari
                    uint16_t pos = xyToLED(drawX, drawY);
                    
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
    if (currentMinute != pacmanLastMinute) {
        pacmanLastMinute = currentMinute;
        initPacmanEffect();
        return;
    }

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

        // secondi / seconds (solo se la E fa parte dell'orario, cioè minuti > 0)
        if (currentBlink == 1 && currentMinute > 0) {
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
// ============================================================================
// EFFETTI INTEGRATI DALLA PRUNCA EDITION V1.2.6 (by Marco Prunca)
// ============================================================================
// ===== EFFETTO ARKANOID =====
void initArkanoidEffect() {
    Serial.println("=== INIT ARKANOID CHIAMATO ===");

    // Reset stato effetto
    arkanoidEffectActive = true;
    arkanoidCompletionTime = 0;
    arkanoidStartTime = millis();            // Salva tempo inizio per timeout
    lastArkanoidUpdate = millis();

    // Reset pixel rivelati
    memset(arkanoidTextPixels, 0, sizeof(arkanoidTextPixels));

    // Popola targetPixels con le lettere dell'orario corrente
    memset(targetPixels, 0, sizeof(targetPixels));
    const uint8_t* hourWord = (const uint8_t*)pgm_read_ptr(&HOUR_WORDS[currentHour]);
    displayWordToTarget(WORD_SONO_LE);
    displayWordToTarget(hourWord);
    if (currentMinute > 0) {
        displayWordToTarget(WORD_E);
        displayMinutesToTarget(currentMinute);
        displayWordToTarget(WORD_MINUTI);
    }

    // Posiziona la pallina al centro
    arkanoidBallX = 8.0f;
    arkanoidBallY = 8.0f;

    // Velocità iniziale casuale usando random8 di FastLED
    arkanoidVelX = (random8(2) == 0 ? -1.0f : 1.0f) * (0.6f + random8(40) / 100.0f);
    arkanoidVelY = (random8(2) == 0 ? -1.0f : 1.0f) * (0.6f + random8(40) / 100.0f);

    // Colori - cicla attraverso lo spettro
    arkanoidTextHue = random8();
    arkanoidBallColor = CRGB::Yellow;
    arkanoidTextColor = CHSV(arkanoidTextHue, 255, 255);

    Serial.println("Effetto ARKANOID inizializzato OK");
    Serial.print("Ball pos: ");
    Serial.print(arkanoidBallX);
    Serial.print(", ");
    Serial.println(arkanoidBallY);
    Serial.print("Velocita: ");
    Serial.print(arkanoidVelX);
    Serial.print(", ");
    Serial.println(arkanoidVelY);
}

void updateArkanoidEffect() {
    static uint32_t lastDebug = 0;
    uint32_t currentMillis = millis();

    // Debug ogni secondo
    if (currentMillis - lastDebug > 1000) {
        Serial.println("*** ARKANOID UPDATE chiamato ***");
        lastDebug = currentMillis;
    }

    // Se l'effetto è completato, mostra l'orario statico
    if (arkanoidCompletionTime != 0) {
        FastLED.clear();
        for (uint16_t i = 0; i < NUM_LEDS; i++) {
            if (arkanoidTextPixels[i]) {
                leds[i] = arkanoidTextColor;
            }
        }

        // Mostra i secondi lampeggianti se abilitato
        if (currentBlink == 1) {
            showSeconds(currentSecond, arkanoidTextColor);
        }

        FastLED.show();
        return;
    }

    // Timeout: dopo 15 secondi completa automaticamente l'orario
    if (currentMillis - arkanoidStartTime > 15000) {
        // Rivela tutte le lettere rimanenti
        for (uint16_t i = 0; i < NUM_LEDS; i++) {
            if (targetPixels[i]) {
                arkanoidTextPixels[i] = true;
            }
        }
        arkanoidCompletionTime = currentMillis;
        Serial.println("Effetto ARKANOID: timeout 15s - completamento automatico");
        return;
    }

    // Aggiornamento periodico
    if (currentMillis - lastArkanoidUpdate < ARKANOID_SPEED) return;
    lastArkanoidUpdate = currentMillis;

    // Anima il colore del testo
    arkanoidTextHue += 2;
    arkanoidTextColor = CHSV(arkanoidTextHue, 255, 255);

    // Posizione corrente della pallina (arrotondata) PRIMA del movimento
    int16_t oldBallX = (int16_t)arkanoidBallX;
    int16_t oldBallY = (int16_t)arkanoidBallY;

    // Muovi la pallina
    arkanoidBallX += arkanoidVelX;
    arkanoidBallY += arkanoidVelY;

    // Rimbalzo sui bordi
    if (arkanoidBallX < 0) {
        arkanoidBallX = 0;
        arkanoidVelX = -arkanoidVelX;
    }
    if (arkanoidBallX >= MATRIX_WIDTH) {
        arkanoidBallX = MATRIX_WIDTH - 1;
        arkanoidVelX = -arkanoidVelX;
    }
    if (arkanoidBallY < 0) {
        arkanoidBallY = 0;
        arkanoidVelY = -arkanoidVelY;
    }
    if (arkanoidBallY >= MATRIX_HEIGHT) {
        arkanoidBallY = MATRIX_HEIGHT - 1;
        arkanoidVelY = -arkanoidVelY;
    }

    // Posizione corrente della pallina (arrotondata) DOPO il movimento
    int16_t ballPixelX = (int16_t)arkanoidBallX;
    int16_t ballPixelY = (int16_t)arkanoidBallY;
    uint16_t ballPos = xyToLED(ballPixelX, ballPixelY);

    // Controlla collisione con lettere target (non ancora rivelate)
    if (ballPos < NUM_LEDS && targetPixels[ballPos] && !arkanoidTextPixels[ballPos]) {
        // Rivela la lettera
        arkanoidTextPixels[ballPos] = true;

        // Rimbalzo intelligente - inverti solo la direzione appropriata
        // Calcola da quale direzione è arrivata la pallina
        int16_t deltaX = ballPixelX - oldBallX;
        int16_t deltaY = ballPixelY - oldBallY;

        // Inverti la direzione dominante o entrambe se diagonale perfetta
        if (abs(deltaX) > abs(deltaY)) {
            arkanoidVelX = -arkanoidVelX;
        } else if (abs(deltaY) > abs(deltaX)) {
            arkanoidVelY = -arkanoidVelY;
        } else {
            // Movimento diagonale - inverti entrambe
            arkanoidVelX = -arkanoidVelX;
            arkanoidVelY = -arkanoidVelY;
        }

        // Aggiungi piccola variazione casuale per evitare pattern ripetitivi
        arkanoidVelX += (random8(20) - 10) / 100.0f;
        arkanoidVelY += (random8(20) - 10) / 100.0f;

        // Normalizza velocità per mantenere costante
        float speed = sqrt(arkanoidVelX * arkanoidVelX + arkanoidVelY * arkanoidVelY);
        if (speed > 0.1f) {
            arkanoidVelX = (arkanoidVelX / speed) * ARKANOID_BALL_SPEED;
            arkanoidVelY = (arkanoidVelY / speed) * ARKANOID_BALL_SPEED;
        }
    }
    // Le lettere già rivelate NON bloccano la pallina - passa attraverso

    // Cancella tutto
    FastLED.clear();

    // Disegna le lettere rivelate
    for (uint16_t i = 0; i < NUM_LEDS; i++) {
        if (arkanoidTextPixels[i]) {
            leds[i] = arkanoidTextColor;
        }
    }

    // Disegna la pallina (sempre visibile, sopra le lettere)
    ballPixelX = (int16_t)arkanoidBallX;
    ballPixelY = (int16_t)arkanoidBallY;
    ballPos = xyToLED(ballPixelX, ballPixelY);
    if (ballPos < NUM_LEDS) {
        leds[ballPos] = arkanoidBallColor;
    }

    // Controlla se tutte le lettere sono state rivelate
    bool allRevealed = true;
    for (uint16_t i = 0; i < NUM_LEDS; i++) {
        if (targetPixels[i] && !arkanoidTextPixels[i]) {
            allRevealed = false;
            break;
        }
    }

    if (allRevealed && arkanoidCompletionTime == 0) {
        arkanoidCompletionTime = currentMillis;
        Serial.println("Effetto ARKANOID completato!");
    }

    FastLED.show();
}

// Funzioni per effetto CHRISTMAS
void initChristmasEffect() {
    FastLED.clear();
    lastChristmasUpdate = 0;
    christmasColorIndex = 0;

    // Inizializza posizioni casuali per le lucine natalizie (non sulle lettere dell'orario)
    for (uint8_t i = 0; i < CHRISTMAS_NUM_LIGHTS; i++) {
        bool validPosition = false;
        while (!validPosition) {
            christmasLightPositions[i] = random8(NUM_LEDS);
            // Verifica che la posizione non sia occupata dalle lettere dell'orario
            if (!targetPixels[christmasLightPositions[i]]) {
                validPosition = true;
            }
        }
        christmasLightHues[i] = random8();  // Hue casuale per ogni lucina
    }

    Serial.println("Effetto CHRISTMAS inizializzato");
}

void updateChristmasEffect() {
    uint32_t now = millis();

    // Aggiorna i colori ogni CHRISTMAS_SPEED ms
    if (now - lastChristmasUpdate >= CHRISTMAS_SPEED) {
        lastChristmasUpdate = now;
        christmasColorIndex = (christmasColorIndex + 1) % 4;

        // Aggiorna gli hue delle lucine
        for (uint8_t i = 0; i < CHRISTMAS_NUM_LIGHTS; i++) {
            christmasLightHues[i] += 3;
        }
    }

    FastLED.clear();

    // Colori natalizi per le diverse parti dell'orario
    uint8_t colorOffset = 0;

    // SONO LE - Primo colore
    CRGB color1 = christmasColors[christmasColorIndex];
    displayWord(WORD_SONO_LE, color1);

    // Aggiungi sparkle casuale
    for (uint8_t i = 0; i < 10; i++) {
        uint8_t ledIndex = pgm_read_byte(&WORD_SONO_LE[i]);
        if (ledIndex != 8 && random8() < CHRISTMAS_SPARKLE_CHANCE) {
            leds[ledIndex] = CRGB::White;
        }
    }

    // ORA - Secondo colore
    colorOffset = (christmasColorIndex + 1) % 4;
    CRGB color2 = christmasColors[colorOffset];
    const uint8_t* hourWord = (const uint8_t*)pgm_read_ptr(&HOUR_WORDS[currentHour]);
    displayWord(hourWord, color2);

    // Aggiungi sparkle alle ore
    for (uint8_t i = 0; i < 20; i++) {
        uint8_t ledIndex = pgm_read_byte(&hourWord[i]);
        if (ledIndex == 8) break;
        if (random8() < CHRISTMAS_SPARKLE_CHANCE) {
            leds[ledIndex] = CRGB::White;
        }
    }

    if (currentMinute > 0) {
        // E - Terzo colore
        colorOffset = (christmasColorIndex + 2) % 4;
        CRGB color3 = christmasColors[colorOffset];
        displayWord(WORD_E, color3);

        // MINUTI - Quarto colore
        colorOffset = (christmasColorIndex + 3) % 4;
        CRGB color4 = christmasColors[colorOffset];
        showMinutes(currentMinute, color4);

        // MINUTI (parola) - Torna al primo colore
        displayWord(WORD_MINUTI, color1);

        // Aggiungi sparkle ai minuti
        if (random8() < CHRISTMAS_SPARKLE_CHANCE) {
            leds[116] = CRGB::White;
        }

        // Secondi
        if (currentBlink == 1) {
            showSeconds(currentSecond, color2);
        }
    }

    // Aggiungi lucine natalizie sparse che pulsano
    for (uint8_t i = 0; i < CHRISTMAS_NUM_LIGHTS; i++) {
        uint8_t brightness = beatsin8(60 + i * 10, 50, 255);  // Pulsazione con fase diversa per ogni lucina
        leds[christmasLightPositions[i]] = CHSV(christmasLightHues[i], 200, brightness);
    }

    FastLED.show();
}

// ===== EFFETTO SNOWFALL (NEVE CHE CADE) =====
void initSnowfallEffect() {
    FastLED.clear();
    lastSnowfallUpdate = 0;
    snowfallTextHue = 0;

    // Popola targetPixels con le lettere dell'orario corrente
    memset(targetPixels, 0, sizeof(targetPixels));
    const uint8_t* hourWord = (const uint8_t*)pgm_read_ptr(&HOUR_WORDS[currentHour]);
    displayWordToTarget(WORD_SONO_LE);
    displayWordToTarget(hourWord);
    if (currentMinute > 0) {
        displayWordToTarget(WORD_E);
        displayMinutesToTarget(currentMinute);
        displayWordToTarget(WORD_MINUTI);
    }

    // Resetta accumulo neve
    for (uint8_t i = 0; i < MATRIX_WIDTH; i++) {
        snowAccumulation[i] = 0;
    }

    // Inizializza fiocchi di neve
    for (uint8_t i = 0; i < SNOWFALL_NUM_FLAKES; i++) {
        snowflakes[i].x = random(MATRIX_WIDTH);
        snowflakes[i].y = -random(0, 10);  // Partono da fuori schermo
        snowflakes[i].speed = 0.15f + (random(20) / 100.0f);  // Velocità variabile
        snowflakes[i].brightness = 150 + random(105);  // Luminosità variabile
        snowflakes[i].active = true;
    }

    // Inizializza lucine natalizie (non su lettere orario)
    for (uint8_t i = 0; i < SNOWFALL_NUM_XMAS_LIGHTS; i++) {
        bool validPosition = false;
        while (!validPosition) {
            snowfallXmasLightPos[i] = random(NUM_LEDS);
            // Verifica che non sia su una lettera dell'orario
            if (!targetPixels[snowfallXmasLightPos[i]]) {
                validPosition = true;
            }
        }
        snowfallXmasLightHue[i] = random8();  // Colore casuale
        snowfallXmasLightOn[i] = random(2);   // Stato iniziale casuale
        snowfallXmasLightTimer[i] = millis() + random(500, 2000);  // Timer casuale
    }

    Serial.println("Effetto SNOWFALL inizializzato");
}

void updateSnowfallEffect() {
    uint32_t now = millis();

    // Aggiorna effetto ogni SNOWFALL_SPEED ms
    if (now - lastSnowfallUpdate >= SNOWFALL_SPEED) {
        lastSnowfallUpdate = now;

        // Aggiorna hue per effetto cristallo ghiacciato
        snowfallTextHue += 2;

        // Aggiorna posizioni fiocchi di neve
        for (uint8_t i = 0; i < SNOWFALL_NUM_FLAKES; i++) {
            if (snowflakes[i].active) {
                snowflakes[i].y += snowflakes[i].speed;

                // Leggero movimento orizzontale casuale (effetto vento)
                if (random(10) < 3) {
                    snowflakes[i].x += (random(3) - 1) * 0.3f;
                    // Mantieni entro i bordi
                    if (snowflakes[i].x < 0) snowflakes[i].x = 0;
                    if (snowflakes[i].x >= MATRIX_WIDTH) snowflakes[i].x = MATRIX_WIDTH - 1;
                }

                // Se raggiunge il fondo o l'accumulo, ricomincia dall'alto
                uint8_t xPos = (uint8_t)snowflakes[i].x;
                if (snowflakes[i].y >= (MATRIX_HEIGHT - snowAccumulation[xPos])) {
                    // Incrementa accumulo (max 3 pixel)
                    if (snowAccumulation[xPos] < 3 && random(10) < 5) {
                        snowAccumulation[xPos]++;
                    }
                    // Riavvia fiocco dall'alto
                    snowflakes[i].x = random(MATRIX_WIDTH);
                    snowflakes[i].y = -random(0, 5);
                    snowflakes[i].speed = 0.15f + (random(20) / 100.0f);
                    snowflakes[i].brightness = 150 + random(105);
                }
            }
        }

        // Aggiorna lucine natalizie (intermittenza casuale)
        for (uint8_t i = 0; i < SNOWFALL_NUM_XMAS_LIGHTS; i++) {
            if (now >= snowfallXmasLightTimer[i]) {
                snowfallXmasLightOn[i] = !snowfallXmasLightOn[i];
                snowfallXmasLightTimer[i] = now + random(300, 1500);  // Nuovo timer casuale
                snowfallXmasLightHue[i] += 5;  // Cambia leggermente il colore
            }
        }
    }

    FastLED.clear();

    // Disegna accumulo di neve sul fondo (colore azzurro-bianco)
    for (uint8_t x = 0; x < MATRIX_WIDTH; x++) {
        for (uint8_t y = 0; y < snowAccumulation[x]; y++) {
            uint16_t led = xyToLED(x, MATRIX_HEIGHT - 1 - y);
            if (led < NUM_LEDS) {
                // Colore bianco-azzurrino con leggero scintillio
                uint8_t brightness = 200 + random(55);
                leds[led] = CRGB(brightness, brightness, 255);
            }
        }
    }

    // Disegna fiocchi di neve
    for (uint8_t i = 0; i < SNOWFALL_NUM_FLAKES; i++) {
        if (snowflakes[i].active && snowflakes[i].y >= 0 && snowflakes[i].y < MATRIX_HEIGHT) {
            uint8_t xPos = (uint8_t)snowflakes[i].x;
            uint8_t yPos = (uint8_t)snowflakes[i].y;
            uint16_t led = xyToLED(xPos, yPos);

            if (led < NUM_LEDS && !targetPixels[led]) {  // Non sovrapporre alle lettere
                // Fiocco bianco brillante
                leds[led] = CRGB(snowflakes[i].brightness, snowflakes[i].brightness, 255);
            }
        }
    }

    // Disegna lucine natalizie intermittenti
    for (uint8_t i = 0; i < SNOWFALL_NUM_XMAS_LIGHTS; i++) {
        if (snowfallXmasLightOn[i]) {
            uint8_t brightness = beatsin8(40 + i * 10, 100, 255);  // Pulsazione
            leds[snowfallXmasLightPos[i]] = CHSV(snowfallXmasLightHue[i], 255, brightness);
        }
    }

    // Disegna orario con effetto cristallo ghiacciato (blu-bianco shimmer)
    CRGB iceColor = CHSV(snowfallTextHue + 140, 180, 255);  // Colore ghiaccio ciclico

    // SONO LE
    displayWord(WORD_SONO_LE, iceColor);
    // Aggiungi sparkle random alle lettere (effetto cristallo)
    for (uint8_t i = 0; i < 10; i++) {
        uint8_t ledIndex = pgm_read_byte(&WORD_SONO_LE[i]);
        if (ledIndex != 8 && random8() < 30) {
            leds[ledIndex] = CRGB::White;  // Scintillio bianco
        }
    }

    // ORA
    CRGB hourColor = CHSV(snowfallTextHue + 150, 200, 255);
    const uint8_t* hourWord = (const uint8_t*)pgm_read_ptr(&HOUR_WORDS[currentHour]);
    displayWord(hourWord, hourColor);
    // Sparkle sulle ore
    for (uint8_t i = 0; i < 20; i++) {
        uint8_t ledIndex = pgm_read_byte(&hourWord[i]);
        if (ledIndex == 8) break;
        if (random8() < 25) {
            leds[ledIndex] = CRGB::White;
        }
    }

    // MINUTI
    if (currentMinute > 0) {
        // E
        CRGB eColor = CHSV(snowfallTextHue + 160, 220, 255);
        displayWord(WORD_E, eColor);

        // Numero minuti
        CRGB minColor = CHSV(snowfallTextHue + 170, 240, 255);
        showMinutes(currentMinute, minColor);

        // MINUTI (parola)
        displayWord(WORD_MINUTI, iceColor);

        // Secondi
        if (currentBlink == 1) {
            showSeconds(currentSecond, hourColor);
        }
    }

    FastLED.show();
}

// Funzioni per effetto LASER
void initLaserEffect() {
    FastLED.clear();
    laserEffectActive = true;
    laserCompletionTime = 0;
    laserEffectStart = millis();
    lastLaserUpdate = 0;
    laserHue = random8();  // Colore base casuale per i laser
    laserTextColor = CRGB::White;  // Colore testo rivelato: bianco

    // Reset array pixel della scritta
    memset(laserTextPixels, 0, sizeof(laserTextPixels));

    // Popola targetPixels con le lettere dell'orario corrente
    memset(targetPixels, 0, sizeof(targetPixels));
    const uint8_t* hourWord = (const uint8_t*)pgm_read_ptr(&HOUR_WORDS[currentHour]);
    displayWordToTarget(WORD_SONO_LE);
    displayWordToTarget(hourWord);
    if (currentMinute > 0) {
        displayWordToTarget(WORD_E);
        displayMinutesToTarget(currentMinute);
        displayWordToTarget(WORD_MINUTI);
    }

    // Crea lista di tutti i pixel target (lettere dell'orario)
    uint16_t targetList[NUM_LEDS];
    uint16_t targetCount = 0;
    for (uint16_t i = 0; i < NUM_LEDS; i++) {
        if (targetPixels[i]) {
            targetList[targetCount++] = i;
        }
    }

    // Inizializza i raggi laser
    for (uint8_t i = 0; i < LASER_NUM_BEAMS; i++) {
        laserBeams[i].active = true;
        laserBeams[i].x = random(MATRIX_WIDTH);  // Posizione X casuale
        laserBeams[i].y = MATRIX_HEIGHT - 1;     // Parte dal basso
        laserBeams[i].speed = LASER_RISE_SPEED + (random(20) / 100.0f);  // Velocità variabile
        laserBeams[i].hue = laserHue + (i * 20);  // Colore con variazione

        // Assegna un target casuale dalle lettere disponibili
        if (targetCount > 0) {
            uint16_t targetIndex = targetList[random(targetCount)];
            // Converti indice LED in coordinate X,Y
            uint8_t targetY = targetIndex / MATRIX_WIDTH;
            uint8_t targetX;
            if (targetY % 2 == 0) {
                // Riga pari: da destra a sinistra
                targetX = MATRIX_WIDTH - 1 - (targetIndex % MATRIX_WIDTH);
            } else {
                // Riga dispari: da sinistra a destra
                targetX = targetIndex % MATRIX_WIDTH;
            }
            laserBeams[i].hasTarget = true;
            laserBeams[i].targetX = targetX;
            laserBeams[i].targetY = targetY;
        } else {
            laserBeams[i].hasTarget = false;
        }
    }

    Serial.println("Effetto LASER inizializzato");
}

void updateLaserEffect() {
    uint32_t now = millis();

    // Timeout di sicurezza: se l'effetto dura più di 15 secondi, termina
    if (now - laserEffectStart >= LASER_MAX_DURATION && laserCompletionTime == 0) {
        laserCompletionTime = now;
        laserEffectActive = false;
        Serial.println("LASER completato per timeout");
    }

    // Se l'effetto è completato, mostra solo l'orario rivelato
    if (laserCompletionTime != 0) {
        FastLED.clear();
        for (uint16_t i = 0; i < NUM_LEDS; i++) {
            if (laserTextPixels[i]) {
                leds[i] = laserTextColor;
            }
        }
        // Secondi lampeggianti se abilitato
        if (currentBlink == 1) {
            showSeconds(currentSecond, laserTextColor);
        }
        FastLED.show();
        return;
    }

    // Aggiorna effetto ogni LASER_SPEED ms
    if (now - lastLaserUpdate >= LASER_SPEED) {
        lastLaserUpdate = now;

        // Aggiorna hue per effetto arcobaleno sui laser
        laserHue += 1;

        // Aggiorna posizioni raggi laser
        bool allBeamsInactive = true;
        for (uint8_t i = 0; i < LASER_NUM_BEAMS; i++) {
            if (laserBeams[i].active) {
                allBeamsInactive = false;

                // Muovi il raggio verso l'alto
                laserBeams[i].y -= laserBeams[i].speed;

                // Se il raggio ha un target, dirigilo verso il target
                if (laserBeams[i].hasTarget) {
                    // Calcola distanza dal target
                    float distX = (float)laserBeams[i].targetX - laserBeams[i].x;

                    // Muovi il raggio verso il target X (lentamente)
                    if (abs(distX) > 0.5f) {
                        laserBeams[i].x += (distX > 0) ? 0.3f : -0.3f;
                    }

                    // Controlla se il raggio ha raggiunto il target
                    if (abs(laserBeams[i].y - laserBeams[i].targetY) <= 1.0f &&
                        abs(laserBeams[i].x - laserBeams[i].targetX) <= 1.0f) {
                        // Rivela il pixel target
                        uint16_t ledIndex = xyToLED(laserBeams[i].targetX, laserBeams[i].targetY);
                        if (ledIndex < NUM_LEDS && targetPixels[ledIndex]) {
                            laserTextPixels[ledIndex] = true;
                        }

                        // Trova nuovo target non ancora rivelato
                        uint16_t targetList[NUM_LEDS];
                        uint16_t targetCount = 0;
                        for (uint16_t j = 0; j < NUM_LEDS; j++) {
                            if (targetPixels[j] && !laserTextPixels[j]) {
                                targetList[targetCount++] = j;
                            }
                        }

                        if (targetCount > 0) {
                            // Assegna nuovo target
                            uint16_t targetIndex = targetList[random(targetCount)];
                            uint8_t targetY = targetIndex / MATRIX_WIDTH;
                            uint8_t targetX;
                            if (targetY % 2 == 0) {
                                targetX = MATRIX_WIDTH - 1 - (targetIndex % MATRIX_WIDTH);
                            } else {
                                targetX = targetIndex % MATRIX_WIDTH;
                            }
                            laserBeams[i].targetX = targetX;
                            laserBeams[i].targetY = targetY;
                            // Resetta posizione al basso
                            laserBeams[i].y = MATRIX_HEIGHT - 1;
                        }
                    }
                }

                // Disattiva il raggio se esce dallo schermo in alto
                if (laserBeams[i].y < -LASER_BEAM_LENGTH) {
                    // Riavvia dal basso con nuova posizione X casuale
                    laserBeams[i].y = MATRIX_HEIGHT - 1;
                    laserBeams[i].x = random(MATRIX_WIDTH);
                }
            }
        }

        // Verifica se tutte le lettere sono state rivelate
        bool allLettersRevealed = true;
        for (uint16_t i = 0; i < NUM_LEDS; i++) {
            if (targetPixels[i] && !laserTextPixels[i]) {
                allLettersRevealed = false;
                break;
            }
        }

        // Se tutte le lettere sono rivelate, completa l'effetto
        if (allLettersRevealed && laserCompletionTime == 0) {
            laserCompletionTime = now;
            laserEffectActive = false;
            Serial.println("LASER completato - tutte le lettere rivelate");
        }
    }

    // Rendering
    FastLED.clear();

    // Disegna i raggi laser
    for (uint8_t i = 0; i < LASER_NUM_BEAMS; i++) {
        if (laserBeams[i].active) {
            // Disegna il raggio come una linea verticale di LASER_BEAM_LENGTH pixel
            for (uint8_t j = 0; j < LASER_BEAM_LENGTH; j++) {
                int16_t drawY = (int16_t)laserBeams[i].y + j;

                if (drawY >= 0 && drawY < MATRIX_HEIGHT) {
                    uint8_t drawX = (uint8_t)laserBeams[i].x;
                    if (drawX < MATRIX_WIDTH) {
                        uint16_t ledIndex = xyToLED(drawX, drawY);

                        if (ledIndex < NUM_LEDS) {
                            // Colore laser con gradiente di luminosità (più luminoso in testa)
                            uint8_t brightness = 255 - (j * 40);
                            leds[ledIndex] = CHSV(laserBeams[i].hue, 255, brightness);
                        }
                    }
                }
            }
        }
    }

    // Disegna le lettere già rivelate
    for (uint16_t i = 0; i < NUM_LEDS; i++) {
        if (laserTextPixels[i]) {
            leds[i] = laserTextColor;
        }
    }

    FastLED.show();
}

// ===== EFFETTO FUOCO =====
void initFuocoEffect() {
    FastLED.clear();
    lastFuocoUpdate = 0;
    fuocoStartTime = millis();        // Inizializza timer di 15 secondi
    fuocoEffectActive = true;         // Attiva l'effetto
    fuocoTextColor = CRGB::Cyan;      // Colore testo: ciano per contrasto con fuoco

    // Reset array calore e pixel della scritta
    memset(fuocoHeat, 0, sizeof(fuocoHeat));
    memset(fuocoTextPixels, 0, sizeof(fuocoTextPixels));

    // Popola fuocoTextPixels con le lettere dell'orario corrente
    memset(targetPixels, 0, sizeof(targetPixels));
    const uint8_t* hourWord = (const uint8_t*)pgm_read_ptr(&HOUR_WORDS[currentHour]);
    displayWordToTarget(WORD_SONO_LE);
    displayWordToTarget(hourWord);
    if (currentMinute > 0) {
        displayWordToTarget(WORD_E);
        displayMinutesToTarget(currentMinute);
        displayWordToTarget(WORD_MINUTI);
    }

    // Copia i target pixels nelle lettere del fuoco
    for (uint16_t i = 0; i < NUM_LEDS; i++) {
        if (targetPixels[i]) {
            fuocoTextPixels[i] = true;
        }
    }

    Serial.println("Effetto FUOCO inizializzato - durata 15 secondi");
}

void updateFuocoEffect() {
    uint32_t now = millis();

    // Controlla se sono passati 15 secondi - se sì, ferma l'effetto
    if (fuocoEffectActive && (now - fuocoStartTime >= FUOCO_MAX_DURATION)) {
        fuocoEffectActive = false;
        Serial.println("Effetto FUOCO completato dopo 15 secondi");
    }

    // Se l'effetto non è più attivo, mostra solo il testo statico
    if (!fuocoEffectActive) {
        FastLED.clear();
        CRGB textColor = clockColors.getColor();
        for (uint16_t i = 0; i < NUM_LEDS; i++) {
            if (fuocoTextPixels[i]) {
                leds[i] = textColor;
            }
        }
        if (currentBlink == 1) {
            showSeconds(currentSecond, textColor);
        }
        FastLED.show();
        return;
    }

    // Aggiorna effetto ogni FUOCO_SPEED ms
    if (now - lastFuocoUpdate >= FUOCO_SPEED) {
        lastFuocoUpdate = now;

        // Step 1: Raffredda ogni cella un po'
        for (uint16_t i = 0; i < NUM_LEDS; i++) {
            uint8_t cooling = random(0, ((FUOCO_COOLING * 10) / MATRIX_HEIGHT) + 2);
            if (cooling >= fuocoHeat[i]) {
                fuocoHeat[i] = 0;
            } else {
                fuocoHeat[i] = fuocoHeat[i] - cooling;
            }
        }

        // Step 2: Il calore scende (propagazione verso il basso) - FORMA TRIANGOLARE INVERTITA
        for (int8_t y = 0; y < MATRIX_HEIGHT - 2; y++) {
            // Calcola i limiti X per forma triangolare invertita
            // La punta (y=0) è stretta, la base (y=15) è larga 16 pixel
            uint8_t triangleY = MATRIX_HEIGHT - 1 - y;  // Inverti y per forma triangolare
            uint8_t x_min = triangleY / 2;
            uint8_t x_max = MATRIX_WIDTH - 1 - (triangleY / 2);

            for (uint8_t x = x_min; x <= x_max; x++) {
                uint16_t currentIdx = xyToLED(x, y);
                uint16_t aboveIdx = xyToLED(x, y + 1);
                uint16_t above2Idx = xyToLED(x, y + 2);

                if (currentIdx < NUM_LEDS && aboveIdx < NUM_LEDS && above2Idx < NUM_LEDS) {
                    fuocoHeat[currentIdx] = (fuocoHeat[aboveIdx] + fuocoHeat[above2Idx] + fuocoHeat[above2Idx]) / 3;
                }
            }
        }

        // Step 3: Genera nuove scintille casuali in alto - FORMA TRIANGOLARE INVERTITA
        for (uint8_t x = 0; x < MATRIX_WIDTH; x++) {
            if (random8() < FUOCO_SPARKING) {
                // Crea scintille solo nelle prime FUOCO_HEIGHT righe dall'alto
                uint8_t y = MATRIX_HEIGHT - 1 - random8(min(FUOCO_HEIGHT, MATRIX_HEIGHT));

                // Verifica che la posizione sia dentro la forma triangolare invertita
                uint8_t triangleY = MATRIX_HEIGHT - 1 - y;
                uint8_t x_min = triangleY / 2;
                uint8_t x_max = MATRIX_WIDTH - 1 - (triangleY / 2);

                if (x >= x_min && x <= x_max) {
                    uint16_t idx = xyToLED(x, y);
                    if (idx < NUM_LEDS) {
                        fuocoHeat[idx] = qadd8(fuocoHeat[idx], random8(160, 255));
                    }
                }
            }
        }
    }

    // Rendering
    FastLED.clear();

    // Disegna il fuoco con palette rosso-arancione-giallo - FORMA TRIANGOLARE INVERTITA
    for (uint16_t i = 0; i < NUM_LEDS; i++) {
        // Ottieni coordinate del LED
        uint8_t y = i / MATRIX_WIDTH;
        uint8_t x = i % MATRIX_WIDTH;

        // Verifica che sia dentro la forma triangolare invertita
        uint8_t triangleY = MATRIX_HEIGHT - 1 - y;
        uint8_t x_min = triangleY / 2;
        uint8_t x_max = MATRIX_WIDTH - 1 - (triangleY / 2);

        // Disegna solo se dentro la forma triangolare e nelle FUOCO_HEIGHT righe dall'alto
        if (y >= (MATRIX_HEIGHT - FUOCO_HEIGHT) && x >= x_min && x <= x_max) {
            // Converti il calore in colore fuoco
            uint8_t temperature = fuocoHeat[i];
            CRGB fireColor;

            if (temperature < 85) {
                // Rosso scuro -> rosso
                fireColor = CRGB(temperature * 3, 0, 0);
            } else if (temperature < 170) {
                // Rosso -> arancione
                uint8_t t = temperature - 85;
                fireColor = CRGB(255, t * 3, 0);
            } else {
                // Arancione -> giallo
                uint8_t t = temperature - 170;
                fireColor = CRGB(255, 255, t * 3);
            }

            leds[i] = fireColor;
        }
    }

    // Disegna le lettere dell'orario sotto il fuoco
    for (uint16_t i = 0; i < NUM_LEDS; i++) {
        if (fuocoTextPixels[i]) {
            // Ottieni coordinate del LED
            uint8_t y = i / MATRIX_WIDTH;

            // Se il LED è nella zona delle fiamme (alto), usa colore contrastante
            // Altrimenti usa il colore normale delle lettere
            if (y >= (MATRIX_HEIGHT - FUOCO_HEIGHT) && fuocoHeat[i] > 80) {
                // Zona fiamme: usa ciano brillante per massimo contrasto
                leds[i] = CRGB::Cyan;
            } else {
                // Zona sotto fiamme o fiamme deboli: usa il colore testo normale
                leds[i] = fuocoTextColor;
            }
        }
    }

    // Secondi lampeggianti se abilitato
    if (currentBlink == 1) {
        showSeconds(currentSecond, fuocoTextColor);
    }

    FastLED.show();
}

// ===== EFFETTO BLACK HOLE =====
void initBlackHoleEffect() {
    FastLED.clear();
    blackholeEffectActive = true;
    blackholeCompletionTime = 0;
    blackholeStartTime = millis();
    lastBlackholeUpdate = 0;
    blackholeHue = random8();
    blackholeTextColor = CRGB::White;

    // Reset array pixel della scritta
    memset(blackholeTextPixels, 0, sizeof(blackholeTextPixels));

    // Popola targetPixels con le lettere dell'orario corrente
    memset(targetPixels, 0, sizeof(targetPixels));
    const uint8_t* hourWord = (const uint8_t*)pgm_read_ptr(&HOUR_WORDS[currentHour]);
    displayWordToTarget(WORD_SONO_LE);
    displayWordToTarget(hourWord);
    if (currentMinute > 0) {
        displayWordToTarget(WORD_E);
        displayMinutesToTarget(currentMinute);
        displayWordToTarget(WORD_MINUTI);
    }

    // Crea lista di tutti i pixel target (lettere dell'orario)
    uint16_t targetList[NUM_LEDS];
    uint16_t targetCount = 0;
    for (uint16_t i = 0; i < NUM_LEDS; i++) {
        if (targetPixels[i]) {
            targetList[targetCount++] = i;
        }
    }

    // Inizializza particelle dal bordo esterno verso il centro
    for (uint8_t i = 0; i < BLACKHOLE_NUM_PARTICLES; i++) {
        blackholeParticles[i].active = true;

        // Posizione iniziale casuale sul bordo esterno della matrice
        uint8_t edge = random(4); // 0=top, 1=right, 2=bottom, 3=left
        switch (edge) {
            case 0: // Top
                blackholeParticles[i].x = random(MATRIX_WIDTH);
                blackholeParticles[i].y = 0;
                break;
            case 1: // Right
                blackholeParticles[i].x = MATRIX_WIDTH - 1;
                blackholeParticles[i].y = random(MATRIX_HEIGHT);
                break;
            case 2: // Bottom
                blackholeParticles[i].x = random(MATRIX_WIDTH);
                blackholeParticles[i].y = MATRIX_HEIGHT - 1;
                break;
            case 3: // Left
                blackholeParticles[i].x = 0;
                blackholeParticles[i].y = random(MATRIX_HEIGHT);
                break;
        }

        // Calcola distanza e angolo dal centro
        float dx = blackholeParticles[i].x - blackholeCenterX;
        float dy = blackholeParticles[i].y - blackholeCenterY;
        blackholeParticles[i].distance = sqrt(dx * dx + dy * dy);
        blackholeParticles[i].angle = atan2(dy, dx);

        blackholeParticles[i].hue = blackholeHue + (i * 6);
        blackholeParticles[i].brightness = 255;

        // Assegna target casuale dalle lettere
        blackholeParticles[i].hasTarget = false;
        if (targetCount > 0 && i < targetCount) {
            uint16_t targetIndex = targetList[i % targetCount];
            uint8_t targetY = targetIndex / MATRIX_WIDTH;
            uint8_t targetX;
            if (targetY % 2 == 0) {
                targetX = MATRIX_WIDTH - 1 - (targetIndex % MATRIX_WIDTH);
            } else {
                targetX = targetIndex % MATRIX_WIDTH;
            }
            blackholeParticles[i].targetX = targetX;
            blackholeParticles[i].targetY = targetY;
            blackholeParticles[i].hasTarget = true;
        }
    }

    Serial.println("Effetto BLACK HOLE inizializzato");
}

void updateBlackHoleEffect() {
    uint32_t now = millis();

    // Timeout: se passati 15 secondi, completa l'effetto
    if (now - blackholeStartTime >= BLACKHOLE_MAX_DURATION && blackholeCompletionTime == 0) {
        blackholeCompletionTime = now;
        blackholeEffectActive = false;
        Serial.println("BLACK HOLE completato per timeout");
    }

    // Se completato, mostra solo l'orario rivelato
    if (blackholeCompletionTime != 0) {
        FastLED.clear();
        for (uint16_t i = 0; i < NUM_LEDS; i++) {
            if (blackholeTextPixels[i]) {
                leds[i] = blackholeTextColor;
            }
        }
        if (currentBlink == 1) {
            showSeconds(currentSecond, blackholeTextColor);
        }
        FastLED.show();
        return;
    }

    // Aggiorna effetto ogni BLACKHOLE_SPEED ms
    if (now - lastBlackholeUpdate >= BLACKHOLE_SPEED) {
        lastBlackholeUpdate = now;
        blackholeHue += 2; // Aggiorna hue per effetto arcobaleno

        // Aggiorna ogni particella
        for (uint8_t i = 0; i < BLACKHOLE_NUM_PARTICLES; i++) {
            if (blackholeParticles[i].active) {
                // Movimento spirale verso il centro
                blackholeParticles[i].angle += BLACKHOLE_SPIRAL_SPEED; // Rotazione
                blackholeParticles[i].distance -= BLACKHOLE_PULL_SPEED; // Avvicinamento al centro

                // Se ha raggiunto il centro
                if (blackholeParticles[i].distance <= 0.5f) {
                    // Se ha un target, rivelalo
                    if (blackholeParticles[i].hasTarget) {
                        uint16_t ledIndex = xyToLED(blackholeParticles[i].targetX, blackholeParticles[i].targetY);
                        if (ledIndex < NUM_LEDS && targetPixels[ledIndex]) {
                            blackholeTextPixels[ledIndex] = true;
                        }
                    }

                    // FAR RIPARTIRE LA PARTICELLA invece di disattivarla
                    // Posizione iniziale casuale sul bordo esterno
                    uint8_t edge = random(4); // 0=top, 1=right, 2=bottom, 3=left
                    switch (edge) {
                        case 0: // Top
                            blackholeParticles[i].x = random(MATRIX_WIDTH);
                            blackholeParticles[i].y = 0;
                            break;
                        case 1: // Right
                            blackholeParticles[i].x = MATRIX_WIDTH - 1;
                            blackholeParticles[i].y = random(MATRIX_HEIGHT);
                            break;
                        case 2: // Bottom
                            blackholeParticles[i].x = random(MATRIX_WIDTH);
                            blackholeParticles[i].y = MATRIX_HEIGHT - 1;
                            break;
                        case 3: // Left
                            blackholeParticles[i].x = 0;
                            blackholeParticles[i].y = random(MATRIX_HEIGHT);
                            break;
                    }

                    // Ricalcola distanza e angolo dal centro
                    float dx = blackholeParticles[i].x - blackholeCenterX;
                    float dy = blackholeParticles[i].y - blackholeCenterY;
                    blackholeParticles[i].distance = sqrt(dx * dx + dy * dy);
                    blackholeParticles[i].angle = atan2(dy, dx);
                    blackholeParticles[i].hue = blackholeHue + (i * 6);
                    blackholeParticles[i].brightness = 255;

                } else {
                    // Calcola nuova posizione dalla distanza e angolo
                    blackholeParticles[i].x = blackholeCenterX + cos(blackholeParticles[i].angle) * blackholeParticles[i].distance;
                    blackholeParticles[i].y = blackholeCenterY + sin(blackholeParticles[i].angle) * blackholeParticles[i].distance;

                    // Diminuisci luminosità man mano che si avvicina al centro
                    blackholeParticles[i].brightness = (uint8_t)(255 * (blackholeParticles[i].distance / 11.0f));
                }
            }
        }
    }

    // Rendering
    FastLED.clear();

    // Disegna il buco nero al centro (zona scura)
    for (int8_t dy = -1; dy <= 1; dy++) {
        for (int8_t dx = -1; dx <= 1; dx++) {
            int8_t x = (int8_t)blackholeCenterX + dx;
            int8_t y = (int8_t)blackholeCenterY + dy;
            if (x >= 0 && x < MATRIX_WIDTH && y >= 0 && y < MATRIX_HEIGHT) {
                uint16_t idx = xyToLED(x, y);
                if (idx < NUM_LEDS) {
                    leds[idx] = CRGB::Black; // Centro nero
                }
            }
        }
    }

    // Disegna le particelle in spirale
    for (uint8_t i = 0; i < BLACKHOLE_NUM_PARTICLES; i++) {
        if (blackholeParticles[i].active) {
            int8_t px = (int8_t)(blackholeParticles[i].x + 0.5f);
            int8_t py = (int8_t)(blackholeParticles[i].y + 0.5f);

            if (px >= 0 && px < MATRIX_WIDTH && py >= 0 && py < MATRIX_HEIGHT) {
                uint16_t idx = xyToLED(px, py);
                if (idx < NUM_LEDS) {
                    CRGB particleColor = CHSV(blackholeParticles[i].hue, 255, blackholeParticles[i].brightness);
                    leds[idx] = particleColor;
                }
            }
        }
    }

    // Disegna le lettere rivelate
    for (uint16_t i = 0; i < NUM_LEDS; i++) {
        if (blackholeTextPixels[i]) {
            leds[i] = blackholeTextColor;
        }
    }

    // Secondi lampeggianti se abilitato
    if (currentBlink == 1) {
        showSeconds(currentSecond, blackholeTextColor);
    }

    FastLED.show();
}

// ===== EFFETTO FIRE1 (Fire2012 style) =====
void initFire1Effect() {
    FastLED.clear();
    lastFire1Update = 0;

    // Reset array calore
    memset(fire1Heat, 0, sizeof(fire1Heat));

    // Inizializza altezze delle lingue di fuoco con valori casuali
    for (uint8_t x = 0; x < MATRIX_WIDTH; x++) {
        fire1TongueHeight[x] = FIRE1_HEIGHT - random8(3, 5); // Altezza variabile tra 5-7 righe
    }

    Serial.println("Effetto FIRE1 (Fire2012 style) inizializzato");
}

void updateFire1Effect() {
    uint32_t now = millis();

    // Aggiorna effetto ogni FIRE1_SPEED ms
    if (now - lastFire1Update >= FIRE1_SPEED) {
        lastFire1Update = now;

        // Step 1: Raffredda ogni cella nell'area dell'effetto (righe dall'alto)
        // y=15 (alto) fino a y=(MATRIX_HEIGHT - FIRE1_HEIGHT) = y=6 (basso dell'effetto)
        uint8_t minY = MATRIX_HEIGHT - FIRE1_HEIGHT;  // y=6
        for (int8_t y = MATRIX_HEIGHT - 1; y >= minY; y--) {
            for (uint8_t x = 0; x < MATRIX_WIDTH; x++) {
                uint16_t idx = xyToLED(x, y);
                if (idx < NUM_LEDS) {
                    uint8_t cooling = random(0, ((FIRE1_COOLING * 10) / FIRE1_HEIGHT) + 2);
                    if (cooling >= fire1Heat[idx]) {
                        fire1Heat[idx] = 0;
                    } else {
                        fire1Heat[idx] = fire1Heat[idx] - cooling;
                    }
                }
            }
        }

        // Step 2: Il calore scende (propagazione verso il basso - INVERTITO)
        // Partiamo dall'alto verso il basso
        for (int8_t y = minY; y <= MATRIX_HEIGHT - 3; y++) {
            for (uint8_t x = 0; x < MATRIX_WIDTH; x++) {
                uint16_t currentIdx = xyToLED(x, y);
                uint16_t aboveIdx = xyToLED(x, y + 1);
                uint16_t above2Idx = xyToLED(x, y + 2);

                if (currentIdx < NUM_LEDS && aboveIdx < NUM_LEDS && above2Idx < NUM_LEDS) {
                    fire1Heat[currentIdx] = (fire1Heat[aboveIdx] + fire1Heat[above2Idx] + fire1Heat[above2Idx]) / 3;
                }
            }
        }

        // Step 3: Genera nuove scintille casuali in ALTO (INVERTITO)
        for (uint8_t x = 0; x < MATRIX_WIDTH; x++) {
            if (random8() < FIRE1_SPARKING) {
                // Crea scintille nelle ultime 2-3 righe dall'alto (y=15, 14, 13)
                uint8_t yOffset = random8(3);
                uint8_t y = MATRIX_HEIGHT - 1 - yOffset;  // y=15, 14, o 13
                uint16_t idx = xyToLED(x, y);
                if (idx < NUM_LEDS) {
                    fire1Heat[idx] = qadd8(fire1Heat[idx], random8(160, 255));
                }
            }
        }

        // Step 4: Aggiorna altezze delle lingue di fuoco (bordo frastagliato)
        // Ogni colonna varia casualmente la sua altezza per simulare le lingue di fuoco
        for (uint8_t x = 0; x < MATRIX_WIDTH; x++) {
            // Variazione casuale lenta: aumenta, diminuisce o resta uguale
            if (random8() < 60) {  // 23% di probabilità di cambiare
                int8_t change = random8(3) - 1; // -1, 0, o +1
                int8_t newHeight = fire1TongueHeight[x] + change;
                // Limita tra 5 e 9 righe (FIRE1_HEIGHT = 10, quindi variazione di ±1-2 righe)
                if (newHeight >= 5 && newHeight <= 9) {
                    fire1TongueHeight[x] = newHeight;
                }
            }
        }
    }

    // Rendering - Disegna l'effetto fuoco con bordo frastagliato
    FastLED.clear();

    // Disegna fuoco con altezza variabile per ogni colonna (lingue di fuoco)
    for (uint8_t x = 0; x < MATRIX_WIDTH; x++) {
        // Calcola l'altezza minima per questa colonna (bordo frastagliato)
        uint8_t colMinY = MATRIX_HEIGHT - fire1TongueHeight[x];

        // Disegna il fuoco dall'alto fino al bordo variabile
        for (int8_t y = MATRIX_HEIGHT - 1; y >= colMinY; y--) {
            uint16_t idx = xyToLED(x, y);
            if (idx < NUM_LEDS) {
                // Converti il calore in colore usando palette fuoco (Fire2012 style)
                uint8_t temperature = fire1Heat[idx];
                CRGB fireColor;

                // Palette fuoco: Nero -> Rosso -> Arancione -> Giallo -> Bianco
                if (temperature < 85) {
                    // Nero -> Rosso scuro -> Rosso
                    fireColor = CRGB(temperature * 3, 0, 0);
                } else if (temperature < 170) {
                    // Rosso -> Arancione
                    uint8_t t = temperature - 85;
                    fireColor = CRGB(255, t * 3, 0);
                } else {
                    // Arancione -> Giallo -> Bianco
                    uint8_t t = temperature - 170;
                    fireColor = CRGB(255, 255, t * 3);
                }

                leds[idx] = fireColor;
            }
        }
    }

    // Sovrapponi l'orario con colore contrastante (bianco/ciano)
    CRGB textColor = clockColors.getColor();
    // Forza colore chiaro per contrasto con fuoco
    if (textColor.r < 128 && textColor.g < 128 && textColor.b < 128) {
        textColor = CRGB::Cyan;  // Se colore troppo scuro, usa ciano
    }

    // Popola targetPixels con le lettere dell'orario corrente
    memset(targetPixels, 0, sizeof(targetPixels));
    const uint8_t* hourWord = (const uint8_t*)pgm_read_ptr(&HOUR_WORDS[currentHour]);
    displayWordToTarget(WORD_SONO_LE);
    displayWordToTarget(hourWord);
    if (currentMinute > 0) {
        displayWordToTarget(WORD_E);
        displayMinutesToTarget(currentMinute);
        displayWordToTarget(WORD_MINUTI);
    }

    // Disegna le lettere dell'orario sopra il fuoco
    for (uint16_t i = 0; i < NUM_LEDS; i++) {
        if (targetPixels[i]) {
            leds[i] = textColor;
        }
    }

    // Secondi lampeggianti se abilitato
    if (currentBlink == 1) {
        showSeconds(currentSecond, textColor);
    }

    FastLED.show();
}

// ==================== EFFETTO FIREWORK ====================
// Effetto fuochi d'artificio con esplosioni e particelle
void initFireworkEffect() {
    FastLED.clear();
    fireworkEffectActive = true;
    fireworkCompletionTime = 0;
    fireworkStartTime = millis();
    lastFireworkUpdate = 0;
    nextFireworkTime = millis();

    // Reset tutti i fuochi d'artificio
    for (uint8_t i = 0; i < MAX_FIREWORKS; i++) {
        fireworks[i].state = 0;
    }

    // Reset tutte le particelle
    for (uint8_t i = 0; i < MAX_PARTICLES; i++) {
        fireworkParticles[i].active = false;
    }

    Serial.println("Effetto FIREWORK inizializzato");
}

void updateFireworkEffect() {
    uint32_t now = millis();

    // Timeout: se passati 15 secondi, completa l'effetto
    if (now - fireworkStartTime >= FIREWORK_DURATION_MS && fireworkCompletionTime == 0) {
        fireworkCompletionTime = now;
        fireworkEffectActive = false;
        Serial.println("FIREWORK completato per timeout");
    }

    // Se completato, mostra solo l'orario statico
    if (fireworkCompletionTime != 0) {
        FastLED.clear();

        // Sovrapponi l'orario
        CRGB textColor = clockColors.getColor();
        if (textColor.r < 150 && textColor.g < 150 && textColor.b < 150) {
            textColor = CRGB::White;
        }

        memset(targetPixels, 0, sizeof(targetPixels));
        const uint8_t* hourWord = (const uint8_t*)pgm_read_ptr(&HOUR_WORDS[currentHour]);
        displayWordToTarget(WORD_SONO_LE);
        displayWordToTarget(hourWord);
        if (currentMinute > 0) {
            displayWordToTarget(WORD_E);
            displayMinutesToTarget(currentMinute);
            displayWordToTarget(WORD_MINUTI);
        }

        for (uint16_t i = 0; i < NUM_LEDS; i++) {
            if (targetPixels[i]) {
                leds[i] = textColor;
            }
        }

        if (currentBlink == 1) {
            showSeconds(currentSecond, textColor);
        }

        FastLED.show();
        return;
    }

    // Aggiorna effetto ogni FIREWORK_SPEED ms
    if (now - lastFireworkUpdate >= FIREWORK_SPEED) {
        lastFireworkUpdate = now;

        // Crea nuovi fuochi d'artificio casuali
        if (now >= nextFireworkTime) {
            // Cerca uno slot libero per un nuovo firework
            for (uint8_t i = 0; i < MAX_FIREWORKS; i++) {
                if (fireworks[i].state == 0) {
                    // Crea nuovo firework in posizione casuale
                    fireworks[i].x = random(2, MATRIX_WIDTH - 2);
                    fireworks[i].y = random(4, MATRIX_HEIGHT - 4);
                    fireworks[i].color = CHSV(random8(), 255, 255);  // Colore casuale
                    fireworks[i].state = 1;
                    fireworks[i].startTime = now;

                    // Trova particelle libere per questo firework
                    uint8_t particlesCreated = 0;
                    uint8_t particlesPerFirework = random(12, 20);  // Numero variabile di particelle

                    for (uint8_t p = 0; p < MAX_PARTICLES && particlesCreated < particlesPerFirework; p++) {
                        if (!fireworkParticles[p].active) {
                            if (particlesCreated == 0) {
                                fireworks[i].particleStart = p;
                            }

                            // Crea particella con velocità radiale
                            float angle = (particlesCreated * TWO_PI) / particlesPerFirework;
                            float speed = random(50, 150) / 100.0f;  // Velocità variabile

                            fireworkParticles[p].x = fireworks[i].x;
                            fireworkParticles[p].y = fireworks[i].y;
                            fireworkParticles[p].vx = cos(angle) * speed;
                            fireworkParticles[p].vy = sin(angle) * speed;
                            fireworkParticles[p].brightness = 255;
                            fireworkParticles[p].color = fireworks[i].color;
                            fireworkParticles[p].active = true;
                            fireworkParticles[p].lifetime = random(80, 120);  // Tempo di vita variabile

                            particlesCreated++;
                        }
                    }

                    fireworks[i].particleCount = particlesCreated;

                    // Prossimo firework tra 300-800ms
                    nextFireworkTime = now + random(300, 800);
                    break;
                }
            }
        }

        // Aggiorna tutte le particelle attive
        for (uint8_t p = 0; p < MAX_PARTICLES; p++) {
            if (fireworkParticles[p].active) {
                // Applica gravità (verso il basso)
                fireworkParticles[p].vy += 0.08f;

                // Aggiorna posizione
                fireworkParticles[p].x += fireworkParticles[p].vx;
                fireworkParticles[p].y += fireworkParticles[p].vy;

                // Diminuisci luminosità e tempo di vita
                if (fireworkParticles[p].lifetime > 0) {
                    fireworkParticles[p].lifetime--;
                    fireworkParticles[p].brightness = map(fireworkParticles[p].lifetime, 0, 120, 0, 255);
                } else {
                    fireworkParticles[p].active = false;
                }

                // Disattiva se esce dalla matrice
                if (fireworkParticles[p].x < 0 || fireworkParticles[p].x >= MATRIX_WIDTH ||
                    fireworkParticles[p].y < 0 || fireworkParticles[p].y >= MATRIX_HEIGHT) {
                    fireworkParticles[p].active = false;
                }
            }
        }

        // Controlla se i firework hanno finito (tutte le particelle inattive)
        for (uint8_t i = 0; i < MAX_FIREWORKS; i++) {
            if (fireworks[i].state == 1) {
                bool anyActive = false;
                for (uint8_t p = fireworks[i].particleStart;
                     p < fireworks[i].particleStart + fireworks[i].particleCount && p < MAX_PARTICLES; p++) {
                    if (fireworkParticles[p].active) {
                        anyActive = true;
                        break;
                    }
                }
                if (!anyActive) {
                    fireworks[i].state = 0;  // Firework terminato
                }
            }
        }
    }

    // Rendering
    FastLED.clear();

    // Disegna tutte le particelle attive
    for (uint8_t p = 0; p < MAX_PARTICLES; p++) {
        if (fireworkParticles[p].active) {
            int16_t px = (int16_t)fireworkParticles[p].x;
            int16_t py = (int16_t)fireworkParticles[p].y;

            if (px >= 0 && px < MATRIX_WIDTH && py >= 0 && py < MATRIX_HEIGHT) {
                uint16_t idx = xyToLED(px, py);
                if (idx < NUM_LEDS) {
                    CRGB particleColor = fireworkParticles[p].color;
                    particleColor.nscale8(fireworkParticles[p].brightness);

                    // Blend con il colore esistente per permettere sovrapposizioni
                    leds[idx] += particleColor;
                }
            }
        }
    }

    // Sovrapponi l'orario con colore contrastante
    CRGB textColor = clockColors.getColor();
    // Forza colore brillante per visibilità
    if (textColor.r < 150 && textColor.g < 150 && textColor.b < 150) {
        textColor = CRGB::White;
    }

    // Popola targetPixels con le lettere dell'orario corrente
    memset(targetPixels, 0, sizeof(targetPixels));
    const uint8_t* hourWord = (const uint8_t*)pgm_read_ptr(&HOUR_WORDS[currentHour]);
    displayWordToTarget(WORD_SONO_LE);
    displayWordToTarget(hourWord);
    if (currentMinute > 0) {
        displayWordToTarget(WORD_E);
        displayMinutesToTarget(currentMinute);
        displayWordToTarget(WORD_MINUTI);
    }

    // Disegna le lettere dell'orario sopra i fireworks
    for (uint16_t i = 0; i < NUM_LEDS; i++) {
        if (targetPixels[i]) {
            leds[i] = textColor;
        }
    }

    // Secondi lampeggianti se abilitato
    if (currentBlink == 1) {
        showSeconds(currentSecond, textColor);
    }

    FastLED.show();
}

// ==================== EFFETTO DRIFT ====================
// Effetto con blob colorati che driftano lentamente sulla matrice
void initDriftEffect() {
    FastLED.clear();
    driftEffectActive = true;
    driftCompletionTime = 0;
    driftStartTime = millis();
    lastDriftUpdate = 0;
    driftHue = random8();

    // Inizializza i blob
    for (uint8_t i = 0; i < MAX_DRIFT_BLOBS; i++) {
        driftBlobs[i].x = random(0, MATRIX_WIDTH * 10) / 10.0f;
        driftBlobs[i].y = random(0, MATRIX_HEIGHT * 10) / 10.0f;
        driftBlobs[i].vx = (random(-20, 20) / 100.0f);  // Velocità molto bassa
        driftBlobs[i].vy = (random(-20, 20) / 100.0f);
        driftBlobs[i].color = CHSV(random8(), 255, 255);
        driftBlobs[i].size = random(15, 30) / 10.0f;  // Dimensione 1.5-3.0
        driftBlobs[i].active = true;
    }

    Serial.println("Effetto DRIFT inizializzato");
}

void updateDriftEffect() {
    uint32_t now = millis();

    // Timeout: se passati 15 secondi, completa l'effetto
    if (now - driftStartTime >= DRIFT_DURATION_MS && driftCompletionTime == 0) {
        driftCompletionTime = now;
        driftEffectActive = false;
        Serial.println("DRIFT completato per timeout");
    }

    // Se completato, mostra solo l'orario statico
    if (driftCompletionTime != 0) {
        FastLED.clear();

        // Sovrapponi l'orario
        CRGB textColor = clockColors.getColor();
        if (textColor.r < 150 && textColor.g < 150 && textColor.b < 150) {
            textColor = CRGB::White;
        }

        memset(targetPixels, 0, sizeof(targetPixels));
        const uint8_t* hourWord = (const uint8_t*)pgm_read_ptr(&HOUR_WORDS[currentHour]);
        displayWordToTarget(WORD_SONO_LE);
        displayWordToTarget(hourWord);
        if (currentMinute > 0) {
            displayWordToTarget(WORD_E);
            displayMinutesToTarget(currentMinute);
            displayWordToTarget(WORD_MINUTI);
        }

        for (uint16_t i = 0; i < NUM_LEDS; i++) {
            if (targetPixels[i]) {
                leds[i] = textColor;
            }
        }

        if (currentBlink == 1) {
            showSeconds(currentSecond, textColor);
        }

        FastLED.show();
        return;
    }

    // Aggiorna effetto ogni DRIFT_SPEED ms
    if (now - lastDriftUpdate >= DRIFT_SPEED) {
        lastDriftUpdate = now;

        // Fade/blur di tutto il display per creare l'effetto scia
        for (uint16_t i = 0; i < NUM_LEDS; i++) {
            leds[i].nscale8(DRIFT_BLUR_AMOUNT);
        }

        // Cambia gradualmente l'hue globale
        driftHue++;

        // Aggiorna e disegna ogni blob
        for (uint8_t b = 0; b < MAX_DRIFT_BLOBS; b++) {
            if (driftBlobs[b].active) {
                // Aggiorna posizione
                driftBlobs[b].x += driftBlobs[b].vx;
                driftBlobs[b].y += driftBlobs[b].vy;

                // Rimbalzo sui bordi con cambio di direzione fluido
                if (driftBlobs[b].x < 0) {
                    driftBlobs[b].x = 0;
                    driftBlobs[b].vx = -driftBlobs[b].vx;
                } else if (driftBlobs[b].x >= MATRIX_WIDTH) {
                    driftBlobs[b].x = MATRIX_WIDTH - 0.1f;
                    driftBlobs[b].vx = -driftBlobs[b].vx;
                }

                if (driftBlobs[b].y < 0) {
                    driftBlobs[b].y = 0;
                    driftBlobs[b].vy = -driftBlobs[b].vy;
                } else if (driftBlobs[b].y >= MATRIX_HEIGHT) {
                    driftBlobs[b].y = MATRIX_HEIGHT - 0.1f;
                    driftBlobs[b].vy = -driftBlobs[b].vy;
                }

                // Aggiorna colore gradualmente
                driftBlobs[b].color.hue += 1;

                // Disegna il blob con gradiente radiale
                int16_t centerX = (int16_t)driftBlobs[b].x;
                int16_t centerY = (int16_t)driftBlobs[b].y;

                // Disegna area intorno al centro del blob
                for (int16_t dy = -3; dy <= 3; dy++) {
                    for (int16_t dx = -3; dx <= 3; dx++) {
                        int16_t px = centerX + dx;
                        int16_t py = centerY + dy;

                        if (px >= 0 && px < MATRIX_WIDTH && py >= 0 && py < MATRIX_HEIGHT) {
                            // Calcola distanza dal centro
                            float distance = sqrt(dx * dx + dy * dy);

                            // Intensità basata sulla distanza (effetto gradiente)
                            if (distance < driftBlobs[b].size) {
                                float intensity = 255 * (1.0f - (distance / driftBlobs[b].size));

                                uint16_t idx = xyToLED(px, py);
                                if (idx < NUM_LEDS) {
                                    CRGB blobColor = driftBlobs[b].color;
                                    blobColor.nscale8((uint8_t)intensity);

                                    // Blend additivo per creare sovrapposizioni luminose
                                    leds[idx] += blobColor;
                                }
                            }
                        }
                    }
                }
            }
        }
    }

    // Sovrapponi l'orario con colore contrastante
    CRGB textColor = clockColors.getColor();
    // Forza colore brillante per visibilità
    if (textColor.r < 150 && textColor.g < 150 && textColor.b < 150) {
        textColor = CRGB::White;
    }

    // Popola targetPixels con le lettere dell'orario corrente
    memset(targetPixels, 0, sizeof(targetPixels));
    const uint8_t* hourWord = (const uint8_t*)pgm_read_ptr(&HOUR_WORDS[currentHour]);
    displayWordToTarget(WORD_SONO_LE);
    displayWordToTarget(hourWord);
    if (currentMinute > 0) {
        displayWordToTarget(WORD_E);
        displayMinutesToTarget(currentMinute);
        displayWordToTarget(WORD_MINUTI);
    }

    // Disegna le lettere dell'orario sopra l'effetto drift
    for (uint16_t i = 0; i < NUM_LEDS; i++) {
        if (targetPixels[i]) {
            leds[i] = textColor;
        }
    }

    // Secondi lampeggianti se abilitato
    if (currentBlink == 1) {
        showSeconds(currentSecond, textColor);
    }

    FastLED.show();
}


// ============================================================================
// NUOVI EFFETTI V1.3.0 - framework comune + 11 effetti
// Gli effetti non girano mai in contemporanea: lo stato fx* è condiviso
// ============================================================================
#define FX_MAX_DURATION 15000   // durata massima fase animata (poi rivela tutto)
#define FX_WAIT_AFTER 5000      // attesa con orario completo prima di tornare statico

bool fxRevealed[NUM_LEDS] = {false};   // pixel orario già rivelati
bool fxActive = false;
uint32_t fxStartTime = 0;
uint32_t fxCompletionTime = 0;
uint32_t lastFxUpdate = 0;

// Popola targetPixels con le lettere dell orario corrente
void fxLoadTargets() {
    memset(targetPixels, 0, sizeof(targetPixels));
    const uint8_t* hourWord = (const uint8_t*)pgm_read_ptr(&HOUR_WORDS[currentHour]);
    displayWordToTarget(WORD_SONO_LE);
    displayWordToTarget(hourWord);
    if (currentMinute > 0) {
        displayWordToTarget(WORD_E);
        displayMinutesToTarget(currentMinute);
        displayWordToTarget(WORD_MINUTI);
    }
}

void fxInitCommon() {
    fxActive = true;
    fxStartTime = millis();
    fxCompletionTime = 0;
    lastFxUpdate = 0;
    memset(fxRevealed, 0, sizeof(fxRevealed));
    fxLoadTargets();
}

bool fxAllRevealed() {
    for (uint16_t i = 0; i < NUM_LEDS; i++) {
        if (targetPixels[i] && !fxRevealed[i]) return false;
    }
    return true;
}

// Gestione comune: effetto non attivo / completato (attesa) / timeout.
// Ritorna true se la update non deve proseguire con la fase animata.
bool fxHandleCompletion(CRGB textColor) {
    uint32_t now = millis();
    if (!fxActive) {
        FastLED.clear();
        showCurrentTime();
        FastLED.show();
        return true;
    }
    if (fxCompletionTime != 0) {
        if (now - fxCompletionTime >= FX_WAIT_AFTER) {
            fxActive = false;
            return true;
        }
        FastLED.clear();
        for (uint16_t i = 0; i < NUM_LEDS; i++) {
            if (fxRevealed[i]) leds[i] = textColor;
        }
        if (currentBlink == 1) showSeconds(currentSecond, textColor);
        FastLED.show();
        return true;
    }
    if (now - fxStartTime >= FX_MAX_DURATION) {
        // Tempo scaduto: rivela tutto e passa alla fase di attesa
        for (uint16_t i = 0; i < NUM_LEDS; i++) {
            if (targetPixels[i]) fxRevealed[i] = true;
        }
        fxCompletionTime = now;
    }
    return false;
}

// ===== EFFETTO PLASMA (continuo) =====
uint8_t plasmaTime = 0;

void initPlasmaEffect() {
    plasmaTime = 0;
    lastFxUpdate = 0;
    fxLoadTargets();
}

void updatePlasmaEffect() {
    uint32_t now = millis();
    if (now - lastFxUpdate < 40) return;
    lastFxUpdate = now;
    plasmaTime += 2;
    for (uint8_t y = 0; y < MATRIX_HEIGHT; y++) {
        for (uint8_t x = 0; x < MATRIX_WIDTH; x++) {
            uint16_t pos = xyToLED(x, y);
            uint8_t v = sin8(x * 16 + plasmaTime) / 3
                      + sin8(y * 16 - plasmaTime) / 3
                      + sin8((x + y) * 8 + plasmaTime / 2) / 3;
            leds[pos] = CHSV(v + plasmaTime, 255, 140);
            if (targetPixels[pos]) leds[pos] = CRGB::White;
        }
    }
    if (currentBlink == 1 && currentMinute > 0) showSeconds(currentSecond, CRGB::White);
    FastLED.show();
}

// ===== EFFETTO AURORA BOREALE (continuo) =====
uint16_t auroraTime = 0;

void initAuroraEffect() {
    auroraTime = random16();
    lastFxUpdate = 0;
    fxLoadTargets();
}

void updateAuroraEffect() {
    uint32_t now = millis();
    if (now - lastFxUpdate < 50) return;
    lastFxUpdate = now;
    auroraTime += 3;
    for (uint8_t y = 0; y < MATRIX_HEIGHT; y++) {
        for (uint8_t x = 0; x < MATRIX_WIDTH; x++) {
            uint16_t pos = xyToLED(x, y);
            uint8_t n = inoise8(x * 25, y * 25 + auroraTime, auroraTime / 2);
            uint8_t hue = 96 + scale8(n, 100);          // verde -> ciano -> viola
            uint8_t bri = 40 + scale8(inoise8(x * 35 + 500, y * 35, auroraTime), 180);
            leds[pos] = CHSV(hue, 230, bri);
            if (targetPixels[pos]) leds[pos] = CRGB::White;
        }
    }
    if (currentBlink == 1 && currentMinute > 0) showSeconds(currentSecond, CRGB::White);
    FastLED.show();
}

// ===== EFFETTO RADAR =====
uint8_t radarPixelAngle[NUM_LEDS];   // angolo precalcolato di ogni pixel (0-255)
uint8_t radarAngle = 0;

void initRadarEffect() {
    fxInitCommon();
    radarAngle = 0;
    for (uint8_t y = 0; y < MATRIX_HEIGHT; y++) {
        for (uint8_t x = 0; x < MATRIX_WIDTH; x++) {
            float a = atan2f(y - 7.5f, x - 7.5f);            // -PI..PI
            radarPixelAngle[xyToLED(x, y)] = (uint8_t)((a + PI) * 256.0f / (2.0f * PI));
        }
    }
}

void updateRadarEffect() {
    if (fxHandleCompletion(CRGB(120, 255, 140))) return;
    uint32_t now = millis();
    if (now - lastFxUpdate < 25) return;
    lastFxUpdate = now;
    radarAngle += 2;
    FastLED.clear();
    for (uint16_t i = 0; i < NUM_LEDS; i++) {
        uint8_t diff = radarAngle - radarPixelAngle[i];      // wrap automatico uint8
        if (diff < 48) {
            leds[i] = CHSV(96, 255, 255 - diff * 5);          // fascio verde con scia
        }
        if (diff < 5 && targetPixels[i]) fxRevealed[i] = true;
        if (fxRevealed[i]) leds[i] = CRGB(120, 255, 140);
    }
    if (currentBlink == 1 && currentMinute > 0) showSeconds(currentSecond, CRGB(120, 255, 140));
    FastLED.show();
    if (fxAllRevealed()) fxCompletionTime = now;
}

// ===== EFFETTO CIELO STELLATO =====
uint32_t lastStarReveal = 0;

void initStarsEffect() {
    fxInitCommon();
    lastStarReveal = 0;
    FastLED.clear();
}

void updateStarsEffect() {
    if (fxHandleCompletion(CRGB(255, 220, 180))) return;
    uint32_t now = millis();
    if (now - lastFxUpdate < 50) return;
    lastFxUpdate = now;
    fadeToBlackBy(leds, NUM_LEDS, 30);
    // Stelle che twinklano sullo sfondo
    if (random8() < 90) {
        uint16_t pos = random16(NUM_LEDS);
        if (!targetPixels[pos]) leds[pos] = CHSV(160, random8(60), 120 + random8(135));
    }
    // La costellazione (orario) si compone una lettera alla volta
    if (now - lastStarReveal > 300) {
        lastStarReveal = now;
        uint16_t candidates[NUM_LEDS];
        uint16_t n = 0;
        for (uint16_t i = 0; i < NUM_LEDS; i++) {
            if (targetPixels[i] && !fxRevealed[i]) candidates[n++] = i;
        }
        if (n > 0) fxRevealed[candidates[random16(n)]] = true;
        else fxCompletionTime = now;
    }
    for (uint16_t i = 0; i < NUM_LEDS; i++) {
        if (fxRevealed[i]) leds[i] = CRGB(255, 220, 180);    // bianco caldo
    }
    if (currentBlink == 1 && currentMinute > 0) showSeconds(currentSecond, CRGB(255, 220, 180));
    FastLED.show();
}

// ===== EFFETTO BATTITO =====
const uint8_t PROGMEM heartShape[][2] = {
    {6,5},{7,5},{9,5},{10,5},
    {5,6},{8,6},{11,6},
    {5,7},{11,7},
    {6,8},{10,8},
    {7,9},{9,9},
    {8,10}
};
#define HEART_PIXELS 14
float beatRadius = 0;
bool beatWaveActive = false;
uint32_t lastBeat = 0;

void initHeartbeatEffect() {
    fxInitCommon();
    beatRadius = 0;
    beatWaveActive = false;
    lastBeat = 0;
}

void updateHeartbeatEffect() {
    if (fxHandleCompletion(CRGB(255, 120, 140))) return;
    uint32_t now = millis();
    if (now - lastFxUpdate < 30) return;
    lastFxUpdate = now;
    FastLED.clear();
    // Nuovo battito: parte una onda anulare
    if (now - lastBeat > 1300) {
        lastBeat = now;
        beatWaveActive = true;
        beatRadius = 0;
    }
    if (beatWaveActive) {
        beatRadius += 0.35f;
        if (beatRadius > 13.0f) beatWaveActive = false;
        for (uint8_t y = 0; y < MATRIX_HEIGHT; y++) {
            for (uint8_t x = 0; x < MATRIX_WIDTH; x++) {
                float dx = x - 7.5f, dy = y - 7.5f;
                float d = sqrtf(dx * dx + dy * dy);
                uint16_t pos = xyToLED(x, y);
                if (fabsf(d - beatRadius) < 1.2f) leds[pos] = CHSV(250, 255, 170);
                if (d <= beatRadius && targetPixels[pos]) fxRevealed[pos] = true;
            }
        }
    }
    // Cuore pulsante al centro
    uint8_t b = beatsin8(46, 90, 255);
    for (uint8_t i = 0; i < HEART_PIXELS; i++) {
        uint8_t hx = pgm_read_byte(&heartShape[i][0]);
        uint8_t hy = pgm_read_byte(&heartShape[i][1]);
        leds[xyToLED(hx, hy)] = CHSV(250, 255, b);
    }
    for (uint16_t i = 0; i < NUM_LEDS; i++) {
        if (fxRevealed[i]) leds[i] = CRGB(255, 120, 140);
    }
    if (currentBlink == 1 && currentMinute > 0) showSeconds(currentSecond, CRGB(255, 120, 140));
    FastLED.show();
    if (!beatWaveActive && fxAllRevealed()) fxCompletionTime = now;
}

// ===== EFFETTO TETRIS =====
struct TetrisBlock {
    int8_t x;
    float y;
    uint8_t targetY;
    CRGB color;
    bool active;
};
#define TETRIS_MAX_BLOCKS 4
TetrisBlock tetrisBlocks[TETRIS_MAX_BLOCKS];
CRGB tetrisPixelColor[NUM_LEDS];
const CRGB tetrisPalette[7] = {
    CRGB(0,240,240), CRGB(240,240,0), CRGB(160,0,240), CRGB(0,240,0),
    CRGB(240,0,0), CRGB(0,0,240), CRGB(240,160,0)
};

void initTetrisEffect() {
    fxInitCommon();
    for (uint8_t i = 0; i < TETRIS_MAX_BLOCKS; i++) tetrisBlocks[i].active = false;
}

void updateTetrisEffect() {
    if (fxHandleCompletion(CRGB::White)) return;
    uint32_t now = millis();
    if (now - lastFxUpdate < 30) return;
    lastFxUpdate = now;
    // Spawn: per ogni slot libero cerca, in una colonna casuale, il pixel target
    // non rivelato piu in basso (riempimento dal basso, gravita credibile)
    for (uint8_t i = 0; i < TETRIS_MAX_BLOCKS; i++) {
        if (tetrisBlocks[i].active || random8() > 70) continue;
        uint8_t col = random8(MATRIX_WIDTH);
        bool colBusy = false;
        for (uint8_t j = 0; j < TETRIS_MAX_BLOCKS; j++) {
            if (tetrisBlocks[j].active && tetrisBlocks[j].x == col) colBusy = true;
        }
        if (colBusy) continue;
        for (int8_t y = MATRIX_HEIGHT - 1; y >= 0; y--) {
            uint16_t pos = xyToLED(col, y);
            if (targetPixels[pos] && !fxRevealed[pos]) {
                tetrisBlocks[i].x = col;
                tetrisBlocks[i].y = -1.0f;
                tetrisBlocks[i].targetY = y;
                tetrisBlocks[i].color = tetrisPalette[random8(7)];
                tetrisBlocks[i].active = true;
                break;
            }
        }
    }
    FastLED.clear();
    for (uint8_t i = 0; i < TETRIS_MAX_BLOCKS; i++) {
        if (!tetrisBlocks[i].active) continue;
        tetrisBlocks[i].y += 0.45f;
        if (tetrisBlocks[i].y >= tetrisBlocks[i].targetY) {
            uint16_t pos = xyToLED(tetrisBlocks[i].x, tetrisBlocks[i].targetY);
            fxRevealed[pos] = true;
            tetrisPixelColor[pos] = tetrisBlocks[i].color;
            tetrisBlocks[i].active = false;
        } else {
            int8_t by = (int8_t)tetrisBlocks[i].y;
            if (by >= 0) leds[xyToLED(tetrisBlocks[i].x, by)] = tetrisBlocks[i].color;
        }
    }
    for (uint16_t i = 0; i < NUM_LEDS; i++) {
        if (fxRevealed[i]) leds[i] = tetrisPixelColor[i];
    }
    if (currentBlink == 1 && currentMinute > 0) showSeconds(currentSecond, CRGB::White);
    FastLED.show();
    bool falling = false;
    for (uint8_t i = 0; i < TETRIS_MAX_BLOCKS; i++) if (tetrisBlocks[i].active) falling = true;
    if (!falling && fxAllRevealed()) fxCompletionTime = now;
}

// ===== EFFETTO SNAKE =====
#define SNAKE_MAX_LEN 40
int8_t snakeX[SNAKE_MAX_LEN];
int8_t snakeY[SNAKE_MAX_LEN];
uint8_t snakeLen = 3;

void initSnakeEffect() {
    fxInitCommon();
    snakeLen = 3;
    for (uint8_t i = 0; i < snakeLen; i++) { snakeX[i] = 8 - i; snakeY[i] = 15; }
}

void updateSnakeEffect() {
    if (fxHandleCompletion(CRGB::White)) return;
    uint32_t now = millis();
    if (now - lastFxUpdate < 70) return;
    lastFxUpdate = now;
    // Cibo: pixel target non rivelato piu vicino alla testa (distanza manhattan)
    int16_t bestDist = 999;
    int8_t foodX = -1, foodY = -1;
    for (uint8_t y = 0; y < MATRIX_HEIGHT; y++) {
        for (uint8_t x = 0; x < MATRIX_WIDTH; x++) {
            uint16_t pos = xyToLED(x, y);
            if (targetPixels[pos] && !fxRevealed[pos]) {
                int16_t d = abs(x - snakeX[0]) + abs(y - snakeY[0]);
                if (d < bestDist) { bestDist = d; foodX = x; foodY = y; }
            }
        }
    }
    if (foodX < 0) { fxCompletionTime = now; return; }
    // Muovi la testa verso il cibo (prima orizzontale, poi verticale)
    int8_t nx = snakeX[0], ny = snakeY[0];
    if (nx != foodX) nx += (foodX > nx) ? 1 : -1;
    else if (ny != foodY) ny += (foodY > ny) ? 1 : -1;
    for (uint8_t i = snakeLen - 1; i > 0; i--) { snakeX[i] = snakeX[i-1]; snakeY[i] = snakeY[i-1]; }
    snakeX[0] = nx; snakeY[0] = ny;
    // Mangia
    if (nx == foodX && ny == foodY) {
        fxRevealed[xyToLED(foodX, foodY)] = true;
        if (snakeLen < SNAKE_MAX_LEN) {
            snakeX[snakeLen] = snakeX[snakeLen-1];
            snakeY[snakeLen] = snakeY[snakeLen-1];
            snakeLen++;
        }
    }
    FastLED.clear();
    for (uint16_t i = 0; i < NUM_LEDS; i++) {
        if (fxRevealed[i]) leds[i] = CRGB(180, 180, 180);
    }
    // Cibo lampeggiante
    if ((now / 250) % 2 == 0) leds[xyToLED(foodX, foodY)] = CRGB::Red;
    // Corpo verde con gradiente (testa piu chiara)
    for (uint8_t i = 0; i < snakeLen; i++) {
        leds[xyToLED(snakeX[i], snakeY[i])] = CHSV(96, 255, 255 - (i * 200 / snakeLen));
    }
    if (currentBlink == 1 && currentMinute > 0) showSeconds(currentSecond, CRGB(180, 180, 180));
    FastLED.show();
}

// ===== EFFETTO SABBIA =====
struct SandGrain {
    int8_t x;
    float y;
    uint8_t targetY;
    bool active;
};
#define SAND_MAX_GRAINS 6
SandGrain sandGrains[SAND_MAX_GRAINS];

void initSandEffect() {
    fxInitCommon();
    for (uint8_t i = 0; i < SAND_MAX_GRAINS; i++) sandGrains[i].active = false;
}

void updateSandEffect() {
    if (fxHandleCompletion(CRGB(230, 200, 120))) return;
    uint32_t now = millis();
    if (now - lastFxUpdate < 25) return;
    lastFxUpdate = now;
    // Spawn: riempi le lettere partendo dalla riga piu bassa rimasta
    for (uint8_t i = 0; i < SAND_MAX_GRAINS; i++) {
        if (sandGrains[i].active || random8() > 90) continue;
        int8_t lowestY = -1;
        for (int8_t y = MATRIX_HEIGHT - 1; y >= 0 && lowestY < 0; y--) {
            for (uint8_t x = 0; x < MATRIX_WIDTH; x++) {
                uint16_t pos = xyToLED(x, y);
                if (targetPixels[pos] && !fxRevealed[pos]) { lowestY = y; break; }
            }
        }
        if (lowestY < 0) break;
        // Colonna casuale tra quelle con un target su quella riga
        uint8_t cols[MATRIX_WIDTH];
        uint8_t n = 0;
        for (uint8_t x = 0; x < MATRIX_WIDTH; x++) {
            uint16_t pos = xyToLED(x, lowestY);
            if (targetPixels[pos] && !fxRevealed[pos]) cols[n++] = x;
        }
        if (n == 0) continue;
        uint8_t col = cols[random8(n)];
        bool busy = false;
        for (uint8_t j = 0; j < SAND_MAX_GRAINS; j++) {
            if (sandGrains[j].active && sandGrains[j].x == col && sandGrains[j].targetY == lowestY) busy = true;
        }
        if (busy) continue;
        sandGrains[i].x = col;
        sandGrains[i].y = -1.0f;
        sandGrains[i].targetY = lowestY;
        sandGrains[i].active = true;
    }
    FastLED.clear();
    for (uint8_t i = 0; i < SAND_MAX_GRAINS; i++) {
        if (!sandGrains[i].active) continue;
        sandGrains[i].y += 0.7f;
        if (sandGrains[i].y >= sandGrains[i].targetY) {
            fxRevealed[xyToLED(sandGrains[i].x, sandGrains[i].targetY)] = true;
            sandGrains[i].active = false;
        } else {
            int8_t gy = (int8_t)sandGrains[i].y;
            if (gy >= 0) leds[xyToLED(sandGrains[i].x, gy)] = CRGB(230, 200, 120);
        }
    }
    for (uint16_t i = 0; i < NUM_LEDS; i++) {
        if (fxRevealed[i]) leds[i] = CRGB(230, 200, 120);
    }
    if (currentBlink == 1 && currentMinute > 0) showSeconds(currentSecond, CRGB(230, 200, 120));
    FastLED.show();
    bool falling = false;
    for (uint8_t i = 0; i < SAND_MAX_GRAINS; i++) if (sandGrains[i].active) falling = true;
    if (!falling && fxAllRevealed()) fxCompletionTime = now;
}

// ===== EFFETTO TEMPORALE =====
#define STORM_DROPS 12
#define STORM_BOLT_MAX 14
float stormDropY[STORM_DROPS];
uint8_t stormDropX[STORM_DROPS];
float stormDropSpeed[STORM_DROPS];
uint32_t nextFlashTime = 0;
uint32_t flashStart = 0;
uint8_t stormBoltX[STORM_BOLT_MAX];   // percorso a zigzag del fulmine
uint8_t stormBoltLen = 0;

void initStormEffect() {
    fxInitCommon();
    for (uint8_t i = 0; i < STORM_DROPS; i++) {
        stormDropX[i] = random8(MATRIX_WIDTH);
        stormDropY[i] = -(float)random8(MATRIX_HEIGHT);
        stormDropSpeed[i] = 0.5f + random8(50) / 100.0f;
    }
    nextFlashTime = millis() + 800;
    flashStart = 0;
}

void updateStormEffect() {
    if (fxHandleCompletion(CRGB(200, 220, 255))) return;
    uint32_t now = millis();
    if (now - lastFxUpdate < 30) return;
    lastFxUpdate = now;
    FastLED.clear();
    bool flashing = (flashStart != 0 && now - flashStart < 110);
    // Lampo: fulmine a zigzag che rivela un pezzo di orario.
    // NON si accende tutta la matrice in bianco: a piena luminosita il picco
    // di corrente (256 LED quasi bianchi) manda in brownout l alimentazione
    if (!flashing && now >= nextFlashTime) {
        flashStart = now;
        flashing = true;
        nextFlashTime = now + 1200 + random16(2300);
        // Genera il percorso del fulmine: scende dal bordo alto con zigzag
        uint8_t bx = 2 + random8(MATRIX_WIDTH - 4);
        stormBoltLen = 8 + random8(STORM_BOLT_MAX - 8 + 1);
        for (uint8_t y = 0; y < stormBoltLen; y++) {
            stormBoltX[y] = bx;
            int8_t step = (int8_t)random8(3) - 1;        // -1, 0, +1
            bx = constrain(bx + step, 0, MATRIX_WIDTH - 1);
        }
        uint16_t candidates[NUM_LEDS];
        uint16_t n = 0;
        for (uint16_t i = 0; i < NUM_LEDS; i++) {
            if (targetPixels[i] && !fxRevealed[i]) candidates[n++] = i;
        }
        // Ogni lampo rivela circa un quinto delle lettere rimaste
        uint16_t toReveal = n / 5 + 1;
        for (uint16_t k = 0; k < toReveal && n > 0; k++) {
            uint16_t idx = random16(n);
            fxRevealed[candidates[idx]] = true;
            candidates[idx] = candidates[--n];
        }
    }
    if (flashing) {
        // Schiarita leggera del cielo + fulmine disegnato (pochi pixel bianchi)
        fill_solid(leds, NUM_LEDS, CRGB(22, 22, 34));
        for (uint8_t y = 0; y < stormBoltLen; y++) {
            leds[xyToLED(stormBoltX[y], y)] = CRGB(255, 255, 255);
            // Alone tenue ai lati del fulmine
            if (stormBoltX[y] > 0) leds[xyToLED(stormBoltX[y] - 1, y)] += CRGB(40, 40, 60);
            if (stormBoltX[y] < MATRIX_WIDTH - 1) leds[xyToLED(stormBoltX[y] + 1, y)] += CRGB(40, 40, 60);
        }
    }
    // Pioggia
    for (uint8_t i = 0; i < STORM_DROPS; i++) {
        stormDropY[i] += stormDropSpeed[i];
        if (stormDropY[i] >= MATRIX_HEIGHT) {
            stormDropY[i] = -1.0f;
            stormDropX[i] = random8(MATRIX_WIDTH);
        }
        int8_t dy = (int8_t)stormDropY[i];
        if (dy >= 0) leds[xyToLED(stormDropX[i], dy)] = CRGB(40, 80, 200);
    }
    for (uint16_t i = 0; i < NUM_LEDS; i++) {
        if (fxRevealed[i]) leds[i] = CRGB(200, 220, 255);
    }
    if (currentBlink == 1 && currentMinute > 0) showSeconds(currentSecond, CRGB(200, 220, 255));
    FastLED.show();
    if (fxAllRevealed()) fxCompletionTime = now;
}

// ===== EFFETTO BOLLE =====
#define BUBBLES_NUM 8
float bubbleX[BUBBLES_NUM];
float bubbleY[BUBBLES_NUM];
float bubbleSpeed[BUBBLES_NUM];
uint8_t bubblePhase[BUBBLES_NUM];

void initBubblesEffect() {
    fxInitCommon();
    for (uint8_t i = 0; i < BUBBLES_NUM; i++) {
        bubbleX[i] = random8(MATRIX_WIDTH);
        bubbleY[i] = MATRIX_HEIGHT + random8(10);
        bubbleSpeed[i] = 0.15f + random8(20) / 100.0f;
        bubblePhase[i] = random8();
    }
}

void updateBubblesEffect() {
    if (fxHandleCompletion(CRGB::White)) return;
    uint32_t now = millis();
    if (now - lastFxUpdate < 35) return;
    lastFxUpdate = now;
    FastLED.clear();
    for (uint8_t i = 0; i < BUBBLES_NUM; i++) {
        bubbleY[i] -= bubbleSpeed[i];
        bubblePhase[i] += 4;
        // Oscillazione orizzontale sinusoidale
        float bx = bubbleX[i] + (sin8(bubblePhase[i]) - 128) / 64.0f;
        int8_t ix = (int8_t)(bx + 0.5f);
        int8_t iy = (int8_t)(bubbleY[i] + 0.5f);
        if (iy < 0) {
            bubbleX[i] = random8(MATRIX_WIDTH);
            bubbleY[i] = MATRIX_HEIGHT + random8(6);
            bubbleSpeed[i] = 0.15f + random8(20) / 100.0f;
            continue;
        }
        if (ix >= 0 && ix < MATRIX_WIDTH && iy < MATRIX_HEIGHT) {
            uint16_t pos = xyToLED(ix, iy);
            // La bolla scoppia sulla lettera e la rivela
            if (targetPixels[pos] && !fxRevealed[pos]) {
                fxRevealed[pos] = true;
                bubbleY[i] = MATRIX_HEIGHT + random8(6);
                bubbleX[i] = random8(MATRIX_WIDTH);
                continue;
            }
            leds[pos] = CHSV(140, 120, 180);
        }
    }
    for (uint16_t i = 0; i < NUM_LEDS; i++) {
        if (fxRevealed[i]) leds[i] = CRGB::White;
    }
    if (currentBlink == 1 && currentMinute > 0) showSeconds(currentSecond, CRGB::White);
    FastLED.show();
    if (fxAllRevealed()) fxCompletionTime = now;
}

// ===== EFFETTO PONG =====
float pongBallX = 8, pongBallY = 8;
float pongVelX = 0.45f, pongVelY = 0.3f;
float pongPadL = 8, pongPadR = 8;
#define PONG_PAD_H 4

void initPongEffect() {
    fxInitCommon();
    pongBallX = 8; pongBallY = 8;
    pongVelX = (random8() & 1) ? 0.45f : -0.45f;
    pongVelY = (random8(60) - 30) / 100.0f;
    if (fabsf(pongVelY) < 0.1f) pongVelY = 0.2f;
    pongPadL = 8; pongPadR = 8;
}

void updatePongEffect() {
    if (fxHandleCompletion(CRGB::White)) return;
    uint32_t now = millis();
    if (now - lastFxUpdate < 30) return;
    lastFxUpdate = now;
    pongBallX += pongVelX;
    pongBallY += pongVelY;
    // Rimbalzo alto/basso
    if (pongBallY <= 0) { pongBallY = 0; pongVelY = fabsf(pongVelY); }
    if (pongBallY >= MATRIX_HEIGHT - 1) { pongBallY = MATRIX_HEIGHT - 1; pongVelY = -fabsf(pongVelY); }
    // Racchette: rimbalzo con variazione di angolo
    if (pongBallX <= 1) { pongBallX = 1; pongVelX = fabsf(pongVelX); pongVelY += (random8(40) - 20) / 100.0f; }
    if (pongBallX >= MATRIX_WIDTH - 2) { pongBallX = MATRIX_WIDTH - 2; pongVelX = -fabsf(pongVelX); pongVelY += (random8(40) - 20) / 100.0f; }
    pongVelY = constrain(pongVelY, -0.6f, 0.6f);
    // Le racchette inseguono la pallina (mai un errore: e solo scenografia)
    float targetPad = pongBallY - PONG_PAD_H / 2.0f;
    pongPadL += constrain(targetPad - pongPadL, -0.4f, 0.4f);
    pongPadR += constrain(targetPad - pongPadR, -0.4f, 0.4f);
    pongPadL = constrain(pongPadL, 0, MATRIX_HEIGHT - PONG_PAD_H);
    pongPadR = constrain(pongPadR, 0, MATRIX_HEIGHT - PONG_PAD_H);
    // La pallina rivela le lettere che attraversa
    int8_t bx = (int8_t)(pongBallX + 0.5f);
    int8_t by = (int8_t)(pongBallY + 0.5f);
    uint16_t bpos = xyToLED(bx, by);
    if (targetPixels[bpos]) fxRevealed[bpos] = true;
    FastLED.clear();
    for (uint16_t i = 0; i < NUM_LEDS; i++) {
        if (fxRevealed[i]) leds[i] = CRGB(150, 150, 150);
    }
    for (uint8_t i = 0; i < PONG_PAD_H; i++) {
        leds[xyToLED(0, (int8_t)pongPadL + i)] = CRGB::White;
        leds[xyToLED(MATRIX_WIDTH - 1, (int8_t)pongPadR + i)] = CRGB::White;
    }
    leds[bpos] = CRGB::Yellow;
    if (currentBlink == 1 && currentMinute > 0) showSeconds(currentSecond, CRGB(150, 150, 150));
    FastLED.show();
    if (fxAllRevealed()) fxCompletionTime = now;
}

// ===== EFFETTO LAVA LAMP (continuo) =====
#define LAVA_BLOBS 4
float lavaX[LAVA_BLOBS], lavaY[LAVA_BLOBS];
float lavaVX[LAVA_BLOBS], lavaVY[LAVA_BLOBS];
float lavaR[LAVA_BLOBS];

void initLavaEffect() {
    lastFxUpdate = 0;
    fxLoadTargets();
    for (uint8_t i = 0; i < LAVA_BLOBS; i++) {
        lavaX[i] = 3 + random8(10);
        lavaY[i] = random8(MATRIX_HEIGHT);
        lavaVX[i] = (random8(20) - 10) / 200.0f;                          // drift laterale lentissimo
        lavaVY[i] = ((random8(30) + 10) / 200.0f) * ((i & 1) ? 1 : -1);   // meta sale, meta scende
        lavaR[i] = 2.0f + random8(15) / 10.0f;                            // raggio 2.0 - 3.5
    }
}

void updateLavaEffect() {
    uint32_t now = millis();
    if (now - lastFxUpdate < 45) return;
    lastFxUpdate = now;
    for (uint8_t i = 0; i < LAVA_BLOBS; i++) {
        lavaX[i] += lavaVX[i];
        lavaY[i] += lavaVY[i];
        // Rimbalzi morbidi ai bordi (i blob possono uscire un poco, come nella lampada vera)
        if (lavaX[i] < 1.0f)  { lavaX[i] = 1.0f;  lavaVX[i] = fabsf(lavaVX[i]); }
        if (lavaX[i] > 14.0f) { lavaX[i] = 14.0f; lavaVX[i] = -fabsf(lavaVX[i]); }
        if (lavaY[i] < -2.0f) { lavaY[i] = -2.0f; lavaVY[i] = fabsf(lavaVY[i]); }
        if (lavaY[i] > 17.0f) { lavaY[i] = 17.0f; lavaVY[i] = -fabsf(lavaVY[i]); }
        // Ogni tanto la spinta termica cambia un poco
        if (random8() < 4) {
            lavaVY[i] += (random8(20) - 10) / 400.0f;
            lavaVY[i] = constrain(lavaVY[i], -0.22f, 0.22f);
        }
    }
    // Campo metaball: somma dei contributi di ogni blob su ogni pixel
    for (uint8_t y = 0; y < MATRIX_HEIGHT; y++) {
        for (uint8_t x = 0; x < MATRIX_WIDTH; x++) {
            float field = 0;
            for (uint8_t i = 0; i < LAVA_BLOBS; i++) {
                float dx = x - lavaX[i];
                float dy = y - lavaY[i];
                field += (lavaR[i] * lavaR[i]) / (dx * dx + dy * dy + 0.5f);
            }
            uint16_t pos = xyToLED(x, y);
            if (field > 1.0f) {
                // Dentro la lava: dal rosso scuro del bordo al giallo del nucleo
                float f = field - 1.0f;
                uint8_t hue = (f > 2.5f) ? 30 : (uint8_t)(f * 12.0f);
                uint8_t bri = (f > 2.7f) ? 255 : (uint8_t)(120 + f * 50.0f);
                leds[pos] = CHSV(hue, 255, bri);
            } else {
                leds[pos] = CRGB(15, 0, 25);   // vetro della lampada: viola scurissimo
            }
            if (targetPixels[pos]) leds[pos] = CRGB::White;
        }
    }
    if (currentBlink == 1 && currentMinute > 0) showSeconds(currentSecond, CRGB::White);
    FastLED.show();
}

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
       EEPROM.write(EEPROM_SCROLL_SPEED_ADDR_H, (defaultSpeed >> 8) & 0xFF);
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
    // A ore piene (minuti = 0) la E non fa parte dell'orario: mai accenderla
    if (currentMinute == 0) return;
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
        case MODE_ARKANOID:
            return (arkanoidCompletionTime != 0);
        case MODE_LASER:
            return (laserCompletionTime != 0);
        case MODE_FUOCO:
            return !fuocoEffectActive;
        case MODE_BLACKHOLE:
            return (blackholeCompletionTime != 0);
        case MODE_FIRE1:
            return (now - lastEffectChangeTime >= FIRE1_DURATION_MS);
        case MODE_FIREWORK:
            return (fireworkCompletionTime != 0);
        case MODE_DRIFT:
            return (driftCompletionTime != 0);
        case MODE_RADAR:
        case MODE_STARS:
        case MODE_HEARTBEAT:
        case MODE_TETRIS:
        case MODE_SNAKE:
        case MODE_SAND:
        case MODE_STORM:
        case MODE_BUBBLES:
        case MODE_PONG:
            // Effetti col framework fx comune
            return (fxCompletionTime != 0) || !fxActive;
        case MODE_MATRIX:
        case MODE_FAST:
        case MODE_RAINBOW:
        case MODE_CHRISTMAS:
        case MODE_SNOWFALL:
        case MODE_PLASMA:
        case MODE_AURORA:
        case MODE_LAVA:
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
    // Edge-triggered: esegui una sola volta per minuto, non per tutto il secondo 0
    static uint8_t lastResetMinute = 255;
    if (currentSecond != 0) {
        lastResetMinute = 255;
    }
    if (currentSecond == 0 && effectCompletedTime != 0 && lastResetMinute != currentMinute) {
        lastResetMinute = currentMinute;
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
        } else if (currentMode == MODE_ARKANOID) {
            initArkanoidEffect();
        } else if (currentMode == MODE_LASER) {
            initLaserEffect();
        } else if (currentMode == MODE_FUOCO) {
            initFuocoEffect();
        } else if (currentMode == MODE_BLACKHOLE) {
            initBlackHoleEffect();
        } else if (currentMode == MODE_FIREWORK) {
            initFireworkEffect();
        } else if (currentMode == MODE_DRIFT) {
            initDriftEffect();
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
           arkanoidEffectActive = false;
           laserEffectActive = false;
           fuocoEffectActive = false;
           blackholeEffectActive = false;
           fireworkEffectActive = false;
           driftEffectActive = false;
           fxActive = false;
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
           if (currentMode == MODE_ARKANOID) initArkanoidEffect();
           if (currentMode == MODE_CHRISTMAS) initChristmasEffect();
           if (currentMode == MODE_SNOWFALL) initSnowfallEffect();
           if (currentMode == MODE_LASER) initLaserEffect();
           if (currentMode == MODE_FUOCO) initFuocoEffect();
           if (currentMode == MODE_BLACKHOLE) initBlackHoleEffect();
           if (currentMode == MODE_FIRE1) initFire1Effect();
           if (currentMode == MODE_FIREWORK) initFireworkEffect();
           if (currentMode == MODE_DRIFT) initDriftEffect();
           if (currentMode == MODE_PLASMA) initPlasmaEffect();
           if (currentMode == MODE_AURORA) initAuroraEffect();
           if (currentMode == MODE_RADAR) initRadarEffect();
           if (currentMode == MODE_STARS) initStarsEffect();
           if (currentMode == MODE_HEARTBEAT) initHeartbeatEffect();
           if (currentMode == MODE_TETRIS) initTetrisEffect();
           if (currentMode == MODE_SNAKE) initSnakeEffect();
           if (currentMode == MODE_SAND) initSandEffect();
           if (currentMode == MODE_STORM) initStormEffect();
           if (currentMode == MODE_BUBBLES) initBubblesEffect();
           if (currentMode == MODE_PONG) initPongEffect();
           if (currentMode == MODE_LAVA) initLavaEffect();
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
    else if(currentMode == MODE_ARKANOID) {
        updateArkanoidEffect();
    }
    else if(currentMode == MODE_CHRISTMAS) {
        updateChristmasEffect();
    }
    else if(currentMode == MODE_SNOWFALL) {
        updateSnowfallEffect();
    }
    else if(currentMode == MODE_LASER) {
        updateLaserEffect();
    }
    else if(currentMode == MODE_FUOCO) {
        updateFuocoEffect();
    }
    else if(currentMode == MODE_BLACKHOLE) {
        updateBlackHoleEffect();
    }
    else if(currentMode == MODE_FIRE1) {
        updateFire1Effect();
    }
    else if(currentMode == MODE_FIREWORK) {
        updateFireworkEffect();
    }
    else if(currentMode == MODE_DRIFT) {
        updateDriftEffect();
    }
    else if(currentMode == MODE_PLASMA) {
        updatePlasmaEffect();
    }
    else if(currentMode == MODE_AURORA) {
        updateAuroraEffect();
    }
    else if(currentMode == MODE_RADAR) {
        updateRadarEffect();
    }
    else if(currentMode == MODE_STARS) {
        updateStarsEffect();
    }
    else if(currentMode == MODE_HEARTBEAT) {
        updateHeartbeatEffect();
    }
    else if(currentMode == MODE_TETRIS) {
        updateTetrisEffect();
    }
    else if(currentMode == MODE_SNAKE) {
        updateSnakeEffect();
    }
    else if(currentMode == MODE_SAND) {
        updateSandEffect();
    }
    else if(currentMode == MODE_STORM) {
        updateStormEffect();
    }
    else if(currentMode == MODE_BUBBLES) {
        updateBubblesEffect();
    }
    else if(currentMode == MODE_PONG) {
        updatePongEffect();
    }
    else if(currentMode == MODE_LAVA) {
        updateLavaEffect();
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

                currentPreset = (currentPreset + 1) % 40;
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

       case 19:
           currentMode = MODE_ARKANOID;
           manualModeSelected = false;  // Permetti overlay digitale
           initArkanoidEffect();
           break;

       case 20:
           currentMode = MODE_CHRISTMAS;
           manualModeSelected = false;  // Permetti overlay digitale
           initChristmasEffect();
           break;

       case 21:
           currentMode = MODE_SNOWFALL;
           manualModeSelected = false;  // Permetti overlay digitale
           initSnowfallEffect();
           break;

       case 22:
           currentMode = MODE_LASER;
           manualModeSelected = false;  // Permetti overlay digitale
           initLaserEffect();
           break;

       case 23:
           currentMode = MODE_FUOCO;
           manualModeSelected = false;  // Permetti overlay digitale
           initFuocoEffect();
           break;

       case 24:
           currentMode = MODE_BLACKHOLE;
           manualModeSelected = false;  // Permetti overlay digitale
           initBlackHoleEffect();
           break;

       case 25:
           currentMode = MODE_FIRE1;
           manualModeSelected = false;  // Permetti overlay digitale
           initFire1Effect();
           break;

       case 26:
           currentMode = MODE_FIREWORK;
           manualModeSelected = false;  // Permetti overlay digitale
           initFireworkEffect();
           break;

       case 27:
           currentMode = MODE_DRIFT;
           manualModeSelected = false;  // Permetti overlay digitale
           initDriftEffect();
           break;

       case 28: currentMode = MODE_PLASMA;    manualModeSelected = false; initPlasmaEffect();    break;
       case 29: currentMode = MODE_AURORA;    manualModeSelected = false; initAuroraEffect();    break;
       case 30: currentMode = MODE_RADAR;     manualModeSelected = false; initRadarEffect();     break;
       case 31: currentMode = MODE_STARS;     manualModeSelected = false; initStarsEffect();     break;
       case 32: currentMode = MODE_HEARTBEAT; manualModeSelected = false; initHeartbeatEffect(); break;
       case 33: currentMode = MODE_TETRIS;    manualModeSelected = false; initTetrisEffect();    break;
       case 34: currentMode = MODE_SNAKE;     manualModeSelected = false; initSnakeEffect();     break;
       case 35: currentMode = MODE_SAND;      manualModeSelected = false; initSandEffect();      break;
       case 36: currentMode = MODE_STORM;     manualModeSelected = false; initStormEffect();     break;
       case 37: currentMode = MODE_BUBBLES;   manualModeSelected = false; initBubblesEffect();   break;
       case 38: currentMode = MODE_PONG;      manualModeSelected = false; initPongEffect();      break;
       case 39: currentMode = MODE_LAVA;      manualModeSelected = false; initLavaEffect();      break;

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
   
   uint16_t baseAddr = modeSettingsAddr(preset);
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
   
   // Quadrante a 24 ore: usa l'ora diretta come tutti gli altri effetti
   const uint8_t* hourWord = (const uint8_t*)pgm_read_ptr(&HOUR_WORDS[currentHour]);
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

        // Calcolo a 16 bit: in uint8_t step*increment wrappa a 256 e con luminosità
        // vicino al massimo il target non veniva mai raggiunto (fade-in infinito)
        uint16_t currentBrightLevel = (uint16_t)slowState.step * slowState.increment;
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


    if (currentMinute != tronLastMinute) {
        tronLastMinute = currentMinute;
        tronNeedsInit = true;
        tronEffectActive = true;
        return;
    }

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
    // 🔧 FIX "E" ore piene MOTO - spegne LED 116 se minuti == 0
    if (currentMinute == 0) {
        motoTextPixels[116] = false;
    }

    uint32_t currentMillis = millis();
    if (currentMinute != motoLastMinute) {
        motoLastMinute = currentMinute;
        initMotoEffect();
        return;
    }
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
        // A ore piene (minuti = 0) la E non fa parte dell'orario: non accenderla
        if (currentMinute > 0) {
            showSeconds(currentSecond, motoTextColor);
        }
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