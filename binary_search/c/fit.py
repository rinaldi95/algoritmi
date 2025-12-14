import pandas as pd, numpy as np, matplotlib.pyplot as plt

df = pd.read_csv("results.csv")
df = df[df["alg"] == "binary"]  # filtra se ci sono più algoritmi

df["log2n"] = np.log2(df["n"])
coef = np.polyfit(df["log2n"], df["ns_per_call"], 1)
yhat = np.polyval(coef, df["log2n"])
ss_res = np.sum((df["ns_per_call"] - yhat) ** 2)
ss_tot = np.sum((df["ns_per_call"] - df["ns_per_call"].mean()) ** 2)
r2 = 1 - ss_res / ss_tot

print("ns ≈ {:.3f} * log2(n) + {:.3f}, R² = {:.4f}".format(coef[0], coef[1], r2))

plt.figure()
plt.plot(df["log2n"], df["ns_per_call"], marker="o", label="binary data")
plt.plot(df["log2n"], yhat, label="fit ~ log2(n)")
plt.xlabel("log2(n)")
plt.ylabel("ns per call")
plt.legend()
plt.show()
