# Architecture du Système

## Vue d'ensemble

Le système est composé de quatre éléments principaux :

1. Un capteur ultrason HC-SR04
2. Un microcontrôleur ESP32-WROOM-32E
3. Un écran LCD I2C LCM1602
4. Un broker MQTT Mosquitto sur PC

Flux de données :

Capteur HC-SR04 --> ESP32 --> LCD I2C (affichage local)
                       |
                       +--> WiFi --> Broker MQTT --> Terminal PC

## Composants

### 1. Capteur ultrason HC-SR04

Rôle : mesure de distance par émission et réception d'ondes ultrasonores.

Principe :
- L'ESP32 envoie une impulsion de 10 microsecondes sur la broche Trig.
- Le capteur émet une salve d'ultrasons à 40 kHz.
- L'onde rebondit sur un obstacle et revient.
- Le capteur envoie un signal sur la broche Echo dont la durée
  correspond au temps aller-retour.
- Formule : distance (cm) = (durée en microsecondes / 2) / 29.1

Caractéristiques :
- Distance minimale : 2 cm
- Distance maximale : 400 cm
- Précision : environ 0.5 cm
- Alimentation : 5V
- Communication : 2 broches (Trig, Echo)

### 2. Microcontrôleur ESP32-WROOM-32E

Rôle : traitement des données et communication.

Caractéristiques :
- Processeur dual-core 240 MHz
- Mémoire flash : 4 Mo
- WiFi 2.4 GHz intégré
- Bluetooth intégré
- GPIO : 34 broches programmables

Broches utilisées :
| Broche ESP32 | Fonction        | Connecté à        |
|--------------|-----------------|-------------------|
| GPIO 18      | Sortie digitale | Trig du HC-SR04   |
| GPIO 19      | Entrée digitale | Echo du HC-SR04   |
| GPIO 21      | I2C SDA         | SDA du LCD I2C    |
| GPIO 22      | I2C SCL         | SCL du LCD I2C    |
| VIN (5V)     | Alimentation    | VCC capteur et LCD|
| GND          | Masse           | GND capteur et LCD|

### 3. Écran LCD I2C LCM1602

Rôle : affichage local des mesures.

Caractéristiques :
- Résolution : 16 caractères x 2 lignes
- Contrôleur : HD44780
- Module I2C : PCF8574
- Adresse I2C : 0x27
- Tension : 5V

Avantage du module I2C :
- Réduit le nombre de broches nécessaires de 6 à 2
- Communication via le bus I2C (SDA, SCL)

### 4. Broker MQTT Mosquitto

Rôle : réception, filtrage et redistribution des messages MQTT.

Caractéristiques :
- Version : 2.1.2
- Port : 1883 (non sécurisé)
- Protocole : MQTT 3.1.1
- Écoute sur : 0.0.0.0 (toutes les interfaces)

Configuration actuelle :
- allow_anonymous true
- Pas de chiffrement
- Pas d'authentification

## Protocole MQTT

### Principe publish/subscribe

MQTT utilise un modèle publish/subscribe avec un broker central.

Trois acteurs :
- Publisher : envoie des messages sur un topic (l'ESP32)
- Broker : reçoit et redistribue les messages (Mosquitto)
- Subscriber : s'abonne à un topic et reçoit les messages (terminal PC)

### Topics utilisés

| Topic                    | Direction | Description                |
|--------------------------|-----------|----------------------------|
| lampadaire/1/data        | ESP32 ->  | Données du capteur         |
| lampadaire/1/command     | -> ESP32  | Commandes vers l'ESP32     |

### Format des messages

Les messages sont au format JSON.

Exemple de données :
{
  "lampadaire": "1",
  "distance_cm": 45.3,
  "status": "OK"
}

Exemple de commande :
ON

### Niveaux de QoS

| QoS | Signification      | Utilisation dans le projet |
|-----|--------------------|----------------------------|
| 0   | Au plus une fois   | Oui (par défaut)           |
| 1   | Au moins une fois  | Non                        |
| 2   | Exactement une fois| Non                        |

## Flux complet des données

1. Mesure physique
   - L'ESP32 envoie une impulsion sur Trig
   - Le capteur émet des ultrasons
   - L'onde rebondit sur un obstacle
   - Le capteur envoie un signal sur Echo

2. Calcul
   - L'ESP32 mesure la durée du signal Echo
   - Calcule la distance : (durée / 2) / 29.1

3. Affichage local
   - L'ESP32 envoie la distance au LCD via I2C
   - Le LCD affiche la distance en cm

4. Préparation du message
   - L'ESP32 construit un payload JSON
   - Inclut l'identifiant, la distance et le statut

5. Publication MQTT
   - L'ESP32 publie sur le topic lampadaire/1/data
   - Le broker reçoit le message

6. Redistribution
   - Le broker envoie le message à tous les subscribers
   - Le terminal PC reçoit le message

7. Affichage distant
   - Le terminal affiche le topic et le payload

## Limites actuelles

- Pas de chiffrement des communications
- Pas d'authentification des clients
- Pas de contrôle d'accès aux topics
- Données visibles en clair sur le réseau

Ces limites seront adressées dans les prochaines étapes du projet.