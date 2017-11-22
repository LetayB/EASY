import math
class Hadamard:
    def getHadamard(n):
        if(n == 0):
            return 1
        elif (n==1):
            return[[1,1],[1,-1]]
        else:
            return np.asarray(np.concatenate((np.concatenate((hadamard(n-1),hadamard(n-1)),axis=1),np.concatenate((hadamard(n-1),np.dot(-1,hadamard(n-1))),axis=1)),axis=0))
    
    def __init__(self, nbUser):
        nb = (math.ceil(math.log(nbUser,2)))
        self.matriceHadamard = getHadamard(nb)
    
    def afficheToi():
        print(self.matriceHadamard)