# word-frequency-analyzer
Analyse de fréquence de mots en C avec une structure hybride ABR et arbre tournoi.

# Projet ABRnois — Arbres Binaires de Recherche avec Priorité

Ce projet explore une structure de données hybride appelée **ABRnois**, à l'intersection des **arbres binaires de recherche** (ABR) et des **arbres tournois**. 

##  Objectifs du projet

- Stocker et organiser efficacement les mots les plus fréquents issus d’un ou plusieurs corpus textuels.
- Associer à chaque mot un compteur d’occurrences (priorité).
- Permettre :
  - une **insertion rapide** selon l’ordre lexicographique,
  - une **remontée par priorité** (grâce à des rotations),
  - une **extraction ordonnée** des mots les plus fréquents.

Ce projet a pour but final de reconstituer automatiquement un fichier `frequents.txt` contenant les mots les plus utilisés, accompagnés de leur pourcentage de fréquence.

##  Fonctionnalités principales

- Insertion d’un mot dans un ABRnois avec mise à jour des priorités.
- Suppression du mot ayant la priorité maximale.
- Génération graphique de l’arbre (optionnelle, en PDF).
- Export d’un fichier texte listant les mots les plus fréquents d’un ou plusieurs corpus.
- Support d’options avancées :
  - `-g` : génération de fichiers PDF pour visualiser les insertions et suppressions.
  - `-n <nombre>` : limitation au `n` mots les plus fréquents.

##  Technologies et concepts utilisés

###  Langage :
- **C** (structuration mémoire bas-niveau, gestion dynamique)

###  Notions abordées :
- Structures de données avancées :
  - Arbres binaires de recherche (ABR)
  - Arbres tournois
  - Rotations gauche/droite
- Algorithmes d’insertion/suppression avec propriétés mixtes
- Gestion de fichiers et traitement de texte
- Génération dynamique de visualisations (PDFs d’arbres)
- Listes chaînées triées
- Manipulation d’arguments en ligne de commande (`argc`, `argv`)

##  Structure du projet

- `abrnois.c` : Fichier principal contenant toutes les structures et fonctions.
- `frequents.txt` : Fichier de sortie généré.
- `corpus_1.txt`, `corpus_2.txt`, ... : Corpus textuels à analyser.
- `insertion_X.pdf`, `suppression_X.pdf` : Visualisations optionnelles des arbres.

##  Exemple d'utilisation

```bash
# Compilation
gcc -Wall abrnois.c -o abrnois
ou
clang -std=c17 -Wall -Wfatal-errors abrnois.c -o abrnois

# Exécution simple
./abrnois frequents.txt corpus_test1.txt corpus_test2.txt

# Avec visualisation des étapes
./abrnois frequents.txt corpus_test1.txt -g

# Extraire uniquement les 1500 mots les plus fréquents
./abrnois -n 1500 frequents.txt corpus_1.txt


