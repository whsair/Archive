import math
import random
import numpy as np


def g(w, x, y, delta):
    p = np.dot(w, x.T)

    if np.abs(y - p) < delta:
        return 0.0
    if y >= (p + delta):
        return np.square(y - p - delta)
    else:
        return np.square(y - p + delta)


def gradient_g(w, x, y, delta):
    p = np.dot(w, x.T)

    if np.abs(y - p) < delta:
        return 0.0
    if y >= (p + delta):
        return -2.0 * (y - p - delta)*x
    else:
        return -2.0 * (y - p + delta)*x


def f(x, y, w, delta, lamda):
    m = x.shape[0]
    s = 0.0
    for k in range(m):
        s += g(w, x[k], y[k], delta)
    return (1 / float(m)) * s + np.sum(np.square(w))


def gradient_f(x, y, w, delta, lamda):
    m = x.shape[0]
    s = np.array([0.0, 0.0])
    for k in range(m):
        s += gradient_g(w, x[k], y[k], delta)

    return (1 / float(m)) * s + 2 * lamda * w


def bgd_l2(data, y, w, eta, delta, lam, num_iter):
    history_fw = []
    new_w = w
    for i in range(num_iter):
        history_fw.append(f(data, y, new_w, delta, lam))
        new_w -= eta * gradient_f(data, y, new_w, delta, lam)
    return new_w, history_fw


def sgd_l2(data, y, w, eta, delta, lam, num_iter, i=-1):
    history_fw = []
    new_w = w
    m = data.shape[0]

    for j in range(1, num_iter+1):
        if i == -1:
            id = random.randint(0, m - 1)
        else:
            id = i
        new_w -= (1 / np.sqrt(j)) * gradient_f(np.array([data[id]]), np.array([y[id]]), new_w, delta, lam)
        history_fw.append(f(data, y, new_w, delta, lam))
    return new_w, history_fw
