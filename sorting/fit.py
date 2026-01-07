import csv
import pandas as pd
import numpy as np
import matplotlib.pyplot as plt
from collections import defaultdict


filename="risultati.csv"

rows=[]
with open(filename, newline="") as f:
    reader=csv.DictReader(f)
    for row in reader:
        rows.append(row)

data = defaultdict(list)

for row in rows:
    n = int(row["n"])
    alg = row["algorithm"]
    t = float(row["ns_per_call"])
    data[alg].append((n, t))

for alg in data:
    data[alg].sort(key=lambda x: x[0])

series = {}
for alg, pts in data.items():
    ns = [p[0] for p in pts]
    ts = [p[1] for p in pts]
    series[alg] = (ns, ts)

def f_n2(n, a, b):
    return a * n * n + b

def f_nlogn(n, a, b):
    return a * n * np.log2(n) + b

fits = {}
fits_r2 = {}

for alg, (ns, ts) in series.items():
    x = np.array(ns, dtype=float)
    y = np.array(ts, dtype=float)

    X_n2 = np.column_stack([x * x, np.ones_like(x)])
    a2, b2 = np.linalg.lstsq(X_n2, y, rcond=None)[0]

    X_nlogn = np.column_stack([x * np.log2(x), np.ones_like(x)])
    aL, bL = np.linalg.lstsq(X_nlogn, y, rcond=None)[0]

    fits[alg] = {"n2": (a2, b2), "nlogn": (aL, bL)}
    yhat_n2 = f_n2(x, a2, b2)
    yhat_nlogn = f_nlogn(x, aL, bL)
    ss_tot = np.sum((y - y.mean()) ** 2)
    r2_n2 = 1.0 if ss_tot == 0 else 1.0 - (np.sum((y - yhat_n2) ** 2) / ss_tot)
    r2_nlogn = 1.0 if ss_tot == 0 else 1.0 - (np.sum((y - yhat_nlogn) ** 2) / ss_tot)
    fits_r2[alg] = {"n2": r2_n2, "nlogn": r2_nlogn}

print("R2 summary:")
for alg in fits_r2:
    r2_n2 = fits_r2[alg]["n2"]
    r2_nlogn = fits_r2[alg]["nlogn"]
    print(f"{alg}: n^2={r2_n2:.4f}, nlogn={r2_nlogn:.4f}")

plt.figure(figsize=(8, 5))

for alg, (ns, ts) in series.items():
    plt.plot(ns, ts, marker="o", label=f"{alg} data")

plt.xlabel("n")
plt.ylabel("ns per call")
plt.title("Sorting benchmark (raw)")
plt.legend()
plt.xscale("log")
plt.yscale("log")

for alg, (ns, ts) in series.items():
    x = np.array(ns, dtype=float)

    a2, b2 = fits[alg]["n2"]
    aL, bL = fits[alg]["nlogn"]

    y_n2 = f_n2(x, a2, b2)
    y_nlogn = f_nlogn(x, aL, bL)

    r2_n2 = fits_r2[alg]["n2"]
    r2_nlogn = fits_r2[alg]["nlogn"]
    plt.plot(x, y_n2, linestyle="--", alpha=0.5, label=f"{alg} n^2 R2={r2_n2:.3f}")
    plt.plot(x, y_nlogn, linestyle=":", alpha=0.5, label=f"{alg} nlogn R2={r2_nlogn:.3f}")

plt.tight_layout()
plt.savefig("benchmark.png", dpi=150)
plt.show()


    
