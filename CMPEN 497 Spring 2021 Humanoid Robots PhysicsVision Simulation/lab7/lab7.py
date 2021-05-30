import numpy as np
import pandas as pd
import matplotlib.pyplot as plt



filename = "locations.csv"


names = ['Segments', 'X','Y']

df = pd.read_csv(filename,names = names).copy()

segment_names = ["Head_and_Neck", "Trunk",
                 "R_Upper_Arm", "R_Forearm", "R_Hand",
                 "L_Upper_Arm", "L_Forearm", "L_Hand",
                 "R_Thigh", "R_Shank", "R_Foot",
                 "L_Thigh", "L_Shank", "L_Foot"]

prox_dist_pairs = [(0, 1), (2, 3),
                   (4, 5), (5, 6), (6, 7),
                   (8, 9), (9, 10), (10, 11),
                   (12, 13), (13, 14), (14, 15),
                   (16, 17), (17, 18), (18, 19)]

length_vals = [0.5002, 0.4310,
               0.5772, 0.4574, 0.7900,
               0.5772, 0.4574, 0.7900,
               0.4095, 0.4395, 0.4415,
               0.4095, 0.4395, 0.4415]
weights = [0.0694, 0.4346,
           0.0271, 0.0162, 0.0061,
           0.0271, 0.0162, 0.0061,
           0.1416, 0.0433, 0.0137,
           0.1416, 0.0433, 0.0137]

names = ['segment name', 'proximal name', 'distal name', 'segment CoM x', 'segment CoM y']

var1, var2, var3, var4, var5 = [], [], [], [], []

var0 = segment_names
var5 = weights
for i in range(len(length_vals)):
    pro_id = prox_dist_pairs[i][0]
    dis_id = prox_dist_pairs[i][1]
    ##print(pro_id)
    var1 = var1 + [df.iloc[int(pro_id), 0]]
    var2 = var2 + [df.iloc[int(dis_id), 0]]
    var3 = var3 + [int(np.around((1.0 - length_vals[i]) * df.iloc[pro_id, 1] + length_vals[i] * df.iloc[dis_id, 1]))]
    var4 = var4 + [int(np.around((1.0 - length_vals[i]) * df.iloc[pro_id, 2] + length_vals[i] * df.iloc[dis_id, 2]))]

data = {}
t = [var0, var1, var2, var3, var4]
for i in range(len(t)):
    data[names[i]] = t[i]
df_output = pd.DataFrame(data)

print("Output: segment name, proximal name, distal name, segment CoM x, segment CoM y")
for i in range(len(df_output['segment name'])):
    print(f"{df_output['segment name'][i]}, {df_output['proximal name'][i]}, {df_output['distal name'][i]}, {df_output['segment CoM x'][i]}, {df_output['segment CoM y'][i]}")

com_x = 0.0
com_y = 0.0
for i in range(len(var5)):
    com_x += df_output['segment CoM x'][i] * var5[i]
    com_y += df_output['segment CoM y'][i] * var5[i]
print(f'Overall Center of Mass: ({int(np.around(com_x))}, {int(np.around(com_y))})')
