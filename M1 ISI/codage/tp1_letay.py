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

#################
#   Fonctions   #
#################
 
def canalParfait(message):
	print "|      Passage dans le canal parfait      |" 
	return message
 
def matrixHadamard(n):
	if n == 0:
		return  np.matrix([1])
	matrix =  matrixHadamard(n-1)
	return np.concatenate((np.concatenate((matrix,matrix),axis=1),np.concatenate((matrix,np.negative(matrix)),axis = 1)))
 
def matrixHadamardTerminal(n,matrix):
	if n == 0:
		return  matrix
	matrix = np.concatenate((np.concatenate((matrix,matrix),axis=1),np.concatenate((matrix,np.negative(matrix)),axis = 1)))
	return matrixHadamardTerminal(n-1,matrix)
 
def matrixHadamard2(n):
	return matrixHadamardTerminal(n,np.matrix([1]))
 
def codage(seq_id,message):
	coded_message = []
	for x in range(len(message)):	
		for y in range(len(seq_id)):
			coded_message.append( message[x] * seq_id[y] )
	return coded_message
 
def decodage(seq_id,message):
	decoded_message = []
	y = 0
	somme = 0.0
	for x in range(len(message)):
		somme += message[x] * seq_id[y]
		y += 1
		if y % (len(seq_id) ) == 0:
			decoded_message.append( int(somme/len(seq_id)))
			y = 0
			somme = 0.0
	return decoded_message
 
 
def generate_message(taille):
	message = []
	for x in range(0,taille):
		val = random.randint(0,1)
		if val == 0:
			message.append(-1)
		else:
			message.append(1)
	return message

def nb_bit_differents(t1,t2):
	r = 0
	for x in range(len(t1)):
		if t1[x] != t2[x]:
			r += 1
	return r
 
######################
#   Initialisation   #
######################
 
if len(argv) < 3:
	print "Erreur : nombre d'arguments invalide, Usage : <nb_users> <taille_messages>"
	exit(1)
 
NB_USERS 	= int(argv[1])
LEN_MSG 	= int(argv[2])
OFFSET		= 0
 
a_messagesInit 		= [[] for x in range(0,NB_USERS)]
a_messageSentCoded	= [[] for x in range(0,NB_USERS)]
a_messagesReceiv	= [[] for x in range(0,NB_USERS)]
 
 
n = math.ceil(math.log(float(NB_USERS),2)) # k < 2^n
 
m_had = matrixHadamard(n)
 
###############################
#   Generation des messages   #
###############################
 
for x in range(0,NB_USERS):
	a_messagesInit[x] = generate_message(LEN_MSG)
	print "Message " + str(x+1) + ":"
	print a_messagesInit[x]
 
 
##############
#   Codage   #
##############
 
c_msg_f = [0] * LEN_MSG * len(m_had)
 
for x in range(0,NB_USERS):
	cle_tmp = np.squeeze(np.asarray(m_had[x+OFFSET%len(m_had)]))
	a_messageSentCoded[x] = codage(cle_tmp,a_messagesInit[x])
	c_msg_f = np.add(c_msg_f,a_messageSentCoded[x])

 
print "==========================================="
msg_retour = canalParfait(c_msg_f)
print "==========================================="
print c_msg_f
 
 
 
print "Apres passage dans le canal : " 
print msg_retour
 
print "Nombre de bits contenant une erreur après passage dans le canal :"
print nb_bit_differents(c_msg_f,msg_retour)
 
################
#   Decodage   #
################
bits_erreurs = 0.0
 
for x in range(0,NB_USERS):
	cle_tmp = np.squeeze(np.asarray(m_had[x+OFFSET%len(m_had)]))
	a_messagesReceiv[x] = decodage(cle_tmp,c_msg_f)
	print "Message " + str(x+1) + " :"
	print a_messagesReceiv[x]
	bits_erreurs += nb_bit_differents(a_messagesReceiv[x],a_messagesInit[x])

print "Nombre de bits contenant une erreur après décodage :"
print int(bits_erreurs)

