#pragma once
#include "Percolation.hpp"
#include <vector>
#include <random>
#include <cmath>
#include <stdexcept>
#include <iostream>

class PercolationStats {
private:
    std::vector<double> thresholds;
    int n;
    int trials;
    double sampleMean;
    double sampleStddev;
    
    void calculateStats() {
        // Calculate mean
        double sum = 0.0;
        for (double threshold : thresholds) {
            sum += threshold;
        }
        sampleMean = sum / trials;
        
        // Calculate standard deviation
        double sumSquaredDiffs = 0.0;
        for (double threshold : thresholds) {
            double diff = threshold - sampleMean;
            sumSquaredDiffs += diff * diff;
        }
        sampleStddev = std::sqrt(sumSquaredDiffs / (trials - 1));
    }

public:
    // perform independent trials on an n-by-n grid
    PercolationStats(int n, int trials) {
        if (n <= 0) {
            throw std::invalid_argument("Grid size n must be positive");
        }
        if (trials <= 0) {
            throw std::invalid_argument("Number of trials must be positive");
        }
        
        this->n = n;
        this->trials = trials;
        thresholds.reserve(trials);
        
        // Random number generation
        std::random_device rd;
        std::mt19937 gen(rd());
        std::uniform_int_distribution<> dis(0, n - 1);
        
        // Perform trials
        for (int t = 0; t < trials; t++) {
            Percolation perc(n);
            
            // Keep opening sites until system percolates
            while (!perc.percolates()) {
                int row, col;
                // Find a blocked site to open
                do {
                    row = dis(gen);
                    col = dis(gen);
                } while (perc.isOpen(row, col));
                
                perc.open(row, col);
            }
            
            // Calculate and store threshold for this trial
            double threshold = static_cast<double>(perc.numberOfOpenSites()) / (n * n);
            thresholds.push_back(threshold);
        }
        
        // Calculate statistics
        calculateStats();
    }
    
    // sample mean of percolation threshold
    double mean() {
        return sampleMean;
    }
    
    // sample standard deviation of percolation threshold
    double stddev() {
        return sampleStddev;
    }
    
    // low endpoint of 95% confidence interval
    double confidenceLow() {
        double margin = 1.96 * sampleStddev / std::sqrt(trials);
        return sampleMean - margin;
    }
    
    // high endpoint of 95% confidence interval
    double confidenceHigh() {
        double margin = 1.96 * sampleStddev / std::sqrt(trials);
        return sampleMean + margin;
    }
    
    // test client
    static void test() {
        std::cout << "Testing PercolationStats class..." << std::endl;
        
        // Test with small grid for quick results
        int testN = 10;
        int testTrials = 30;
        
        std::cout << "Running " << testTrials << " trials on " << testN << "x" << testN << " grid..." << std::endl;
        
        PercolationStats stats(testN, testTrials);
        
        std::cout << "Mean: " << stats.mean() << std::endl;
        std::cout << "Standard deviation: " << stats.stddev() << std::endl;
        std::cout << "95% confidence interval: [" << stats.confidenceLow() 
                  << ", " << stats.confidenceHigh() << "]" << std::endl;
        
        // Test error cases
        try {
            PercolationStats invalidStats(-1, 10);
            std::cout << "ERROR: Should have thrown exception for invalid n" << std::endl;
        } catch (const std::invalid_argument& e) {
            std::cout << "Correctly caught invalid argument for n: " << e.what() << std::endl;
        }
        
        try {
            PercolationStats invalidStats(10, -1);
            std::cout << "ERROR: Should have thrown exception for invalid trials" << std::endl;
        } catch (const std::invalid_argument& e) {
            std::cout << "Correctly caught invalid argument for trials: " << e.what() << std::endl;
        }
        
        std::cout << "PercolationStats tests completed." << std::endl;
    }
};