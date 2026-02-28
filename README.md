# Oraquadra2+
Le parole in tempo

## 📘 Descrizione

Un progetto per la comunità, per un simpatico orologio derivato dal progetto Word Clock, ma totalmente personalizzato e con tutto il necessario per poterselo replicare facilmente. Video tutorial qui:
https://youtu.be/ZyrPDAisFMc

questa è una evoluzione del progetto Oraquadra con le seguenti caratteristiche aggiunte:

* Nuovo quadrante con visualizzazione a 24 ore
* Connessione WiFi con ESP32
* Sincronizzazione automatic con un server NTP
* Form Factor più compatto
* Nuovi effetti tipo Matrix e tanti altri
* Controllo avanzato via WEB 

![389731360-8817e1d0-f8ae-471e-a67c-8fd8ebf861da](https://github.com/user-attachments/assets/98046c31-f3d6-46ae-81a4-cae0753d610b)

![EPXX-OraQuadra2plus](https://github.com/user-attachments/assets/e030988a-5eb3-4cf2-b682-1b09b61c3026)

![Cover](https://github.com/user-attachments/assets/fd213337-6217-4251-b41b-f0d67f0e2865)

![Cover2](https://github.com/user-attachments/assets/828c185f-c8fa-4d4c-9b91-5f06e091d789)

![WEB](https://github.com/user-attachments/assets/d72093b1-b926-4fad-a468-2cf7cf5aa3b5)


## 👨‍💻 Programmazione modulo

* Programmatore WEB (Solo per chrome): https://davidegatti.altervista.org/installaEsp32.php?progetto=oraQuadra2

---
## 🎛️ Schema pratico di assemblaggio

![Screenshot 2025-11-11 alle 22 35 37](https://github.com/user-attachments/assets/af88961f-5cc7-4927-bac8-cc82e0cce98b)


---
## 🎛️ Materiali

*  Matrice 16x16 RGB AMZ: https://amzn.to/4il5OQh
*  Matrice 16x16 RGB ALI: https://s.click.aliexpress.com/e/_DFRyDXJ
*  ESP32C3 AMZ: https://amzn.to/4kt5G2W
*  ESP32C3 ALI: https://s.click.aliexpress.com/e/_DDST7Bj
*  Pulsanti Capacitivi AMZ: https://amzn.to/438dXD7
*  Pulsanti Capacitivi ALI: https://s.click.aliexpress.com/e/_onoDssm
*  Link Affiliati

---
# 📝 Revisioni


V1.0.1 - 21/02/2025
*  Rimappatura nuovo quadrante e aggiunta gestione nuove WORD
*  Sistemazione troncature minuti quando è il primo minuto della decade   21 = VENT UN / 31 = TRENT UN   

V1.0.2 - 20/05/2025
*  aggiunta la possibilità di usare pulsanti normali oltre a quelli touch
*  BUTTON_LOGIC_INVERTED = 1 per pulsanti normali NA verso GND
*  BUTTON_LOGIC_INVERTED = 0 per pulsanti touch

V1.1.0 - 1/11/2025 by Marco Prunca
*  aggiunto effetto MOTO con scia arcobaleno 
*  MODIFICATO: la scia colora le scritte dell'orario man mano che passa
*  aggiunto effetto GALAGA - sparo dal basso che colpisce le lettere

V1.2.1 - 20/11/2025 - By Marco Prunca
*  aggiunto effetto PACMAN - Pacman che insegue fantasmi colorando le scritte
*  aggiunta interfaccia web per il controllo completo

V1.2.2 - 22/11/2025 - By Davide Gatti
*  aggiunto effetto DIGITALE - orologio digitale su schermo oraquadra
*  aggiunta modalità giorno/sera/notte per la regolazione della luminosità su tre fasce orarie
*  aggiunte scritte scorrevoli per informare l'utente sullo stato della connettività WIFI
*  sistemati alcuni bug relativi a vari effetti e memorizzazione parametri di configurazione
*  Sitemato grazie a Alessndro Spagnoletti e Paolo Sambinello l'effetto matrix che ora è corretto !!!!

V1.2.3 - 27/11/2025 - By Marco Prunca
* Sisteamti alcuni bug per la gestione WEB

V1.2.4 - 02/12/2025 - By Leotech e Paolo Sambinello
* Aggiunta funzione Testo scorrevole da web server
* Aggiunta possibilità di attivare/disattivare il quadrante digitale che si attiva ogni 6 sec per rendere visibile l'ora per postazioni più lontane. 
* L'attivazione/disattivazione del quadrante digitale che si alterna agli effetti selezionati può avvenire o da web server o mediante la pressione prolungata (6 sec) di uno dei 2 pulsanti. 
* Aggiunta palette di colori avanzata per selezionare molteplici più colori del testo scorrevole o delle parole di alcuni effetti.
* Aggiunti possibilità di selezionare varie dimensioni di font
* Aggiunto effetto rainbow
* Sistemati e migliorati effetti goccie a tron

V1.2.5 - 07/12/2025 - By Davide Gatti
* Gestione testo scorrevole come entità a parte e non come preset
* Aggiunta palette colori per il testo scorrevole
* Aggiunta modalità per mostrare ora in formato testuale
* Sistemata modalità notturna e luminosità giornaliera del testo scorrevole.

V1.2.6 - 10/12/2025 - By leotech
* Sistemato un problema che impediva la memorizzazione della velocità di scorrimento del testo con la data

V1.2.7 - 23/12/2025 - By Davide Gatti
* Sistemato un problema che a volte mostrava un ora sbagliata in quanto non conteggiava correttamente l'ora legale.

V1.2.8 - 28/02/2026 - By Davide Gatti
* Sistemato un bug che in assenza di testo scorrevole e di orologio dgitale, non faceva commutare i vari effetti al cambio minuto
* Sistemato un bug nell'effto moto dove in orari pieni tipo 22:00/12:00 dove non ci sono minuti, rimaneva accesa la E

---
# 📝 Revisioni Prunca Edition

Versione gestita da Marco Prunca derivata dalla versione 1.2.4 di oraquadra 2 originale.
Questa versione ha preso una strada differente rispetto al progetto originale e ha una serie di effetti aggiuntivi, e modalità operative del testo scorrevole differente
Oramai questa versione ha preso una sua strada e non è più integrabile al progetto originale, per cui viaggerà in modo separato.
Nella repository troverete sempre le versioni aggiornate dei due progetti e anche nella programmazione web

V1.2.6 - 22/12/2025 - By Marco Prunca 
* Aggiunti svariati effetti Natalizi e sistemate le funzioni di scroll testo ed effetti

---
## 🧾 Licenza

Questo progetto è distribuito con licenza
**Creative Commons – Attribuzione – Non Commerciale 4.0 Internazionale (CC BY-NC 4.0)**.

Puoi condividerlo e modificarlo liberamente, **citando l’autore**
(Davide Gatti / [survivalhacking](https://github.com/survivalhacking)) e **senza scopi commerciali**.

🔗 [https://creativecommons.org/licenses/by-nc/4.0/](https://creativecommons.org/licenses/by-nc/4.0/)

