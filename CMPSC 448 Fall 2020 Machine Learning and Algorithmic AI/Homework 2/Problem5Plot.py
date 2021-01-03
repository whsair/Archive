import random
import numpy as np
import matplotlib.pyplot as plt
from Problem5 import bgd_l2, sgd_l2


if __name__ == '__main__':
    # Put the code for the plots here, you can use different functions for each
    # part

    data = np.load('data.npy')
    x = data[:,0]
    y = data[:,1]
    x = np.c_[np.ones(x.shape[0]), x]
    w = np.array([0.0,0.0])

    # new_w, history_fw
    w1, h1 = bgd_l2(x.copy(), y.copy(), w.copy(), 0.05, 0.1, 0.001, 50)
    w2, h2 = bgd_l2(x.copy(), y.copy(), w.copy(), 0.10, 0.01, 0.001, 50)
    w3, h3 = bgd_l2(x.copy(), y.copy(), w.copy(), 0.10, 0, 0.001, 100)
    w4, h4 = bgd_l2(x.copy(), y.copy(), w.copy(), 0.10, 0, 0, 100)

    ws_1, hs1 = sgd_l2(x.copy(), y.copy(), w.copy(), 1, 0.1, 0.5, 800)
    ws_2, hs2 = sgd_l2(x.copy(), y.copy(), w.copy(), 1, 0.01, 0.1, 800)
    ws_3, hs3 = sgd_l2(x.copy(), y.copy(), w.copy(), 1, 0, 0, 40)
    ws_4, hs4 = sgd_l2(x.copy(), y.copy(), w.copy(), 1, 0, 0, 800)


    plt.figure(1)
    plt.subplot()
    print(h1)
    plt.plot(h1, linestyle='-', label = "1")
    plt.plot(h2, linestyle='--', label = "2")
    plt.plot(h3, linestyle='-.', label = "3")
    plt.plot(h4, linestyle=':', label = "4")
    plt.xlabel("Time Steps")
    plt.ylabel("Objective function f(w)")
    plt.title("Gradient Descent")
    plt.legend()
    #plt.show()
    #plt.savefig('hw2q501.pdf')

    print(hs4)
    plt.figure(2)
    plt.subplot()
    plt.plot(hs1, linestyle='-', label="1")
    plt.plot(hs2, linestyle='--', label="2")
    plt.plot(hs3, linestyle='-.', label="3")
    plt.plot(hs4, linestyle=':', label="4")
    plt.xlabel("Time Steps")
    plt.ylabel("Objective function f(w)")
    plt.title("Stochastic Gradient Descent (defalut mode)")
    plt.legend()

    plt.show()
    #plt.savefig('hw2q502.pdf')

    '''
    #additional test when i != -1
    ws_f, hsf = sgd_l2(x.copy(), y.copy(), 0, 1, 0.01, 0.1, 1, 2)
    print(hsf)
    '''
