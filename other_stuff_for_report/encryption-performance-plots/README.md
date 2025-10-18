# Encryption Performance Plots

This project aims to analyze and visualize the performance differences of various encryption algorithms, specifically focusing on their encryption and decryption times across different file sizes. The primary algorithms being tested include AES, CAMELLIA, and SM4.

## Project Structure

- **src/**: Contains the source code for generating plots and benchmarking.
  - **generate_plots.py**: Orchestrates the generation of plots based on benchmark results.
  - **benchmarks.py**: Parses and organizes benchmark data for plotting.
  - **plotting.py**: Utilizes libraries like Matplotlib or Seaborn to create visual representations of performance data.
  - **utils/**: Contains utility functions and implementations of the encryption algorithms.
    - **crypto_bench.py**: Helper functions for performing encryption and decryption benchmarks.

- **data/**: Stores benchmark data.
  - **raw/**: Contains raw benchmark data files.
  - **processed/**: Stores cleaned and formatted benchmark data for analysis.

- **notebooks/**: Includes Jupyter notebooks for exploratory data analysis.
  - **analysis.ipynb**: Contains visualizations and insights derived from the benchmark data.

- **tests/**: Contains unit tests for the project.
  - **test_benchmarks.py**: Tests for functions in benchmarks.py to ensure data processing works correctly.

- **requirements.txt**: Lists the required Python packages for the project.

- **pyproject.toml**: Configuration file for the project, specifying dependencies and build system management.

## Setup Instructions

1. Clone the repository:
   ```
   git clone <repository-url>
   cd encryption-performance-plots
   ```

2. Install the required packages:
   ```
   pip install -r requirements.txt
   ```

3. Run the benchmarks:
   ```
   python src/generate_plots.py
   ```

4. Open the Jupyter notebook for analysis:
   ```
   jupyter notebook notebooks/analysis.ipynb
   ```

## Usage

- The `generate_plots.py` script will generate plots based on the benchmark results stored in the `data/processed` directory.
- The `analysis.ipynb` notebook can be used to explore the results interactively and gain insights into the performance of the encryption algorithms.

## Contributing

Contributions are welcome! Please feel free to submit a pull request or open an issue for any suggestions or improvements.

## License

This project is licensed under the MIT License. See the LICENSE file for more details.