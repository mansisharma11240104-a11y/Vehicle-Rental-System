/**
 * Wheels & Ways - Vehicle Rental System (Single File Version)
 * Language: C++ (C++11 / C++14 / C++17 compatible)
 * 
 * Data Structures Used:
 * 1. std::unordered_map (Hash Table) - O(1) vehicle lookup by ID
 * 2. std::queue (FIFO Queue)         - Orderly cleaning & maintenance queue
 * 3. std::vector (Dynamic Array)     - Rental transaction ledger & audit history
 */

#include <iostream>
#include <string>
#include <vector>
#include <unordered_map>
#include <queue>
#include <memory>
#include <iomanip>
#include <algorithm>
#include <limits>
#include <sstream>

// ============================================================================
// ENUMS & TRANSACTION DATA STRUCTURE
// ============================================================================
enum class VehicleType {
    TWO_WHEELER,
    FOUR_WHEELER
};

enum class VehicleStatus {
    AVAILABLE,
    RENTED,
    IN_CLEANING
};

struct Transaction {
    std::string transactionId;
    std::string vehicleId;
    std::string vehicleName;
    std::string category;
    std::string customerName;
    std::string customerPhone;
    int bookedDays;
    int actualDays;
    int overdueDays;
    double dailyRate;
    double baseFare;
    double lateFineRate;
    double totalFine;
    double totalBill;

    void printReceipt() const {
        std::cout << "\n==========================================================\n";
        std::cout << "                 RENTAL RECEIPT & INVOICE                \n";
        std::cout << "==========================================================\n";
        std::cout << " Invoice ID     : " << transactionId << "\n";
        std::cout << " Customer Name  : " << customerName << "\n";
        std::cout << " Phone Number   : " << customerPhone << "\n";
        std::cout << " Vehicle ID     : " << vehicleId << " (" << vehicleName << ")\n";
        std::cout << " Category       : " << category << "\n";
        std::cout << "----------------------------------------------------------\n";
        std::cout << " Booked Duration: " << bookedDays << " day(s) @ Rs. " << std::fixed << std::setprecision(2) << dailyRate << "/day\n";
        std::cout << " Actual Days    : " << actualDays << " day(s)\n";
        std::cout << " Base Rental Fare: Rs. " << std::setw(8) << baseFare << "\n";

        if (overdueDays > 0) {
            std::cout << " [OVERDUE] Days : " << overdueDays << " day(s) @ Rs. " << lateFineRate << "/day fine\n";
            std::cout << " Late Return Fine: Rs. " << std::setw(8) << totalFine << "\n";
        } else {
            std::cout << " Late Return Fine: Rs.     0.00 (Returned on time)\n";
        }
        std::cout << "----------------------------------------------------------\n";
        std::cout << " TOTAL CHARGE   : Rs. " << std::setw(8) << totalBill << "\n";
        std::cout << "==========================================================\n\n";
    }
};

// ============================================================================
// OOP VEHICLE HIERARCHY
// ============================================================================
class Vehicle {
protected:
    std::string id;
    std::string brand;
    std::string model;
    VehicleType type;
    VehicleStatus status;
    double dailyRate;
    double lateFinePerDay;

    std::string currentCustomerName;
    std::string currentCustomerPhone;
    int rentedForDays;

public:
    Vehicle(std::string id, std::string brand, std::string model, VehicleType type,
            double dailyRate, double lateFinePerDay)
        : id(id), brand(brand), model(model), type(type),
          status(VehicleStatus::AVAILABLE), dailyRate(dailyRate),
          lateFinePerDay(lateFinePerDay), currentCustomerName(""),
          currentCustomerPhone(""), rentedForDays(0) {}

    virtual ~Vehicle() = default;

    std::string getId() const { return id; }
    std::string getBrand() const { return brand; }
    std::string getModel() const { return model; }
    std::string getFullName() const { return brand + " " + model; }
    VehicleType getType() const { return type; }
    VehicleStatus getStatus() const { return status; }
    
    std::string getStatusString() const {
        switch (status) {
            case VehicleStatus::AVAILABLE: return "AVAILABLE";
            case VehicleStatus::RENTED: return "RENTED";
            case VehicleStatus::IN_CLEANING: return "IN CLEANING";
            default: return "UNKNOWN";
        }
    }

