from __future__ import print_function
import numpy as np
from xlrd import open_workbook
from sklearn import svm

data = open_workbook("train_data.xlsx")
# train_data = open_workbook("output_with_timestamp.xls")
table = data.sheet_by_index(0)

hw6_table = open_workbook("output_with_timestamp.xls").sheet_by_index(0)

tmp_lst = []

# print(hw6_table.row_values(1))

for row in xrange(1, hw6_table.nrows):
    curList = hw6_table.row_values(row)[3:]
    curList2 = list()
    curList2.append(np.mean(curList[:3]))
    curList2.append(np.var(curList[:3]))
    curList2.append(curList[3])
    tmp_lst.append(curList2)

hw6_arr = np.array(tmp_lst)

# print(hw6_arr.shape)

# '''
row_length = table.nrows
# print(row_length) #4501
# print(row_length) #5330

win1, win2, win3 = 50, 100, 150  # window size, unit: NO.of data points, non-overlapping windows

lst = []

train_data_length = row_length - 1 - 1500  # 2/3 data, from 1 to 3000
# test_data_length = row_length - 1 - train_data_length  # 1/3 data from 3001 to 4500
test_data_length = 5329
for i in range(row_length - 1):
    lst.append(table.row_values(i + 1))
# print(table.row(5)[1]) # row 6, column 2

total_array = np.array(lst)  # convert list a to array b
# print(total_array.shape)
# size of b is 4500*21

cal_mat = np.eye(row_length - 1, 10)

for i in range(row_length - 1):
    cal_mat[i, 0] = np.sqrt(
        total_array[i, 1] * total_array[i, 1] + total_array[i, 2] * total_array[i, 2] + total_array[i, 3] * total_array[
            i, 3])
    cal_mat[i, 1] = total_array[i, 4]
    cal_mat[i, 2] = np.sqrt(
        total_array[i, 5] * total_array[i, 5] + total_array[i, 6] * total_array[i, 6] + total_array[i, 7] * total_array[
            i, 7])
    cal_mat[i, 3] = total_array[i, 8]
    cal_mat[i, 4] = np.sqrt(
        total_array[i, 9] * total_array[i, 9] + total_array[i, 10] * total_array[i, 10] + total_array[i, 11] *
        total_array[i, 11])
    cal_mat[i, 5] = total_array[i, 12]
    cal_mat[i, 6] = np.sqrt(
        total_array[i, 13] * total_array[i, 13] + total_array[i, 14] * total_array[i, 14] + total_array[i, 15] *
        total_array[i, 15])
    cal_mat[i, 7] = total_array[i, 16]
    cal_mat[i, 8] = np.sqrt(
        total_array[i, 17] * total_array[i, 17] + total_array[i, 18] * total_array[i, 18] + total_array[i, 19] *
        total_array[i, 19])
    cal_mat[i, 9] = total_array[i, 20]


def getErrorRate(win):
    quo_train = (train_data_length // win + 1 if train_data_length % win != 0 else train_data_length // win)
    d_train = np.eye(win, 1)
    e_train = np.eye(quo_train, 15)
    f_train = np.eye(quo_train * 5, 2)
    f_target = np.eye(quo_train * 5, 1)
    for i in range(quo_train):
        for k in range(5):
            for j in range(win):
                d_train[j, 0] = cal_mat[win * i + j, 2 * k]
                j = j + 1
            e_train[i, k * 3] = np.average(d_train)  # c[0,0] to c[200,0]
            e_train[i, k * 3 + 1] = np.var(d_train)
            e_train[i, k * 3 + 2] = k + 1

    for i in range(quo_train * 5):
        f_train[i, 0] = e_train[i % quo_train, i // quo_train * 3]
        f_train[i, 1] = e_train[i % quo_train, i // quo_train * 3 + 1]
        f_target[i, 0] = e_train[i % quo_train, i // quo_train * 3 + 2]

    quo_test = (test_data_length // win + 1 if test_data_length % win != 0 else test_data_length // win)
    d_test = np.eye(win, 1)
    e_test = np.eye(quo_test, 15)
    f_test = np.eye(quo_test * 5, 2)
    f_result = np.eye(quo_test * 5, 1)
    f_res = np.array(quo_test * 5)
    for i in range(quo_train, quo_train + quo_test + 1):
        for k in range(5):
            for j in range(win):
                d_test[j, 0] = cal_mat[win * (i - quo_train) + j, 2 * k]
                j = j + 1
            e_test[i - quo_train - 1, k * 3] = np.average(d_test)  # c[0,0] to c[200,0]
            e_test[i - quo_train - 1, k * 3 + 1] = np.var(d_test)
            e_test[i - quo_train - 1, k * 3 + 2] = k + 1

    for i in range(quo_test * 5):
        f_test[i, 0] = e_test[i % quo_test, i // quo_test * 3]
        f_test[i, 1] = e_test[i % quo_test, i // quo_test * 3 + 1]
        f_result[i, 0] = e_test[i % quo_test, i // quo_test * 3 + 2]

    f_res = f_result[:, 0]

    C = 1.0
    # X = [e[:,0],e[:,2],e[:,4],e[:,6],e[:,8]]
    X = f_train[:, :2]
    y = f_target
    clf = svm.SVC(decision_function_shape='ovo')
    svc = svm.LinearSVC(C=C).fit(X, y)  # svm.SVC(kernel='linear', C=C).fit(X, y)
    clf.fit(X, y)
    a = clf.predict(f_test[:, :2])
    error = np.count_nonzero(a - f_res) / len(f_res)
    print(error)


def get_err_rate(win):
    quo_train = (train_data_length // win + 1 if train_data_length % win != 0 else train_data_length // win)
    d_train = np.eye(win, 1)
    e_train = np.eye(quo_train, 15)
    f_train = np.eye(quo_train * 5, 2)
    f_target = np.eye(quo_train * 5, 1)
    for i in range(quo_train):
        for k in range(5):
            for j in range(win):
                d_train[j, 0] = cal_mat[win * i + j, 2 * k]
                j = j + 1
            e_train[i, k * 3] = np.average(d_train)  # c[0,0] to c[200,0]
            e_train[i, k * 3 + 1] = np.var(d_train)
            e_train[i, k * 3 + 2] = k + 1

    for i in range(quo_train * 5):
        f_train[i, 0] = e_train[i % quo_train, i // quo_train * 3]
        f_train[i, 1] = e_train[i % quo_train, i // quo_train * 3 + 1]
        f_target[i, 0] = e_train[i % quo_train, i // quo_train * 3 + 2]

    f_res = hw6_arr[:, 2]

    X = f_train[:, :2]
    y = f_target
    clf = svm.SVC(decision_function_shape='ovo')
    clf.fit(X, y)
    a = clf.predict(hw6_arr[:, :2])
    # print(a)
    error = np.count_nonzero(a - f_res) / len(f_res)
    print(error)


get_err_rate(win1)
get_err_rate(win2)
get_err_rate(win3)
