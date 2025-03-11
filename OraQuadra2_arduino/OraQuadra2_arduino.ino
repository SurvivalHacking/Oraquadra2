// OraQuadra V2.0 - per nuovo pannello frontale
// By Davide Gatti SURVIVAL HACKING  www.survivalhacking.it
//
// Setup:
//
// Pulsante 1 = per la regolazione dell'ora
// Pulsante 2 = per la regolazione deli minuti
// Pulsante 1 + Pulsante 2 = cambio preset 0=Sfumato bianco  1-5=Scrittura lenta vari colori  6-10=Scrittira veloce vari colori 11=Matrix classico 12=Multicolor fill
//
// Cambiati valori di luminosità di default per evitare che si bruci Arduino per l'eccessivo assorbimento della matrice LED 
//
// Modificato da Angelo Carrieri il 08/01/2025 con l'aggiunta di altri due modalità
// Modo 11=Effetto matrix classico
// Modo 12=Effetto multicolor fill
//
//
// 13/01/2025 Cambiata organizzazione lettere sul pannello frontale grazie a Luca Bertalmio, l'ora viene rappresentat con le 24 ore con anche scritte verticali
//

#include "RTClib.h"
#include <Adafruit_NeoPixel.h>
#include <EEPROM.h>

RTC_DS3231 rtc;       // Istanza per usare l'RTC

//Definizioni per neopixel
#define PIN   4        // pin neopixel
#define NUMPIXELS 256  // 16 x 16
#define GIORNO 64     // Luminosità di giorno (Attenzione, non alzare troppo questo valore visto che la matrice è alimentata dall'arduino che potrebbe bruciarsi se esagerate)
#define NOTTE 32       // Luminosità di notte

Adafruit_NeoPixel strip(NUMPIXELS, PIN, NEO_GRB + NEO_KHZ800);
byte intBrightness;

//Definizione colori
uint32_t Red = strip.Color(255, 0, 0); 
uint32_t Green = strip.Color( 0, 255, 0);
uint32_t Blue = strip.Color( 0, 0, 255);
uint32_t White = strip.Color( 255, 255, 255);
uint32_t Yellow = strip.Color( 255, 255, 0);
uint32_t Purple = strip.Color( 60, 0, 255);
uint32_t Gray = strip.Color( 128, 128, 128);
uint32_t Off = strip.Color( 0, 0, 0);

byte P1= 3; // Tasto O  avanzamento ore
byte P2= 2; // Tasto M  avanzamento minuti

byte oracorrente;        // variabile ora corrente
byte oraprecedente;      // variabile ora precedente
byte minutocorrente;     // variabile minuto corrente
byte minutoprecedente;   // variabile minuto precedente
byte mode;               // variabile per la modalità   
byte modo;               // variabile per la modalità mirror
byte Presets;            // Preset corrente
uint32_t Colore;         // Variabile per il passaggio colore
byte ledstrip[NUMPIXELS];   // Array di 256 byte per mappare i led da accendere per effetto Matrix

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


//Definizione testi vari
byte  txtSONOLEORE[] = {15,14,13,12,10,9,7,6,5,8};
byte  txtSURVIVAL[] = {15,11,6,16,25,75,89,97,115,121,141,140,134,130,144,8};
byte  txtMINUTI[] = {250,251,252,253,254,255,8};

