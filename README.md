# README - Gestion de comptes bancaires (Client-Serveur en C)

## Description du Projet
Ce projet implémente une application de gestion de comptes bancaires en utilisant une architecture client-serveur basée sur les sockets en C. Deux versions de communication sont disponibles :
- **TCP** : Communication avec connexion pour des interactions fiables.
- **UDP** : Communication sans connexion pour une plus grande efficacité.

Le serveur gère plusieurs comptes bancaires et traite les commandes envoyées par les clients via des sockets TCP ou UDP. Le client permet à un utilisateur d'interagir avec le serveur en envoyant des commandes pour consulter et manipuler les données des comptes.

---

## Fonctionnalités
### Commandes supportées :
1. **AJOUT <id_client> <id_compte> <password> <somme>** : Ajoute une somme au compte spécifié.
2. **RETRAIT <id_client> <id_compte> <password> <somme>** : Retire une somme du compte spécifié.
3. **SOLDE <id_client> <id_compte> <password>** : Affiche le solde actuel du compte.
4. **OPERATIONS <id_client> <id_compte> <password>** : Affiche les 10 dernières opérations effectuées sur le compte.

Le serveur répond aux commandes par :
- **OK** : Si l'opération a réussi.
- **KO** : Si l'opération a échoué (erreur d'identification, solde insuffisant, etc.).
- **RES_SOLDE** : Retourne le solde et la dernière date d'opération.
- **RES_OPERATIONS** : Retourne les 10 dernières opérations sous forme de texte structuré.

---

## Pré-requis
1. Un compilateur C comme **gcc**.
2. Une machine supportant les sockets réseau (Linux/macOS/Windows).
3. Les bibliothèques suivantes :
   - `pthread` pour Linux/macOS.
   - `winsock2` pour Windows.

---

## Compilation
### Avec Makefile :
1. Assurez-vous que les fichiers `serveur_tcp.c`, `client_tcp.c`, `serveur_udp.c`, `client_udp.c` et `Makefile` sont dans le même répertoire.
2. Exécutez la commande suivante :
   ```bash
   make
   ```
   Cela génère les fichiers exécutables `serveur_tcp`, `client_tcp`, `serveur_udp` et `client_udp`.

### Sans Makefile (manuellement) :
- **Pour TCP** :
  - Sous Linux/macOS :
    ```bash
    gcc -o serveur_tcp serveur_tcp.c -pthread
    gcc -o client_tcp client_tcp.c
    ```
  - Sous Windows :
    ```bash
    gcc -o serveur_tcp serveur_tcp.c -lws2_32
    gcc -o client_tcp client_tcp.c -lws2_32
    ```

- **Pour UDP** :
  - Sous Linux/macOS :
    ```bash
    gcc -o serveur_udp serveur_udp.c
    gcc -o client_udp client_udp.c
    ```
  - Sous Windows :
    ```bash
    gcc -o serveur_udp serveur_udp.c -lws2_32
    gcc -o client_udp client_udp.c -lws2_32
    ```

---

## Exécution
### Pour TCP :
1. **Démarrer le serveur :**
   - Sous Linux/macOS :
     ```bash
     ./serveur_tcp
     ```
   - Sous Windows :
     ```bash
     serveur_tcp.exe
     ```
   Le serveur commence à écouter les connexions sur le port 8080.

2. **Lancer le client :**
   - Ouvrez un autre terminal.
   - Sous Linux/macOS :
     ```bash
     ./client_tcp
     ```
   - Sous Windows :
     ```bash
     client_tcp.exe
     ```

3. **Interagir avec le serveur :**
   - Entrez les commandes au format décrit dans la section "Commandes supportées".

### Pour UDP :
1. **Démarrer le serveur :**
   - Sous Linux/macOS :
     ```bash
     ./serveur_udp
     ```
   - Sous Windows :
     ```bash
     serveur_udp.exe
     ```
   Le serveur commence à écouter les connexions sur le port 8081.

2. **Lancer le client :**
   - Ouvrez un autre terminal.
   - Sous Linux/macOS :
     ```bash
     ./client_udp
     ```
   - Sous Windows :
     ```bash
     client_udp.exe
     ```

3. **Interagir avec le serveur :**
   - Entrez les commandes au format décrit dans la section "Commandes supportées".

---

## Structure des fichiers
- **serveur_tcp.c** : Contient la logique serveur pour les connexions TCP.
- **client_tcp.c** : Implémente le client utilisant des sockets TCP.
- **serveur_udp.c** : Contient la logique serveur pour les connexions UDP.
- **client_udp.c** : Implémente le client utilisant des sockets UDP.
- **Makefile** : Automatisation de la compilation pour les quatre programmes.
- **README** : Instructions pour compiler et exécuter le projet.

---

## Notes importantes
1. Les identifiants des comptes et les mots de passe par défaut sont :
   - Compte 1 : ID = 1, Mot de passe = pass123
   - Compte 2 : ID = 2, Mot de passe = pass456
2. Le serveur UDP est sans connexion, donc plusieurs clients peuvent interagir simultanément sans ouvrir de session.
3. Testez le projet sur un environnement Linux/macOS pour une compatibilité optimale.

---

## Résultats des Tests
### Comparaison TCP et UDP
Voici une capture d'écran démontrant les différences entre TCP et UDP lors de l'exécution des commandes :

![Démonstration TCP et UDP](image)

**Observation :**
- **TCP** : Fiable, les messages arrivent dans l'ordre et sans perte.
- **UDP** : Plus rapide, mais des pertes ou des désordres peuvent survenir si le réseau est instable.

---

## Auteur
TOURE SEKOUBA
