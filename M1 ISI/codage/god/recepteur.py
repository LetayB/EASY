#recepteur.py
from scipy.linalg import hadamard

class Recepteur:

	"""docstring for Recepteur"""

	def __init__(self, nbUsers):
		self.nbUsers = nbUsers
		self.matriceHadamard = hadamard(nbUsers)

	def decodageMessage(self, numUser, message):
		print message
		tailleBoutMessage = len(self.matriceHadamard[numUser-1])
		newMessage = [-2]*(len(message)/tailleBoutMessage)
		tableauTest = [-1]*len(self.matriceHadamard[numUser-1])
		# print ("tailleMessage =>")
		# print len(newMessage)
		j = 0
		for x in range(0,len(newMessage)):
			i = 0
			for y in self.matriceHadamard[numUser-1]:
				if message[j + i] == y:
					tableauTest[i] = 1
					pass
				elif message[j + i] == y * -1:
					tableauTest[i] = 0
					pass
				else:
					tableauTest[i] == -1
					pass
				i = i + 1
				pass
			j = j + i
			newMessage[x] = tableauTest[0]
			pass

		
		# print ("message code => ")
		# print message
		# print ("ligne matrice hadamard =>")
		# print self.matriceHadamard[numUser-1]
		# print ("message decode => ")
		# print newMessage
		return newMessage
		pass
