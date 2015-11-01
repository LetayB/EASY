import numpy as np
from random import randint
from sklearn.decomposition import PCA
from sklearn.preprocessing import StandardScaler
import math

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

# VARIABLES
classes = [[0 for x in range(0, dimension_reducted)] for y in range(0, 10)]
numbersofimages = [0] * 10
matrices = [[0 for y in range(0, dimension_reducted)] for z in range(0, 10)]

for x in range(0, len(array)):
    curcl = int(arraycl[x])
    numbersofimages[curcl] += 1
    for y in range(0, dimension_reducted):
        classes[curcl][y] += array[x][y]

# MOYENNES
for x in range(0, 10):
    for y in range(0, dimension_reducted):
        classes[x][y] /= numbersofimages[int(arraycl[x])]

# MATRICE DE COVARIATION
groupes = [[] for x in range(0, 10)]
for x in range(0, len(array)):
    curcl = int(arraycl[x])
    groupes[curcl].append(array[x])

for x in range(0, 10):
    matrices[x] = np.cov(np.transpose(groupes[x]))

externecycle = 1
internecycle = len(array3eval)
#f = open('eval_cl.ascii', 'w')
v = [0] * externecycle
for ttt in range(0, externecycle):
    count = 0
    matricedeconfusion = [[] for ii in range(0, 10)]
    for tt in range(0, internecycle):
        index = randint(0, len(array2) - 1)
        test = array3eval[tt]
        answer = arraycl2[index]
        elems = [0] * 10
        for x in range(0, 10):
            X = test
            Y = classes[x]
            Z = X - Y  # vector hor
            Z1 = np.transpose(Z)  # vector vert
            Z2 = np.linalg.inv(matrices[x])  # matrice
            Z4 = np.dot(Z1, Z2)  # vector
            Z5 = np.dot(Z4, Z)  # valeur
            e = math.pow(math.e, -Z5 / 2)
            m1 = math.pow(2 * math.pi, float(dimension_reducted) / 2)
            m2 = math.pow(np.linalg.det(matrices[x]), (1.0 / 2))
            p = e / (m1 * m2)
            elems[x] = p
        maxim = elems[0]
        i = 0
        for x in range(0, 10):
            if elems[x] > maxim:
                maxim = elems[x]
                i = x
        # print(str(answer)+'est probablement '+str(i))
        #f.write(str(i)+'\n')
        matricedeconfusion[int(answer)].append(i)
        if answer == i:
            count += 1
    # print(count)
    v[ttt] = count
    print('pourcentage de vraisemblance:' + str(float(count) / internecycle))
    #f.close()
    # for ii in range(0, 10):
    #     print(matricedeconfusion[ii])
m = v[0]
mi = v[0]
for x in range(0, externecycle):
    if v[x] < mi:
        mi = v[x]
    if v[x] > m:
        m = v[x]
print('max:' + str(m))
print('min:' + str(mi))
