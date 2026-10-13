#ifndef RENTAL_SYSTEM_H
#define RENTAL_SYSTEM_H

#include <iostream>
#include <vector>
#include <unordered_map>
#include <queue>
#include <memory>
#include <string>
#include "Vehicle.h"
#include "TwoWheeler.h"
#include "FourWheeler.h"
#include "Transaction.h"

class RentalSystem {
private:
    // DATA STRUCTURE 1: Hash Map (Unordered Map) for O(1) fast ID-based vehicle lookup
    std::unordered_map<std::string, std::shared_ptr<Vehicle>> vehicleRegistry;

    // Preserving order of addition for catalog listing
    std::vector<std::string> vehicleKeys;

    // DATA STRUCTURE 2: FIFO Queue for cleaning / maintenance line
    std::queue<std::string> cleaningBayQueue;

    // DATA STRUCTURE 3: Vector for rental audit log & transaction history
    std::vector<Transaction> transactionHistory;

    int nextTransactionSeq;

    // Internal helper methods
    std::string generateTransactionId();
    void printTableDivider() const;
    void printTableHeader() const;
    void printVehicleRow(const std::shared_ptr<Vehicle>& v, int serialNo) const;

public:
    RentalSystem();

    // Data initialization
    void loadSampleData();

    // Browsing & Display operations
    void displayAllVehicles() const;
    void displayTwoWheelers() const;
    void displayFourWheelers() const;
    void displayByStatus(VehicleStatus status) const;
    void searchVehicle() const;

    // Core Rental Operations
    void rentVehicle();
    void returnVehicle();

    // Cleaning & Maintenance Queue Operations
    void viewCleaningBay() const;
    void processNextInCleaningBay();
    void processAllInCleaningBay();

    // Analytics & History
    void displayTransactionHistory() const;
    void displayFinancialSummary() const;

    // Management
    void addNewVehicle();
};

#endif // RENTAL_SYSTEM_H
