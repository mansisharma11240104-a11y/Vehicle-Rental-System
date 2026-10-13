#ifndef TRANSACTION_H
#define TRANSACTION_H

#include <string>
#include <iostream>
#include <iomanip>

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
    std::string returnDate;

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

#endif // TRANSACTION_H
