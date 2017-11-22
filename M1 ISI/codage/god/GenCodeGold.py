from genCodeLongueurMax import GenCodeLongueurMax

class GenCodeGold(object):
    """description of class"""

    def xorGold(self,tabBit1,tabBit2): #2 tableaux de meme longueurs
        tmp = tabBit1
        for i in range(len(tabBit1)):
            tmp[i] = (tabBit1[i]+tabBit2[i])%2
        return tmp

    def __init__(self, longueur, polynome):
        cLM1 = GenCodeLongueurMax([1,1,1,1,1],longueur,polynome)
        tabBit1 = cLM1.codeLM
        cLM2 = GenCodeLongueurMax(tabBit1,longueur,polynome)
        tabBit2 = cLM2.codeLM
        self.codeGold = self.xorGold(tabBit1,tabBit2)
