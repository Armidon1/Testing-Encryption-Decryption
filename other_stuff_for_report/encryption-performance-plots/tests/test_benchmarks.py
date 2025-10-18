import unittest
from src.benchmarks import parse_benchmark_data, process_benchmark_data

class TestBenchmarkFunctions(unittest.TestCase):

    def setUp(self):
        # Sample raw data for testing
        self.raw_data = [
            {"algorithm": "AES", "file_size": 1024, "encryption_time": 0.05, "decryption_time": 0.04},
            {"algorithm": "CAMELLIA", "file_size": 1024, "encryption_time": 0.06, "decryption_time": 0.05},
            {"algorithm": "SM4", "file_size": 1024, "encryption_time": 0.07, "decryption_time": 0.06},
            {"algorithm": "AES", "file_size": 2048, "encryption_time": 0.10, "decryption_time": 0.09},
            {"algorithm": "CAMELLIA", "file_size": 2048, "encryption_time": 0.11, "decryption_time": 0.10},
            {"algorithm": "SM4", "file_size": 2048, "encryption_time": 0.12, "decryption_time": 0.11},
        ]

    def test_parse_benchmark_data(self):
        parsed_data = parse_benchmark_data(self.raw_data)
        self.assertEqual(len(parsed_data), 6)
        self.assertIn("algorithm", parsed_data[0])
        self.assertIn("file_size", parsed_data[0])
        self.assertIn("encryption_time", parsed_data[0])
        self.assertIn("decryption_time", parsed_data[0])

    def test_process_benchmark_data(self):
        parsed_data = parse_benchmark_data(self.raw_data)
        processed_data = process_benchmark_data(parsed_data)
        self.assertEqual(len(processed_data), 3)  # Expecting 3 algorithms
        self.assertIn("AES", processed_data)
        self.assertIn("CAMELLIA", processed_data)
        self.assertIn("SM4", processed_data)

if __name__ == '__main__':
    unittest.main()