//Definizione delle stringhe delle ore
byte  txtUNA[] = {57,70,89,8};
byte  txtDUE[] = {111,112,143,8};
byte  txtTRE[] = {21,22,23,8};
byte  txtQUATTRO[] = {56,57,58,59,60,61,62,8};
byte  txtCINQUE[] = {46,49,78,81,110,113,8};
byte  txtSEI[] = {34,33,32,8};
byte  txtSETTE[] = {91,92,93,94,95,8};
byte  txtOTTO[] = {28,29,30,31,8};
byte  txtNOVE[] = {101,100,99,98,8};
byte  txtDIECI[] = {68,67,66,65,64,8};
byte  txtUNDICI[] = {50,51,52,53,54,55,8};
byte  txtDODOCI[] = {109,108,107,106,105,104,8};
byte  txtTREDICI[] = {21,22,23,24,25,26,27,8};
byte  txtQUATTORDICI[] = {45,44,43,42,41,40,39,38,37,36,35,8};
byte  txtQUINDICI[] = {45,50,77,82,109,114,141,146,8};
byte  txtSEDICI[] = {83,84,85,86,87,88,8};
byte  txtDICIASSETTE[] = {85,86,87,88,89,90,91,92,93,94,95,8};
byte  txtDICIOTTO[] = {24,25,26,27,28,29,30,31,8};
byte  txtDICIANNOVE[] = {107,106,105,104,103,102,101,100,99,98,8};
byte  txtVENTI[] = {16,47,48,79,80,8};
byte  txtVENTUNO[] = {75,74,73,72,71,70,69,8};
byte  txtVENTIDUE[] = {16,47,48,79,80,111,112,143,8};
byte  txtVENTITRE[] = {16,17,18,19,20,21,22,23,8};
byte  txtZERO[] = {3,2,1,0,8};

// Definizione delle stringhe dei minuti
byte  txtE[] = {116,8};
byte  txtMUNO[] = {157,158,159,8};
byte  txtMUN[] = {239,238,8};
byte  txtMDUE[] = {246,247,248,8};
byte  txtMTRE[] = {210,211,212,8};
byte  txtMQUATTRO[] = {207,206,205,204,203,202,201,8};
byte  txtMCINQUE[] = {240,241,242,243,244,245,8};
byte  txtMSEI[] = {226,225,224,8};
byte  txtMSETTE[] = {219,220,221,222,223,8};
byte  txtMOTTO[] = {163,162,161,160,8};
byte  txtMNOVE[] = {231,230,229,228,8};
byte  txtMDIECI[] = {176,177,178,179,180,8};
byte  txtMUNDICI[] = {239,238,237,236,235,234,8};
byte  txtMDODICI[] = {169,168,167,166,165,164,8};
byte  txtMTREDICI[] = {210,211,212,213,214,215,216,8};
byte  txtMQUATTORDICI[] = {181,182,183,184,185,186,187,188,189,190,191,8};
byte  txtMQUINDICI[] = {200,199,198,197,196,195,194,193,8};
byte  txtMSEDICI[] = {175,174,173,172,171,170,8};
byte  txtMDICIASSETTE[] = {213,214,215,216,217,218,219,220,221,222,223,8};
byte  txtMDICIOTTO[] = {167,166,165,164,163,162,161,160,8};
byte  txtMDICIANNOVE[] = {237,236,235,234,233,232,231,230,229,228,8};
byte  txtMVENT[] = {138,137,136,135,8};
byte  txtMVENTI[] = {138,137,136,135,134,8};
byte  txtMTRENT[] = {133,132,131,130,129,8};
byte  txtMTRENTA[] = {133,132,131,130,129,128,8};
byte  txtMQUARANT[] = {119,120,121,122,123,124,125,8};
byte  txtMQUARANTA[] = {119,120,121,122,123,124,125,126,8};
byte  txtMCINQUANT[] = {148,149,150,151,152,153,154,155,8};
byte  txtMCINQUANTA[] = {148,149,150,151,152,153,154,155,156,8};


