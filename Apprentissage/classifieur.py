
# -*- coding: utf-8 -*-

##################
#   LIBRAIRIES   #
##################

import numpy as np
import math 
from random import randint
from sklearn.decomposition import PCA
from sklearn.preprocessing import StandardScaler

##################
#   CONSTANTES   #
##################

#Nombre de classes
NB_CLASS = 10
#Dimension choisie pour la projection
DIMENSION = 9

################
#   FICHIERS   #
################

#Ouvertures des différents fichiers
#Et chargement des données provenant des fichiers dans des tableaux
#Corpus d'apprentissage
f = open('appr.ascii', 'r')
array_appr = np.loadtxt(f)
#Liste des classes du corpus d'apprentissage
f = open('appr_cl.ascii', 'r')
arraycl = np.loadtxt(f)
#Corpus de développement
f = open('dev.ascii', 'r')
array_dev = np.loadtxt(f)
#Liste des classes du corpus de développement
f = open('dev_cl.ascii', 'r')
array_dev_cl = np.loadtxt(f)
#COrpus d'évaluation
f = open('eval.ascii', 'r')
array_eval = np.loadtxt(f)

#Fermeture du fichier
f.close()

########################################
#   Analyse en Composante Principale   #
########################################

array_appr = StandardScaler().fit_transform(array_appr)
pca = PCA(n_components=DIMENSION)
array_appr = pca.fit_transform(array_appr)
array_dev = StandardScaler().fit_transform(array_dev)
pca = PCA(n_components=DIMENSION)
array_dev = pca.fit_transform(array_dev)
array_eval = StandardScaler().fit_transform(array_eval)
pca = PCA(n_components=DIMENSION)
array_eval = pca.fit_transform(array_eval)

################################
#   Declaration de tableaux    #
################################

#array_meanByClass contient la moyenne des valeurs pour les différentes classes
array_meanByClass = [[0 for x in range(0, DIMENSION)] for y in range(0, NB_CLASS)]
#array_nbImageByClass contient les images triées par classes
#list_dev[0] contient les images de la classe 0
array_imageByClass = [[] for x in range(0, NB_CLASS)]

array_matricesByClass = [[0 for y in range(0, DIMENSION)] for z in range(0, NB_CLASS)]

##############################
#   Phase d'apprentissage    #
##############################


for x in range(0, len(array_appr)):
    #On traite une nouvelle ligne 
    tmp_class = int(arraycl[x])
    #On range les images par classes
    array_imageByClass[tmp_class].append(array_appr[x])
    #On ajoute la valeur de chaque image à la somme pour calculer la moeynne
    for y in range(0, DIMENSION):
        array_meanByClass[tmp_class][y] += array_appr[x][y]

#Calcul des moyennes par classes
for x in range(0, NB_CLASS):
    #Division de la somme des valeurs par le nombre d'images
    for y in range(0, DIMENSION):
        array_meanByClass[x][y] /= len(array_imageByClass[int(arraycl[x])])

  
#On calcule chaque matrice de covariance par classe
for x in range(0, NB_CLASS):
    array_matricesByClass[x] = np.cov(np.transpose(array_imageByClass[x]))


externecycle = 1
internecycle = len(array_eval)
#f = open('eval_cl.ascii', 'w')
v = [0] * externecycle
for ttt in range(0, externecycle):
    count = 0
    matricedeconfusion = [[] for ii in range(0, NB_CLASS)]
    for tt in range(0, internecycle):
        index = randint(0, len(array_dev) - 1)
        test = array_eval[tt]
        answer = array_dev_cl[index]
        elems = [0] * NB_CLASS
        for x in range(0, NB_CLASS):
            X = test
            Y = array_meanByClass[x]
            Z = X - Y  # vector hor
            Z1 = np.transpose(Z)  # vector vert
            Z2 = np.linalg.inv(array_matricesByClass[x])  # matrice
            Z4 = np.dot(Z1, Z2)  # vector
            Z5 = np.dot(Z4, Z)  # valeur
            e = math.pow(math.e, -Z5 / 2)
            m1 = math.pow(2 * math.pi, float(DIMENSION) / 2)
            m2 = math.pow(np.linalg.det(array_matricesByClass[x]), (1.0 / 2))
            p = e / (m1 * m2)
            elems[x] = p
        maxim = elems[0]
        i = 0
        for x in range(0, NB_CLASS):
            if elems[x] > maxim:
                maxim = elems[x]
                i = x
        # print(str(answer)+'est probablement '+str(i))
        #f.write(str(i)+'\n')
        matricedeconfusion[int(answer)].append(i)
        if answer == i:
            count += 1
    # print(count)
    v[ttt] = count
    print('pourcentage de vraisemblance:' + str(float(count) / internecycle))
    #for ii in range(0, 10):
    #    print(matricedeconfusion[ii])
m = v[0]
mi = v[0]
for x in range(0, externecycle):
    if v[x] < mi:
        mi = v[x]
    if v[x] > m:
        m = v[x]
print('max:' + str(m))
print('min:' + str(mi))



