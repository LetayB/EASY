#from emetteur import Emetteur
#from canalIdeal import CanalIdeal
#from recepteur import Recepteur

#def stringToBinary(st):
#	stBin = ' '.join(format(ord(x), 'b') for x in st)
#	stBin = stBin.replace(" ","")
#	stBin = list(stBin)
#	for x in range(0,len(stBin)):
#		stBin[x]=int(stBin[x])
#		pass
#	return stBin
#	pass



#emetteur1 = Emetteur(4)
#canalIdeal = CanalIdeal()
#recepteur1 = Recepteur(4)

#st = "schmilblikS"
#stBin = stringToBinary(st)

#messageOriginal = [1,0,0,0]
## messageOriginal = stBin

#messageCode = emetteur1.codageMessage(1,messageOriginal)
#newMessageCode = canalIdeal.transfert(messageCode)
#messageDecode = recepteur1.decodageMessage(1,newMessageCode)

#print ("message original puis message decode")
#print messageOriginal
#print messageDecode

#print ("test")
#emetteur1.codageMessages([[1,0,0,0],[1,0,1,0]])