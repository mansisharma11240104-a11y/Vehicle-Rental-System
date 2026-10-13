#include "RentalSystem.h"
#include <iomanip>
#include <algorithm>
#include <limits>
#include <sstream>

RentalSystem::RentalSystem() : nextTransactionSeq(1001) {
    loadSampleData();
}

std::string RentalSystem::generateTransactionId() {
    std::string tid = "TXN-" + std::to_string(nextTransactionSeq++);
    return tid;
}

void RentalSystem::loadSampleData() {
    // 2-Wheelers
    auto tw1 = std::make_shared<TwoWheeler>("TW101", "Honda", "Activa 6G", 350.0, 100.0, 110, false, true);
    auto tw2 = std::make_shared<TwoWheeler>("TW102", "Royal Enfield", "Classic 350", 900.0, 300.0, 350, true, true);
    auto tw3 = std::make_shared<TwoWheeler>("TW103", "Yamaha", "R15 V4", 750.0, 250.0, 155, true, true);
    auto tw4 = std::make_shared<TwoWheeler>("TW104", "TVS", "Jupiter 125", 380.0, 120.0, 125, false, true);

    // 4-Wheelers
    auto fw1 = std::make_shared<FourWheeler>("FW201", "Maruti", "Swift Dzire", 1600.0, 500.0, 5, "Manual", "Petrol");
    auto fw2 = std::make_shared<FourWheeler>("FW202", "Hyundai", "Creta SX", 2500.0, 800.0, 5, "Automatic", "Diesel");
    auto fw3 = std::make_shared<FourWheeler>("FW203", "Mahindra", "Thar 4x4", 3200.0, 1000.0, 4, "Manual", "Diesel");
    auto fw4 = std::make_shared<FourWheeler>("FW204", "Toyota", "Innova Crysta", 3800.0, 1200.0, 7, "Automatic", "Diesel");

    // Pre-populate some states to demonstrate functionality immediately:
    // TW102 is currently rented
    tw2->rentOut("Rahul Sharma", "9876543210", 3);

    // FW203 is currently rented
    fw3->rentOut("Priya Patel", "9123456789", 2);

    // TW104 is in cleaning queue
    tw4->markAsCleaning();

    // Register into Hash Map (Data Structure 1) and Key list
    auto registerVehicle = [this](const std::shared_ptr<Vehicle>& v) {
        vehicleRegistry[v->getId()] = v;
        vehicleKeys.push_back(v->getId());
    };

    registerVehicle(tw1);
    registerVehicle(tw2);
    registerVehicle(tw3);
    registerVehicle(tw4);
    registerVehicle(fw1);
    registerVehicle(fw2);
    registerVehicle(fw3);
    registerVehicle(fw4);

    // Put TW104 into Cleaning Queue (Data Structure 2)
    cleaningBayQueue.push(tw4->getId());

    // Pre-load an initial completed transaction in Vector (Data Structure 3)
    Transaction sampleTxn;
    sampleTxn.transactionId = "TXN-1000";
    sampleTxn.vehicleId = "FW201";
    sampleTxn.vehicleName = "Maruti Swift Dzire";
    sampleTxn.category = "4-Wheeler";
    sampleTxn.customerName = "Amit Kumar";
    sampleTxn.customerPhone = "9988776655";
    sampleTxn.bookedDays = 2;
    sampleTxn.actualDays = 3;
    sampleTxn.overdueDays = 1;
    sampleTxn.dailyRate = 1600.0;
    sampleTxn.baseFare = 3200.0;
    sampleTxn.lateFineRate = 500.0;
    sampleTxn.totalFine = 500.0;
    sampleTxn.totalBill = 3700.0;
    sampleTxn.returnDate = "2026-10-12";
    transactionHistory.push_back(sampleTxn);
}

void RentalSystem::printTableDivider() const {
    std::cout << "+-----+--------+-----------+----------------------+--------------------------+-----------+----------+-------------+-----------------------------+\n";
}

