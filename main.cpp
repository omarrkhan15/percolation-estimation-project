#include "Percolation.hpp"
#include "PercolationQuickFind.hpp"
#include "PercolationStat.hpp"
#include "Stopwatch.hpp"
#include <iostream>
#include <iomanip>
#include <vector>
#include <random>
#include <string>

// Quick Find version of PercolationStats for comparison
class PercolationStatsQuickFind {
private:
    std::vector<double> thresholds;
    int n;
    int trials;
    double sampleMean;
    double sampleStddev;
    
    void calculateStats() {
        double sum = 0.0;
        for (double threshold : thresholds) {
            sum += threshold;
        }
        sampleMean = sum / trials;
        
        double sumSquaredDiffs = 0.0;
        for (double threshold : thresholds) {
            double diff = threshold - sampleMean;
            sumSquaredDiffs += diff * diff;
        }
        sampleStddev = std::sqrt(sumSquaredDiffs / (trials - 1));
    }

public:
    PercolationStatsQuickFind(int n, int trials) : n(n), trials(trials) {
        if (n <= 0 || trials <= 0) {
            throw std::invalid_argument("n and trials must be positive");
        }
        
        thresholds.reserve(trials);
        std::random_device rd;
        std::mt19937 gen(rd());
        std::uniform_int_distribution<> dis(0, n - 1);
        
        for (int t = 0; t < trials; t++) {
            PercolationQuickFind perc(n);
            
            while (!perc.percolates()) {
                int row, col;
                do {
                    row = dis(gen);
                    col = dis(gen);
                } while (perc.isOpen(row, col));
                
                perc.open(row, col);
            }
            
            double threshold = static_cast<double>(perc.numberOfOpenSites()) / (n * n);
            thresholds.push_back(threshold);
        }
        
        calculateStats();
    }
    
    double mean() { return sampleMean; }
    double stddev() { return sampleStddev; }
    double confidenceLow() {
        double margin = 1.96 * sampleStddev / std::sqrt(trials);
        return sampleMean - margin;
    }
    double confidenceHigh() {
        double margin = 1.96 * sampleStddev / std::sqrt(trials);
        return sampleMean + margin;
    }
};

void runPercolationStats(int n, int trials) {
    std::cout << "Running PercolationStats with Weighted Quick-Union:" << std::endl;
    std::cout << "n = " << n << ", trials = " << trials << std::endl;
    
    Stopwatch sw;
    PercolationStats stats(n, trials);
    double elapsed = sw.elapsedTime();
    
    std::cout << std::fixed << std::setprecision(6);
    std::cout << "mean()           = " << stats.mean() << std::endl;
    std::cout << "stddev()         = " << stats.stddev() << std::endl;
    std::cout << "confidenceLow()  = " << stats.confidenceLow() << std::endl;
    std::cout << "confidenceHigh() = " << stats.confidenceHigh() << std::endl;
    std::cout << "elapsed time     = " << elapsed << std::endl;
    std::cout << std::endl;
}

void performanceComparison() {
    std::cout << "=== PERFORMANCE COMPARISON ===" << std::endl;
    std::cout << "Comparing Quick-Find vs Weighted Quick-Union" << std::endl;
    std::cout << std::endl;
    
    // Test different grid sizes
    std::vector<int> testSizes = {10, 20, 50, 100, 150, 200};
    const int trials = 100;
    const double timeLimit = 60.0; // 1 minute limit
    
    std::cout << std::setw(8) << "n" 
              << std::setw(15) << "Quick-Find (s)"
              << std::setw(20) << "Weighted QU (s)"
              << std::setw(12) << "Speedup" << std::endl;
    std::cout << std::string(55, '-') << std::endl;
    
    for (int n : testSizes) {
        std::cout << std::setw(8) << n;
        
        // Test Quick-Find
        Stopwatch swQF;
        try {
            PercolationStatsQuickFind statsQF(n, trials);
            double timeQF = swQF.elapsedTime();
            
            if (timeQF > timeLimit) {
                std::cout << std::setw(15) << ">60.0" << std::setw(20) << "-" << std::setw(12) << "-" << std::endl;
                std::cout << "Quick-Find exceeded time limit at n=" << n << std::endl;
                break;
            }
            
            // Test Weighted Quick-Union
            Stopwatch swWQU;
            PercolationStats statsWQU(n, trials);
            double timeWQU = swWQU.elapsedTime();
            
            double speedup = timeQF / timeWQU;
            
            std::cout << std::fixed << std::setprecision(3);
            std::cout << std::setw(15) << timeQF
                      << std::setw(20) << timeWQU
                      << std::setw(12) << speedup << "x" << std::endl;
                      
        } catch (const std::exception& e) {
            std::cout << std::setw(15) << "TIMEOUT" << std::setw(20) << "-" << std::setw(12) << "-" << std::endl;
            break;
        }
    }
    
    std::cout << std::endl;
    
    // Find maximum n for each algorithm within time limit
    std::cout << "Finding maximum n within 60 seconds for 100 trials:" << std::endl;
    
    // Quick-Find maximum
    int maxNQuickFind = 0;
    for (int n = 50; n <= 1000; n += 50) {
        Stopwatch sw;
        try {
            PercolationStatsQuickFind stats(n, trials);
            double elapsed = sw.elapsedTime();
            if (elapsed <= timeLimit) {
                maxNQuickFind = n;
                std::cout << "Quick-Find n=" << n << " completed in " << elapsed << "s" << std::endl;
            } else {
                break;
            }
        } catch (...) {
            break;
        }
    }
    
    // Weighted Quick-Union maximum
    int maxNWeightedQU = 0;
    for (int n = 100; n <= 2000; n += 100) {
        Stopwatch sw;
        try {
            PercolationStats stats(n, trials);
            double elapsed = sw.elapsedTime();
            if (elapsed <= timeLimit) {
                maxNWeightedQU = n;
                std::cout << "Weighted Quick-Union n=" << n << " completed in " << elapsed << "s" << std::endl;
            } else {
                break;
            }
        } catch (...) {
            break;
        }
    }
    
    std::cout << std::endl;
    std::cout << "RESULTS:" << std::endl;
    std::cout << "Maximum n for Quick-Find (within 60s): " << maxNQuickFind << std::endl;
    std::cout << "Maximum n for Weighted Quick-Union (within 60s): " << maxNWeightedQU << std::endl;
    std::cout << "Performance improvement: " << (double)maxNWeightedQU / maxNQuickFind << "x" << std::endl;
}

int main(int argc, char* argv[]) {
    std::cout << "CSE247 Assignment #1 - Percolation Threshold Estimation" << std::endl;
    std::cout << "=========================================================" << std::endl;
    std::cout << std::endl;
    
    // Run unit tests
    std::cout << "=== UNIT TESTS ===" << std::endl;
    Percolation::test();
    std::cout << std::endl;
    PercolationStats::test();
    std::cout << std::endl;
    
    // Check command line arguments
    if (argc == 3) {
        int n = std::stoi(argv[1]);
        int trials = std::stoi(argv[2]);
        
        std::cout << "=== COMMAND LINE EXECUTION ===" << std::endl;
        runPercolationStats(n, trials);
    } else {
        // Default examples from assignment
        std::cout << "=== EXAMPLE RUNS ===" << std::endl;
        runPercolationStats(200, 100);
        runPercolationStats(200, 100);  // Run twice to show variation
        runPercolationStats(2, 100000);
    }
    
    // Performance comparison
    std::cout << std::endl;
    performanceComparison();
    
    return 0;
}