    double getDailyRate() const { return dailyRate; }
    double getLateFinePerDay() const { return lateFinePerDay; }
    std::string getCustomerName() const { return currentCustomerName; }
    std::string getCustomerPhone() const { return currentCustomerPhone; }
    int getRentedForDays() const { return rentedForDays; }

    void rentOut(const std::string& name, const std::string& phone, int days) {
        status = VehicleStatus::RENTED;
        currentCustomerName = name;
        currentCustomerPhone = phone;
        rentedForDays = days;
    }

    void markAsCleaning() {
        status = VehicleStatus::IN_CLEANING;
        currentCustomerName = "";
        currentCustomerPhone = "";
        rentedForDays = 0;
    }

    void markAsAvailable() {
        status = VehicleStatus::AVAILABLE;
        currentCustomerName = "";
        currentCustomerPhone = "";
        rentedForDays = 0;
    }

    virtual std::string getCategoryName() const = 0;
    virtual std::string getSpecificDetails() const = 0;

    void printCard() const {
        std::cout << "--------------------------------------------------\n";
        std::cout << " Vehicle ID    : " << id << "\n";
        std::cout << " Name          : " << getFullName() << "\n";
        std::cout << " Category      : " << getCategoryName() << "\n";
        std::cout << " Specifications: " << getSpecificDetails() << "\n";
        std::cout << " Daily Rate    : Rs. " << std::fixed << std::setprecision(2) << dailyRate << "/day\n";
        std::cout << " Late Fine Rate: Rs. " << lateFinePerDay << "/day\n";
        std::cout << " Status        : " << getStatusString() << "\n";
        if (status == VehicleStatus::RENTED) {
            std::cout << " Rented To     : " << currentCustomerName << " (" << currentCustomerPhone << ")\n";
            std::cout << " Booked For    : " << rentedForDays << " day(s)\n";
        }
        std::cout << "--------------------------------------------------\n";
    }
};

class TwoWheeler : public Vehicle {
private:
    int engineCc;
    bool isGeared;
    bool helmetIncluded;

public:
    TwoWheeler(std::string id, std::string brand, std::string model,
               double dailyRate, double lateFinePerDay,
               int engineCc, bool isGeared, bool helmetIncluded = true)
        : Vehicle(id, brand, model, VehicleType::TWO_WHEELER, dailyRate, lateFinePerDay),
          engineCc(engineCc), isGeared(isGeared), helmetIncluded(helmetIncluded) {}

    std::string getCategoryName() const override { return "2-Wheeler"; }

    std::string getSpecificDetails() const override {
        std::string details = std::to_string(engineCc) + "cc, ";
        details += (isGeared ? "Geared" : "Automatic / Scooter");
        details += (helmetIncluded ? ", Helmet Incl." : "");
        return details;
    }
};

class FourWheeler : public Vehicle {
private:
    int seatingCapacity;
    std::string transmission;
    std::string fuelType;

public:
    FourWheeler(std::string id, std::string brand, std::string model,
                double dailyRate, double lateFinePerDay,
                int seatingCapacity, std::string transmission, std::string fuelType)
        : Vehicle(id, brand, model, VehicleType::FOUR_WHEELER, dailyRate, lateFinePerDay),
          seatingCapacity(seatingCapacity), transmission(transmission), fuelType(fuelType) {}

    std::string getCategoryName() const override { return "4-Wheeler"; }

    std::string getSpecificDetails() const override {
        return std::to_string(seatingCapacity) + " Seater, " + transmission + ", " + fuelType;
    }
};

// ============================================================================
// SYSTEM CONTROLLER WITH 3 DATA STRUCTURES
// ============================================================================
class RentalSystem {
private:
    // DATA STRUCTURE 1: Hash Map for O(1) Fast Vehicle Lookup by ID
    std::unordered_map<std::string, std::shared_ptr<Vehicle>> vehicleRegistry;
    std::vector<std::string> vehicleKeys;

    // DATA STRUCTURE 2: FIFO Queue for Washing/Maintenance Pipeline
    std::queue<std::string> cleaningBayQueue;

    // DATA STRUCTURE 3: Vector for Transaction History & Financial Log
    std::vector<Transaction> transactionHistory;

    int nextTransactionSeq;

    std::string generateTransactionId() {
        return "TXN-" + std::to_string(nextTransactionSeq++);
    }

