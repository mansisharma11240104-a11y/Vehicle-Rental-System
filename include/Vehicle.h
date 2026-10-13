#ifndef VEHICLE_H
#define VEHICLE_H

#include <string>
#include <iostream>
#include <iomanip>

enum class VehicleType {
    TWO_WHEELER,
    FOUR_WHEELER
};

enum class VehicleStatus {
    AVAILABLE,
    RENTED,
    IN_CLEANING
};

class Vehicle {
protected:
    std::string id;
    std::string brand;
    std::string model;
    VehicleType type;
    VehicleStatus status;
    double dailyRate;
    double lateFinePerDay;

    // Rental tracking
    std::string currentCustomerName;
    std::string currentCustomerPhone;
    int rentedForDays;

public:
    Vehicle(std::string id, std::string brand, std::string model, VehicleType type,
            double dailyRate, double lateFinePerDay);
    virtual ~Vehicle() = default;

    // Getters
    std::string getId() const;
    std::string getBrand() const;
    std::string getModel() const;
    std::string getFullName() const;
    VehicleType getType() const;
    VehicleStatus getStatus() const;
    std::string getStatusString() const;
    double getDailyRate() const;
    double getLateFinePerDay() const;
    std::string getCustomerName() const;
    std::string getCustomerPhone() const;
    int getRentedForDays() const;

    // Operations
    void rentOut(const std::string& customerName, const std::string& phone, int days);
    void markAsCleaning();
    void markAsAvailable();

    // Virtual interfaces
    virtual std::string getCategoryName() const = 0;
    virtual std::string getSpecificDetails() const = 0;
    virtual void printCard() const;
};

#endif // VEHICLE_H