void setup () {
  if (EEPROM.read(0) != 0x55) {           // read configuration byte. If it is not 55h is not configured, than store a default values
    EEPROM.write(0, 0x55);                // write configuret marker
    EEPROM.write(1, 0);                   // scrivi il Preset di default
  }
  
  Presets=(EEPROM.read(1));               // Leggi eeprom e imposta il preset memorizzato
  Preset(Presets);                        // imposta il modo grafico del relativo preset

  strip.begin();                          // inizializza matrice neo pixel
  strip.clear();                          // spegni tutti i led della matrice  
  

  rainbow(1);                             // faiun effetto arcobaleno
  for (int i =0 ; i < 500; i++) {        // Scrivi SURVIVAL HACKING in multicolor e poi flash bianchi 
    paintWordFast(random(254),strip.Color( random(254), random(254), random(254)));
    strip.show();
  }
  
  
  for (int i =0 ; i < 5; i++) {           // Scrivi SURVIVAL HACKING in multicolor e poi flash bianchi 
    paintWordFast(txtSURVIVAL, Red);
    paintWordFast(txtSURVIVAL, Blue);
    paintWordFast(txtSURVIVAL, Yellow);
    paintWordFast(txtSURVIVAL, Green);
    paintWordFast(txtSURVIVAL, White);
  }
//  delay(1000);
  for (int i =0 ; i < 100; i++) {        // Scrivi SURVIVAL HACKING in multicolor e poi flash bianchi 
    paintWordFast(random(255),strip.Color(0,0,0));
    paintWordFast(random(255),strip.Color(0,0,0));
    paintWordFast(random(255),strip.Color(0,0,0));
    paintWordFast(random(255),strip.Color(0,0,0));
    paintWordFast(random(255),strip.Color(0,0,0));
    paintWordFast(random(255),strip.Color(0,0,0));
    paintWordFast(random(255),strip.Color(0,0,0));
    paintWordFast(random(255),strip.Color(0,0,0));
    paintWordFast(random(255),strip.Color(0,0,0));
    paintWordFast(random(255),strip.Color(0,0,0));
    strip.show();
  }
  strip.clear();
  paintWordSpeed(txtSURVIVAL, White, 200);
  delay(1000);

  strip.clear();                          // spegni tutti i led della matrice  
  strip.show();                           // aggiorna matrice neopixel

  pinMode(P1,INPUT_PULLUP);               // definisce il tipo di ingresso digitale con pullup
  pinMode(P2,INPUT_PULLUP);               // definisce il tipo di ingresso digitale con pullup

  oraprecedente =99;                      // impostazione variabile per aggiornamento forzato ora
  minutoprecedente =99;                   // impostazione variabile per aggiornamento forzato ora

  if (! rtc.begin()) {                    // inizializza RTC  
    while (1) delay(10);                  // se non trova l'RTC si blocca qui all'infinito
  }

  if (rtc.lostPower()) {                  // se viene rilevata una perdita di alimentazione l'RTC si imposta con la data e ora programmata
    rtc.adjust(DateTime(F(__DATE__), F(__TIME__)));
  }
}

void loop () {
  if(!digitalRead(P1) && !digitalRead(P2)) {  // Se si schiacciano i due pulsanti in contemporanea si cambia il preset
    Presets+=1;
    if (Presets > 12) Presets=0;
    Preset(Presets);
    EEPROM.write(1, Presets);               // scrivi Preset in EEPROM
  } else if(!digitalRead(P1)) {             // se si preme il pulsante H si regolano le ore
    DateTime now = rtc.now();               // leggi ora dall'RTC
    int ora = now.hour();       
    int minuti = now.minute();
    ora+=1;                                 // incrementa ora 
    if (ora > 23 ) ora = 0;                 // controlla limiti 
    rtc.adjust(DateTime(2024,10,18,ora,minuti,0)); // scrivi ora nell'RTC
    delay(100); 
    mode=2;                                 // imposta modalità aggiornamento led veloce 
  } else if(!digitalRead(P2)) {             // se si preme il pulsante M si regolano i minuti 
    DateTime now = rtc.now();               // leggi ora dall'RTC
    int ora = now.hour();
    int minuti = now.minute();
    minuti+=1;                              // incrementa minuti
    if (minuti > 59 ) minuti = 0;           // controlla limiti 
    rtc.adjust(DateTime(2024,10,18,ora,minuti,0)); // scrivi ora nell'RTC
    delay(100);
    mode=2;                                 // imposta modalità aggiornamento led veloce 
  } else {
    mode=modo;                              // se non sono premuti tasti metti la modalità impostata dai Preset
  }
  DateTime now = rtc.now();               // leggi ora dall'RTC
  if((now.hour() < 7) || (now.hour() >= 19)){  // se nella fascia oraria dalle 19 alle 7 abbassa la luminosità
    intBrightness =  NOTTE;               // setta la luminosità notturna
  } else {
    intBrightness =  GIORNO;              // setta la luminosità giorno
  }
  strip.setBrightness(intBrightness);     // imposta la luminosità nella matrice neopixel
  strip.show();       	      			      // aggiorna la matrice

  minutocorrente = now.minute();          // imposta variabile locale dei minuti correnti
  oracorrente = now.hour();               // imposta variabile locale delle ore correnti

  paintWord(txtSONOLEORE, Colore, mode);   // scrivi 'SONO LE ORE'


  if (oracorrente != oraprecedente) {     // verifica se + cambiata l'ora
    AggiornaOre(oraprecedente,Off,2);     // cancella l'ora precedente
    AggiornaOre(oracorrente,Colore,mode); // scrivi l'ora corrente
    oraprecedente=oracorrente;            // imposta l'ora precedente
    strip.show();				                  // aggiorna la matrice
  }  

  if (minutocorrente != minutoprecedente) {       // verifica se + cambiati i minuti
    AggiornaMinuti(minutoprecedente,Off,2);       // cancella i minuti precedenti
    if (minutocorrente == 0) {                    // se minuti = 0 non scrivere "E"
      paintWord(txtE, Off,2);                     // cancella "E"
      paintWord(txtMINUTI, Off,2);                // cancella "MINUTI"
    } else {
      paintWord(txtE, Colore,mode);               // Se minuti <> 0 scrivi "E"
      AggiornaMinuti(minutocorrente,Colore,mode); // scrivi i minuti correnti
      paintWord(txtMINUTI, Colore,mode);          // Se minuti <> 0 scrivi "MINUTI"
    }
    minutoprecedente=minutocorrente;              // imposta l'ora precedente
    switch (modo) {
      case 0:
        strip.show();
        break;
      case 1:
        strip.show();
        break;
      case 2:
        strip.show();
        break;
      case 3:
        MatrixEffect();
        break;
      case 4:
        SnakeEffect();
        break;
    }

  }  
  strip.show();     				                      // aggiorna la matrice
  delay(500);                                     // attesa di un secondo per far pulsare la scritta se in modalità 0
}


