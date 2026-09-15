# Analyse de Sécurité

## État actuel du système

Le système actuel est fonctionnel mais non sécurisé. Il utilise MQTT
sur le port 1883 sans chiffrement ni authentification.

## Vulnérabilités identifiées

| Numéro | Vulnérabilité                | Risque          | Impact   |
|--------|------------------------------|-----------------|----------|
| 1      | Absence de chiffrement       | Interception    | Élevé    |
| 2      | Absence d'authentification   | Usurpation      | Critique |
| 3      | Port 1883 en clair           | Sniffing        | Élevé    |
| 4      | allow_anonymous true         | Accès libre     | Critique |
| 5      | Absence de contrôle d'accès  | Publication libre| Élevé   |

## Détail des vulnérabilités

### Vulnérabilité 1 : Absence de chiffrement

Description :
Les messages MQTT circulent en clair sur le réseau WiFi. Un attaquant
positionné sur le même réseau peut capturer les données.

Scénario d'attaque :
Un attaquant se connecte au même point d'accès WiFi que l'ESP32.
Il utilise un outil comme Wireshark pour capturer le trafic réseau.
Il peut lire toutes les distances mesurées par le capteur.

Impact :
- Fuite d'informations sur l'activité du lampadaire
- Possibilité de cartographier les déplacements
- Violation de la vie privée

### Vulnérabilité 2 : Absence d'authentification

Description :
N'importe quel client peut se connecter au broker MQTT sans fournir
d'identifiants. Le broker accepte toutes les connexions.

Scénario d'attaque :
Un attaquant se connecte au broker sur le port 1883.
Il publie de fausses données sur le topic lampadaire/1/data.
Le système de supervision reçoit ces fausses données et les traite
comme légitimes.

Impact :
- Injection de fausses données
- Décisions erronées du système central
- Perte de confiance dans le système

### Vulnérabilité 3 : Port 1883 en clair

Description :
Le port 1883 est le port MQTT standard non chiffré. Toutes les
communications sont en texte brut.

Scénario d'attaque :
Un attaquant capture le trafic avec tcpdump ou Wireshark.
Il peut reconstituer les messages MQTT et extraire les payloads JSON.

Impact :
- Lecture des données en clair
- Possibilité de rejouer les messages (replay attack)

### Vulnérabilité 4 : allow_anonymous true

Description :
La configuration Mosquitto autorise les connexions anonymes.
Aucun identifiant n'est requis.

Scénario d'attaque :
Un attaquant découvre l'adresse IP du broker (par scan réseau).
Il s'y connecte directement et publie ou s'abonne à tous les topics.

Impact :
- Accès total au broker
- Publication de fausses données
- Saturation du broker (déni de service)

### Vulnérabilité 5 : Absence de contrôle d'accès

Description :
Il n'y a pas d'ACL (Access Control List). Tous les clients peuvent
publier et s'abonner à tous les topics.

Scénario d'attaque :
Un attaquant s'abonne au topic lampadaire/1/command et envoie
des commandes malveillantes (extinction, allumage intempestif).

Impact :
- Contrôle non autorisé du système
- Perturbation du service

## Contre-mesures prévues

### Contre-mesure 1 : Chiffrement TLS

Objectif :
Chiffrer toutes les communications MQTT.

Mise en oeuvre :
- Passage du port 1883 au port 8883
- Génération de certificats X.509 avec OpenSSL
- Configuration de Mosquitto pour utiliser TLS
- Configuration de l'ESP32 pour utiliser TLS

Bénéfices :
- Confidentialité des données
- Protection contre l'interception
- Protection contre le replay

### Contre-mesure 2 : Authentification par certificat (mTLS)

Objectif :
Vérifier l'identité de chaque client.

Mise en oeuvre :
- Génération d'un certificat unique par ESP32
- Configuration du broker pour exiger un certificat client
- Configuration de l'ESP32 pour présenter son certificat

Bénéfices :
- Seuls les clients autorisés peuvent se connecter
- Impossible de se faire passer pour un autre client
- Traçabilité des connexions

### Contre-mesure 3 : Déploiement d'une PKI

Objectif :
Gérer le cycle de vie des certificats.

Architecture prévue :
- Root CA (autorité de certification racine, hors ligne)
- Intermediate CA (autorité intermédiaire, signe les certificats)
- Certificats serveur (pour le broker MQTT)
- Certificats clients (pour les ESP32)

Bénéfices :
- Gestion centralisée des certificats
- Révocation possible en cas de compromission
- Scalabilité (ajout de nouveaux appareils)

### Contre-mesure 4 : Contrôle d'accès (ACL)

Objectif :
Restreindre les droits de chaque client.

Mise en oeuvre :
- Fichier ACL dans Mosquitto
- Chaque ESP32 ne peut publier que sur son propre topic
- Chaque ESP32 ne peut s'abonner qu'à son topic de commande

Bénéfices :
- Principe du moindre privilège
- Limitation de l'impact en cas de compromission

### Contre-mesure 5 : Détection d'anomalies par IA

Objectif :
Détecter les comportements anormaux en temps réel.

Mise en oeuvre :
- Collecte de données normales pendant une période d'apprentissage
- Entraînement d'un modèle de machine learning
- Déploiement du modèle sur l'ESP32 (TensorFlow Lite Micro)
- Détection des anomalies et alerte

Bénéfices :
- Détection d'attaques inconnues
- Réaction en temps réel
- Complément aux autres mesures

## Plan de sécurisation

| Étape | Action                              | Statut      |
|-------|-------------------------------------|-------------|
| 1     | Déploiement PKI (Root CA, Inter CA) | À faire     |
| 2     | Génération des certificats          | À faire     |
| 3     | Configuration TLS sur Mosquitto     | À faire     |
| 4     | Configuration mTLS sur Mosquitto    | À faire     |
| 5     | Intégration certificats dans ESP32  | À faire     |
| 6     | Mise en place des ACL               | À faire     |
| 7     | Test de l'usurpation d'identité     | À faire     |
| 8     | Détection d'anomalies par IA        | À faire     |

## Conclusion

Le système actuel est fonctionnel mais présente des vulnérabilités
critiques. La sécurisation par TLS, mTLS, PKI et ACL est indispensable
pour un déploiement en environnement réel.

L'ajout d'une couche de détection d'anomalies par IA permettra de
détecter les attaques inconnues et de réagir en temps réel.