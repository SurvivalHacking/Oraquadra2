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

![Cover](https://github.com/user-attachments/assets/fd213337-6217-4251-b41b-f0d67f0e2865)

![Screenshot 2025-11-23 alle 22 28 35](https://github.com/user-attachments/assets/e43dcfa6-1aaa-45d2-9443-738955a1e8fd)

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

---
## 🧾 Licenza

Questo progetto è distribuito con licenza
**Creative Commons – Attribuzione – Non Commerciale 4.0 Internazionale (CC BY-NC 4.0)**.

Puoi condividerlo e modificarlo liberamente, **citando l’autore**
(Davide Gatti / [survivalhacking](https://github.com/survivalhacking)) e **senza scopi commerciali**.

🔗 [https://creativecommons.org/licenses/by-nc/4.0/](https://creativecommons.org/licenses/by-nc/4.0/)