    void printTableDivider() const {
        std::cout << "+-----+--------+-----------+----------------------+--------------------------+-----------+----------+-------------+-----------------------------+\n";
    }

    void printTableHeader() const {
        printTableDivider();
        std::cout << "| " << std::left
                  << std::setw(3) << "#" << " | "
                  << std::setw(6) << "ID" << " | "
                  << std::setw(9) << "Category" << " | "
                  << std::setw(20) << "Brand & Model" << " | "
                  << std::setw(24) << "Specifications" << " | "
                  << std::setw(9) << "Rate/Day" << " | "
                  << std::setw(8) << "Fine/Day" << " | "
                  << std::setw(11) << "Status" << " | "
                  << std::setw(27) << "Current Rental Info" << " |\n";
        printTableDivider();
    }

    void printVehicleRow(const std::shared_ptr<Vehicle>& v, int serialNo) const {
        std::string rentalInfo = "-";
        if (v->getStatus() == VehicleStatus::RENTED) {
            rentalInfo = v->getCustomerName() + " (" + std::to_string(v->getRentedForDays()) + "d)";
        } else if (v->getStatus() == VehicleStatus::IN_CLEANING) {
            rentalInfo = "In Washing Queue";
        } else {
            rentalInfo = "Available for Rent";
        }

        std::ostringstream rateStream, fineStream;
        rateStream << "Rs." << std::fixed << std::setprecision(0) << v->getDailyRate();
        fineStream << "Rs." << std::fixed << std::setprecision(0) << v->getLateFinePerDay();

        std::cout << "| " << std::left
                  << std::setw(3) << serialNo << " | "
                  << std::setw(6) << v->getId() << " | "
                  << std::setw(9) << v->getCategoryName() << " | "
                  << std::setw(20) << (v->getFullName().substr(0, 20)) << " | "
                  << std::setw(24) << (v->getSpecificDetails().substr(0, 24)) << " | "
                  << std::setw(9) << rateStream.str() << " | "
                  << std::setw(8) << fineStream.str() << " | "
                  << std::setw(11) << v->getStatusString() << " | "
                  << std::setw(27) << (rentalInfo.substr(0, 27)) << " |\n";
    }

public:
    RentalSystem() : nextTransactionSeq(1001) {
        loadSampleData();
    }

    void loadSampleData() {
        auto tw1 = std::make_shared<TwoWheeler>("TW101", "Honda", "Activa 6G", 350.0, 100.0, 110, false, true);
        auto tw2 = std::make_shared<TwoWheeler>("TW102", "Royal Enfield", "Classic 350", 900.0, 300.0, 350, true, true);
        auto tw3 = std::make_shared<TwoWheeler>("TW103", "Yamaha", "R15 V4", 750.0, 250.0, 155, true, true);
        auto tw4 = std::make_shared<TwoWheeler>("TW104", "TVS", "Jupiter 125", 380.0, 120.0, 125, false, true);

        auto fw1 = std::make_shared<FourWheeler>("FW201", "Maruti", "Swift Dzire", 1600.0, 500.0, 5, "Manual", "Petrol");
        auto fw2 = std::make_shared<FourWheeler>("FW202", "Hyundai", "Creta SX", 2500.0, 800.0, 5, "Automatic", "Diesel");
        auto fw3 = std::make_shared<FourWheeler>("FW203", "Mahindra", "Thar 4x4", 3200.0, 1000.0, 4, "Manual", "Diesel");
        auto fw4 = std::make_shared<FourWheeler>("FW204", "Toyota", "Innova Crysta", 3800.0, 1200.0, 7, "Automatic", "Diesel");

        // Set realistic starting states
        tw2->rentOut("Rahul Sharma", "9876543210", 3);
        fw3->rentOut("Priya Patel", "9123456789", 2);
        tw4->markAsCleaning();

        auto addV = [this](const std::shared_ptr<Vehicle>& v) {
            vehicleRegistry[v->getId()] = v;
            vehicleKeys.push_back(v->getId());
        };

        addV(tw1); addV(tw2); addV(tw3); addV(tw4);
        addV(fw1); addV(fw2); addV(fw3); addV(fw4);

        cleaningBayQueue.push(tw4->getId());

        Transaction t0;
        t0.transactionId = "TXN-1000";
        t0.vehicleId = "FW201";
        t0.vehicleName = "Maruti Swift Dzire";
        t0.category = "4-Wheeler";
        t0.customerName = "Amit Kumar";
        t0.customerPhone = "9988776655";
        t0.bookedDays = 2;
        t0.actualDays = 3;
        t0.overdueDays = 1;
        t0.dailyRate = 1600.0;
        t0.baseFare = 3200.0;
        t0.lateFineRate = 500.0;
        t0.totalFine = 500.0;
        t0.totalBill = 3700.0;
        transactionHistory.push_back(t0);
    }