void Preset(int Passedpreset) {                   // imposta preset grafico. 0=sufmato bianco 1-5=Scriturra lenta vari colori  6-10=Scrittura rapida vari colori  11=Effetto Matrix
  switch (Passedpreset) { 
  case 0:
    modo=0;
    Colore=White;
    break;    
  case 1:
    modo=1;
    Colore=White;
    break;    
  case 2:
    modo=1;
    Colore=Blue;
    break;    
  case 3:
    modo=1;
    Colore=Red;
    break;    
  case 4:
    modo=1;
    Colore=Purple;
    break;    
  case 5:
    modo=1;
    Colore=Green;
    break;    
  case 6:
    modo=2;
    Colore=White;
    break;    
  case 7:
    modo=2;
    Colore=Blue;
    break;    
  case 8:
    modo=2;
    Colore=Red;
    break;    
  case 9:
    modo=2;
    Colore=Purple;
    break;    
  case 10:
    modo=2;
    Colore=Green;
    break;    
  case 11:
    modo=3;
    Colore=White;
    clearStrip(); //Azzera i valori della striscia led
    break;    
  case 12:
    modo=4;
    Colore=Yellow;
    clearStrip(); //Azzera i valori della striscia led
    break;    
  }
  strip.clear();
  strip.show();
  oraprecedente =99;
  minutoprecedente =99;
  mode=modo;
  delay(500);
}


