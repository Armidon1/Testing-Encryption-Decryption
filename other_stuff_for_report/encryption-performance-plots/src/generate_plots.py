#!/usr/bin/env python3
"""Generate comparison plots from benchmark text output.

Usage:
  python3 src/generate_plots.py --input data/raw/results.txt --out output

Produces PNG files in the output directory.
"""
import argparse
import os
import re
from pathlib import Path

import pandas as pd
import matplotlib.pyplot as plt
import seaborn as sns

LINE_RE = re.compile(r"^(Encryption|Decryption)\s*-\s*([A-Z0-9]+)\s*-\s*([0-9]+)\s*-\s*([0-9]*\.?[0-9]+)\s*$", re.IGNORECASE)


def parse_results_file(path):
    rows = []
    with open(path, "r", encoding="utf-8") as f:
        for raw in f:
            line = raw.strip()
            if not line:
                continue
            m = LINE_RE.match(line)
            if m:
                op = m.group(1).capitalize()
                algo = m.group(2).upper()
                size = int(m.group(3))
                time_ms = float(m.group(4))
                rows.append({"operation": op, "algorithm": algo, "file_size": size, "time_ms": time_ms})
            else:
                # ignore header/trailer lines
                continue
    return pd.DataFrame(rows)


def ensure_dir(p):
    Path(p).mkdir(parents=True, exist_ok=True)


def plot_by_operation(df, out_dir):
    # One figure per operation (Encryption / Decryption)
    for op, g in df.groupby("operation"):
        plt.figure(figsize=(10, 6))
        sns.lineplot(data=g, x="file_size", y="time_ms", hue="algorithm", marker="o")
        plt.xscale("log")
        plt.xlabel("File size (bytes) [log scale]")
        plt.ylabel("Time (ms)")
        plt.title(f"{op} — time vs file size by algorithm")
        plt.grid(True, which="both", ls="--", alpha=0.4)
        plt.legend(title="Algorithm")
        plt.tight_layout()
        out_path = os.path.join(out_dir, f"{op.lower()}_time_vs_size.png")
        plt.savefig(out_path, dpi=150)
        plt.close()


def plot_grouped_bars(df, out_dir):
    # grouped bar plot: for each file_size show algorithms side-by-side
    for op, g in df.groupby("operation"):
        plt.figure(figsize=(10, 6))
        g = g.copy()
        g["file_size_str"] = g["file_size"].astype(str)
        sns.barplot(data=g, x="file_size_str", y="time_ms", hue="algorithm")
        plt.xlabel("File size (bytes)")
        plt.ylabel("Time (ms)")
        plt.title(f"{op} — performance by file size and algorithm")
        plt.tight_layout()
        out_path = os.path.join(out_dir, f"{op.lower()}_grouped_bars.png")
        plt.savefig(out_path, dpi=150)
        plt.close()


def plot_comparison(df, out_dir):
    # Combined comparison: for each algorithm, show encryption and decryption times side-by-side for each size
    df2 = df.pivot_table(index=["file_size", "algorithm"], columns="operation", values="time_ms").reset_index()
    dfm = df2.melt(id_vars=["file_size", "algorithm"], value_vars=["Encryption", "Decryption"], var_name="operation", value_name="time_ms")
    g = sns.catplot(data=dfm, x="file_size", y="time_ms", hue="operation", col="algorithm", kind="bar", col_wrap=3, sharey=False, height=3.5)
    g.fig.suptitle("Encryption vs Decryption per algorithm and file size", y=1.02)
    out_path = os.path.join(out_dir, "enc_vs_dec_per_algo.png")
    g.savefig(out_path, dpi=150, bbox_inches="tight")
    plt.close()


def plot_per_size(df, out_dir):
    """For each file size produce a grouped bar chart of Encryption vs Decryption per algorithm."""
    for size in sorted(df["file_size"].unique()):
        sub = df[df["file_size"] == size]
        # pivot so algorithms are rows, operations are columns
        pivot = sub.pivot_table(index="algorithm", columns="operation", values="time_ms")
        # ensure both columns exist
        for col in ["Encryption", "Decryption"]:
            if col not in pivot.columns:
                pivot[col] = 0.0
        ax = pivot.plot(kind="bar", figsize=(8, 5))
        ax.set_title(f"Encryption vs Decryption — file size: {size} bytes")
        ax.set_ylabel("Time (ms)")
        ax.set_xlabel("Algorithm")
        ax.grid(axis="y", ls="--", alpha=0.4)
        plt.tight_layout()
        out_path = os.path.join(out_dir, f"per_size_{size}.png")
        plt.savefig(out_path, dpi=150)
        plt.close()


def main():
    parser = argparse.ArgumentParser()
    parser.add_argument("--input", "-i", required=True, help="Path to raw results text file")
    parser.add_argument("--out", "-o", required=True, help="Output directory for PNG files")
    args = parser.parse_args()

    df = parse_results_file(args.input)
    if df.empty:
        print("No benchmark rows found in the input file. Check formatting.")
        return

    ensure_dir(args.out)

    print("Dataframe summary:\n", df)

    sns.set(style="whitegrid")
    plot_by_operation(df, args.out)
    plot_grouped_bars(df, args.out)
    plot_comparison(df, args.out)
    plot_per_size(df, args.out)

    print(f"Plots saved to: {args.out}")


if __name__ == "__main__":
    main()