import time
import random
from Crypto.Cipher import AES, DES, ARC4
from Crypto.Random import get_random_bytes

def generate_random_data(size):
    return get_random_bytes(size)

def benchmark_aes(data):
    key = get_random_bytes(16)  # AES-128
    cipher = AES.new(key, AES.MODE_EAX)
    
    start_time = time.time()
    ciphertext, tag = cipher.encrypt_and_digest(data)
    encryption_time = time.time() - start_time

    start_time = time.time()
    cipher.decrypt(ciphertext)
    decryption_time = time.time() - start_time

    return encryption_time, decryption_time

def benchmark_des(data):
    key = get_random_bytes(8)  # DES
    cipher = DES.new(key, DES.MODE_EAX)
    
    start_time = time.time()
    ciphertext = cipher.encrypt(data)
    encryption_time = time.time() - start_time

    start_time = time.time()
    cipher.decrypt(ciphertext)
    decryption_time = time.time() - start_time

    return encryption_time, decryption_time

def benchmark_arc4(data):
    key = get_random_bytes(16)  # ARC4
    cipher = ARC4.new(key)
    
    start_time = time.time()
    ciphertext = cipher.encrypt(data)
    encryption_time = time.time() - start_time

    start_time = time.time()
    cipher.decrypt(ciphertext)
    decryption_time = time.time() - start_time

    return encryption_time, decryption_time

def run_benchmarks(file_sizes):
    results = {}
    for size in file_sizes:
        data = generate_random_data(size)
        aes_times = benchmark_aes(data)
        des_times = benchmark_des(data)
        arc4_times = benchmark_arc4(data)
        
        results[size] = {
            'AES': aes_times,
            'DES': des_times,
            'ARC4': arc4_times
        }
    return results