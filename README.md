# 🌡️ Bot Telegram IoT : Surveillance & Contrôle (ESP32)

## 📝 Description
Ce projet implémente un système IoT basé sur un microcontrôleur ESP32. Il permet de surveiller la température ambiante et de contrôler des actionneurs (LEDs) à distance via une interface conversationnelle utilisant l'API Telegram. 

Une alerte automatique est déclenchée sur le smartphone de l'utilisateur si la température dépasse un seuil critique (23°C).

## 🛠️ Matériel Utilisé
* **Microcontrôleur :** Carte de développement ESP32
* **Capteur :** Sonde de température DS18B20 (Protocole 1-Wire)
* **Actionneurs :** 2x LEDs (Rouge/Verte)
* **Passifs :** 2x Résistances 220 Ω (LEDs), 1x Résistance de tirage 4.7 kΩ (DS18B20)

## ⚙️ Fonctionnalités du Bot
Les commandes suivantes sont disponibles via l'application Telegram :
* `/start` : Affiche le menu d'accueil et les commandes disponibles.
* `/reading` : Interroge le capteur DS18B20 et renvoie la température actuelle en °C et °F.
* `/led1_on` / `/led1_off` : Contrôle l'état de la LED 1 (GPIO 13).
* `/led2_on` / `/led2_off` : Contrôle l'état de la LED 2 (GPIO 12).
* `/state` : Renvoie l'état actuel (ON/OFF) des deux LEDs.

## 🔌 Schéma Électrique
![Schéma de câblage Fritzing/Wokwi](https://github.com/AmineHajSaleh/projet-iot-telegram-esp32-ds18b20/blob/main/Screenshot%202026-05-14%20183111.png
)

## 🚀 Installation et Utilisation
1. Cloner ce dépôt :
   ```bash
   git clone [https://github.com/ton-pseudo/nom-du-repo.git](https://github.com/ton-pseudo/nom-du-repo.git)