    void displayAllVehicles() const {
        std::cout << "\n===================================================================================================================================\n";
        std::cout << "                                                    COMPLETE VEHICLE FLEET INVENTORY                                              \n";
        std::cout << "===================================================================================================================================\n";
        printTableHeader();
        int serial = 1;
        for (const auto& key : vehicleKeys) {
            auto it = vehicleRegistry.find(key);
            if (it != vehicleRegistry.end()) {
                printVehicleRow(it->second, serial++);
            }
        }
        printTableDivider();
        std::cout << "Total fleet count: " << vehicleKeys.size() << " vehicles\n\n";
    }

    void displayTwoWheelers() const {
        std::cout << "\n===================================================================================================================================\n";
        std::cout << "                                                    2-WHEELER FLEET (BIKES & SCOOTERS)                                            \n";
        std::cout << "===================================================================================================================================\n";
        printTableHeader();
        int serial = 1;
        for (const auto& key : vehicleKeys) {
            auto v = vehicleRegistry.at(key);
            if (v->getType() == VehicleType::TWO_WHEELER) {
                printVehicleRow(v, serial++);
            }
        }
        printTableDivider();
        std::cout << "\n";
    }

    void displayFourWheelers() const {
        std::cout << "\n===================================================================================================================================\n";
        std::cout << "                                                    4-WHEELER FLEET (CARS & SUVS)                                                 \n";
        std::cout << "===================================================================================================================================\n";
        printTableHeader();
        int serial = 1;
        for (const auto& key : vehicleKeys) {
            auto v = vehicleRegistry.at(key);
            if (v->getType() == VehicleType::FOUR_WHEELER) {
                printVehicleRow(v, serial++);
            }
        }
        printTableDivider();
        std::cout << "\n";
    }

    void displayByStatus(VehicleStatus targetStatus) const {
        std::string statusName = (targetStatus == VehicleStatus::AVAILABLE) ? "AVAILABLE" :
                                 (targetStatus == VehicleStatus::RENTED) ? "CURRENTLY RENTED" : "IN CLEANING / MAINTENANCE";

        std::cout << "\n===================================================================================================================================\n";
        std::cout << "                                                VEHICLES FILTERED BY STATUS: " << statusName << "\n";
        std::cout << "===================================================================================================================================\n";
        printTableHeader();
        int serial = 1;
        int count = 0;
        for (const auto& key : vehicleKeys) {
            auto v = vehicleRegistry.at(key);
            if (v->getStatus() == targetStatus) {
                printVehicleRow(v, serial++);
                count++;
            }
        }
        printTableDivider();
        std::cout << "Count matching [" << statusName << "]: " << count << " vehicle(s)\n\n";
    }

    void searchVehicle() const {
        std::cout << "\nEnter Vehicle ID to search (e.g. TW101, FW201): ";
        std::string id;
        std::cin >> id;

        // Fast O(1) Hash Table Lookup
        auto it = vehicleRegistry.find(id);
        if (it == vehicleRegistry.end()) {
            std::cout << " [!] Vehicle with ID '" << id << "' not found.\n";
            return;
        }

        std::cout << "\n [OK] Vehicle Found:\n";
        it->second->printCard();
    }