void AggiornaOre(byte ora,uint32_t Colore, byte modo) { // in base all'ora passata, scrive stringa dell'ora nella matrice
 switch (ora) { 
    case 99:
      break;
    case 0:
    case 24:
      paintWord(txtZERO, Colore, modo);
      break;
    case 12: 
      paintWord(txtDODOCI, Colore, modo);
      break;
    case 1:
      paintWord(txtUNA, Colore, modo);
      break;
    case 13:
      paintWord(txtTREDICI, Colore, modo);
      break;
    case 2:
      paintWord(txtDUE, Colore, modo);
      break;
    case 14:
      paintWord(txtQUATTORDICI, Colore, modo);
      break;
    case 3:
      paintWord(txtTRE, Colore, modo);
      break;
    case 15:
      paintWord(txtQUINDICI, Colore, modo);
      break;
    case 4:
      paintWord(txtQUATTRO, Colore, modo);
      break;
    case 16:
      paintWord(txtSEDICI, Colore, modo);
      break;
    case 5:
      paintWord(txtCINQUE, Colore, modo);
      break;
    case 17:
      paintWord(txtDICIASSETTE, Colore, modo);
      break;
    case 6:
      paintWord(txtSEI, Colore, modo);
      break;
    case 18:
      paintWord(txtDICIOTTO, Colore, modo);
      break;
    case 7:
      paintWord(txtSETTE, Colore, modo);
      break;
    case 19:
      paintWord(txtDICIANNOVE, Colore, modo);
      break;
    case 8:
      paintWord(txtOTTO, Colore, modo);
      break;
    case 20:
      paintWord(txtVENTI, Colore, modo);
      break;
    case 9:
      paintWord(txtNOVE, Colore, modo);
      break;
    case 21:
      paintWord(txtVENTUNO, Colore, modo);
      break;
    case 10:
      paintWord(txtDIECI, Colore, modo);
      break;
    case 22:
      paintWord(txtVENTIDUE, Colore, modo);
      break;
    case 11:
      paintWord(txtUNDICI, Colore, modo);
      break;
    case 23:
      paintWord(txtVENTITRE, Colore, modo);
      break;
    }
}


