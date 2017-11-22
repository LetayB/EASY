import numpy as np

class GenCodeLongueurMax:
    """description of class"""

    def xorLM(self,tabBit,listCoeff):
        newBit = tabBit[(listCoeff[0]-1)]
        for i in range(1,len(listCoeff)):
            tmp = tabBit[listCoeff[i]-1]
            newBit = (tmp+newBit)%2
        pass
        return newBit
    
    def decalage(self,tabBit, newBit):
        tmp = tabBit
        for i in range(len(tabBit)-1):
            tmp[len(tabBit)-i-1] = tabBit[len(tabBit)-i-2]
        tmp[0] = newBit
        pass
        return tmp

    def __init__(self, init, longueur, polynome):
        #init -> point de depart, [1]*longueur la premiere fois
        #longueur -> longueur qui vient du calcul de 2^n - 1
        #polynome -> tableau des bits a utiliser dans le xor (de la forme [3,1] par exemple si longueur 3, ou [5,2,1] si longueur 5 etc....)
        tmp = np.array(init).copy()
        for i in range(longueur):
            newBit = self.xorLM(tmp,polynome)
            tmp = self.decalage(tmp, newBit)
        self.codeLM = tmp