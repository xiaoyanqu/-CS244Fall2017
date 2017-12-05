import numpy as np
from sklearn import svm

"""XQ Dataset"""
# A = np.genfromtxt("data_standing.txt", delimiter=' ')
# B = np.genfromtxt("data_sitting.txt", delimiter=' ')
# C = np.genfromtxt("data_sleeping.txt", delimiter=' ')
# D = np.genfromtxt("data_walking.txt", delimiter=' ')

A = np.genfromtxt("standing.txt", delimiter=' ')
B = np.genfromtxt("sitting.txt", delimiter=' ')
C = np.genfromtxt("sleeping.txt", delimiter=' ')
D = np.genfromtxt("walking.txt", delimiter=' ')

data_standing = np.zeros((12000, 3))
data_sitting = np.zeros((12000, 3))
data_sleeping = np.zeros((12000, 3))
data_walking = np.zeros((12000, 3))

counter = 0

while counter < 12000:
    data_standing[counter][0] = np.mean(A[counter:counter + 100])
    data_standing[counter][1] = np.var(A[counter:counter + 100])
    data_standing[counter][2] = 3  # change this for different activity
    counter += 1

counter = 0

while counter < 12000:
    data_sitting[counter][0] = np.mean(B[counter:counter + 100])
    data_sitting[counter][1] = np.var(B[counter:counter + 100])
    data_sitting[counter][2] = 1  # change this for different activity
    counter += 1

counter = 0

while counter < 12000:
    data_sleeping[counter][0] = np.mean(C[counter:counter + 100])
    data_sleeping[counter][1] = np.var(C[counter:counter + 100])
    data_sleeping[counter][2] = 2  # change this for different activity
    counter += 1

counter = 0

while counter < 12000:
    data_walking[counter][0] = np.mean(D[counter:counter + 100])
    data_walking[counter][1] = np.var(D[counter:counter + 100])
    data_walking[counter][2] = 4  # change this for different activity
    counter += 1

Xtr = np.zeros((32000, 2))
Ytr = np.zeros(32000)
Xva = np.zeros((16000, 2))
Yva = np.zeros(16000)

for i in range(8000):
    j = i * 4
    Xtr[j][0] = data_standing[i][0]
    Xtr[j][1] = data_standing[i][1]

    Xtr[j + 1][0] = data_sitting[i][0]
    Xtr[j + 1][1] = data_sitting[i][1]

    Xtr[j + 2][0] = data_sleeping[i][0]
    Xtr[j + 2][1] = data_sleeping[i][1]

    Xtr[j + 3][0] = data_walking[i][0]
    Xtr[j + 3][1] = data_walking[i][1]

    Ytr[j] = data_standing[i][2]
    Ytr[j + 1] = data_sitting[i][2]
    Ytr[j + 2] = data_sleeping[i][2]
    Ytr[j + 3] = data_walking[i][2]

p = 0

for i in range(8000, 12000):
    Xva[p][0] = data_standing[i][0]
    Xva[p][1] = data_standing[i][1]

    Xva[p + 1][0] = data_sitting[i][0]
    Xva[p + 1][1] = data_sitting[i][1]

    Xva[p + 2][0] = data_sleeping[i][0]
    Xva[p + 2][1] = data_sleeping[i][1]

    Xva[p + 3][0] = data_walking[i][0]
    Xva[p + 3][1] = data_walking[i][1]

    Yva[p] = data_standing[i][2]
    Yva[p + 1] = data_sitting[i][2]
    Yva[p + 2] = data_sleeping[i][2]
    Yva[p + 3] = data_walking[i][2]

    p += 4

C = 1.0
clf = svm.SVC(decision_function_shape='ovo')
# svc = svm.LinearSVC(C=C).fit(Xtr, Ytr)
clf.fit(Xtr, Ytr)
res = clf.predict(Xva)

error = np.count_nonzero(res - Yva) / 16000
print error