    void rentVehicle() {
        std::cout << "\n==========================================================\n";
        std::cout << "                     RENT A VEHICLE                       \n";
        std::cout << "==========================================================\n";
        std::cout << "Enter Vehicle ID to rent: ";
        std::string id;
        std::cin >> id;

        auto it = vehicleRegistry.find(id);
        if (it == vehicleRegistry.end()) {
            std::cout << " [!] Vehicle ID '" << id << "' does not exist in registry.\n";
            return;
        }

        auto vehicle = it->second;

        if (vehicle->getStatus() == VehicleStatus::RENTED) {
            std::cout << " [X] Sorry, this vehicle is ALREADY RENTED to "
                      << vehicle->getCustomerName() << " for " << vehicle->getRentedForDays() << " day(s).\n";
            return;
        }

        if (vehicle->getStatus() == VehicleStatus::IN_CLEANING) {
            std::cout << " [X] This vehicle is currently IN THE CLEANING & SERVICE BAY.\n";
            std::cout << "     Please wash and inspect it first from the cleaning menu.\n";
            return;
        }

        std::cout << "\nVehicle Selected: " << vehicle->getFullName() << " (" << vehicle->getCategoryName() << ")\n";
        std::cout << "Rate: Rs. " << vehicle->getDailyRate() << "/day | Late Fine: Rs. " << vehicle->getLateFinePerDay() << "/day\n";

        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        std::string custName, custPhone;
        int rentalDays = 0;

        std::cout << "Enter Customer Full Name: ";
        std::getline(std::cin, custName);
        if (custName.empty()) {
            std::cout << " [!] Customer name cannot be empty.\n";
            return;
        }

        std::cout << "Enter Customer Phone Number: ";
        std::getline(std::cin, custPhone);

        std::cout << "Enter Rental Duration (in days): ";
        while (!(std::cin >> rentalDays) || rentalDays <= 0) {
            std::cout << " [!] Invalid input. Please enter positive days: ";
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        }

        double estimatedCost = rentalDays * vehicle->getDailyRate();
        std::cout << "\n----------------------------------------------------------\n";
        std::cout << " Rental Duration : " << rentalDays << " day(s)\n";
        std::cout << " Base Daily Rate : Rs. " << vehicle->getDailyRate() << "\n";
        std::cout << " Estimated Cost  : Rs. " << std::fixed << std::setprecision(2) << estimatedCost << "\n";
        std::cout << " Late Fine Policy: Rs. " << vehicle->getLateFinePerDay() << "/day if delayed\n";
        std::cout << "----------------------------------------------------------\n";
        std::cout << "Confirm rental booking? (y/n): ";
        char confirm;
        std::cin >> confirm;

        if (confirm == 'y' || confirm == 'Y') {
            vehicle->rentOut(custName, custPhone, rentalDays);
            std::cout << "\n [SUCCESS] Vehicle " << vehicle->getId() << " successfully rented to " << custName << "!\n";
        } else {
            std::cout << " [*] Rental booking cancelled.\n";
        }
    }

    void returnVehicle() {
        std::cout << "\n==========================================================\n";
        std::cout << "                    RETURN A VEHICLE                      \n";
        std::cout << "==========================================================\n";
        std::cout << "Enter Vehicle ID to return: ";
        std::string id;
        std::cin >> id;

        auto it = vehicleRegistry.find(id);
        if (it == vehicleRegistry.end()) {
            std::cout << " [!] Vehicle ID '" << id << "' not found.\n";
            return;
        }

        auto vehicle = it->second;

        if (vehicle->getStatus() != VehicleStatus::RENTED) {
            std::cout << " [!] Cannot return: Vehicle is not rented (Current Status: "
                      << vehicle->getStatusString() << ").\n";
            return;
        }

        std::cout << "\nVehicle   : " << vehicle->getFullName() << " (" << vehicle->getCategoryName() << ")\n";
        std::cout << "Rented To : " << vehicle->getCustomerName() << " (" << vehicle->getCustomerPhone() << ")\n";
        std::cout << "Booked For: " << vehicle->getRentedForDays() << " day(s)\n";
        std::cout << "Daily Rate: Rs. " << vehicle->getDailyRate() << " | Late Fine Rate: Rs. " << vehicle->getLateFinePerDay() << "/day\n\n";

        int actualDays = 0;
        std::cout << "Enter Actual Number of Days the vehicle was used: ";
        while (!(std::cin >> actualDays) || actualDays <= 0) {
            std::cout << " [!] Invalid input. Enter actual days (>= 1): ";
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        }

        int bookedDays = vehicle->getRentedForDays();
        int overdueDays = std::max(0, actualDays - bookedDays);
        double baseFare = bookedDays * vehicle->getDailyRate();
        double fine = overdueDays * vehicle->getLateFinePerDay();
        double totalBill = baseFare + fine;

        // Transaction Record (Vector DS)
        Transaction txn;
        txn.transactionId = generateTransactionId();
        txn.vehicleId = vehicle->getId();
        txn.vehicleName = vehicle->getFullName();
        txn.category = vehicle->getCategoryName();
        txn.customerName = vehicle->getCustomerName();
        txn.customerPhone = vehicle->getCustomerPhone();
        txn.bookedDays = bookedDays;
        txn.actualDays = actualDays;
        txn.overdueDays = overdueDays;
        txn.dailyRate = vehicle->getDailyRate();
        txn.baseFare = baseFare;
        txn.lateFineRate = vehicle->getLateFinePerDay();
        txn.totalFine = fine;
        txn.totalBill = totalBill;

        transactionHistory.push_back(txn);

        // Display bill receipt
        txn.printReceipt();

        // Enqueue to Cleaning (Queue DS)
        vehicle->markAsCleaning();
        cleaningBayQueue.push(vehicle->getId());

        std::cout << " [CLEANING DISPATCH] Vehicle " << vehicle->getId()
                  << " has been routed to the Cleaning Bay Queue.\n";
        std::cout << " Cleaning Queue Position: #" << cleaningBayQueue.size() << "\n";
    }