void AggiornaMinuti(byte minuti, uint32_t Colore, byte modo) { // in base alminuto passato, scrive stringa del minuto nella matrice
  switch (minuti) { 
    case 99:
      break;
    case 0:
      break;
    case 1:
      paintWord(txtMUNO, Colore, modo);
      break;
    case 2:
      paintWord(txtMDUE, Colore, modo);
      break;
    case 3:
      paintWord(txtMTRE, Colore, modo);
      break;
    case 4:
      paintWord(txtMQUATTRO, Colore, modo);
      break;
    case 5:
      paintWord(txtMCINQUE, Colore, modo);
      break;
    case 6:
      paintWord(txtMSEI, Colore, modo);
      break;
    case 7:
      paintWord(txtMSETTE, Colore, modo);
      break;
    case 8:
      paintWord(txtMOTTO, Colore, modo);
      break;
    case 9:
      paintWord(txtMNOVE, Colore, modo);
      break;
    case 10:
      paintWord(txtMDIECI, Colore, modo);
      break;
    case 11:
      paintWord(txtMUNDICI, Colore, modo);
      break;
    case 12:
      paintWord(txtMDODICI, Colore, modo);
      break;
    case 13:
      paintWord(txtMTREDICI, Colore, modo);
      break;
    case 14:
      paintWord(txtMQUATTORDICI, Colore, modo);
      break;
    case 15:
      paintWord(txtMQUINDICI, Colore, modo);
      break;
    case 16:
      paintWord(txtMSEDICI, Colore, modo);
      break;
    case 17:
      paintWord(txtMDICIASSETTE, Colore, modo);
      break;
    case 18:
      paintWord(txtMDICIOTTO, Colore, modo);
      break;
    case 19:
      paintWord(txtMDICIANNOVE, Colore, modo);
      break;
    case 20:
      paintWord(txtMVENTI, Colore, modo);
      break;
    case 21:
      paintWord(txtMVENT, Colore, modo);
      paintWord(txtMUN, Colore, modo);
      break;
    case 22:
      paintWord(txtMVENTI, Colore, modo);
      paintWord(txtMDUE, Colore, modo);
      break;
    case 23:
      paintWord(txtMVENTI, Colore, modo);
      paintWord(txtMTRE, Colore, modo);
      break;
    case 24:
      paintWord(txtMVENTI, Colore, modo);
      paintWord(txtMQUATTRO, Colore, modo);
      break;
    case 25:
      paintWord(txtMVENTI, Colore, modo);
      paintWord(txtMCINQUE, Colore, modo);
      break;
    case 26:
      paintWord(txtMVENTI, Colore, modo);
      paintWord(txtMSEI, Colore, modo);
      break;
    case 27:
      paintWord(txtMVENTI, Colore, modo);
      paintWord(txtMSETTE, Colore, modo);
      break;
    case 28:
      paintWord(txtMVENT, Colore, modo);
      paintWord(txtMOTTO, Colore, modo);
      break;
    case 29:
      paintWord(txtMVENTI, Colore, modo);
      paintWord(txtMNOVE, Colore, modo);
      break;
    case 30:
      paintWord(txtMTRENTA, Colore, modo);
      break;
    case 31:
      paintWord(txtMTRENT, Colore, modo);
      paintWord(txtMUN, Colore, modo);
      break;
    case 32:
      paintWord(txtMTRENTA, Colore, modo);
      paintWord(txtMDUE, Colore, modo);
      break;
    case 33:
      paintWord(txtMTRENTA, Colore, modo);
      paintWord(txtMTRE, Colore, modo);
      break;
    case 34:
      paintWord(txtMTRENTA, Colore, modo);
      paintWord(txtMQUATTRO, Colore, modo);
      break;
    case 35:
      paintWord(txtMTRENTA, Colore, modo);
      paintWord(txtMCINQUE, Colore, modo);
      break;
    case 36:
      paintWord(txtMTRENTA, Colore, modo);
      paintWord(txtMSEI, Colore, modo);
      break;
    case 37:
      paintWord(txtMTRENTA, Colore, modo);
      paintWord(txtMSETTE, Colore, modo);
      break;
    case 38:
      paintWord(txtMTRENT, Colore, modo);
      paintWord(txtMOTTO, Colore, modo);
      break;
    case 39:
      paintWord(txtMTRENTA, Colore, modo);
      paintWord(txtMNOVE, Colore, modo);
      break;
    case 40:
      paintWord(txtE, Colore, modo);
      paintWord(txtMQUARANTA, Colore, modo);
      break;
    case 41:
      paintWord(txtMQUARANT, Colore, modo);
      paintWord(txtMUN, Colore, modo);
      break;
    case 42:
      paintWord(txtMQUARANTA, Colore, modo);
      paintWord(txtMDUE, Colore, modo);
      break;
    case 43:
      paintWord(txtMQUARANTA, Colore, modo);
      paintWord(txtMTRE, Colore, modo);
      break;
    case 44:
      paintWord(txtMQUARANTA, Colore, modo);
      paintWord(txtMQUATTRO, Colore, modo);
      break;
    case 45:
      paintWord(txtMQUARANTA, Colore, modo);
      paintWord(txtMCINQUE, Colore, modo);
      break;
    case 46:
      paintWord(txtMQUARANTA, Colore, modo);
      paintWord(txtMSEI, Colore, modo);
      break;
    case 47:
      paintWord(txtMQUARANTA, Colore, modo);
      paintWord(txtMSETTE, Colore, modo);
      break;
    case 48:
      paintWord(txtMQUARANT, Colore, modo);
      paintWord(txtMOTTO, Colore, modo);
      break;
    case 49:
      paintWord(txtMQUARANTA, Colore, modo);
      paintWord(txtMNOVE, Colore, modo);
      break;
    case 50:
      paintWord(txtMCINQUANTA, Colore, modo);
      break;
    case 51:
      paintWord(txtMCINQUANT, Colore, modo);
      paintWord(txtMUN, Colore, modo);
      break;
    case 52:
      paintWord(txtMCINQUANTA, Colore, modo);
      paintWord(txtMDUE, Colore, modo);
      break;
    case 53:
      paintWord(txtMCINQUANTA, Colore, modo);
      paintWord(txtMTRE, Colore, modo);
      break;
    case 54:
      paintWord(txtMCINQUANTA, Colore, modo);
      paintWord(txtMQUATTRO, Colore, modo);
      break;
    case 55:
      paintWord(txtMCINQUANTA, Colore, modo);
      paintWord(txtMCINQUE, Colore, modo);
      break;
    case 56:
      paintWord(txtMCINQUANTA, Colore, modo);
      paintWord(txtMSEI, Colore, modo);
      break;
    case 57:
      paintWord(txtMCINQUANTA, Colore, modo);
      paintWord(txtMSETTE, Colore, modo);
      break;
    case 58:
      paintWord(txtMCINQUANT, Colore, modo);
      paintWord(txtMOTTO, Colore, modo);
      break;
    case 59:
      paintWord(txtMCINQUANTA, Colore, modo);
      paintWord(txtMNOVE, Colore, modo);
      break;
    }
}


