# Percolation Threshold Estimation

A Monte Carlo simulation to estimate the percolation threshold using efficient union-find data structures. This project demonstrates the dramatic performance impact of algorithm choice in scientific computing applications.

## 🎯 Overview

This project implements and compares two union-find algorithms to solve the percolation problem:
- **Quick-Find Algorithm** - Simple but inefficient O(n²) operations
- **Weighted Quick-Union** - Optimized with path compression for O(log n) operations

The goal is to estimate the critical probability threshold where a random n×n grid transitions from non-percolating to percolating behavior.

## 🔬 The Percolation Problem

**What is Percolation?**
- Model electrical conductivity in composite materials
- Simulate fluid flow through porous media  
- Analyze network connectivity and robustness

**The Model:**
- n×n grid where each site can be open (conducting) or blocked (insulating)
- System "percolates" if there's a path of open sites from top to bottom
- Question: What fraction of sites must be open for percolation to occur?

## 📊 Performance Results

| Grid Size | Quick-Find | Weighted Quick-Union | Speedup |
|-----------|------------|---------------------|---------|
| n = 10    | 0.009s     | 0.003s             | 2.8x    |
| n = 20    | 0.074s     | 0.010s             | 7.5x    |
| n = 50    | 2.079s     | 0.060s             | 34.8x   |
| n = 100   | 32.541s    | 0.256s             | **127x** |
| n = 200   | >60s       | 1.050s             | **>57x** |

**Maximum Problem Size (60s limit):**
- Quick-Find: n = 100
- Weighted Quick-Union: n = 1200 
- **12x improvement in solvable problem size!**

## 🚀 Quick Start

### Prerequisites
- C++17 compatible compiler (g++, clang++, MSVC)
- Make (optional, for easy compilation)

### Compilation
```bash
# Using Make
make

# Or compile directly
g++ -std=c++17 -O2 -o percolation main.cpp
```

### Running the Program

**Basic usage:**
```bash
./percolation <grid_size> <trials>
./percolation 200 100
```

**Run full test suite:**
```bash


## 📁 Project Structure

```
percolation/
├── Percolation.hpp          # Weighted Quick-Union implementation
├── PercolationQuickFind.hpp # Quick-Find implementation (comparison)
├── PercolationStat.hpp      # Monte Carlo statistics
├── Stopwatch.hpp           # High-precision timer
├── main.cpp                # Test program and performance comparison
├── Comparison.txt          # Detailed performance analysis
└── README.md             # This file
```

## 🔧 Implementation Details

### Key Optimizations
- **Path Compression** - Flattens union-find trees during find operations
- **Union by Size** - Always attaches smaller tree to larger tree
- **Virtual Sites** - Eliminates need to check entire bottom row for percolation
- **Efficient Indexing** - 2D to 1D coordinate mapping

### Algorithm Complexity
- **Quick-Find**: O(n⁴) total complexity for full simulation
- **Weighted Quick-Union**: O(n² log n) total complexity

### Statistical Methods
- Monte Carlo simulation with configurable trial count
- Sample mean and standard deviation calculation
- 95% confidence interval using normal distribution approximation

## 🧪 Scientific Validation

The percolation threshold for infinite square lattices is approximately **0.592746**.

**Our Results:**
- Large grids (n≥200): ~0.591-0.593 ✅
- Small grids (n=2): ~0.667 (expected finite-size effect) ✅
- Confidence intervals: ±0.002 (high precision) ✅

## 📈 Performance Analysis

### Why Weighted Quick-Union Wins
1. **Union Operations**: O(log n) vs O(n²) per operation
2. **Path Compression**: Keeps trees shallow, improving subsequent operations
3. **Weighted Union**: Prevents degenerate linear trees
4. **Compound Effect**: Small improvements per operation × millions of operations = massive speedup

### Real-World Impact
- **Materials Science**: Model electrical conductivity in composites
- **Hydrology**: Simulate groundwater flow and oil reservoir connectivity  
- **Network Analysis**: Study Internet robustness and social network effects

## 🎓 Educational Value

This project demonstrates:
- **Algorithm Analysis**: Big O notation in practice
- **Data Structures**: Union-find optimization techniques
- **Scientific Computing**: Monte Carlo method application
- **Performance Engineering**: How algorithm choice affects feasibility

Perfect for:
- Data structures coursework
- Algorithm analysis understanding  
- Scientific computing applications
- Performance optimization learning



## 📝 Assignment Context

This project was completed as part of **CSE247 Data Structures** coursework, demonstrating the practical impact of efficient algorithms in scientific applications. The assignment emphasizes how proper data structure choice can make the difference between feasible and infeasible computations.

## 🤝 Contributing

Feel free to:
- Optimize the algorithms further
- Add visualization of percolation process
- Implement 3D percolation
- Add parallel processing support

## 📊 Results Summary

**The Bottom Line:** Choosing the right algorithm enabled **12x larger problem sizes** and **127x faster execution** for realistic grids. This isn't just academic - it's the difference between meaningful scientific results and toy examples.

## 📄 License

This project is part of academic coursework. Feel free to use for educational purposes with proper attribution.

---

*"The enormous impact of an efficient algorithm on real-world performance"* - Assignment description that this project perfectly demonstrates! 🚀
