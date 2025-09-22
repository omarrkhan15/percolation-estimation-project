#pragma once
#include <vector>
#include <stdexcept>
#include <iostream>

class Percolation {
private:
    int n;
    std::vector<bool> grid;           // true if site is open
    std::vector<int> parent;          // parent array for union-find
    std::vector<int> size;            // size array for weighted union
    int openSitesCount;
    int virtualTop;                   // virtual top site index
    int virtualBottom;                // virtual bottom site index
    
    // Convert 2D coordinates to 1D index
    int getIndex(int row, int col) const {
        return row * n + col;
    }
    
    // Validate coordinates
    void validate(int row, int col) const {
        if (row < 0 || row >= n || col < 0 || col >= n) {
            throw std::invalid_argument("Index out of bounds");
        }
    }
    
    // Find root with path compression
    int find(int x) {
        if (parent[x] != x) {
            parent[x] = find(parent[x]);  // path compression
        }
        return parent[x];
    }
    
    // Union two components with weighting
    void unionSites(int x, int y) {
        int rootX = find(x);
        int rootY = find(y);
        
        if (rootX == rootY) return;
        
        // Weighted union: attach smaller tree to larger tree
        if (size[rootX] < size[rootY]) {
            parent[rootX] = rootY;
            size[rootY] += size[rootX];
        } else {
            parent[rootY] = rootX;
            size[rootX] += size[rootY];
        }
    }
    
    // Check if two sites are connected
    bool connected(int x, int y) {
        return find(x) == find(y);
    }

public:
    // creates n-by-n grid, with all sites initially blocked
    Percolation(int n) {
        if (n <= 0) {
            throw std::invalid_argument("Grid size must be positive");
        }
        
        this->n = n;
        this->openSitesCount = 0;
        
        // Initialize grid (all blocked)
        grid.resize(n * n, false);
        
        // Initialize union-find with 2 extra sites for virtual top and bottom
        int totalSites = n * n + 2;
        parent.resize(totalSites);
        size.resize(totalSites, 1);
        
        // Initialize parent array
        for (int i = 0; i < totalSites; i++) {
            parent[i] = i;
        }
        
        virtualTop = n * n;
        virtualBottom = n * n + 1;
    }
    
    // opens the site (row, col) if it is not open already
    void open(int row, int col) {
        validate(row, col);
        
        if (isOpen(row, col)) return;
        
        int index = getIndex(row, col);
        grid[index] = true;
        openSitesCount++;
        
        // Connect to virtual top if in top row
        if (row == 0) {
            unionSites(index, virtualTop);
        }
        
        // Connect to virtual bottom if in bottom row
        if (row == n - 1) {
            unionSites(index, virtualBottom);
        }
        
        // Connect to open neighbors
        // Check up
        if (row > 0 && isOpen(row - 1, col)) {
            unionSites(index, getIndex(row - 1, col));
        }
        
        // Check down
        if (row < n - 1 && isOpen(row + 1, col)) {
            unionSites(index, getIndex(row + 1, col));
        }
        
        // Check left
        if (col > 0 && isOpen(row, col - 1)) {
            unionSites(index, getIndex(row, col - 1));
        }
        
        // Check right
        if (col < n - 1 && isOpen(row, col + 1)) {
            unionSites(index, getIndex(row, col + 1));
        }
    }
    
    // is the site (row, col) open?
    bool isOpen(int row, int col) {
        validate(row, col);
        return grid[getIndex(row, col)];
    }
    
    // is the site (row, col) full?
    bool isFull(int row, int col) {
        validate(row, col);
        if (!isOpen(row, col)) return false;
        
        return connected(getIndex(row, col), virtualTop);
    }
    
    // returns the number of open sites
    int numberOfOpenSites() {
        return openSitesCount;
    }
    
    // does the system percolate?
    bool percolates() {
        return connected(virtualTop, virtualBottom);
    }
    
    // unit testing (required)
    static void test() {
        std::cout << "Testing Percolation class..." << std::endl;
        
        // Test basic functionality
        Percolation perc(3);
        
        // Initially no sites should be open
        std::cout << "Initial open sites: " << perc.numberOfOpenSites() << " (expected: 0)" << std::endl;
        std::cout << "Initial percolation: " << (perc.percolates() ? "true" : "false") << " (expected: false)" << std::endl;
        
        // Open some sites
        perc.open(0, 1);
        std::cout << "After opening (0,1) - Open sites: " << perc.numberOfOpenSites() << std::endl;
        std::cout << "Site (0,1) is open: " << (perc.isOpen(0, 1) ? "true" : "false") << std::endl;
        std::cout << "Site (0,1) is full: " << (perc.isFull(0, 1) ? "true" : "false") << std::endl;
        
        perc.open(1, 1);
        perc.open(2, 1);
        
        std::cout << "After opening path (0,1)-(1,1)-(2,1):" << std::endl;
        std::cout << "Open sites: " << perc.numberOfOpenSites() << std::endl;
        std::cout << "System percolates: " << (perc.percolates() ? "true" : "false") << std::endl;
        
        // Test error cases
        try {
            perc.open(-1, 0);
            std::cout << "ERROR: Should have thrown exception for invalid coordinates" << std::endl;
        } catch (const std::invalid_argument& e) {
            std::cout << "Correctly caught invalid argument: " << e.what() << std::endl;
        }
        
        std::cout << "Percolation tests completed." << std::endl;
    }
};