void paintWord(byte arrWord[], uint32_t intColor, byte mode) { // Scrivi su matrice la strina passata, con eventuale colore e modalità 
  if (mode == 0) {                                           // 0=modo sfumato solo bianco
    if (intColor == 0) {
      paintWordFast(arrWord,intColor);
    } else {
      paintWordFast(arrWord,strip.Color( 30, 30, 30));
      delay(100);  
      paintWordFast(arrWord,strip.Color( 60, 60, 60));
      delay(100);  
      paintWordFast(arrWord,strip.Color( 90, 90, 90));
      delay(100);  
      paintWordFast(arrWord,strip.Color( 120, 120, 120));
      delay(100);  
      paintWordFast(arrWord,strip.Color( 150, 150, 150));
      delay(100);  
      paintWordFast(arrWord,strip.Color( 180, 180, 180));
      delay(100);  
      paintWordFast(arrWord,strip.Color( 210, 210, 210));
      delay(100);  
      paintWordFast(arrWord,strip.Color( 240, 240, 240));
      delay(100);  
      paintWordFast(arrWord,intColor);
    }
  } else if (mode == 1) {                                     // 1=modo lento con gestione colore
    paintWordSlow(arrWord, intColor);
  } else if (mode == 2 && modo!=3 && modo!=4){               // 2=modo veloce con gestione colore
    paintWordFast(arrWord, intColor);
  } else if (modo == 3) {                                     // 3=modo effetto Matrix
    paintWordMatrix(arrWord, intColor);
  } else if (modo == 4) {                                     // 4=modo Snake (utilizza lo stesso riemplimento della ledstrip di Matrix)
    paintWordMatrix(arrWord, intColor);
  }
}


void paintWordSlow(byte arrWord[], uint32_t intColor){     // scrittura nella matrice della sctringa passata in modalità lenta
  for(int i = 0; i < strip.numPixels() + 1; i++){
    if(arrWord[i] == 8) {
      strip.show();
      break;
    } else {
      strip.setPixelColor(arrWord[i],intColor);
      strip.show();
      delay(25);
    }
  }
}

void paintWordFast(byte arrWord[], uint32_t intColor){         // scrittura nella matrice della stringa passata in modalità veloce
  for(int i = 0; i < strip.numPixels() + 1; i++){
    if(arrWord[i] == 8) {
      strip.show();
      break;
    } else {
      strip.setPixelColor(arrWord[i],intColor);
   }
  }
}


void paintWordSpeed(byte arrWord[], uint32_t intColor, int speed) {  // scrittura nella matrice della stringa passata in modalità variabile
  for(int i = 0; i < strip.numPixels() + 1; i++){
    if(arrWord[i] == 8) {
      strip.show();
      break;
    } else {
      strip.setPixelColor(arrWord[i],intColor);
      strip.show();
      delay(speed);
    }
  }
}


void rainbow(uint8_t wait) {                // accendi tutto il display con un arcobaleno animato
  uint16_t i, j;
  for(j=0; j<256; j++) {
    for(i=0; i<strip.numPixels(); i++) {
      strip.setPixelColor(i, Wheel((i+j) & 255));
    }
    strip.show();
    delay(wait);
  }
}


uint32_t Wheel(byte WheelPos) {             // calcola il colore nel formato HUE
  WheelPos = 255 - WheelPos;
  if(WheelPos < 85) {
    return strip.Color(255 - WheelPos * 3, 0, WheelPos * 3);
  }
  if(WheelPos < 170) {
    WheelPos -= 85;
    return strip.Color(0, WheelPos * 3, 255 - WheelPos * 3);
  }
  WheelPos -= 170;
  return strip.Color(WheelPos * 3, 255 - WheelPos * 3, 0);
}


//***************************************************
//*****   Aggiunte per effetti Matrix e Snake   *****
//***************************************************
void paintWordMatrix(byte arrWord[], uint32_t intColor){         // imposta i byte delle parole da attivare nella ledstrip
  for(int i = 0; i < strip.numPixels() + 1; i++){
    if(arrWord[i] == 8) {
      break;
    } else {
      setStrip(ledstrip, arrWord[i], intColor!=Off);
    }
  }
}


