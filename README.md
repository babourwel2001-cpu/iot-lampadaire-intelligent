# Projet IoT - Lampadaire Intelligent

## À propos

Je suis Ibrahima BA, étudiant en 2ème année du cycle d'ingénieur
à l'ESIEN, spécialité IoT, Sécurité et Ville Intelligente.

Ce projet est un projet personnel que je mène en parallèle de ma
formation. L'objectif est de construire, étape par étape, un système
IoT complet pour l'éclairage public intelligent, en partant du capteur
jusqu'à la sécurisation par PKI.

Ce dépôt me sert de carnet de bord. J'y documente mon avancement,
mes choix techniques, mes erreurs et mes apprentissages. Il est
ouvert aux conseils, remarques et suggestions.

## Description du projet

Le système collecte des données de distance via un capteur ultrason
HC-SR04. Un microcontrôleur ESP32-WROOM-32E traite ces données, les
affiche localement sur un écran LCD I2C, et les publie via le protocole
MQTT sur un broker Mosquitto installé sur PC.

À terme, le projet intégrera une PKI complète (Root CA, Intermediate CA),
une authentification par certificat (mTLS), une simulation d'attaque par
usurpation d'identité, et une détection d'anomalies par IA.

## État d'avancement

- [x] Lecture d'un capteur ultrason HC-SR04
- [x] Affichage local sur LCD I2C (LCM1602)
- [x] Connexion WiFi
- [x] Communication MQTT (non sécurisée)
- [ ] Sécurisation TLS
- [ ] Authentification par certificat mTLS
- [ ] Déploiement d'une PKI
- [ ] Simulation d'attaque par usurpation d'identité
- [ ] Détection d'anomalies par IA

## Architecture actuelle

Capteur HC-SR04 --> ESP32 --> WiFi --> Broker MQTT --> Terminal PC
                       |
                       +--> LCD I2C

## Matériel utilisé

| Composant        | Modèle          | Quantité |
|------------------|-----------------|----------|
| Microcontrôleur  | ESP32-WROOM-32E | 1        |
| Capteur          | HC-SR04         | 1        |
| Écran            | LCM1602 + I2C   | 1        |

## Logiciel utilisé

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

## Résultats actuels

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

## Conseils et remarques

Ce projet est ouvert aux conseils. Si vous avez des suggestions
sur l'architecture, la sécurité, le code ou la documentation,
n'hésitez pas à ouvrir une issue ou à me contacter.

## Licence

MIT - voir LICENSE

## Contact

Ibrahima BA
Étudiant en 2ème année cycle ingénieur
ESIEN - Spécialité IoT, Sécurité et Ville Intelligente
GitHub : https://github.com/babourwel2001-cpu