void RentalSystem::printTableHeader() const {
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

void RentalSystem::printVehicleRow(const std::shared_ptr<Vehicle>& v, int serialNo) const {
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

void RentalSystem::displayAllVehicles() const {
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

void RentalSystem::displayTwoWheelers() const {
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

void RentalSystem::displayFourWheelers() const {
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

void RentalSystem::displayByStatus(VehicleStatus targetStatus) const {
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

void RentalSystem::searchVehicle() const {
    std::cout << "\nEnter Vehicle ID to search (e.g. TW101, FW201): ";
    std::string id;
    std::cin >> id;

    // Fast O(1) Lookup using Hash Map (Data Structure 1)
    auto it = vehicleRegistry.find(id);
    if (it == vehicleRegistry.end()) {
        std::cout << " [!] Vehicle with ID '" << id << "' not found in the registry.\n";
        return;
    }

    std::cout << "\n [OK] Vehicle Found via Hash Table Lookup:\n";
    it->second->printCard();
}

void RentalSystem::rentVehicle() {
    std::cout << "\n==========================================================\n";
    std::cout << "                     RENT A VEHICLE                       \n";
    std::cout << "==========================================================\n";
    std::cout << "Enter Vehicle ID you wish to rent: ";
    std::string id;
    std::cin >> id;

    // Hash Map Lookup
    auto it = vehicleRegistry.find(id);
    if (it == vehicleRegistry.end()) {
        std::cout << " [!] Error: Vehicle ID '" << id << "' does not exist in registry.\n";
        return;
    }

    auto vehicle = it->second;

    if (vehicle->getStatus() == VehicleStatus::RENTED) {
        std::cout << " [X] Sorry, this vehicle is ALREADY RENTED to "
                  << vehicle->getCustomerName() << " for " << vehicle->getRentedForDays() << " day(s).\n";
        return;
    }

    if (vehicle->getStatus() == VehicleStatus::IN_CLEANING) {
        std::cout << " [X] This vehicle is currently in the CLEANING & SERVICE BAY.\n";
        std::cout << "     Please process it from the cleaning queue first.\n";
        return;
    }

    // Vehicle is AVAILABLE
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
        std::cout << " [!] Invalid input. Please enter a positive number of days: ";
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
        std::cout << " Status updated to RENTED.\n";
    } else {
        std::cout << " [*] Rental booking cancelled.\n";
    }
}

void RentalSystem::returnVehicle() {
    std::cout << "\n==========================================================\n";
    std::cout << "                    RETURN A VEHICLE                      \n";
    std::cout << "==========================================================\n";
    std::cout << "Enter Vehicle ID to return: ";
    std::string id;
    std::cin >> id;

    // Hash Map Lookup
    auto it = vehicleRegistry.find(id);
    if (it == vehicleRegistry.end()) {
        std::cout << " [!] Vehicle ID '" << id << "' not found.\n";
        return;
    }

    auto vehicle = it->second;

    if (vehicle->getStatus() != VehicleStatus::RENTED) {
        std::cout << " [!] Cannot return: Vehicle is not currently rented (Current Status: "
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
        std::cout << " [!] Invalid input. Please enter actual days (>= 1): ";
        std::cin.clear();
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    }

    int bookedDays = vehicle->getRentedForDays();
    int overdueDays = std::max(0, actualDays - bookedDays);
    double baseFare = bookedDays * vehicle->getDailyRate();
    double fine = overdueDays * vehicle->getLateFinePerDay();
    double totalBill = baseFare + fine;

    // Create Transaction Record (Data Structure 3: Vector)
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
    txn.returnDate = "2026-10-13";

    transactionHistory.push_back(txn);

    // Print invoice
    txn.printReceipt();

    // Move Vehicle to Cleaning & Maintenance State
    vehicle->markAsCleaning();

    // Enqueue vehicle into Cleaning Queue (Data Structure 2: FIFO Queue)
    cleaningBayQueue.push(vehicle->getId());

    std::cout << " [CLEANING DISPATCH] Vehicle " << vehicle->getId()
              << " has been routed to the Cleaning Bay Queue.\n";
    std::cout << " Cleaning Queue Position: #" << cleaningBayQueue.size() << "\n";
}

void RentalSystem::viewCleaningBay() const {
    std::cout << "\n==========================================================\n";
    std::cout << "         CLEANING & MAINTENANCE BAY (FIFO QUEUE)          \n";
    std::cout << "==========================================================\n";

    if (cleaningBayQueue.empty()) {
        std::cout << " [i] Cleaning Bay is currently empty. No vehicles awaiting service.\n";
        return;
    }

    std::cout << "Vehicles waiting in FIFO order for washing & sanitization:\n";
    std::cout << "Queue Size: " << cleaningBayQueue.size() << "\n\n";

    // Copy queue to display elements without mutating the member queue
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

void RentalSystem::processNextInCleaningBay() {
    std::cout << "\n==========================================================\n";
    std::cout << "             SERVICING NEXT VEHICLE IN QUEUE              \n";
    std::cout << "==========================================================\n";

    if (cleaningBayQueue.empty()) {
        std::cout << " [i] Cleaning queue is empty. No vehicles to wash.\n";
        return;
    }

    // Pop from FIFO Queue (Data Structure 2)
    std::string nextVehicleId = cleaningBayQueue.front();
    cleaningBayQueue.pop();

    auto it = vehicleRegistry.find(nextVehicleId);
    if (it != vehicleRegistry.end()) {
        it->second->markAsAvailable();
        std::cout << " [SUCCESS] Cleaning and quality check completed for:\n";
        std::cout << " Vehicle ID   : " << nextVehicleId << "\n";
        std::cout << " Model        : " << it->second->getFullName() << "\n";
        std::cout << " New Status   : AVAILABLE FOR RENT\n";
        std::cout << " Remaining vehicles in queue: " << cleaningBayQueue.size() << "\n";
    }
}

void RentalSystem::processAllInCleaningBay() {
    std::cout << "\n==========================================================\n";
    std::cout << "           SERVICING ALL VEHICLES IN CLEANING BAY         \n";
    std::cout << "==========================================================\n";

    if (cleaningBayQueue.empty()) {
        std::cout << " [i] Cleaning queue is empty.\n";
        return;
    }

    int servicedCount = 0;
    while (!cleaningBayQueue.empty()) {
        std::string vid = cleaningBayQueue.front();
        cleaningBayQueue.pop();

        auto it = vehicleRegistry.find(vid);
        if (it != vehicleRegistry.end()) {
            it->second->markAsAvailable();
            std::cout << " [v] Cleaned & Restored: " << vid << " (" << it->second->getFullName() << ")\n";
            servicedCount++;
        }
    }
    std::cout << "\n [SUCCESS] All " << servicedCount << " vehicle(s) have been serviced and marked AVAILABLE!\n";
}

void RentalSystem::displayTransactionHistory() const {
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

void RentalSystem::displayFinancialSummary() const {
    std::cout << "\n==========================================================\n";
    std::cout << "               FINANCIAL & REVENUE SUMMARY                \n";
    std::cout << "==========================================================\n";

    double totalBaseRevenue = 0.0;
    double totalFinesCollected = 0.0;
    int lateReturnsCount = 0;

    for (const auto& t : transactionHistory) {
        totalBaseRevenue += t.baseFare;
        totalFinesCollected += t.totalFine;
        if (t.overdueDays > 0) {
            lateReturnsCount++;
        }
    }

    double grandTotalRevenue = totalBaseRevenue + totalFinesCollected;

    std::cout << " Total Completed Rentals : " << transactionHistory.size() << "\n";
    std::cout << " Base Rental Earnings    : Rs. " << std::fixed << std::setprecision(2) << totalBaseRevenue << "\n";
    std::cout << " Overdue Fines Collected : Rs. " << totalFinesCollected << "\n";
    std::cout << " Number of Late Returns  : " << lateReturnsCount << "\n";
    std::cout << "----------------------------------------------------------\n";
    std::cout << " GRAND TOTAL REVENUE     : Rs. " << grandTotalRevenue << "\n";
    std::cout << "==========================================================\n\n";
}

void RentalSystem::addNewVehicle() {
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

    // Check uniqueness via Hash Map
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
        int cc;
        int gearedInput;
        std::cout << "Enter Engine Capacity (in CC, e.g., 125, 350): ";
        std::cin >> cc;
        std::cout << "Is it Geared? (1 for Yes / Geared, 0 for No / Scooter): ";
        std::cin >> gearedInput;

        auto newVehicle = std::make_shared<TwoWheeler>(id, brand, model, dailyRate, lateFine, cc, gearedInput == 1, true);
        vehicleRegistry[id] = newVehicle;
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

        auto newVehicle = std::make_shared<FourWheeler>(id, brand, model, dailyRate, lateFine, seats, transmission, fuelType);
        vehicleRegistry[id] = newVehicle;
        vehicleKeys.push_back(id);
    }

    std::cout << "\n [SUCCESS] New vehicle [" << id << "] " << brand << " " << model << " added to registry!\n";
}
