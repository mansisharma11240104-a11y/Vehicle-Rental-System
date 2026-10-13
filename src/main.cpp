#include <iostream>
#include <limits>
#include "RentalSystem.h"

void displayBanner() {
    std::cout << "\n==================================================================\n";
    std::cout << "        W E E L S & W A Y S  -  V E H I C L E   R E N T A L       \n";
    std::cout << "               Terminal Management & Dispatch System              \n";
    std::cout << "==================================================================\n";
}

void printMainMenu() {
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
}

int main() {
    RentalSystem system;
    displayBanner();

    int choice = -1;
    while (choice != 0) {
        printMainMenu();
        if (!(std::cin >> choice)) {
            std::cout << "\n [!] Invalid input! Please enter a number between 0 and 10.\n";
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            continue;
        }

        switch (choice) {
            case 1:
                system.displayAllVehicles();
                break;

            case 2: {
                std::cout << "\nSelect Category:\n";
                std::cout << "  1. 2-Wheelers (Bikes & Scooters)\n";
                std::cout << "  2. 4-Wheelers (Cars & SUVs)\n";
                std::cout << "Enter selection (1 or 2): ";
                int catChoice = 0;
                std::cin >> catChoice;
                if (catChoice == 1) system.displayTwoWheelers();
                else if (catChoice == 2) system.displayFourWheelers();
                else std::cout << " [!] Invalid category option.\n";
                break;
            }

            case 3: {
                std::cout << "\nFilter by Status:\n";
                std::cout << "  1. Available for Rent\n";
                std::cout << "  2. Currently Rented\n";
                std::cout << "  3. In Cleaning / Maintenance\n";
                std::cout << "Enter selection (1-3): ";
                int statChoice = 0;
                std::cin >> statChoice;
                if (statChoice == 1) system.displayByStatus(VehicleStatus::AVAILABLE);
                else if (statChoice == 2) system.displayByStatus(VehicleStatus::RENTED);
                else if (statChoice == 3) system.displayByStatus(VehicleStatus::IN_CLEANING);
                else std::cout << " [!] Invalid status option.\n";
                break;
            }

            case 4:
                system.searchVehicle();
                break;

            case 5:
                system.rentVehicle();
                break;

            case 6:
                system.returnVehicle();
                break;

            case 7: {
                std::cout << "\nCleaning & Maintenance Bay Options:\n";
                std::cout << "  1. View FIFO Cleaning Queue\n";
                std::cout << "  2. Wash & Inspect Next Vehicle in Queue\n";
                std::cout << "  3. Wash & Restock All Vehicles in Queue\n";
                std::cout << "Enter choice (1-3): ";
                int cleanChoice = 0;
                std::cin >> cleanChoice;
                if (cleanChoice == 1) system.viewCleaningBay();
                else if (cleanChoice == 2) system.processNextInCleaningBay();
                else if (cleanChoice == 3) system.processAllInCleaningBay();
                else std::cout << " [!] Invalid option.\n";
                break;
            }

            case 8:
                system.displayTransactionHistory();
                break;

            case 9:
                system.displayFinancialSummary();
                break;

            case 10:
                system.addNewVehicle();
                break;

            case 0:
                std::cout << "\nThank you for using Wheels & Ways Vehicle Rental System. Goodbye!\n";
                break;

            default:
                std::cout << "\n [!] Invalid choice! Please select between 0 and 10.\n";
                break;
        }
    }

    return 0;
}
