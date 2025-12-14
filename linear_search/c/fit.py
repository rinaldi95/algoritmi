import pandas as pd, numpy as np, matplotlib.pyplot as plt

df = pd.read_csv("results.csv")
df = df[df["alg"] == "linear"]  # filtra i dati della linear search
df = df.sort_values("n")

coef = np.polyfit(df["n"], df["ns_per_call"], 1)
yhat = np.polyval(coef, df["n"])
ss_res = np.sum((df["ns_per_call"] - yhat) ** 2)
ss_tot = np.sum((df["ns_per_call"] - df["ns_per_call"].mean()) ** 2)
r2 = 1 - ss_res / ss_tot

print("ns ≈ {:.3f} * n + {:.3f}, R² = {:.4f}".format(coef[0], coef[1], r2))

plt.figure()
plt.plot(df["n"], df["ns_per_call"], marker="o", label="linear data")
plt.plot(df["n"], yhat, label="fit ~ n")
plt.xlabel("n")
plt.ylabel("ns per call")
plt.legend()
plt.show()