    void viewCleaningBay() const {
        std::cout << "\n==========================================================\n";
        std::cout << "         CLEANING & MAINTENANCE BAY (FIFO QUEUE)          \n";
        std::cout << "==========================================================\n";

        if (cleaningBayQueue.empty()) {
            std::cout << " [i] Cleaning Bay is currently empty. No vehicles awaiting service.\n";
            return;
        }

        std::cout << "Vehicles waiting in FIFO order for washing & sanitization:\n";
        std::cout << "Queue Size: " << cleaningBayQueue.size() << "\n\n";

        std::queue<std::string> tempQueue = cleaningBayQueue;
        int pos = 1;
        while (!tempQueue.empty()) {
            std::string vid = tempQueue.front();
            tempQueue.pop();

            auto it = vehicleRegistry.find(vid);
            if (it != vehicleRegistry.end()) {
                std::cout << " Position #" << pos++ << " -> Vehicle ID: " << vid
                          << " | " << it->second->getFullName()
                          << " (" << it->second->getCategoryName() << ")\n";
            }
        }
        std::cout << "==========================================================\n";
    }

    void processNextInCleaningBay() {
        std::cout << "\n==========================================================\n";
        std::cout << "             SERVICING NEXT VEHICLE IN QUEUE              \n";
        std::cout << "==========================================================\n";

        if (cleaningBayQueue.empty()) {
            std::cout << " [i] Cleaning queue is empty. No vehicles to wash.\n";
            return;
        }

        std::string nextVehicleId = cleaningBayQueue.front();
        cleaningBayQueue.pop();

        auto it = vehicleRegistry.find(nextVehicleId);
        if (it != vehicleRegistry.end()) {
            it->second->markAsAvailable();
            std::cout << " [SUCCESS] Cleaning and quality check completed for:\n";
            std::cout << " Vehicle ID   : " << nextVehicleId << "\n";
            std::cout << " Model        : " << it->second->getFullName() << "\n";
            std::cout << " New Status   : AVAILABLE FOR RENT\n";
            std::cout << " Remaining in queue: " << cleaningBayQueue.size() << "\n";
        }
    }

    void processAllInCleaningBay() {
        std::cout << "\n==========================================================\n";
        std::cout << "           SERVICING ALL VEHICLES IN CLEANING BAY         \n";
        std::cout << "==========================================================\n";

        if (cleaningBayQueue.empty()) {
            std::cout << " [i] Cleaning queue is empty.\n";
            return;
        }

        int count = 0;
        while (!cleaningBayQueue.empty()) {
            std::string vid = cleaningBayQueue.front();
            cleaningBayQueue.pop();

            auto it = vehicleRegistry.find(vid);
            if (it != vehicleRegistry.end()) {
                it->second->markAsAvailable();
                std::cout << " [v] Restored: " << vid << " (" << it->second->getFullName() << ")\n";
                count++;
            }
        }
        std::cout << "\n [SUCCESS] All " << count << " vehicle(s) cleaned and marked AVAILABLE!\n";
    }

