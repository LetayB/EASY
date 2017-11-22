#emetteur.py
from scipy.linalg import hadamard
class Emetteur:

	"""docstring for Emetteur : 
	- nbUsers : nombre d'utilisateurs
	- matriceHadamard : matrice hadamard correspondante
	"""

	def __init__(self, nbUsers):
		self.nbUsers = nbUsers
		self.matriceHadamard = hadamard(nbUsers)


	def addTab(self, tab1, tab2):
		newTab = [0] * len(tab1)
		print "plop"
		print tab1
		print tab2
		for x in xrange(0, len(tab1)):
			newTab[x] = tab1[x] + tab2[x]
			pass
		pass
		return newTab

	def codageMessage(self, numUser, message):
		tailleMessage = len(message)*len(self.matriceHadamard[numUser-1])
		newMessage = [-2]*tailleMessage

		i = 0
		for x in message: #pour chaque bit du message
			j = 0
			for y in self.matriceHadamard[numUser-1]:#pour chaque element de la ligne correspondante
				if x == 0:
					newMessage[j + i] = y * -1
					pass
				else:
					newMessage[j + i] = y
				j = j + 1
				pass
			i = i + j
			pass
		return newMessage
		pass

	def codageMessages(self, messages):
		numUser = 1 #le premier user a le numero 1
		newMessage = [0] * len(self.codageMessage(1,messages[0]))
		# print len(self.codageMessage(1,messages[0])), "|", self.codageMessage(1,messages[0])
		for x in messages:
			newMessage = self.addTab(newMessage, self.codageMessage(numUser,x))
			# print numUser, "|", newMessage
			numUser = numUser + 1
			pass
		pass
		#print "================"
		#print newMessage
		return newMessage