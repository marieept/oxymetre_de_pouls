# Projet Oxymétrie de Pouls - Simulation et Traitement de Signal

Ce projet simule un oxymètre de pouls en traitant des signaux photopléthysmographiques (PPG) pour calculer la saturation en oxygène (SpO2) et la fréquence cardiaque. Le système applique des filtres numériques, extrait les caractéristiques des signaux, et affiche les résultats en temps réel.

## Membres
Marie EPINAT - Eloïse Grandjean

## Organisation du projet

```
Projet-Oxymetrie/
├── main_sim.c                    # Programme principal de simulation
├── affichage.c / affichage.h     # Écriture des résultats avec verrou
├── fir.c / fir.h                 # Filtre FIR passe-bande (51 coefficients)
├── iir.c / iir.h                 # Filtre IIR passe-haut (coeff. 0.992)
├── mesure.c / mesure.h           # Calcul SpO2 et fréquence cardiaque
├── fichiers.c / fichiers.h       # Gestion des fichiers de données
├── define.h                      # Structures absorp et oxy
├── autotests.h                   # Tests unitaires
├── log1.dat                      # Données de signal (entrée)
├── data.txt                      # Résultats SpO2/pouls (sortie)
└── README.md
```

## Fonctionnalités

### Filtrage numérique
* **Filtre FIR** : Passe-bande à 51 coefficients avec tampon circulaire
* **Filtre IIR** : Passe-haut du premier ordre pour suppression composante DC
* Traitement des signaux AC rouge (ACR) et infrarouge (ACIR)

### Mesure physiologique
* **SpO2** : Calcul via ratio RsIR = (ACR/DCR) / (ACIR/DCIR) avec conversion linéaire par morceaux
* **Fréquence cardiaque** : Détection de pics locaux (seuil 700) et conversion en BPM
* Fenêtre glissante de 1500 échantillons (3 secondes à 500 Hz)

### Affichage
* Écriture dans `data.txt` avec exclusion mutuelle (fichier verrou `.verrouData`)
* Protection contre accès concurrents multiples processus

### Chaîne de traitement
Lecture → Filtre FIR → Filtre IIR → Mesure → Affichage (cycle 10ms)

## Prérequis

```bash
# Compilation
gcc -o oxymetre main_sim.c affichage.c fir.c iir.c mesure.c fichiers.c -lm

# Bibliothèques : stdio.h, stdlib.h, unistd.h, math.h
```

## Utilisation

```bash
# 1. Placer log1.dat dans le répertoire
# 2. Exécuter
./oxymetre

# 3. Lire les résultats dans data.txt
# Format : SpO2 (ligne 1), pouls (ligne 2)
```

**Tests unitaires disponibles** :
* `firTest(filename)` : teste le filtre FIR
* `iirTest(filename)` : teste le filtre IIR
* `mesureTest(filename)` : teste les calculs de mesure

## Structures de données

```c
// Signaux d'absorption
typedef struct {
    float acr, dcr;   // Rouge (AC/DC)
    float acir, dcir; // Infrarouge (AC/DC)
} absorp;

// Résultats physiologiques
typedef struct {
    int spo2;   // Saturation O2 (%)
    int pouls;  // Fréquence cardiaque (BPM)
} oxy;
```

## Paramètres clés

* **Fréquence d'échantillonnage** : 500 Hz
* **Fenêtre de mesure** : 1500 échantillons (3 secondes)
* **Ordre filtre FIR** : 51 coefficients
* **Coefficient IIR** : 0.992
* **Seuil détection pics** : 700
* **Valeurs typiques** : SpO2 70-100%, pouls 40-180 BPM

## Résultats

* **data.txt** : mis à jour toutes les 10ms avec SpO2 et pouls
* **Synchronisation** : verrou automatique pour accès concurrent
* **Robustesse** : détection EOF et valeurs nulles