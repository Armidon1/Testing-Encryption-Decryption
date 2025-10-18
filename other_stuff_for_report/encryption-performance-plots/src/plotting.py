import matplotlib.pyplot as plt
import seaborn as sns
import pandas as pd

def plot_encryption_decryption_times(benchmark_data):
    """
    Plots the encryption and decryption times for various algorithms and file sizes.

    Parameters:
    benchmark_data (pd.DataFrame): A DataFrame containing the benchmark results with columns
                                    ['Algorithm', 'File Size', 'Encryption Time', 'Decryption Time'].
    """
    # Set the aesthetic style of the plots
    sns.set(style="whitegrid")

    # Create a figure with subplots
    fig, axes = plt.subplots(2, 1, figsize=(12, 10), sharex=True)

    # Plot encryption times
    sns.lineplot(data=benchmark_data, x='File Size', y='Encryption Time', hue='Algorithm', ax=axes[0])
    axes[0].set_title('Encryption Time by Algorithm and File Size')
    axes[0].set_ylabel('Time (seconds)')
    axes[0].legend(title='Algorithm')

    # Plot decryption times
    sns.lineplot(data=benchmark_data, x='File Size', y='Decryption Time', hue='Algorithm', ax=axes[1])
    axes[1].set_title('Decryption Time by Algorithm and File Size')
    axes[1].set_ylabel('Time (seconds)')
    axes[1].set_xlabel('File Size (bytes)')
    axes[1].legend(title='Algorithm')

    # Adjust layout
    plt.tight_layout()
    plt.show()

def save_plot(benchmark_data, filename):
    """
    Saves the encryption and decryption times plot to a file.

    Parameters:
    benchmark_data (pd.DataFrame): A DataFrame containing the benchmark results.
    filename (str): The filename to save the plot as.
    """
    plt.figure(figsize=(12, 10))
    plot_encryption_decryption_times(benchmark_data)
    plt.savefig(filename)
    plt.close()