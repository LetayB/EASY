#Letay Benoit
# -*- coding: utf-8 -*-
 
##################
#   LIBRAIRIES   #
##################
 
import numpy as np
import math
import random
from sys import argv
import matplotlib.pyplot as plt
import copy
import pprint
 
def gcd(x, y):
    while y != 0:
        (x, y) = (y, x % y)
    return x

def code_longueur_maximale(init,n,polynome):
	sequence = copy.copy(init)
	r = []
	z = 0
	for x in range(0,int(math.pow(2,n)-1)):
		somme = 0
		for y in range(len(polynome)):
			print sequence
			print sequence[polynome[y]-1]
			somme += sequence[polynome[y]-1]
		somme = somme % 2
		for y in reversed(range(1,n)):
			sequence[y] = sequence[y-1]
		sequence[0] = somme
		r.append(copy.copy(sequence))
	return r
 
def next_code_longueur_maximale(init,n,polynome):
	sequence = copy.copy(init)
	somme = 0
	for y in range(len(polynome)):
		somme += sequence[polynome[y]-1]
	somme = somme % 2
	for y in reversed(range(1,n)):
		sequence[y] = sequence[y-1]
	sequence[0] = somme
	return sequence
 
def next_code_gold(m1,m2):
 	r = []
	for x in range(0,len(m1)):
		r.append(int((m1[x] + m2[x])%2))
 	return r

 
def code_JPL(tab_m):
 	r = copy.copy(tab_m[0])
 	for x in range(1,len(tab_m)): #Pour chaque code a longueur maximal
		for i in range(0,len(tab_m)):
			r[x] += tab_m[x][i%len(tab_m[x])]
	for x in range(0,len(r)):
		r[x] %= 2
	return r
 

while 1 :
	choix = raw_input("Entrez le type de code à generer:\n\t1.Code a longueur maximale\n\t2.Code Gold\n\t3.Code JPL\n\t4.Quitter\n")
	if choix == '4' :
		break
	elif choix == '1' :
		l 			= int(raw_input("Entrez le nombre de bits :"))
		n 			= int(raw_input("Entrez la longueur de la sequence maximale de la generer :"))
		polynome 	= map(int, raw_input("Entrez le polynome de generation (forme : 5 2 pour un polynome[5,2]) :").split())
		init 		= map(int, raw_input("Entrez le vecteur d'initialisation (forme 1 1 1 1 pour [1,1,1,1] :").split())
		r = []
		while 1:
			l -= n
			if l < 0 :
				init = next_code_longueur_maximale(init,n,polynome)
				r.append(init[:n-int(math.fabs(l))])
				break
			else :
				init = next_code_longueur_maximale(init,n,polynome)
				r.append(init)
		print ''.join(str(x) for x in np.hstack(r))
	elif choix == '2' :
		l 			= int(raw_input("Entrez le nombre de bits :"))
		n 			= int(raw_input("Entrez la longueur de la sequence a generer :"))
		polynome1 	= map(int, raw_input("Entrez le premier polynome de generation (forme : 5 2 pour un polynome[5,2]) :").split())
		polynome2 	= map(int, raw_input("Entrez le second polynome de generation (forme : 5 2 pour un polynome[5,2]) :").split())
		init 		= map(int, raw_input("Entrez le vecteur d'initialisation (forme 1 1 1 1 pour [1,1,1,1] :").split())
		r = []
		m1 = next_code_longueur_maximale(init,n,polynome1)
		m2 = next_code_longueur_maximale(init,n,polynome2)
		while 1:
			l -= n
			if l < 0 :
				m1 = next_code_longueur_maximale(m1,n,polynome1)
				m2 = next_code_longueur_maximale(m2,n,polynome2)
				init = next_code_gold(m1,m2)
				r.append(init[:n-int(math.fabs(l))])
				break
			else :
				m1 = next_code_longueur_maximale(m1,n,polynome1)
				m2 = next_code_longueur_maximale(m2,n,polynome2)
				init = next_code_gold(m1,m2)
				r.append(init)
		print ''.join(str(x) for x in np.hstack(r))
	elif choix == '3' :
		l 			= int(raw_input("Entrez le nombre de bits :"))
		n 			= int(raw_input("Entrez le nombre de polynome :"))
		polynomes = [0]*n
		longueurs = [0]*n
		polynomes[0] = 1
		print "x"
		polynomes[1] = 2
		print "x"
		for x in range(n):
			longueurs[x] 	= int(raw_input("Entrez la longueur de la sequence :"))
			polynomes[x] 	= map(int, raw_input("Entrez le polynome de generation (forme : 5 2 pour un polynome[5,2]) :").split())
		codes = [0] * n
		r = []
		while 1:
			l -= n
			if l < 0 :
				for x in range(n):
					codes[x] = copy.copy(next_code_longueur_maximale([1]*longueurs[x],longueurs[x],polynomes[x]))
				tmp1 = code_JPL(copy.copy(codes))
				print tmp1
				r.append(tmp1[:n-int(math.fabs(l))])
				break
			else :
				for x in range(n):
					codes[x] = copy.copy(next_code_longueur_maximale([1]*longueurs[x],longueurs[x],polynomes[x]))
				tmp2 = code_JPL(copy.copy(codes))
				print tmp2
				r.append(tmp2)
		print r
		print np.ravel(r)
		print ''.join(str(x) for x in np.hstack(r))

