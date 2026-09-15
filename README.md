# Projet IoT - Lampadaire Intelligent

## Description

Système de collecte de données pour l'éclairage public intelligent.
Un capteur ultrason (HC-SR04) mesure une distance, un microcontrôleur
ESP32-WROOM-32E traite les données, les affiche sur un écran LCD I2C,
et les publie via le protocole MQTT sur un broker Mosquitto.

Ce projet est réalisé dans le cadre de la formation d'ingénieur
en IoT, Sécurité et Ville Intelligente à l'ESIR Rennes.

## Objectifs

- Lecture d'un capteur ultrason HC-SR04
- Affichage local sur LCD I2C (LCM1602)
- Connexion WiFi
- Communication MQTT (non sécurisée)
- Sécurisation TLS (à venir)
- Authentification par certificat mTLS (à venir)
- Déploiement d'une PKI (à venir)
- Simulation d'attaque par usurpation d'identité (à venir)
- Détection d'anomalies par IA (à venir)

## Architecture

Capteur HC-SR04 --> ESP32 --> WiFi --> Broker MQTT --> Terminal
                       |
                       +--> LCD I2C

## Matériel

| Composant        | Modèle          | Quantité |
|------------------|-----------------|----------|
| Microcontrôleur  | ESP32-WROOM-32E | 1        |
| Capteur          | HC-SR04         | 1        |
| Écran            | LCM1602 + I2C   | 1        |

## Logiciel

| Logiciel    | Version | Usage          |
|-------------|---------|----------------|
| Arduino IDE | 2.x     | Compilation    |
| Mosquitto   | 2.1.2   | Broker MQTT    |

## Structure du dépôt

iot-lampadaire-intelligent/
|
+-- README.md
+-- LICENSE
+-- .gitignore
|
+-- broker/
|   +-- mosquitto.conf
|
+-- docs/
|   +-- architecture.md
|   +-- securite.md
|   +-- images/
|
+-- firmware/
|   +-- esp32-mqtt-basic/
|       +-- esp32-mqtt-basic.ino
|
+-- scripts/
+-- rapport/

## Installation

Voir docs/installation.md (à venir).

## Résultats

| Métrique          | Valeur     |
|-------------------|------------|
| Distance min      | 2 cm       |
| Distance max      | 400 cm     |
| Précision         | +/- 0.5 cm |
| Fréquence d'envoi | 0.5 Hz     |
| Taille payload    | ~50 octets |

## Documentation

- Architecture : docs/architecture.md
- Sécurité : docs/securite.md

## Licence

MIT - voir LICENSE

## Auteur

babourwel2001-cpu
GitHub : https://github.com/babourwel2001-cpu