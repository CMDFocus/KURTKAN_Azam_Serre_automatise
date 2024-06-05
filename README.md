# Serre automatisée
 Travail de Fin d'Étude 2023-2024 INRACI.

## Sommaire
- [Introduction](#introduction)
- [Caractéristiques](#caractéristiques)
- [Ordinogramme](#ordinogramme)
- [Schéma de principe](#schéma-de-principe)

## Introduction

Le projet a pour but de réaliser une serre automatisée, qui peut se gérer seule avec
des paramètres qu'on lui donne.

Mes grands-parents ont été agriculteurs/fermiers, et mes oncles sont toujours dans
l’agriculture pour la majorité. Ils travaillent dans de grands champs et récoltent
beaucoup de choses. J'ai eu l'idée de mon projet en les regardant. C'est grâce à eux
que j'ai construit ma propre serre.

Dans un premier temps, j’aimerais que l’utilisateur puisse configurer des valeurs
minimales et maximales pour la température, le taux d’humidité ainsi que le niveau
de lumière.

Puis j’aimerais pouvoir faire un autre mode ou on peut sélectionner des options
préréglées pour enregistrer et utiliser des paramètres pour des plantes en particulier.
Par exemple, pouvoir choisir “pomme de terre” et donc il n’y aura pas de réel réglage
à faire.


## Caractéristiques

**Général :**
- Écran texte 2 lignes 16 caractères.
- 3 Boutons-poussoir.

**Capteur :**
- Température : de -40°C à 85°C, précision de ±0,3°C.
- Humidité : de 0% à 100%, précision de ±2 %.
- Lumière : mesuré en tension entre 3000 mV et 100 mV (théorique :
entre 3.3V et 0V)

**Sortie :**
- 2 sorties relais 5 V, 6.7 μA (à l’unité).

**Ouverture de fenêtre :**
- Servomoteur en 3V3 et 90 mA.

**Électronique :**
- Alimentation : 230V AC => 5V DC, 800 mA (sans résistance chauffante)

**Mécanique :**
- Dimensions : 44.2 x 44.6 x 41 (L x l x H)
- Poids : 9.1 kg
- Matériaux :
  - MDF.
  - Plexiglass.


## Ordinogramme

![Diagramme sans nom drawio (14)](https://github.com/CMDFocus/KURTKAN_Azam_Serre_automatise/assets/58083906/cfaae11c-409e-47c1-9b63-a367ea257922)

## Schéma de principe

![Schematic_Serre-autonome_2024-06-03](https://github.com/CMDFocus/KURTKAN_Azam_Serre_automatise/assets/58083906/d61dbd43-000c-4fac-97f2-1b5c1a2c1afe)


## Source

[TerraDome](https://www.instructables.com/TerraDome-Mini-Tropical-Greenhouse-With-Arduino/)

> © INRACI
