import scipy as sp
import numpy as np
import pandas as pd
import matplotlib.pyplot as plt


# reference https://realpython.com/k-means-clustering-python/
# https://benalexkeen.com/k-means-clustering-in-python/

def nearest_distance(x, c):
    d_l = []
    for i in c:
        d = np.sqrt(np.sum((x - i) ** 2))
        d_l.append(d)
    return min(d_l)


def get_distance(a, b):
    # reference: wiki
    return np.sqrt(np.sum((a - b) ** 2))


def k_init(X, k):
    """ k-means++: initialization algorithm

    Parameters
    ----------
    X: array, shape(n ,d)
        Input array of n samples and d features

    k: int
        The number of clusters

    Returns
    -------
    init_centers: array (k, d)
        The initialize centers for kmeans++
    """

    init_centers = []

    first_point = np.random.randint(0, len(X))

    init_centers.append(X.iloc[first_point])

    for i in range(1, k):
        d_x_2 = []

        for j in range(X.shape[0]):
            d_x = nearest_distance(X.iloc[j], init_centers)
            d_x_2.append((d_x)**2)

        d_x_2 = d_x_2 / np.sum(d_x_2)  # http://essedunet.nsd.uib.no/cms/topics/weight/2/6.html

        next = np.random.choice(np.arange(0, X.shape[0]), p=d_x_2)
        init_centers.append(X.iloc[next])

    return init_centers


def k_means_pp(X, k, max_iter):
    """ k-means++ clustering algorithm

    step 1: call k_init() to initialize the centers
    step 2: iteratively refine the assignments

    Parameters
    ----------
    X: array, shape(n ,d)
        Input array of n samples and d features

    k: int
        The number of clusters

    max_iter: int
        Maximum number of iteration

    Returns
    -------
    final_centers: array, shape (k, d)
        The final cluster centers
    """

    centers = k_init(X, k)
    # reference: https://benalexkeen.com/k-means-clustering-in-python/

    for i in range(max_iter):
        d_map = assign_data2clusters(X, centers)
        for j in range(k):
            n = 0
            sum_x = 0
            sum_y = 0
            for k_i in range(len(d_map)):
                if max(d_map[k_i]) == j:
                    sum_x += X.iloc[k_i, 0]
                    sum_y += X.iloc[k_i, 1]
                    n += 1

            centers[j] = [sum_x / n, sum_y / n]
    return centers


def assign_data2clusters(X, C):
    """ Assignments of data to the clusters
    Parameters
    ----------
    X: array, shape(n ,d)
        Input array of n samples and d features

    C: array, shape(k ,d)
        The final cluster centers

    Returns
    -------
    data_map: array, shape(n, k)
        The binary matrix A which shows the assignments of data points (X) to
        the input centers (C).
    """

    d_map = []
    for i in range(len(X)):

        d = []
        t = [0] * len(C)

        for c in C:
            d.append(get_distance(X.iloc[i], c))

        min_id = d.index(min(d))
        t[min_id] = min_id
        d_map.append(t)

    return d_map


def compute_objective(X, C):
    """ Compute the clustering objective for X and C
    Parameters
    ----------
    X: array, shape(n ,d)
        Input array of n samples and d features

    C: array, shape(k ,d)
        The final cluster centers

    Returns
    -------
    accuracy: float
        The objective for the given assigments
    """
    # similar as mse

    acc = 0
    for x in range(len(X)):
        acc += (nearest_distance(X.iloc[x], C)) ** 2

    return acc


if __name__ == '__main__':
    df = pd.read_csv('iris.data')
    df.columns = ['sepal_length', 'sepal_width', 'petal_length', 'petal_width', 'class']

    c = []
    x1 = []
    x2 = []
    plt.figure()
    # 1
    for i in range(df.shape[0]):
        c.append([])
        x1.append([])
        x2.append([])
        c[i] = (df["class"][i])
        x1[i] = df["sepal_length"][i] / df["sepal_width"][i]  # calculate x_1
        x2[i] = df["petal_length"][i] / df["petal_width"][i]  # calculate x_2
        if c[i] == 'Iris-versicolor':
            plt.scatter(x1[i], x2[i], color="black")

        if c[i] == 'Iris-setosa':
            plt.scatter(x1[i], x2[i], color="brown")

        if c[i] == 'Iris-virginica':
            plt.scatter(x1[i], x2[i], color="grey")

    plt.title('problem 1 cluster')
    plt.show()

    # 2
    new_df = pd.DataFrame()
    new_df['x1'] = x1
    new_df['x2'] = x2
    new_df.head()

    accuracy = []
    for i in range(1, 6, 1):
        t = k_means_pp(new_df.copy(), i, 50)
        accuracy.append(compute_objective(new_df.copy(), t))

    plt.plot(range(1, 6, 1), accuracy)
    plt.title('accuracy / k value')
    plt.xlabel('k')
    plt.ylabel('acc')
    plt.show()
    # 3
    best_k = accuracy.index(min(accuracy)) + 1
    print(f"best_k = {best_k}")

    # 4 (1)
    accuracy_2 = []
    input = [25, 50, 100, 150]
    for i in input:
        accuracy_2.append(compute_objective(new_df.copy(), k_means_pp(new_df.copy(), best_k, i)))
    plt.plot(input, accuracy_2)
    plt.title('k mean++ / iterations')
    plt.xlabel('iterations')
    plt.ylabel('acc')
    plt.show()

    # 4 (2)

    # reference:
    # https://towardsdatascience.com/k-means-clustering-algorithm-applications-evaluation-methods-and-drawbacks-aa03e644b48a
    # https://stats.idre.ucla.edu/spss/faq/how-can-i-graph-two-or-more-groups-using-different-symbols/

    C = k_means_pp(new_df.copy(), best_k, 200)
    data_map = assign_data2clusters(new_df.copy(), C)
    label = []

    for i in range(len(data_map)):
        label.append(max(data_map[i]))

    plt.scatter(x1, x2, c=label)
    plt.title('k-means++ cluster')
    plt.xlabel('x1 (sepal_length/sepal_width)')
    plt.ylabel('x2 (petal_length/petal_width)')
    plt.show()
