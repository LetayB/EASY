
# -*- coding: utf-8 -*-

#Import de librairies
import numpy as np
import math
from random import randint
from sklearn.decomposition import PCA
from sklearn.preprocessing import StandardScaler

#Ouverture des différents fichiers
f1 = open('appr_cl.ascii', 'r')
f2 = open('appr.ascii', 'r')
f3 = open('dev_cl.ascii', 'r')
f4 = open('dev.ascii', 'r')
f5eval = open('eval.ascii', 'r')

array = np.loadtxt(f2)
arraycl = np.loadtxt(f1)
array2 = np.loadtxt(f4)
arraycl2 = np.loadtxt(f3)
array3eval = np.loadtxt(f5eval)

#CONSTANTES
NB_CLASS = 10
N = 256
dimension_reducted = 10

# DIMINUTION DES DONNEES
array = StandardScaler().fit_transform(array)
pca = PCA(n_components=dimension_reducted)
array = pca.fit_transform(array)
array2 = StandardScaler().fit_transform(array2)
pca = PCA(n_components=dimension_reducted)
array2 = pca.fit_transform(array2)
array3eval = StandardScaler().fit_transform(array3eval)
pca = PCA(n_components=dimension_reducted)
array3eval = pca.fit_transform(array3eval)



#Variables
list_dev = [0] * NB_CLASS
list_freq = [0] * NB_CLASS
list_matrix = []
ordre_classe = []
float_image = [0.0] * 256

count = 0
i = 0
nb_values = 0.0

#On remplit le tableau list_dev du compte des différentes classes
#list_dev[0] contient le nombre de classe 0
with open('appr_cl.ascii', 'r') as file:
    for line in file:
        list_dev[int(line)] += 1	#On incremente la case du tableau correspondante
        ordre_classe.append(int(line))
        nb_values += 1	#On calcule le nombre de valeurs
        count += 1
print("Nombre de valeur par classes :")
print(list_dev)


count = 0
#Calcul de la fréquence de chaque classes
#list_freq[0] contient la frequence de classe 0
for i in list_dev:
	list_freq[count] = i/nb_values
	count += 1
print("Frequence par classe :")
print(list_freq)

list_pictures = [0] * int(nb_values)

#On remplit une liste d'image 
count = 0
with open('appr.ascii', 'r') as file:
    for line in file:
    	float_image = [float(x) for x in line.split()]
    	image = np.array(float_image)
    	list_pictures[count] = float_image
    	count += 1
        	

print("Première image :")
print(list_pictures[0])

list_pictures_by_class = [[] for x in range(	NB_CLASS) ] 

count = 0
#Trie les images par classes
for x in ordre_classe:
    list_pictures_by_class[x].append(list_pictures[count])
    count += 1

print("Première image de la classe 0 :")
print(list_pictures_by_class[0][1])

print("Première image de la classe 3 :")
print(list_pictures_by_class[3][1])

print("Mu : Vecteur moyenne")
temp = list_pictures_by_class[0]
mean9 =  np.mean(list_pictures_by_class[9], axis = 0)

print(len(temp))

print("Covariance :")
mat_cov9 = np.cov(temp, rowvar = 0)
print(mat_cov9)
print(np.shape(mat_cov9))
#print test
#print "Taille de ce vecteur"
#print test.size

#On remplit une liste d'image  dev

list_pictures_dev = [0] * int(nb_values)
count = 0
with open('dev.ascii', 'r') as file:
    for line in file:
        float_image = [float(x) for x in line.split()]
        image = np.array(float_image)
        list_pictures_dev[count] = float_image
        count += 1
