import numpy as np

real_values = np.genfromtxt('RealValues.csv', delimiter=',')
view1_points = np.genfromtxt('View1XY.csv', delimiter=',')
view2_points = np.genfromtxt('View2XY.csv', delimiter=',')
k1 = np.genfromtxt('K1.csv', delimiter=',')
r1 = np.genfromtxt('R1.csv', delimiter=',')
t1 = np.genfromtxt('T1.csv', delimiter=',')[:, None]
k2 = np.genfromtxt('K2.csv', delimiter=',')
r2 = np.genfromtxt('R2.csv', delimiter=',')
t2 = np.genfromtxt('T2.csv', delimiter=',')[:, None]

print(f"real value shape: {real_values.shape}\n")
print(f"view1 points shape: {view1_points.shape}\n")
print(f"view2 points shape: {view2_points.shape}\n")
print(f"k1 shape: {k1.shape}\n")
print(f"r1 shape: {r1.shape}\n")
print(f"t1 shape: {t1.shape}\n")
print(f"k2 shape: {k2.shape}\n")
print(f"r2 shape: {r2.shape}\n")
print(f"t2 shape: {t2.shape}\n")

c1 = np.matmul(-r1.T, t1)
c2 = np.matmul(-r2.T, t2)

u1 = np.vstack([view1_points, [1] * view1_points.shape[1]])
u1 = np.matmul(np.linalg.inv(k1), u1)
u1 = np.matmul(r1.T, u1)
for i in range(u1.shape[1]):
    u1[:, i] = u1[:, i] / np.linalg.norm(u1[:, i])
u1_hat = u1

u2 = np.vstack([view2_points, [1] * view2_points.shape[1]])
u2 = np.matmul(np.linalg.inv(k2), u2)
u2 = np.matmul(r2.T, u2)
for i in range(u2.shape[1]):
    u2[:, i] = u2[:, i] / np.linalg.norm(u2[:, i])
u2_hat = u2

final_output = np.zeros([12, 3])
error = np.zeros([12, 3])
# print(final_output.shape[0])
for i in range(final_output.shape[0]):
    # print(u1_hat[:,i].reshape(-1,1).shape)
    # print(u2_hat[:,i].reshape(-1,1).shape)
    sum_i_xx1 = np.eye(3) - np.matmul(u1_hat[:, i].reshape(-1, 1), u1_hat[:, i].reshape(-1, 1).T)
    sum_i_xx2 = np.eye(3) - np.matmul(u2_hat[:, i].reshape(-1, 1), u2_hat[:, i].reshape(-1, 1).T)
    # print(np.matmul(u2_hat[:,i].reshape(-1,1),u2_hat[:,i].reshape(-1,1).T).shape)
    A = sum_i_xx1 + sum_i_xx2
    b = np.matmul(sum_i_xx1, c1) + np.matmul(sum_i_xx2, c2)

    final_output_i = np.linalg.solve(A, b)
    final_output[i,:] = final_output_i[:,0]
    error[i,:] = np.abs(final_output_i[:,0] - real_values[:, i])

print(f'real values: {real_values}\n\n\n')
print(f'Final output: {final_output}\n\n\n')
print(f'Error: {error}')
