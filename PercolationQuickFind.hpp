#pragma once
#include <vector>
#include <stdexcept>
#include <iostream>

// Quick-Find implementation for comparison
class PercolationQuickFind {
private:
    int n;
    std::vector<bool> grid;           // true if site is open
    std::vector<int> id;              // id array for quick-find
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
    
    // Find operation - O(1)
    int find(int x) {
        return id[x];
    }
    
    // Union operation - O(n) - connects all elements with same id
    void unionSites(int x, int y) {
        int idX = find(x);
        int idY = find(y);
        
        if (idX == idY) return;
        
        // Change all entries with id[x] to id[y]
        for (int i = 0; i < int(id.size()); i++) {
            if (id[i] == idX) {
                id[i] = idY;
            }
        }
    }
    
    // Check if two sites are connected
    bool connected(int x, int y) {
        return find(x) == find(y);
    }

public:
    // creates n-by-n grid, with all sites initially blocked
    PercolationQuickFind(int n) {
        if (n <= 0) {
            throw std::invalid_argument("Grid size must be positive");
        }
        
        this->n = n;
        this->openSitesCount = 0;
        
        // Initialize grid (all blocked)
        grid.resize(n * n, false);
        
        // Initialize quick-find with 2 extra sites for virtual top and bottom
        int totalSites = n * n + 2;
        id.resize(totalSites);
        
        // Initialize id array
        for (int i = 0; i < totalSites; i++) {
            id[i] = i;
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
};
