import os
import json
import pandas as pd
import matplotlib.pyplot as plt
from utils.crypto_bench import benchmark_encryption, benchmark_decryption

def load_benchmark_data(file_path):
    with open(file_path, 'r') as f:
        return json.load(f)

def process_benchmark_data(raw_data):
    processed_data = {
        'file_size': [],
        'encryption_time': [],
        'decryption_time': [],
        'algorithm': []
    }
    
    for entry in raw_data:
        processed_data['file_size'].append(entry['file_size'])
        processed_data['encryption_time'].append(entry['encryption_time'])
        processed_data['decryption_time'].append(entry['decryption_time'])
        processed_data['algorithm'].append(entry['algorithm'])
    
    return pd.DataFrame(processed_data)

def benchmark_algorithms(file_sizes, algorithms):
    results = []
    for size in file_sizes:
        for algorithm in algorithms:
            enc_time = benchmark_encryption(algorithm, size)
            dec_time = benchmark_decryption(algorithm, size)
            results.append({
                'file_size': size,
                'encryption_time': enc_time,
                'decryption_time': dec_time,
                'algorithm': algorithm
            })
    return results

def save_benchmark_results(results, output_path):
    with open(output_path, 'w') as f:
        json.dump(results, f)

def main():
    file_sizes = [1024, 2048, 4096, 8192, 16384]  # Example file sizes in bytes
    algorithms = ['AES', 'CAMELLIA', 'SM4']
    
    results = benchmark_algorithms(file_sizes, algorithms)
    output_path = os.path.join('data', 'raw', 'benchmark_results.json')
    save_benchmark_results(results, output_path)

if __name__ == "__main__":
    main()