void clearStrip() {
  memset(ledstrip, 0, sizeof(ledstrip));  
}


void setStrip(byte* ledstrip, int pos, bool on) {
  if (on) 
    ledstrip[pos] = 1;
  else
    ledstrip[pos] = 0;
}


byte getStrip(byte* ledstrip, int pos) {
  return (ledstrip[pos]);
}


void MatrixEffect() {
  int inizio=0;
  int incremento=0;
  int i = 0;
  // Genera pause casuali per la velocità di ogni lettera che scende
  byte dly1[16] = { random(10), random(10), random(10), random(10), random(10), random(10), random(10), random(10), random(10), random(10), random(10), random(10), random(10), random(10), random(10), random(10) };
  byte ptr[16] = { 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0 }; // Puntatore al led di ogni colonna

  for ( byte t = 0; t < 200; t++) { // Contatore per i cambiamenti di colore       
      for ( byte c = 0; c < 16; c++) { // Scorre tutte le colonne e verifica che il tempo non sia trascorso

          if ((t > (dly1[c]*(ptr[c] + 1))) && ptr[c] < 16 ) { // Se il tempo è stato superato, cancella il valore corrente e accende il led di bianco
            if (ptr[c] % 2 == 0) {
              inizio = ptr[c] * 16;
              incremento = 1;
            } else {
              inizio = ptr[c] * 16 + 15;
              incremento = -1;
            }

            i = inizio + (c * incremento);
            strip.setPixelColor(i,Green);
            strip.show();   
//            delay(1);

            ptr[c]++; 

            if (ptr[c] < 16) { 
              if (ptr[c] % 2 == 0) {
                inizio = ptr[c] * 16;
                incremento = 1;
              } else {
                inizio = ptr[c] * 16 + 15;
                incremento = -1;
              }

              i = inizio + (c * incremento);
              strip.setPixelColor(i,Gray);
              strip.show(); 
            } else {
              strip.setPixelColor(i,Green);
              strip.show();
            }
          }
      }
  }
  // Pulizia dello schermo
  byte dly2[16] = { random(10), random(10), random(10), random(10), random(10), random(10), random(10), random(10), random(10), random(10), random(10), random(10), random(10), random(10), random(10), random(10) };
  byte ptr2[16] = { 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0 }; 

  for ( byte t = 0; t < 200; t++) { 
      for ( byte c = 0; c < 16; c++) { 

          if ((t > (dly2[c]*(ptr2[c] + 1))) && ptr2[c] < 16 ) { 
             if (ptr2[c] % 2 == 0 ) {
               inizio = ptr2[c] * 16;
               incremento = 1;
             } else {
               inizio = ptr2[c] * 16 + 15;
               incremento = -1;
             }

             i = inizio + (c * incremento);
             byte ct = getStrip(ledstrip, i);
             if (ct != 0) { 
               strip.setPixelColor(i,Colore);
             } else {            
               strip.setPixelColor(i,Off);
             }
               
             strip.show();   
//             delay(1);

             ptr2[c]++; // Incrementa la riga e imposta il valore
                
             if (ptr2[c] < 16) { 
               if (ptr2[c] % 2 == 0) {
                 inizio = ptr2[c] * 16;
                 incremento = 1;
               } else {
                 inizio = ptr2[c] * 16 + 15;
                 incremento = -1;
               }

               i = inizio + (c * incremento);

               strip.setPixelColor(i,Green);
               strip.show(); 
             } else {
               if (getStrip(ledstrip,i) == 0) {
                 strip.setPixelColor(i,Off);
                 strip.show();
               }     
             }
          }
      }
  }
}


void SnakeEffect() {            
  uint16_t i, j;
  for(i=strip.numPixels()-1; i>0; i--) {
    strip.setPixelColor(i, Wheel((i) & 255));
    strip.show();
  }
  delay(50);
  for(i=0; i<strip.numPixels(); i++) {
    if (getStrip(ledstrip, i) != 0) { 
      strip.setPixelColor(i,Colore);
    } else {            
      strip.setPixelColor(i,Off);
    }
    strip.show();   
  }
}