    void displayTransactionHistory() const {
        std::cout << "\n========================================================================================================\n";
        std::cout << "                                  RENTAL TRANSACTION HISTORY (AUDIT LOG)                                \n";
        std::cout << "========================================================================================================\n";

        if (transactionHistory.empty()) {
            std::cout << " No completed transactions recorded yet.\n";
            return;
        }

        std::cout << "+----------+--------+----------------------+-----------------+------+-------+---------+----------+----------+\n";
        std::cout << "| TXN ID   | Veh ID | Model                | Customer        | Book | Exact | Base(Rs)| Fine(Rs) | Total(Rs)|\n";
        std::cout << "+----------+--------+----------------------+-----------------+------+-------+---------+----------+----------+\n";

        for (const auto& t : transactionHistory) {
            std::cout << "| " << std::left
                      << std::setw(8) << t.transactionId << " | "
                      << std::setw(6) << t.vehicleId << " | "
                      << std::setw(20) << (t.vehicleName.substr(0, 20)) << " | "
                      << std::setw(15) << (t.customerName.substr(0, 15)) << " | "
                      << std::setw(4) << t.bookedDays << " | "
                      << std::setw(5) << t.actualDays << " | "
                      << std::setw(7) << std::fixed << std::setprecision(0) << t.baseFare << " | "
                      << std::setw(8) << t.totalFine << " | "
                      << std::setw(8) << t.totalBill << " |\n";
        }
        std::cout << "+----------+--------+----------------------+-----------------+------+-------+---------+----------+----------+\n\n";
    }

    void displayFinancialSummary() const {
        std::cout << "\n==========================================================\n";
        std::cout << "               FINANCIAL & REVENUE SUMMARY                \n";
        std::cout << "==========================================================\n";

        double totalBaseRevenue = 0.0;
        double totalFinesCollected = 0.0;
        int lateReturnsCount = 0;

        for (const auto& t : transactionHistory) {
            totalBaseRevenue += t.baseFare;
            totalFinesCollected += t.totalFine;
            if (t.overdueDays > 0) lateReturnsCount++;
        }

        std::cout << " Total Completed Rentals : " << transactionHistory.size() << "\n";
        std::cout << " Base Rental Earnings    : Rs. " << std::fixed << std::setprecision(2) << totalBaseRevenue << "\n";
        std::cout << " Overdue Fines Collected : Rs. " << totalFinesCollected << "\n";
        std::cout << " Number of Late Returns  : " << lateReturnsCount << "\n";
        std::cout << "----------------------------------------------------------\n";
        std::cout << " GRAND TOTAL REVENUE     : Rs. " << (totalBaseRevenue + totalFinesCollected) << "\n";
        std::cout << "==========================================================\n\n";
    }

    void addNewVehicle() {
        std::cout << "\n==========================================================\n";
        std::cout << "                     ADD NEW VEHICLE                      \n";
        std::cout << "==========================================================\n";
        std::cout << "Choose Category:\n";
        std::cout << "  1. 2-Wheeler (Motorcycle / Scooter)\n";
        std::cout << "  2. 4-Wheeler (Car / SUV)\n";
        std::cout << "Select (1 or 2): ";
        int choice = 0;
        std::cin >> choice;

        if (choice != 1 && choice != 2) {
            std::cout << " [!] Invalid category choice.\n";
            return;
        }

        std::string id, brand, model;
        double dailyRate = 0, lateFine = 0;

        std::cout << "Enter Unique Vehicle ID (e.g., TW105 or FW205): ";
        std::cin >> id;

        if (vehicleRegistry.find(id) != vehicleRegistry.end()) {
            std::cout << " [!] Error: Vehicle ID '" << id << "' already exists!\n";
            return;
        }

        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        std::cout << "Enter Brand (e.g., Honda, Hyundai, Tata): ";
        std::getline(std::cin, brand);

        std::cout << "Enter Model Name (e.g., City, Activa, Nexon): ";
        std::getline(std::cin, model);

        std::cout << "Enter Daily Rental Rate (in Rs.): ";
        std::cin >> dailyRate;

        std::cout << "Enter Late Return Fine per day (in Rs.): ";
        std::cin >> lateFine;

        if (choice == 1) {
            int cc, gearedInput;
            std::cout << "Enter Engine Capacity (in CC): ";
            std::cin >> cc;
            std::cout << "Is it Geared? (1 for Yes, 0 for No): ";
            std::cin >> gearedInput;

            auto newV = std::make_shared<TwoWheeler>(id, brand, model, dailyRate, lateFine, cc, gearedInput == 1, true);
            vehicleRegistry[id] = newV;
            vehicleKeys.push_back(id);
        } else {
            int seats;
            std::string transmission, fuelType;
            std::cout << "Enter Seating Capacity (e.g. 5, 7): ";
            std::cin >> seats;
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            std::cout << "Enter Transmission (Manual / Automatic): ";
            std::getline(std::cin, transmission);
            std::cout << "Enter Fuel Type (Petrol / Diesel / Electric): ";
            std::getline(std::cin, fuelType);

            auto newV = std::make_shared<FourWheeler>(id, brand, model, dailyRate, lateFine, seats, transmission, fuelType);
            vehicleRegistry[id] = newV;
            vehicleKeys.push_back(id);
        }

        std::cout << "\n [SUCCESS] Vehicle [" << id << "] " << brand << " " << model << " registered successfully!\n";
    }
};

