from genCodeLongueurMax import GenCodeLongueurMax
import numpy as np

class genCodeJPL:
    """description of class"""

    #def xorGold(self,tabBit1,tabBit2): #2 tableaux de meme longueurs
    #    tmp = tabBit1
    #    for i in range(len(tabBit1)):
    #        tmp[i] = (tabBit1[i]+tabBit2[i])%2
    #    return tmp

    def __init__(self, nbCodeLM, longueur, polynome):
        tmp = [1]*polynome[0]
        tabCodeLM = [[]*polynome[0]]*nbCodeLM
        for i in range(nbCodeLM):
            print i
            tabCodeLM[i] = GenCodeLongueurMax(tmp,longueur,polynome).codeLM
            tmp = np.array(tabCodeLM[i]).copy()
            self.codeJPL = tmp

            #finir code JPL
            #polynome donnés par l'user [tableau de polynome]