// ============================================================================
// MAIN APPLICATION ENTRY POINT
// ============================================================================
int main() {
    RentalSystem system;

    std::cout << "\n==================================================================\n";
    std::cout << "        W E E L S & W A Y S  -  V E H I C L E   R E N T A L       \n";
    std::cout << "               Terminal Management & Dispatch System              \n";
    std::cout << "==================================================================\n";

    int choice = -1;
    while (choice != 0) {
        std::cout << "\n+------------------------ MAIN MENU -----------------------------+\n";
        std::cout << "| 1. Display All Vehicles (Complete Fleet Inventory)             |\n";
        std::cout << "| 2. View Vehicles by Category (2-Wheelers vs 4-Wheelers)        |\n";
        std::cout << "| 3. Filter Vehicles by Status (Available / Rented / Cleaning)   |\n";
        std::cout << "| 4. Search Vehicle by ID                                        |\n";
        std::cout << "| 5. Rent a Vehicle                                              |\n";
        std::cout << "| 6. Return a Vehicle (Calculate Late Fine & Enqueue to Clean)   |\n";
        std::cout << "| 7. Cleaning & Maintenance Bay (FIFO Queue Operations)          |\n";
        std::cout << "| 8. View Completed Transaction History & Invoices               |\n";
        std::cout << "| 9. View Financial & Revenue Analytics                          |\n";
        std::cout << "| 10. Add New Vehicle to Fleet                                   |\n";
        std::cout << "| 0. Exit System                                                 |\n";
        std::cout << "+----------------------------------------------------------------+\n";
        std::cout << "Enter your choice [0-10]: ";

        if (!(std::cin >> choice)) {
            std::cout << "\n [!] Invalid input! Please enter a number between 0 and 10.\n";
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            continue;
        }

        switch (choice) {
            case 1: system.displayAllVehicles(); break;
            case 2: {
                std::cout << "\nSelect Category (1: 2-Wheelers, 2: 4-Wheelers): ";
                int cat; std::cin >> cat;
                if (cat == 1) system.displayTwoWheelers();
                else if (cat == 2) system.displayFourWheelers();
                else std::cout << " [!] Invalid category choice.\n";
                break;
            }
            case 3: {
                std::cout << "\nFilter Status (1: Available, 2: Rented, 3: Cleaning): ";
                int st; std::cin >> st;
                if (st == 1) system.displayByStatus(VehicleStatus::AVAILABLE);
                else if (st == 2) system.displayByStatus(VehicleStatus::RENTED);
                else if (st == 3) system.displayByStatus(VehicleStatus::IN_CLEANING);
                else std::cout << " [!] Invalid status choice.\n";
                break;
            }
            case 4: system.searchVehicle(); break;
            case 5: system.rentVehicle(); break;
            case 6: system.returnVehicle(); break;
            case 7: {
                std::cout << "\nCleaning Bay (1: View Queue, 2: Wash Next, 3: Wash All): ";
                int cb; std::cin >> cb;
                if (cb == 1) system.viewCleaningBay();
                else if (cb == 2) system.processNextInCleaningBay();
                else if (cb == 3) system.processAllInCleaningBay();
                else std::cout << " [!] Invalid option.\n";
                break;
            }
            case 8: system.displayTransactionHistory(); break;
            case 9: system.displayFinancialSummary(); break;
            case 10: system.addNewVehicle(); break;
            case 0: std::cout << "\nThank you for using Wheels & Ways Vehicle Rental System!\n"; break;
            default: std::cout << "\n [!] Invalid choice! Enter 0 to 10.\n"; break;
        }
    }

    return 0;
}
